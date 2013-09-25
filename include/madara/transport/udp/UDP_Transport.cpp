#include "madara/transport/udp/UDP_Transport.h"
#include "madara/transport/udp/UDP_Transport_Read_Thread.h"
#include "madara/transport/Transport_Context.h"
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
  : Base (id, config, context),
  thread_ (0), valid_setup_ (false)
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
  // call base setup method to initialize certain common variables
  Base::setup ();

  if (settings_.hosts_.size () > 0)
  {
    for (size_t i = 0; i < settings_.hosts_.size (); ++i)
    {
      addresses_[settings_.hosts_[i]].set (settings_.hosts_[i].c_str ());
    }

    // open the broadcast socket to any port for sending
    if (socket_.open (ACE_Addr::sap_any) == -1)
      std::cout << "UDP socket failed to open\n";

    int send_buff_size = 0, tar_buff_size (settings_.queue_length);
    int rcv_buff_size = 0;
    int opt_len = sizeof (int);

    socket_.get_option (SOL_SOCKET, SO_SNDBUF,
      (void *)&send_buff_size, &opt_len);

    socket_.get_option (SOL_SOCKET, SO_RCVBUF,
      (void *)&rcv_buff_size, &opt_len);

    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "UDP_Transport::UDP_Transport:" \
      " default socket buff size is send=%d, rcv=%d\n",
      send_buff_size, rcv_buff_size));

    if (send_buff_size < tar_buff_size)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "UDP_Transport::UDP_Transport:" \
        " setting send buff size to settings.queue_length (%d)\n",
        tar_buff_size));

      socket_.set_option (SOL_SOCKET, SO_SNDBUF,
        (void *)&tar_buff_size, opt_len);

      socket_.get_option (SOL_SOCKET, SO_SNDBUF,
        (void *)&send_buff_size, &opt_len);

      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "UDP_Transport::UDP_Transport:" \
        " current socket buff size is send=%d, rcv=%d\n",
        send_buff_size, rcv_buff_size));
    }

    if (rcv_buff_size < tar_buff_size)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO
        "UDP_Transport::UDP_Transport:" \
        " setting rcv buff size to settings.queue_length (%d)\n",
        tar_buff_size));

      socket_.set_option (SOL_SOCKET, SO_SNDBUF,
        (void *)&tar_buff_size, opt_len);

      socket_.get_option (SOL_SOCKET, SO_SNDBUF,
        (void *)&rcv_buff_size, &opt_len);

      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO
        "UDP_Transport::UDP_Transport:" \
        " current socket buff size is send=%d, rcv=%d\n",
        send_buff_size, rcv_buff_size));
    }

    thread_ = new Madara::Transport::UDP_Transport_Read_Thread (
      settings_, id_, context_, addresses_, socket_,
      send_monitor_, receive_monitor_);

  }

  return this->validate_transport ();
}

long
  Madara::Transport::UDP_Transport::send_data (
  const Madara::Knowledge_Records & orig_updates)
{
  long result =
    prep_send (orig_updates, "UDP_Transport::send_data:");

  for (std::map <std::string, ACE_INET_Addr>::const_iterator i =
    addresses_.begin (); i != addresses_.end (); ++i)
  {
    ssize_t bytes_sent = socket_.send (buffer_.get_ptr (),
      (ssize_t)result, i->second);

    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "UDP_Transport::send_data:" \
      " Sent packet to %s with size %d\n",
      i->first.c_str (), bytes_sent));

    send_monitor_.add ((uint32_t)bytes_sent);

    result += (long) bytes_sent;
  }

  MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
    DLINFO "UDP_Transport::send_data:" \
    " Send bandwidth = %d B/s\n",
    send_monitor_.get_bytes_per_second ()));

  return result;
}
