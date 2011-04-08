#include "madara/transport/TCP_Transport.h"
#include "madara/transport/TCP_Transport_Read_Thread.h"
#include "madara/utility/Log_Macros.h"

#include <iostream>

Madara::Transport::TCP_Transport::TCP_Transport (
  const std::string & id,
  Madara::Knowledge_Engine::Thread_Safe_Context & context, const int & reliability)
: id_ (id), context_ (context), thread_ (0), valid_setup_ (false)
{
  this->setup ();
}

Madara::Transport::TCP_Transport::~TCP_Transport ()
{
  this->close ();
}

void
Madara::Transport::TCP_Transport::close (void)
{
  Madara::Transport::Base::close ();

  if (thread_)
  {
    thread_->close ();
    delete thread_;
  }
}

int
Madara::Transport::TCP_Transport::reliability (void) const
{
  return RELIABLE;
}

int
Madara::Transport::TCP_Transport::reliability (const int & setting)
{
  return RELIABLE;
}

int
Madara::Transport::TCP_Transport::setup (void)
{
  
  // TO DO
  
  thread_ = new Madara::Transport::TCP_Transport_Read_Thread (id_, context_);
  
  Madara::Transport::Base::setup ();

  return 0;
}

long
Madara::Transport::TCP_Transport::send_data (const std::string & key, 
                                               const long long & value)
{
  Madara::Transport::Base::send_data (key, value);

  // TO DO
  

  return 0;
}
