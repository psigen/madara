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
  
  uint32_t quality = Madara::max_quality (updates);

  // compute size of this message
  unsigned int size = sizeof (Message_Header) + updates.size () * 
                (MAX_KNOWLEDGE_KEY_LENGTH + sizeof (KNOWLEDGE_VALUE_TYPE));

  // allocate a buffer to send
  char * buffer = (char *) malloc (size);

  // set the header to the beginning of the buffer
  Message_Header * header = (Message_Header *) buffer;

  // set the update to the end of the header
  Message_Update * update = (Message_Update *) (buffer + sizeof (Message_Header));
  
  // Message header format
  // [size|id|domain|originator|type|updates|quality|clock|list of updates]

  // zero out the memory
  memset(buffer, 0, size);

  // get the clock
  header->clock = Madara::Utility::endian_swap (context_.get_clock ());

  // copy the Madara transport version identification
  strncpy (header->madara_id, MADARA_IDENTIFIER, 7);

  // copy the domain from settings
  strncpy (header->domain, this->settings_.domains.c_str (),
    sizeof (header->domain) - 1);

  // get the quality of the key
  header->quality = Madara::Utility::endian_swap (quality);

  // copy the message originator (our id)
  strncpy (header->originator, id_.c_str (), sizeof (header->originator) - 1);

  // only 1 update in a send_data message
  header->updates = updates.size ();

  // send data is generally an assign type. However, Message_Header is
  // flexible enough to support both, and this will simply our read thread
  // handling
  header->type = Madara::Transport::MULTIASSIGN;
  header->type = Madara::Utility::endian_swap (header->type);
  
  // compute size of this message
  header->size = sizeof (Message_Header) + 
                 header->updates * sizeof (Message_Update);
  header->size = Madara::Utility::endian_swap (header->size);
  
  // little endianize the updates after we've used it for the last time
  header->updates = Madara::Utility::endian_swap (header->updates);

  // Message update format
  // [key|value]

  for (Knowledge_Records::const_iterator i = updates.begin ();
    i != updates.end (); ++i, ++update)
  {
    // copy the key and value over
    strncpy (update->key, i->first.c_str (),
      MAX_KNOWLEDGE_KEY_LENGTH - 1);
    
    update->value = i->second->value;

    MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
      DLINFO "Multicast_Transport::send_multiassignment:" \
      " update[%d] => %s=%d\n",
      i, update->key, update->value));

    // little endianize the value
    update->value = Madara::Utility::endian_swap (update->value);
  }

  // send the buffer contents to the multicast address
  
  if (addresses_.size () > 0)
  {
    int bytes_sent = socket_.send (
      buffer, size, addresses_[0]);

    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Broadcast_Transport::send_multiassignment:" \
      " Sent packet with size %d\n",
      bytes_sent));
  }

  // free the buffer we allocated
  free (buffer);

  return 0;
}