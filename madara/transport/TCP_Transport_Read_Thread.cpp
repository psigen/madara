#include "madara/transport/TCP_Transport_Read_Thread.h"
#include "ace/Log_Msg.h"

#include <iostream>

Madara::Transport::TCP_Transport_Read_Thread::TCP_Transport_Read_Thread (
  const std::string & id,
  Madara::Knowledge_Engine::Thread_Safe_Context & context)
  : id_ (id), context_ (context),
    barrier_ (2), terminated_ (false), 
    mutex_ (), is_not_ready_ (mutex_), is_ready_ (false)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) TCP_Transport_Read_Thread started\n"));
  
  this->activate (THR_NEW_LWP | THR_DETACHED, 1);
}

Madara::Transport::TCP_Transport_Read_Thread::~TCP_Transport_Read_Thread ()
{
}

int
Madara::Transport::TCP_Transport_Read_Thread::close (void)
{
  terminated_ = true;
  enter_barrier ();

  return 0;
}

void
Madara::Transport::TCP_Transport_Read_Thread::wait_for_ready (void)
{
  if (!is_ready_)
    is_not_ready_.wait ();
}

int Madara::Transport::TCP_Transport_Read_Thread::enter_barrier (void)
{ 
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) entering barrier.\n"));
 
  barrier_.wait ();

  return 0;
}


int
Madara::Transport::TCP_Transport_Read_Thread::svc (void)
{
  while (false == terminated_.value ())
  {
 
  }

  enter_barrier ();
  return 0;
}