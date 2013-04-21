#include "madara/transport/multicast/Multicast_Transport_Read_Thread.h"
#include "madara/utility/Log_Macros.h"
#include "madara/transport/Message_Header.h"
#include "ace/Time_Value.h"
#include "madara/utility/Utility.h"

#include <iostream>
#include <algorithm>

Madara::Transport::Multicast_Transport_Read_Thread::Multicast_Transport_Read_Thread (
  const Settings & settings, const std::string & id,
  Madara::Knowledge_Engine::Thread_Safe_Context & context,
  const ACE_INET_Addr & address)
  : settings_ (settings), id_ (id), context_ (context),
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

  // check for an on_data_received ruleset
  if (settings_.on_data_received_logic.length () != 0)
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Multicast_Transport_Read_Thread::Multicast_Transport_Read_Thread:" \
      " setting rules to %s\n", 
      settings_.on_data_received_logic.c_str ()));

    Madara::Expression_Tree::Interpreter interpreter;
    on_data_received_ = interpreter.interpret (context_,
      settings_.on_data_received_logic);
  }
  else
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Multicast_Transport_Read_Thread::Multicast_Transport_Read_Thread:" \
      " no permanent rules were set\n"));
  }
  
  // setup the receive buffer
  if (settings_.queue_length > 0)
    buffer_ = new char [settings_.queue_length];

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
  
  // allocate a buffer to send
  char * buffer = buffer_.get_ptr ();
  int64_t buffer_remaining = settings_.queue_length;

  while (false == terminated_.value ())
  {
    if (buffer == 0)
    {
      MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_DEBUG, 
        DLINFO "Multicast_Transport_Read_Thread::svc:" \
        " Unable to allocate buffer of size %d. Exiting thread.\n",
        settings_.queue_length));
    
      break;
    }

    // read the message
    ssize_t bytes_read = socket_.recv ((void *)buffer, 
      settings_.queue_length, remote, 0, &wait_time);
 

    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Multicast_Transport_Read_Thread::svc:" \
      " received a message header of %d bytes from %s:%d\n",
      bytes_read,
      remote.get_host_addr (), remote.get_port_number ()));
    
    if (bytes_read > 0)
    {
      buffer_remaining = (int64_t)bytes_read;
      Message_Header header;
      char * update = header.read (buffer, buffer_remaining);

      // reject the message if it is not KaRL
      if (strncmp (header.madara_id, "KaRL", 4) != 0)
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
      if (strncmp (header.originator, id_.c_str (),
           std::min (sizeof (header.originator), id_.size ())) == 0)
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
      
      // reject the message if it is from a different domain
      if (strncmp (header.domain, settings_.domains.c_str (),
           std::min (sizeof (header.domain), settings_.domains.size ())) != 0)
      {
        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "Multicast_Transport_Read_Thread::svc:" \
          " remote id (%s:%d) in a different domain (%s). Dropping message.\n",
          remote.get_host_addr (), remote.get_port_number (),
          header.domain));
        continue;
      }
      else
      {
        MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
          DLINFO "Multicast_Transport_Read_Thread::svc:" \
          " remote id (%s:%d) message is in our domain\n",
          remote.get_host_addr (), remote.get_port_number ()));
      }
      
      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "Multicast_Transport_Read_Thread::svc:" \
        " iterating over the %d updates\n",
        header.updates));
      
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Multicast_Transport_Read_Thread::svc:" \
        " locking context\n"));
      
      // temporary record for reading from the updates buffer
      Knowledge_Record record;
      record.quality = header.quality;
      record.clock = header.clock;
      std::string key;

      // lock the context
      context_.lock ();
      
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Multicast_Transport_Read_Thread::svc:" \
        " past the lock\n"));

      // iterate over the updates
      for (uint32_t i = 0; i < header.updates; ++i)
      {
        // read converts everything into host format from the update stream
        update = record.read (update, key, buffer_remaining);
        
        if (buffer_remaining < 0)
        {
          MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_DEBUG, 
            DLINFO "Multicast_Transport_Read_Thread::svc:" \
            " unable to process message. Buffer remaining is negative." \
            " Server is likely being targeted by custom KaRL tools.\n"));
          break;
        }

        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "Multicast_Transport_Read_Thread::svc:" \
          " attempting to apply %s=%s\n",
          key.c_str (), record.to_string ().c_str ()));
        
        int result = record.apply (context_, key, header.quality,
          header.clock, false);

        if (result != 1)
        {
          MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
            DLINFO "Multicast_Transport_Read_Thread::svc:" \
            " update %s=%s was rejected\n",
            key.c_str (), record.to_string ().c_str ()));
        }
        else
        {
          MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
            DLINFO "Multicast_Transport_Read_Thread::svc:" \
            " update %s=%s was accepted\n",
            key.c_str (), record.to_string ().c_str ()));
        }
      }
      
      // before we send to others, we first execute rules
      if (settings_.on_data_received_logic.length () != 0)
      {
        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "Multicast_Transport_Read_Thread::svc:" \
          " evaluating rules in %s\n", 
          settings_.on_data_received_logic.c_str ()));

        on_data_received_.evaluate ();
      }
      else
      {
        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "Multicast_Transport_Read_Thread::svc:" \
          " no permanent rules were set\n"));
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