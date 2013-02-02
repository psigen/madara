#include "madara/transport/broadcast/Broadcast_Transport.h"
#include "madara/transport/broadcast/Broadcast_Transport_Read_Thread.h"
#include "madara/utility/Log_Macros.h"
#include "madara/transport/Message_Header.h"
#include "madara/utility/Utility.h"

#include <iostream>

Madara::Transport::Broadcast_Transport::Broadcast_Transport (const std::string & id,
        Madara::Knowledge_Engine::Thread_Safe_Context & context, 
        Settings & config, bool launch_transport)
: Base (config),
  id_ (id), context_ (context), thread_ (0), valid_setup_ (false)
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
    
    // start thread with the addresses (only looks at the first one for now)
    thread_ = new Madara::Transport::Broadcast_Transport_Read_Thread (
                    id_, context_, addresses_[0]);
    
    // open the broadcast socket to any port for sending
    if (socket_.open (ACE_Addr::sap_any) == -1)
      std::cout << "Broadcast socket failed to open\n";

  }
  return this->validate_transport ();
}

long
Madara::Transport::Broadcast_Transport::send_data (
  const Madara::Knowledge_Records & updates)
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
  uint32_t quality = Madara::max_quality (updates);


  // allocate a buffer to send
  char buffer [512000];
  int64_t buffer_remaining = 512000;

  // set the header to the beginning of the buffer
  Message_Header header;
  
  // get the clock
  header.clock = Madara::Utility::endian_swap (context_.get_clock ());

  // copy the Madara transport version identification
  strncpy (header.madara_id, MADARA_IDENTIFIER, 7);

  // copy the domain from settings
  strncpy (header.domain, this->settings_.domains.c_str (),
    sizeof (header.domain) - 1);

  // get the quality of the key
  header.quality = Madara::Utility::endian_swap (quality);

  // copy the message originator (our id)
  strncpy (header.originator, id_.c_str (), sizeof (header.originator) - 1);

  // only 1 update in a send_data message
  header.updates = updates.size ();

  // send data is generally an assign type. However, Message_Header is
  // flexible enough to support both, and this will simply our read thread
  // handling
  header.type = Madara::Transport::MULTIASSIGN;
  
  // compute size of this header
  header.size = header.encoded_size ();

  // set the update to the end of the header
  char * update = header.write (buffer, buffer_remaining);
  uint64_t * message_size = (uint64_t *)buffer;
  
  // Message header format
  // [size|id|domain|originator|type|updates|quality|clock|list of updates]

  // zero out the memory
  //memset(buffer, 0, Madara::Transport::MAX_PACKET_SIZE);

  // Message update format
  // [key|value]
  
  int j = 0;
  for (Knowledge_Records::const_iterator i = updates.begin ();
    i != updates.end (); ++i, ++j)
  {
    update = i->second->write (update, i->first, buffer_remaining);
    
    if (buffer_remaining > 0)
    {
      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "Broadcast_Transport::send_data:" \
        " update[%d] => %s=%s\n",
        j, i->first.c_str (), i->second->to_string ().c_str ()));
    }
    else
    {
    MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_DEBUG, 
      DLINFO "Broadcast_Transport::send_data:" \
      " unable to send due to overflow in buffer for update[%d] => %s=%s\n",
      j, i->first.c_str (), i->second->to_string ().c_str ()));
    }
  }
  
  if (buffer_remaining > 0)
  {
    int size = (int)(Madara::Transport::MAX_PACKET_SIZE - buffer_remaining);
    *message_size = Madara::Utility::endian_swap ((uint64_t)size);

    // send the buffer contents to the multicast address
  
    if (addresses_.size () > 0)
    {
      int bytes_sent = socket_.send(
        buffer, size, addresses_[0]);

      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Broadcast_Transport::send_data:" \
        " Sent packet with size %d\n",
        bytes_sent));
    }
  }

  return 0;
}