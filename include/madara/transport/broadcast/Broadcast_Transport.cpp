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
  long result =
    prep_send (orig_updates, "Broadcast_Transport::send_data:");

  if (addresses_.size () > 0)
  {
    ssize_t bytes_sent = socket_.send(
      buffer_.get_ptr (), (ssize_t)result, addresses_[0]);

    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Broadcast_Transport::send_data:" \
      " Sent packet with size %d\n",
      bytes_sent));

    send_monitor_.add ((uint32_t)bytes_sent);
      
    MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
      DLINFO "Broadcast_Transport::send_data:" \
      " Send bandwidth = %d B/s\n",
      send_monitor_.get_bytes_per_second ()));

    result = (long) bytes_sent;
  }
  
  return result;
}