#include "madara/transport/broadcast/Broadcast_Transport.h"
#include "madara/transport/broadcast/Broadcast_Transport_Read_Thread.h"
#include "madara/transport/Transport_Context.h"
#include "madara/utility/Log_Macros.h"
#include "madara/transport/Reduced_Message_Header.h"
#include "madara/utility/Utility.h"

#include <iostream>

Madara::Transport::Broadcast_Transport::Broadcast_Transport (const std::string & id,
        Madara::Knowledge_Engine::Thread_Safe_Context & context, 
        Settings & config, bool launch_transport)
: Base (config, context),
  id_ (id), thread_ (0), valid_setup_ (false)
{
  if (launch_transport)
    setup ();
}

Madara::Transport::Broadcast_Transport::~Broadcast_Transport ()
{
  close ();
}

void
Madara::Transport::Broadcast_Transport::close (void)
{
  this->invalidate_transport ();

  if (thread_)
  {
    thread_->close ();
    delete thread_;
    thread_ = 0;
  }
  
  socket_.close ();

  this->shutting_down_ = false;
}

int
Madara::Transport::Broadcast_Transport::reliability (void) const
{
  return Madara::Transport::BEST_EFFORT;
}

int
Madara::Transport::Broadcast_Transport::reliability (const int &)
{
  return Madara::Transport::BEST_EFFORT;
}

int
Madara::Transport::Broadcast_Transport::setup (void)
{
  // populate splitters vector for tokenizing multiassignments
  splitters_.resize (2);
  splitters_[0] = "=";
  splitters_[1] = ";";
  
  // check for an on_data_received ruleset
  if (settings_.on_data_received_logic.length () != 0)
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Broadcast_Transport::Broadcast_Transport:" \
      " setting rules to %s\n", 
      settings_.on_data_received_logic.c_str ()));

    Madara::Expression_Tree::Interpreter interpreter;
    on_data_received_ = interpreter.interpret (context_,
      settings_.on_data_received_logic);
  }
  else
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Broadcast_Transport::Broadcast_Transport:" \
      " no permanent rules were set\n"));
  }
  
  // setup the send buffer
  if (settings_.queue_length > 0)
    buffer_ = new char [settings_.queue_length];

  // resize addresses to be the size of the list of hosts
  addresses_.resize (this->settings_.hosts_.size ());

  if (addresses_.size () > 0)
  {
    // convert the string host:port into an ACE address
    for (unsigned int i = 0; i < addresses_.size (); ++i)
    {
      addresses_[i].set (settings_.hosts_[i].c_str ());

      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Broadcast_Transport::Broadcast_Transport:" \
        " settings address[%d] to %s:%d\n", i, 
        addresses_[i].get_host_addr (), addresses_[i].get_port_number ()));
    }
    
    // open the broadcast socket to any port for sending
    if (socket_.open (ACE_Addr::sap_any) == -1)
      std::cout << "Broadcast socket failed to open\n";
    
    // start thread with the addresses (only looks at the first one for now)
    thread_ = new Madara::Transport::Broadcast_Transport_Read_Thread (
                    settings_, id_, context_, addresses_[0], socket_,
                    send_monitor_, receive_monitor_);
    
  }
  return this->validate_transport ();
}

long
Madara::Transport::Broadcast_Transport::send_data (
  const Madara::Knowledge_Records & orig_updates)
{
  // check to see if we are shutting down
  long ret = this->check_transport ();
  if (-1 == ret)
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Broadcast_Transport::send_data: transport has been told to shutdown")); 
    return ret;
  }
  else if (-2 == ret)
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Broadcast_Transport::send_data: transport is not valid")); 
    return ret;
  }
 
  // get the maximum quality from the updates
  uint32_t quality = Madara::max_quality (orig_updates);
  bool reduced = false;
  
  Knowledge_Map filtered_updates;
  
  MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
    DLINFO "Broadcast_Transport::send_data:" \
    " Applying filters before sending...\n"));

  Transport_Context transport_context (Transport_Context::SENDING_OPERATION,
      receive_monitor_.get_bytes_per_second (),
      send_monitor_.get_bytes_per_second ());

  /**
   * filter the updates according to the filters specified by
   * the user in QoS_Transport_Settings (if applicable)
   **/
  for (Knowledge_Records::const_iterator i = orig_updates.begin ();
        i != orig_updates.end (); ++i)
  {
    // filter the record according to the send filter chain
    Knowledge_Record result = settings_.filter_send (*i->second, i->first,
      transport_context);

    if (result.status () != Knowledge_Record::UNCREATED)
      filtered_updates[i->first] = result;
  }
  
  MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
    DLINFO "Broadcast_Transport::send_data:" \
    " Finished applying filters before sending...\n"));
  
  if (filtered_updates.size () == 0)
  {
    MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
      DLINFO "Broadcast_Transport::send_data:" \
      " Filters removed all data. Nothing to send.\n"));

    return 0;
  }

  // allocate a buffer to send
  char * buffer = buffer_.get_ptr ();
  int64_t buffer_remaining = settings_.queue_length;
  
  if (buffer == 0)
  {
    MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_DEBUG, 
      DLINFO "Broadcast_Transport::send_data:" \
      " Unable to allocate buffer of size %d. Exiting thread.\n",
      settings_.queue_length));
    
    return -3;
  }

  
  // set the header to the beginning of the buffer
  Message_Header * header = 0;

  if (settings_.send_reduced_message_header)
  {
    MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
      DLINFO "Broadcast_Transport::send_data:" \
      " Preparing message with reduced message header->\n"));
    header = new Reduced_Message_Header ();
    reduced = true;
  }
  else
  {
    MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
      DLINFO "Broadcast_Transport::send_data:" \
      " Preparing message with normal message header->\n"));
    header = new Message_Header ();
  }

  // get the clock
  header->clock = context_.get_clock ();

  if (!reduced)
  {
    // copy the domain from settings
    strncpy (header->domain, this->settings_.domains.c_str (),
      sizeof (header->domain) - 1);

    // get the quality of the key
    header->quality = quality;

    // copy the message originator (our id)
    strncpy (header->originator, id_.c_str (), sizeof (header->originator) - 1);

    // send data is generally an assign type. However, Message_Header is
    // flexible enough to support both, and this will simply our read thread
    // handling
    header->type = Madara::Transport::MULTIASSIGN;

    // set the time-to-live
    header->ttl = settings_.get_rebroadcast_ttl ();
  }
  
  header->updates = uint32_t (filtered_updates.size ());

  // compute size of this header
  header->size = header->encoded_size ();

  // set the update to the end of the header
  char * update = header->write (buffer, buffer_remaining);
  uint64_t * message_size = (uint64_t *)buffer;
  
  // Message header format
  // [size|id|domain|originator|type|updates|quality|clock|list of updates]

  // zero out the memory
  //memset(buffer, 0, Madara::Transport::MAX_PACKET_SIZE);

  // Message update format
  // [key|value]
  
  int j = 0;
  for (Knowledge_Map::const_iterator i = filtered_updates.begin ();
    i != filtered_updates.end (); ++i, ++j)
  {
    update = i->second.write (update, i->first, buffer_remaining);
    
    if (buffer_remaining > 0)
    {
      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "Broadcast_Transport::send_data:" \
        " update[%d] => encoding %s of type %d and size %d\n",
        j, i->first.c_str (), i->second.type (), i->second.size ()));
    }
    else
    {
    MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_DEBUG, 
      DLINFO "Broadcast_Transport::send_data:" \
      " unable to encode update[%d] => %s of type %d and size %d\n",
      j, i->first.c_str (), i->second.type (), i->second.size ()));
    }
  }
  
  if (buffer_remaining > 0)
  {
    int size = (int)(settings_.queue_length - buffer_remaining);
    *message_size = Madara::Utility::endian_swap ((uint64_t)size);

    // before we send to others, we first execute rules
    if (settings_.on_data_received_logic.length () != 0)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Broadcast_Transport::send_data:" \
        " evaluating rules in %s\n", 
        settings_.on_data_received_logic.c_str ()));

      on_data_received_.evaluate ();

      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Broadcast_Transport::send_data:" \
        " rules have been successfully evaluated\n"));
    }
    else
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Broadcast_Transport::send_data:" \
        " no permanent rules were set\n"));
    }

    // send the buffer contents to the multicast address
  
    if (addresses_.size () > 0)
    {
      ssize_t bytes_sent = socket_.send(
        buffer, (ssize_t)size, addresses_[0]);

      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Broadcast_Transport::send_data:" \
        " Sent packet with size %d\n",
        bytes_sent));

      send_monitor_.add ((uint32_t)bytes_sent);
      
      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "Broadcast_Transport::send_data:" \
        " Send bandwidth = %d B/s\n",
        send_monitor_.get_bytes_per_second ()));
    }
  }
  
  delete header;
  return 0;
}