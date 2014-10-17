#include "Worker_Thread.h"

#ifdef WIN32

#include <process.h>

unsigned __stdcall worker_thread_windows_glue (void * param)
{
  Madara::Threads::Worker_Thread * caller = 
    static_cast < Madara::Threads::Worker_Thread *> (
      param);
  if (caller)
    return (unsigned) caller->svc ();
  else
    return 0;
}

#endif


#include <iostream>
#include <algorithm>

Madara::Threads::Worker_Thread::Worker_Thread ()
  : thread_ (0), control_ (0), data_ (0) 
{
}


Madara::Threads::Worker_Thread::Worker_Thread (
  const std::string & name,
  Base_Thread * thread,
  Knowledge_Engine::Knowledge_Base * control,
  Knowledge_Engine::Knowledge_Base * data)
  : name_ (name), thread_ (thread), control_ (control), data_ (data) 
{
  if (thread && control)
  {
    std::stringstream base_string;
    base_string << name;

    thread->terminated.set_name (
      base_string.str () + ".terminated", *control);
    thread->paused.set_name (
      base_string.str () + ".paused", *control);
    thread->name = name;

    finished_.set_name (
      base_string.str () + ".finished", *control);
    started_.set_name (
      base_string.str () + ".started", *control);

    finished_ = 0;
    started_ = 0;
  }
}

Madara::Threads::Worker_Thread::Worker_Thread (const Worker_Thread & input)
  : name_ (input.name_), thread_ (input.thread_),
    control_ (input.control_), data_ (input.data_),
    finished_ (input.finished_), started_ (input.started_)
{
}

Madara::Threads::Worker_Thread::~Worker_Thread ()
{
}

void
Madara::Threads::Worker_Thread::operator= (const Worker_Thread & input)
{
  if (this != &input)
  {
    this->name_ = input.name_;
    this->thread_ = input.thread_;
    this->control_ = input.control_;
    this->data_ = input.data_;
    this->finished_ = input.started_;
    this->started_ = input.started_;
  }
}

void
Madara::Threads::Worker_Thread::run (void)
{
  int result;

#ifndef WIN32
  result = this->activate ();
#else
  result = 0;
  _beginthreadex(NULL, 0, worker_thread_windows_glue, (void*)this, 0, 0);
    
#endif

  if (result != -1)
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Worker_Thread::Worker_Thread:" \
      " read thread started (result = %d)\n", result));
  }
  else
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Worker_Thread::Worker_Thread:" \
      " failed to create thread. ERRNO = %d\n", ACE_OS::last_error ()));
  }
}

int
Madara::Threads::Worker_Thread::svc (void)
{
  if (thread_)
  {
    started_ = 1;

    thread_->init (*data_);

    thread_->execute ();

    thread_->cleanup ();

    delete thread_;

    finished_ = 1;
  }

  return 0;
}
