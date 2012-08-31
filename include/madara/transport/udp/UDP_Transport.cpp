#include "madara/transport/udp/UDP_Transport.h"
#include "madara/transport/udp/UDP_Transport_Read_Thread.h"
#include "madara/utility/Log_Macros.h"

#include <iostream>

Madara::Transport::UDP_Transport::UDP_Transport (const std::string & id,
        Madara::Knowledge_Engine::Thread_Safe_Context & context, 
        Settings & config, bool launch_transport)
: Base (config),
  id_ (id), context_ (context), thread_ (0), valid_setup_ (false)
{
  this->validate_transport ();
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
  }

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
  thread_ = new Madara::Transport::UDP_Transport_Read_Thread (id_, context_);
  
  return this->validate_transport ();
}

long
Madara::Transport::UDP_Transport::send_data (const std::string & key, 
                                               const long long & value)
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
  
  /// get current lamport clock. 
  unsigned long long cur_clock = context_.get_clock ();

  

  return 0;
}

long
Madara::Transport::UDP_Transport::send_multiassignment (const std::string & expression, unsigned long quality)
{
  // check to see if we are shutting down
  long ret = this->check_transport ();
  if (-1 == ret)
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Splice_DDS_Transport::send_multiassignment: transport is shutting down")); 
    return ret;
  }
  else if (-2 == ret)
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Splice_DDS_Transport::send_multiassignment: transport is not valid")); 
    return ret;
  }

  // get current lamport clock. 
  unsigned long long cur_clock = context_.get_clock ();

  return 0;
}