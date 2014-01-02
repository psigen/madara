#include "Timed_Executor.h"
#include "Timed_Event_Thread.h"
#include "ace/Thread.h"

bool operator< (
  const Madara::Knowledge_Engine::Timed_Event & lhs,
  const Madara::Knowledge_Engine::Timed_Event & rhs)
{
  return lhs.first > lhs.first;
}


Madara::Knowledge_Engine::Timed_Executor::Timed_Executor ()
  : changed_ (mutex_), terminated_ (false),
     thread_info_ (0), threads_ (0), num_threads_ (0)
{
  threads_ref_ = control_plane_.get_ref ("threads");
}
      

Madara::Knowledge_Engine::Timed_Executor::~Timed_Executor ()
{
  shutdown ();
}

void
Madara::Knowledge_Engine::Timed_Executor::add (const Timed_Event & new_event)
{
  Guard guard (mutex_);

  events_.push (new_event);

  changed_.broadcast ();
}

void
Madara::Knowledge_Engine::Timed_Executor::add (const Event & new_event)
{
  Guard guard (mutex_);

  Timed_Event timed_event;

  // setup times
  ACE_Time_Value cur_time = ACE_OS::gettimeofday ();

  // create timed_event
  timed_event.first = cur_time + new_event.period;
  timed_event.second = new Event (new_event);

  // add the timed event to the event queue
  events_.push (timed_event);

  // signal any sleeping threads that a new event has been added
  changed_.broadcast ();
}

ACE_Time_Value
Madara::Knowledge_Engine::Timed_Executor::remove (Timed_Event & cur_event)
{
  // obtain current time
  ACE_Time_Value cur_time = ACE_OS::gettimeofday ();

  Guard guard (mutex_);
  
  // obtain next event in queue
  if (events_.size () > 0)
  {
    MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
      DLINFO "Timed_Executor::remove: " \
      "events queue size is greater than zero.\n"));

    cur_event = events_.top ();

    // if we've hit the timeout, pop
    if (cur_time >= cur_event.first)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Timed_Executor::remove: " \
        "removing top element from events queue.\n"));

      events_.pop ();
    }
    // otherwise, zero out the cur_event event pointer
    else
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Timed_Executor::remove: " \
        "earliest event is not ready yet.\n"));

      cur_event.second = 0;
      cur_time = cur_event.first - cur_time;
    }
  }
  // there are no events in queue
  else
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Timed_Executor::remove: " \
      "Nothing to do. Thread going to sleep.\n"));
    
    ACE_Time_Value max_wait_time;
    max_wait_time.set (5.0);

    ACE_Time_Value max_time = cur_time + max_wait_time;

    mutex_.release ();
    changed_.wait (&max_time);

    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Timed_Executor::remove: " \
      "Condition has changed. Thread waking up.\n"));

    cur_event.second = 0;
    cur_time.set (0.0);
  }

  return cur_time;
}

void
Madara::Knowledge_Engine::Timed_Executor::shutdown (void)
{
  if (thread_info_)
  {
    terminated_ = true;
    
    changed_.broadcast ();

    // wait for all threads to complete
    enter_barrier ();

    Madara::Utility::sleep (1.0);

    delete [] thread_info_;
  
    for (unsigned int i = 0; i < num_threads_; ++i)
    {
      delete threads_[i];
    }

    delete [] threads_;

    thread_info_ = 0;
    threads_ = 0;
    num_threads_ = 0;

    control_plane_.clear ();
  }
}

bool
Madara::Knowledge_Engine::Timed_Executor::is_shutdown (void)
{
  return terminated_;
}

void
Madara::Knowledge_Engine::Timed_Executor::launch_threads (
  unsigned int num_threads)
{
  terminated_ = false;

  // shutdown any existing threads
  if (thread_info_)
  {
    shutdown ();
  }

  thread_info_ = new Timed_Event_Thread_Info [num_threads];
  threads_ = new Timed_Event_Thread * [num_threads];
  num_threads_ = num_threads;
  control_plane_.set (threads_ref_, num_threads_);

  for (unsigned int i = 0; i < num_threads; ++i)
  {
    thread_info_[i].id = i;
    thread_info_[i].executor = this;
    thread_info_[i].control_plane = &control_plane_;
    threads_[i] = new Timed_Event_Thread (thread_info_[i]);
  }
}

ACE_Time_Value
Madara::Knowledge_Engine::Timed_Executor::time_until_next (void)
{
  // get the current time
  ACE_Time_Value cur_time = ACE_OS::gettimeofday ();

  // calculate the time left before next event is due
  cur_time = events_.top ().first - cur_time;

  // return the time
  return cur_time;
}

void 
Madara::Knowledge_Engine::Timed_Executor::lock (void)
{
  mutex_.acquire ();
}

void 
Madara::Knowledge_Engine::Timed_Executor::unlock (void)
{
  mutex_.release ();
}

Madara::Knowledge_Engine::Event
Madara::Knowledge_Engine::fill_event (
  Knowledge_Base & knowledge, const std::string & expression,
      double period, int executions)
{
  Event new_event;
  new_event.executions = 0;
  new_event.intended_executions = executions;
  new_event.knowledge = &knowledge;
  new_event.period.set (period);
  new_event.expression = knowledge.compile (expression);

  return new_event;
}

void 
Madara::Knowledge_Engine::Timed_Executor::enter_barrier (void)
{
  Wait_Settings settings;
  settings.poll_frequency = .5;
  settings.max_wait_time = -1;

  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
    DLINFO "Timed_Executor::enter_barrier: " \
    "Entering barrier.\n"));

  control_plane_.wait (
    "closed = 0 ;>\n"
    ".i [0->threads) (thread.{.i}.closed => ++closed) ;>\n"
    "closed == threads", settings);

  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
    DLINFO "Timed_Executor::enter_barrier: " \
    "Leaving barrier.\n"));
}