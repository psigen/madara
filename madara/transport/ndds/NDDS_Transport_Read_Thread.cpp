#include "madara/transport/ndds/NDDS_Transport_Read_Thread.h"
#include "madara/utility/Log_Macros.h"
#include "madara/knowledge_engine/Update_Types.h"
#include "madara/utility/Utility.h"

#include "ace/OS_NS_Thread.h"

#include <iostream>
#include <sstream>

Madara::Transport::NDDS_Read_Thread::NDDS_Read_Thread (
  const std::string & id,
  Madara::Knowledge_Engine::Thread_Safe_Context & context, 
  DDSDataReader & update_reader)
  : id_ (id), context_ (context),
    barrier_ (2), terminated_ (false), 
    mutex_ (), is_not_ready_ (mutex_), is_ready_ (false)
{
  assignment_symbols_.push_back ("=");
  assignment_symbols_.push_back (";");

  // Add update datareader statuscondition to waitset


  this->activate (THR_NEW_LWP | THR_DETACHED, 1);
  
  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
    DLINFO "NDDS_Read_Thread::NDDS_Read_Thread:" \
    " read thread started\n"));
}

Madara::Transport::NDDS_Read_Thread::~NDDS_Read_Thread ()
{
  close ();
}

int
Madara::Transport::NDDS_Read_Thread::close (void)
{
  terminated_ = true;
  enter_barrier ();

  return 0;
}

void
Madara::Transport::NDDS_Read_Thread::wait_for_ready (void)
{
  if (!is_ready_)
    is_not_ready_.wait ();
}

int Madara::Transport::NDDS_Read_Thread::enter_barrier (void)
{ 
  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
    DLINFO "Splice_Read_Thread::enter_barrier:" \
    " beginning thread barrier for shut down of read thread\n"));

  barrier_.wait ();
  return 0;
}

void Madara::Transport::NDDS_Read_Thread::handle_assignment (
  NDDS_Knowledge_Update & data)
{
}

void Madara::Transport::NDDS_Read_Thread::handle_multiassignment (
  NDDS_Knowledge_Update & data)
{
}

int
Madara::Transport::NDDS_Read_Thread::svc (void)
{

  // if we don't check originator for null, we get phantom sends
  // when the program exits.
  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
    DLINFO "NDDS_Read_Thread::svc:" \
    " entering processing loop.\n"));

  while (false == terminated_.value ())
  {
    ACE_OS::sleep (1);
  }

  enter_barrier ();
  return 0;
}
