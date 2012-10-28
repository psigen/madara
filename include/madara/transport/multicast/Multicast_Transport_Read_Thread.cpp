#include "madara/transport/multicast/Multicast_Transport_Read_Thread.h"
#include "madara/utility/Log_Macros.h"
#include "madara/transport/Message_Header.h"
#include "ace/Time_Value.h"

#include <iostream>
#include <algorithm>

Madara::Transport::Multicast_Transport_Read_Thread::Multicast_Transport_Read_Thread (
  const std::string & id,
  Madara::Knowledge_Engine::Thread_Safe_Context & context,
  const ACE_INET_Addr & address)
  : id_ (id), context_ (context),
    barrier_ (2), terminated_ (false), 
    mutex_ (), is_not_ready_ (mutex_), is_ready_ (false), address_ (address)
{
  // Subscribe
  int port = address_.get_port_number ();
  const char * host = address_.get_host_addr ();

  if (-1 == socket_.join (address_, 1))
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Multicast_Transport_Read_Thread::Multicast_Transport_Read_Thread:" \
      " Error subscribing to multicast address %s:%d\n", host, port));
  } 
  else
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Multicast_Transport_Read_Thread::Multicast_Transport_Read_Thread:" \
      " Success subscribing to multicast address %s:%d\n", host, port));
  }

  this->activate (THR_NEW_LWP | THR_DETACHED, 1);

  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
    DLINFO "Multicast_Transport_Read_Thread::Multicast_Transport_Read_Thread:" \
    " read thread started\n"));
}

Madara::Transport::Multicast_Transport_Read_Thread::~Multicast_Transport_Read_Thread ()
{
  // Unsubscribe
  if (-1 == socket_.leave (address_))
  { 
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Multicast_Transport_Read_Thread::close:" \
      " Error unsubscribing to multicast address\n"));
  }

  socket_.close ();
}

int
Madara::Transport::Multicast_Transport_Read_Thread::close (void)
{
  terminated_ = true;

  enter_barrier ();

  return 0;
}

void
Madara::Transport::Multicast_Transport_Read_Thread::wait_for_ready (void)
{
  if (!is_ready_)
    is_not_ready_.wait ();
}

int Madara::Transport::Multicast_Transport_Read_Thread::enter_barrier (void)
{
  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
    DLINFO "Multicast_Transport_Read_Thread::enter_barrier:" \
    " process requesting shutdown\n"));

  barrier_.wait ();
  return 0;
}


int
Madara::Transport::Multicast_Transport_Read_Thread::svc (void)
{
  ACE_Time_Value wait_time (1);
  ACE_INET_Addr  remote;
  
  // specify a 131k packet size limit. This is unlikely to ever be used
  char buffer[MAX_PACKET_SIZE];

  while (false == terminated_.value ())
  {
    // read the message header
    Message_Header * header;
    int bytes_read = socket_.recv ((void *)buffer, 
      sizeof (buffer), remote, 0, &wait_time);
    
    header = (Message_Header *) buffer;

    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Multicast_Transport_Read_Thread::svc:" \
      " received a message header of %d bytes for %d updates from %s:%d\n",
      bytes_read, header->updates,
      remote.get_host_addr (), remote.get_port_number ()));

    if (bytes_read > 0)
    {
      // reject the message if it is not KaRL
      if (strncmp (header->madara_id, "KaRL", 4) != 0)
      {
        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "Multicast_Transport_Read_Thread::svc:" \
          " dropping non-KaRL message from %s:%d\n",
          remote.get_host_addr (), remote.get_port_number ()));
        continue;
      }
      else
      {
        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "Multicast_Transport_Read_Thread::svc:" \
          " processing KaRL message from %s:%d\n",
          remote.get_host_addr (), remote.get_port_number ()));
      }
    
      // reject the message if it is us as the originator (no update necessary)
      if (strncmp (header->originator, id_.c_str (),
           std::min (sizeof (header->originator), id_.size ())) == 0)
      {
        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "Multicast_Transport_Read_Thread::svc:" \
          " dropping message from ourself\n",
          remote.get_host_addr (), remote.get_port_number ()));
        continue;
      }
      else
      {
        MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
          DLINFO "Multicast_Transport_Read_Thread::svc:" \
          " remote id (%s:%d) is not our own\n",
          remote.get_host_addr (), remote.get_port_number ()));
      }

      // start of updates is right after message header
      Message_Update * update = (Message_Update *)
                                (buffer + sizeof (Message_Header));
      
      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "Multicast_Transport_Read_Thread::svc:" \
        " iterating over the %d updates\n",
        header->updates));
      
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Multicast_Transport_Read_Thread::svc:" \
        " locking context\n", update->key, update->value));

      // lock the context
      context_.lock ();
      
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Multicast_Transport_Read_Thread::svc:" \
        " past the lock\n", update->key, update->value));

      // iterate over the updates
      for (unsigned int i = 0; i < header->updates; ++i, ++update)
      {
        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "Multicast_Transport_Read_Thread::svc:" \
          " attempting to apply %s=%q\n", update->key, update->value));

        int result = update->apply (context_, header->quality,
          header->clock, false);

        if (result != 1)
        {
          MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
            DLINFO "Multicast_Transport_Read_Thread::svc:" \
            " update %s=%q was rejected\n", update->key, update->value));
        }
        else
        {
          MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
            DLINFO "Multicast_Transport_Read_Thread::svc:" \
            " update %s=%q was accepted\n", update->key, update->value));
        }
      }

      // unlock the context
      context_.unlock ();
      context_.set_changed ();
    }
    else
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Multicast_Transport_Read_Thread::svc:" \
        " wait timeout on new messages. Proceeding to next wait\n"));
    }
  }
  
  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
    DLINFO "Multicast_Transport_Read_Thread::svc:" \
    " entering barrier.\n"));

  barrier_.wait ();
  return 0;
}