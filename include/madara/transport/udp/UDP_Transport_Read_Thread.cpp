#include "madara/transport/udp/UDP_Transport_Read_Thread.h"
#include "madara/utility/Log_Macros.h"
#include "madara/utility/Utility.h"
#include "madara/transport/Reduced_Message_Header.h"
#include "ace/Time_Value.h"

#include <iostream>

Madara::Transport::UDP_Transport_Read_Thread::UDP_Transport_Read_Thread (
  const Settings & settings,
  const std::string & id,
  Madara::Knowledge_Engine::Thread_Safe_Context & context,
  std::map <std::string, ACE_INET_Addr> & addresses,
  ACE_SOCK_Dgram & socket,
  Knowledge_Engine::Bandwidth_Monitor & send_monitor,
  Knowledge_Engine::Bandwidth_Monitor & receive_monitor)
  : settings_ (settings), id_ (id), context_ (context),
    barrier_ (2), terminated_ (false), 
    mutex_ (), is_not_ready_ (mutex_), is_ready_ (false),
    addresses_ (addresses),
    write_socket_ (socket),
    send_monitor_ (send_monitor),
    receive_monitor_ (receive_monitor)

{
  if (addresses_.size () > 0)
  {


    // for receiving, we only want to bind to the local port
    ACE_INET_Addr local (addresses_[settings_.hosts_[0]].get_port_number ());
  
    qos_settings_ = dynamic_cast <const QoS_Transport_Settings *> (&settings);

    if (-1 == read_socket_.open (local, 2, 0, 0))
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "UDP_Transport_Read_Thread::UDP_Transport_Read_Thread:" \
        " Error subscribing to udp address %s:%d\n",
        local.get_host_addr (), local.get_port_number ()));
    } 
    else
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "UDP_Transport_Read_Thread::UDP_Transport_Read_Thread:" \
        " Success subscribing to broadcast address %s:%d\n",
        local.get_host_addr (), local.get_port_number ()));
    }
  
    // check for an on_data_received ruleset
    if (settings_.on_data_received_logic.length () != 0)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "UDP_Transport_Read_Thread::UDP_Transport_Read_Thread:" \
        " setting rules to %s\n", 
        settings_.on_data_received_logic.c_str ()));

      Madara::Expression_Tree::Interpreter interpreter;
      on_data_received_ = interpreter.interpret (context_,
        settings_.on_data_received_logic);
    }
    else
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "UDP_Transport_Read_Thread::UDP_Transport_Read_Thread:" \
        " no permanent rules were set\n"));
    }
  
    // setup the receive buffer
    if (settings_.queue_length > 0)
      buffer_ = new char [settings_.queue_length];

    this->activate (THR_NEW_LWP | THR_DETACHED, 1);

    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "UDP_Transport_Read_Thread::UDP_Transport_Read_Thread:" \
      " read thread started\n"));
  }
}

Madara::Transport::UDP_Transport_Read_Thread::~UDP_Transport_Read_Thread ()
{
  // close broadcast port
  if (-1 == read_socket_.close ())
  { 
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "UDP_Transport_Read_Thread::close:" \
      " Error closing datagram socket\n"));
  }
}

int
Madara::Transport::UDP_Transport_Read_Thread::close (void)
{
  terminated_ = true;
  enter_barrier ();

  return 0;
}

void
Madara::Transport::UDP_Transport_Read_Thread::wait_for_ready (void)
{
  if (!is_ready_)
    is_not_ready_.wait ();
}

int Madara::Transport::UDP_Transport_Read_Thread::enter_barrier (void)
{
  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
    DLINFO "UDP_Transport_Read_Thread::enter_barrier:" \
    " beginning thread barrier for shut down of read thread\n"));

  barrier_.wait ();
  return 0;
}


void
Madara::Transport::UDP_Transport_Read_Thread::rebroadcast (
  Message_Header * header,
  const Knowledge_Map & records)
{
  if (header->ttl > 0 && records.size () > 0)
  {
    char * buffer = buffer_.get_ptr ();
    int64_t buffer_remaining = settings_.queue_length;
  
    // keep track of the message_size portion of buffer
    uint64_t * message_size = (uint64_t *)buffer;

    // the number of updates will be the size of the records map
    header->updates = uint32_t (records.size ());

    // set the update to the end of the header
    char * update = header->write (buffer, buffer_remaining);

    for (Knowledge_Map::const_iterator i = records.begin ();
         i != records.end (); ++i)
    {
      update = i->second.write (update, i->first, buffer_remaining);
    }
    
    if (buffer_remaining > 0)
    {
      int size = (int)(settings_.queue_length - buffer_remaining);
      *message_size = Madara::Utility::endian_swap ((uint64_t)size);
    
      for (std::map <std::string, ACE_INET_Addr>::const_iterator i =
             addresses_.begin (); i != addresses_.end (); ++i)
      {
        ssize_t bytes_sent = write_socket_.send (buffer, (ssize_t)size, i->second);
      
        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "UDP_Transport_Read_Thread::rebroadcast:" \
          " Sent packet to %s with size %d\n",
          i->first.c_str (), bytes_sent));

        send_monitor_.add ((uint32_t)bytes_sent);
      }
      
      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "UDP_Transport_Read_Thread::rebroadcast:" \
        " Send bandwidth = %d B/s\n",
        send_monitor_.get_bytes_per_second ()));
    }
    else
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "UDP_Transport_Read_Thread::rebroadcast:" \
        " Not enough buffer for rebroadcasting packet\n"));
    }
  }
  else
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "UDP_Transport_Read_Thread::rebroadcast:" \
      " No rebroadcast necessary.\n",
      settings_.queue_length));
  }
}


int
Madara::Transport::UDP_Transport_Read_Thread::svc (void)
{
  ACE_Time_Value wait_time (1);
  ACE_INET_Addr  remote;
  
  // allocate a buffer to send
  char * buffer = buffer_.get_ptr ();
  int64_t buffer_remaining = settings_.queue_length;

  while (false == terminated_.value ())
  {
    Knowledge_Map rebroadcast_records;

    if (buffer == 0)
    {
      MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_DEBUG, 
        DLINFO "UDP_Transport_Read_Thread::svc:" \
        " Unable to allocate buffer of size %d. Exiting thread.\n",
        settings_.queue_length));
    
      break;
    }

    // read the message
    ssize_t bytes_read = read_socket_.recv ((void *)buffer, 
      settings_.queue_length, remote, 0, &wait_time);
 

    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "UDP_Transport_Read_Thread::svc:" \
      " received a message header of %d bytes from %s:%d\n",
      bytes_read,
      remote.get_host_addr (), remote.get_port_number ()));
    
    if (bytes_read > 0)
    {
      // tell the receive bandwidth monitor about the transaction
      receive_monitor_.add (bytes_read);
      
      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "UDP_Transport_Read_Thread::rebroadcast:" \
        " Receive bandwidth = %d B/s\n",
        receive_monitor_.get_bytes_per_second ()));

      buffer_remaining = (int64_t)bytes_read;
      bool is_reduced = false;
      Message_Header * header;

      // check the buffer for a reduced message header
      if (Reduced_Message_Header::reduced_message_header_test (buffer))
      {
        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "UDP_Transport_Read_Thread::svc:" \
          " processing reduced KaRL message from %s:%d\n",
          remote.get_host_addr (), remote.get_port_number ()));

        header = new Reduced_Message_Header ();
        is_reduced = true;
      }
      else if (Message_Header::message_header_test (buffer))
      {
        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "UDP_Transport_Read_Thread::svc:" \
          " processing KaRL message from %s:%d\n",
          remote.get_host_addr (), remote.get_port_number ()));
        
        header = new Message_Header ();
      }
      else
      {
        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "UDP_Transport_Read_Thread::svc:" \
          " dropping non-KaRL message from %s:%d\n",
          remote.get_host_addr (), remote.get_port_number ()));
        continue;
      }
          
      char * update = header->read (buffer, buffer_remaining);

      if (!is_reduced)
      {
        // reject the message if it is us as the originator (no update necessary)
        if (strncmp (header->originator, id_.c_str (),
          std::min (sizeof (header->originator), id_.size ())) == 0)
        {
          MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
            DLINFO "UDP_Transport_Read_Thread::svc:" \
            " dropping message from ourself\n",
            remote.get_host_addr (), remote.get_port_number ()));

          // delete the header and continue to the svc loop
          delete header;
          continue;
        }
        else
        {
          MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
            DLINFO "UDP_Transport_Read_Thread::svc:" \
            " remote id (%s:%d) is not our own\n",
            remote.get_host_addr (), remote.get_port_number ()));
        }

        std::stringstream remote_buffer;
        remote_buffer << remote.get_host_addr ();
        remote_buffer << ":";
        remote_buffer << remote.get_port_number ();

        if (!qos_settings_ || qos_settings_->is_trusted (remote_buffer.str ()))
        {
          MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
            DLINFO "UDP_Transport_Read_Thread::svc:" \
            " remote id (%s:%d) is trusted\n",
            remote.get_host_addr (), remote.get_port_number ()));
        }
        else
        {
          MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
            DLINFO "UDP_Transport_Read_Thread::svc:" \
            " dropping message from untrusted peer (%s:%d)\n",
            remote.get_host_addr (), remote.get_port_number ()));

          // delete the header and continue to the svc loop
          delete header;
          continue;
        }

        std::string originator (header->originator);
        
        if (!qos_settings_ || qos_settings_->is_trusted (originator))
        {
          MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
            DLINFO "UDP_Transport_Read_Thread::svc:" \
            " remote id (%s) is trusted\n",
            originator.c_str ()));
        }
        else
        {
          MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
            DLINFO "UDP_Transport_Read_Thread::svc:" \
            " dropping message from untrusted peer (%s)\n",
            originator.c_str ()));

          // delete the header and continue to the svc loop
          delete header;
          continue;
        }



        // reject the message if it is from a different domain
        if (strncmp (header->domain, settings_.domains.c_str (),
          std::min (sizeof (header->domain), settings_.domains.size ())) != 0)
        {
          MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
            DLINFO "UDP_Transport_Read_Thread::svc:" \
            " remote id (%s:%d) in a different domain (%s). Dropping message.\n",
            remote.get_host_addr (), remote.get_port_number (),
            header->domain));

          // delete the header and continue to the svc loop
          delete header;
          continue;
        }
        else
        {
          MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
            DLINFO "UDP_Transport_Read_Thread::svc:" \
            " remote id (%s:%d) message is in our domain\n",
            remote.get_host_addr (), remote.get_port_number ()));
        }
      }
      
      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "UDP_Transport_Read_Thread::svc:" \
        " iterating over the %d updates\n",
        header->updates));
      
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "UDP_Transport_Read_Thread::svc:" \
        " locking context\n"));
      
      // temporary record for reading from the updates buffer
      Knowledge_Record record;
      record.quality = header->quality;
      record.clock = header->clock;
      std::string key;

      // lock the context
      context_.lock ();
      
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "UDP_Transport_Read_Thread::svc:" \
        " past the lock\n"));

      // iterate over the updates
      for (uint32_t i = 0; i < header->updates; ++i)
      {
        // read converts everything into host format from the update stream
        update = record.read (update, key, buffer_remaining);
        
        if (buffer_remaining < 0)
        {
          MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_DEBUG, 
            DLINFO "UDP_Transport_Read_Thread::svc:" \
            " unable to process message. Buffer remaining is negative." \
            " Server is likely being targeted by custom KaRL tools.\n"));

          // we do not delete the header as this will be cleaned up later
          break;
        }

        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "UDP_Transport_Read_Thread::svc:" \
          " attempting to apply %s=%s\n",
          key.c_str (), record.to_string ().c_str ()));

        uint32_t receive_bandwidth = receive_monitor_.get_bytes_per_second ();
        uint32_t send_bandwidth = send_monitor_.get_bytes_per_second ();

        Transport_Context transport_context;
        
        // if the tll is 1 or more and we are participating in rebroadcasts
        if (header->ttl > 0 && qos_settings_ &&
            qos_settings_->get_participant_ttl () > 0)
        {
          Knowledge_Record rebroadcast_temp (record);

          // if we have rebroadcast filters, apply them
          if (qos_settings_->get_number_of_rebroadcast_filtered_types () > 0)
          {
            transport_context.set_operation (
              Transport_Context::REBROADCASTING_OPERATION);
            transport_context.set_receive_bandwidth (receive_bandwidth);
            transport_context.set_send_bandwidth (send_bandwidth);
        
            rebroadcast_temp = qos_settings_->filter_rebroadcast (
              record, key,
              transport_context);
          }

          // if the record was not filtered out entirely, add it to map
          if (rebroadcast_temp.status () != Knowledge_Record::UNCREATED)
          {
            MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
              DLINFO "UDP_Transport_Read_Thread::svc:" \
              " update %s=%s is queued for rebroadcast\n",
              key.c_str (), rebroadcast_temp.to_string ().c_str ()));

            rebroadcast_records[key] = rebroadcast_temp;
          }
          else
          {
            MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
              DLINFO "UDP_Transport_Read_Thread::svc:" \
              " update %s was filtered out of rebroadcast\n",
              key.c_str ()));
          }
        }
        
        // use the original message to apply receive filters
        if (qos_settings_ &&
            qos_settings_->get_number_of_received_filtered_types () > 0)
        {
          transport_context.set_operation (Transport_Context::RECEIVING_OPERATION);
          transport_context.set_receive_bandwidth (receive_bandwidth);
          transport_context.set_send_bandwidth (send_bandwidth);
        
          record = qos_settings_->filter_receive (record, key,
            transport_context);
        }

        int result = 0;

        // if receive filter did not delete the update, apply it
        if (record.status () != Knowledge_Record::UNCREATED)
        {
          result = record.apply (context_, key, header->quality,
            header->clock, false);
        }

        if (result != 1)
        {
          MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
            DLINFO "UDP_Transport_Read_Thread::svc:" \
            " update %s=%s was rejected\n",
            key.c_str (), record.to_string ().c_str ()));
        }
        else
        {
          MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
            DLINFO "UDP_Transport_Read_Thread::svc:" \
            " update %s=%s was accepted\n",
            key.c_str (), record.to_string ().c_str ()));
        }
      }
      
      // before we send to others, we first execute rules
      if (settings_.on_data_received_logic.length () != 0)
      {
        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "UDP_Transport_Read_Thread::svc:" \
          " evaluating rules in %s\n", 
          settings_.on_data_received_logic.c_str ()));

        on_data_received_.evaluate ();
      }
      else
      {
        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "UDP_Transport_Read_Thread::svc:" \
          " no permanent rules were set\n"));
      }

      // unlock the context
      context_.unlock ();
      context_.set_changed ();
      
      if (header->ttl > 0 && rebroadcast_records.size () > 0 &&
          qos_settings_ && qos_settings_->get_participant_ttl () > 0)
      {
        --header->ttl;
        header->ttl = std::min (
          qos_settings_->get_participant_ttl (), header->ttl);

        rebroadcast (header, rebroadcast_records);
      }

      // delete header
      delete header;
    }
    else
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "UDP_Transport_Read_Thread::svc:" \
        " wait timeout on new messages. Proceeding to next wait\n"));
    }
  }
  
  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
    DLINFO "UDP_Transport_Read_Thread::svc:" \
    " entering barrier.\n"));

  barrier_.wait ();
  return 0;
}