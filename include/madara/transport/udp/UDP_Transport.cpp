#include "madara/transport/udp/UDP_Transport.h"
#include "madara/transport/udp/UDP_Transport_Read_Thread.h"
#include "madara/utility/Log_Macros.h"
#include "madara/knowledge_engine/Knowledge_Record.h"
#include "madara/transport/Reduced_Message_Header.h"
#include "madara/utility/Utility.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram.h"


#include <iostream>

Madara::Transport::UDP_Transport::UDP_Transport (const std::string & id,
        Madara::Knowledge_Engine::Thread_Safe_Context & context, 
        Settings & config, bool launch_transport)
: Base (config, context),
  id_ (id), thread_ (0), valid_setup_ (false)
{
  if (launch_transport)
    setup ();
}

Madara::Transport::UDP_Transport::~UDP_Transport ()
{
  close ();
}

void
Madara::Transport::UDP_Transport::close (void)
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
Madara::Transport::UDP_Transport::reliability (void) const
{
  return RELIABLE;
}

int
Madara::Transport::UDP_Transport::reliability (const int &)
{
  return RELIABLE;
}

int
Madara::Transport::UDP_Transport::setup (void)
{
  if (settings_.hosts_.size () > 0)
  {
    // check for an on_data_received ruleset
    if (settings_.on_data_received_logic.length () != 0)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "UDP_Transport::UDP_Transport:" \
        " setting rules to %s\n", 
        settings_.on_data_received_logic.c_str ()));

      Madara::Expression_Tree::Interpreter interpreter;
      on_data_received_ = interpreter.interpret (context_,
        settings_.on_data_received_logic);
    }
    else
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "UDP_Transport::UDP_Transport:" \
        " no permanent rules were set\n"));
    }
  
    // setup the send buffer
    if (settings_.queue_length > 0)
      buffer_ = new char [settings_.queue_length];

    for (size_t i = 0; i < settings_.hosts_.size (); ++i)
    {
      addresses_[settings_.hosts_[i]].set (settings_.hosts_[i].c_str ());
    }

    ACE_INET_Addr & address_ref = addresses_[settings_.hosts_[0].c_str ()];

    thread_ = new Madara::Transport::UDP_Transport_Read_Thread (
      settings_, id_, context_, address_ref);
  
      // open the broadcast socket to any port for sending
      if (socket_.open (ACE_Addr::sap_any) == -1)
        std::cout << "UDP socket failed to open\n";

  }

  return this->validate_transport ();
}

long
Madara::Transport::UDP_Transport::send_data (
  const Madara::Knowledge_Records & updates)
{
  // check to see if we are shutting down
  long ret = this->check_transport ();
  if (-1 == ret)
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "UDP_Transport::send_data: transport has been told to shutdown")); 
    return ret;
  }
  else if (-2 == ret)
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "UDP_Transport::send_data: transport is not valid")); 
    return ret;
  }

  uint32_t quality = Madara::max_quality (updates);
  
  bool reduced = false;
  
  // allocate a buffer to send
  char * buffer = buffer_.get_ptr ();
  int64_t buffer_remaining = settings_.queue_length;
  
  if (buffer == 0)
  {
    MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_DEBUG, 
      DLINFO "UDP_Transport::send_data:" \
      " Unable to allocate buffer of size %d. Exiting thread.\n",
      settings_.queue_length));
    
    return -3;
  }

  
  // set the header to the beginning of the buffer
  Message_Header * header = 0;

  if (settings_.send_reduced_message_header)
  {
    MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
      DLINFO "UDP_Transport::send_data:" \
      " Preparing message with reduced message header->\n"));
    header = new Reduced_Message_Header ();
    reduced = true;
  }
  else
  {
    MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
      DLINFO "UDP_Transport::send_data:" \
      " Preparing message with normal message header->\n"));
    header = new Message_Header ();
  }

  // get the clock
  header->clock = Madara::Utility::endian_swap (context_.get_clock ());

  if (!reduced)
  {
    // copy the domain from settings
    strncpy (header->domain, this->settings_.domains.c_str (),
      sizeof (header->domain) - 1);

    // get the quality of the key
    header->quality = Madara::Utility::endian_swap (quality);

    // copy the message originator (our id)
    strncpy (header->originator, id_.c_str (), sizeof (header->originator) - 1);

    // send data is generally an assign type. However, Message_Header is
    // flexible enough to support both, and this will simply our read thread
    // handling
    header->type = Madara::Transport::MULTIASSIGN;
  }

  // only 1 update in a send_data message
  header->updates = uint32_t (updates.size ());

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
  for (Knowledge_Records::const_iterator i = updates.begin ();
    i != updates.end (); ++i, ++j)
  {
    update = i->second->write (update, i->first, buffer_remaining);
    
    if (buffer_remaining > 0)
    {
      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "UDP_Transport::send_data:" \
        " update[%d] => %s=%s\n",
        j, i->first.c_str (), i->second->to_string ().c_str ()));
    }
    else
    {
    MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_DEBUG, 
      DLINFO "UDP_Transport::send_data:" \
      " unable to send due to overflow in buffer for update[%d] => %s=%s\n",
      j, i->first.c_str (), i->second->to_string ().c_str ()));
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
        DLINFO "UDP_Transport::send_data:" \
        " evaluating rules in %s\n", 
        settings_.on_data_received_logic.c_str ()));

      on_data_received_.evaluate ();

      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "UDP_Transport::send_data:" \
        " rules have been successfully evaluated\n"));
    }
    else
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "UDP_Transport::send_data:" \
        " no permanent rules were set\n"));
    }

    for (std::map <std::string, ACE_INET_Addr>::const_iterator i =
           addresses_.begin (); i != addresses_.end (); ++i)
    {
      ssize_t bytes_sent = socket_.send (buffer, (ssize_t)size, i->second);
      
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "UDP_Transport::send_data:" \
        " Sent packet to %s with size %d\n",
        i->first.c_str (), bytes_sent));
    }
  }
  
  delete header;
  return 0;
}
