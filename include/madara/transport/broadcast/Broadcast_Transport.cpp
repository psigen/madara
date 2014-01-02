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
: Base (id, config, context),
  thread_ (0), valid_setup_ (false)
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
  // call base setup method to initialize certain common variables
  Base::setup ();

  // resize addresses to be the size of the list of hosts
  addresses_.resize (this->settings_.hosts.size ());
  
  if (addresses_.size () > 0)
  {
    // convert the string host:port into an ACE address
    for (unsigned int i = 0; i < addresses_.size (); ++i)
    {
      addresses_[i].set (settings_.hosts[i].c_str ());

      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Broadcast_Transport::Broadcast_Transport:" \
        " settings address[%d] to %s:%d\n", i, 
        addresses_[i].get_host_addr (), addresses_[i].get_port_number ()));
    }
    
    // open the broadcast socket to any port for sending
    if (socket_.open (ACE_Addr::sap_any) == -1)
      std::cout << "Broadcast socket failed to open\n";
    else
    {
      int send_buff_size = 0, tar_buff_size (settings_.queue_length);
      int rcv_buff_size = 0;
      int opt_len = sizeof (int);

      socket_.get_option (SOL_SOCKET, SO_SNDBUF,
        (void *)&send_buff_size, &opt_len);

      socket_.get_option (SOL_SOCKET, SO_RCVBUF,
        (void *)&rcv_buff_size, &opt_len);
  
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Broadcast_Transport::Broadcast_Transport:" \
        " default socket buff size is send=%d, rcv=%d\n",
        send_buff_size, rcv_buff_size));
  
      if (send_buff_size < tar_buff_size)
      {
        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "Broadcast_Transport::Broadcast_Transport:" \
          " setting send buff size to settings.queue_length (%d)\n",
          tar_buff_size));
  
        socket_.set_option (SOL_SOCKET, SO_SNDBUF,
          (void *)&tar_buff_size, opt_len);
    
        socket_.get_option (SOL_SOCKET, SO_SNDBUF,
          (void *)&send_buff_size, &opt_len);
    
        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "Broadcast_Transport::Broadcast_Transport:" \
          " current socket buff size is send=%d, rcv=%d\n",
          send_buff_size, rcv_buff_size));
      }
  
      if (rcv_buff_size < tar_buff_size)
      {
        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO
          "Broadcast_Transport::Broadcast_Transport:" \
          " setting rcv buff size to settings.queue_length (%d)\n",
          tar_buff_size));
  
        socket_.set_option (SOL_SOCKET, SO_SNDBUF,
          (void *)&tar_buff_size, opt_len);
    
        socket_.get_option (SOL_SOCKET, SO_SNDBUF,
          (void *)&rcv_buff_size, &opt_len);
    
        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO
          "Broadcast_Transport::Broadcast_Transport:" \
          " current socket buff size is send=%d, rcv=%d\n",
          send_buff_size, rcv_buff_size));
      }

      // start thread with the addresses (only looks at the first one for now)
      thread_ = new Madara::Transport::Broadcast_Transport_Read_Thread (
                      settings_, id_, context_, addresses_[0], socket_,
                      send_monitor_, receive_monitor_, packet_scheduler_);
    }
    
  }
  return this->validate_transport ();
}

long
Madara::Transport::Broadcast_Transport::send_data (
  const Madara::Knowledge_Records & orig_updates)
{
  const char * print_prefix = "Broadcast_Transport::send_data";

  long result = prep_send (orig_updates, print_prefix);

  if (addresses_.size () > 0 && result > 0)
  {
    uint64_t bytes_sent = 0;
    uint64_t packet_size = Message_Header::get_size (buffer_.get_ptr ());

    if (packet_size > settings_.max_fragment_size)
    {
      Fragment_Map map;
      
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "%s:" \
        " fragmenting %Q byte packet (%d bytes is max fragment size)\n",
        print_prefix, packet_size, settings_.max_fragment_size));

      // fragment the message
      frag (buffer_.get_ptr (), settings_.max_fragment_size, map);

      for (Fragment_Map::iterator i = map.begin (); i != map.end (); ++i)
      {
        // send the fragment
        bytes_sent += socket_.send(
          i->second,
          (ssize_t)Message_Header::get_size (i->second),
          addresses_[0]);

#ifdef WIN32
        /**
         * Broadcast on windows appears to be keyed by a timestamp. Because
         * Windows resolution is microseconds and not nanoseconds, this means
         * we have to sleep slightly or the OS will drop every fragment but
         * the last one in that resolution. I can't find any documentation of
         * this "feature", but sleeping for 10us before sending another 
         * fragment seems to fix the issue.
         **/
        Madara::Utility::sleep (0.00001);
#endif
      }
      
      send_monitor_.add ((uint32_t)bytes_sent);

      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "%s:" \
        " Sent fragments totalling %Q bytes\n",
        print_prefix, bytes_sent));

      delete_fragments (map);
    }
    else
    {
      bytes_sent = socket_.send(
        buffer_.get_ptr (), (ssize_t)result, addresses_[0]);
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "%s:" \
        " Sent packet of size %Q\n",
        print_prefix, bytes_sent));
      send_monitor_.add ((uint32_t)bytes_sent);
    }


    MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
      DLINFO "%s:" \
      " Send bandwidth = %d B/s\n",
      print_prefix, send_monitor_.get_bytes_per_second ()));

    result = (long) bytes_sent;
  }
  
  return result;
}