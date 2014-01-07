#include "Timed_Event_Thread.h"
#include "madara/utility/Log_Macros.h"
#include "madara/transport/Reduced_Message_Header.h"
#include "madara/transport/Fragmentation.h"
#include "ace/Time_Value.h"
#include "madara/utility/Utility.h"

#ifdef WIN32

#include <process.h>

unsigned __stdcall timed_thread_windows_glue (void * param)
{
  Madara::Knowledge_Engine::Timed_Event_Thread * caller = 
    static_cast <Madara::Knowledge_Engine::Timed_Event_Thread *> (
      param);

  if (caller)
    return (unsigned) caller->svc ();
  else
    return 0;
}

#endif


#include <iostream>
#include <algorithm>

Madara::Knowledge_Engine::Timed_Event_Thread::Timed_Event_Thread (
  Timed_Event_Thread_Info & info)
  : info_ (info)
{
  int result;

//#ifndef WIN32
    result = this->activate ();
/*#else
    result = 0;
    _beginthreadex(NULL, 0, timed_thread_windows_glue, (void*)this, 0, 0);
    
#endif*/

  if (result != -1)
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Timed_Event_Thread::Timed_Event_Thread:" \
      " thread started (result = %d)\n", result));
  }
  else
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Timed_Event_Thread::Timed_Event_Thread:" \
      " failed to create thread. ERRNO = %d\n", ACE_OS::last_error ()));
  }

  std::stringstream expression;
  expression << "thread.";
  expression << info_.id;
  expression << ".closed = 1";

  thread_closed_ = info_.control_plane->compile (expression.str ());
  queued_or_terminated_ = info_.control_plane->compile (
    "queued > 0 || terminated");
}

Madara::Knowledge_Engine::Timed_Event_Thread::~Timed_Event_Thread ()
{
}


int
Madara::Knowledge_Engine::Timed_Event_Thread::svc (void)
{
  ACE_Time_Value sleep_time;
  ACE_Time_Value zero_time;
  Timed_Event cur_event;
  
  zero_time.set (0.0);

  std::stringstream buffer;
  buffer << "Timed_Event_Thread[";
  buffer << info_.id;
  buffer << "]::svc";
  std::string print_prefix (buffer.str ());
  
  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
    DLINFO "%s: " \
    "Service thread started.\n",
    print_prefix.c_str ()));


  while (!info_.executor->is_shutdown ())
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "%s: " \
      "Attempting to remove an event.\n",
      print_prefix.c_str ()));

    /**
     * remove returns in one of three conditions
     * 1) an event has a timeout, so cur_event.second is valid
     * 2) an event is in the queue but not timed out, so
     *    sleep_time is a valid sleep time (>0.0)
     * 3) there were no events in queue, so we slept on a condition,
     *    and we've now been signaled. It is possible that someone
     *    else has serviced the event since we were signaled. The
     *    result will be 0.0 in sleep_time and a need to try to
     *    remove again.
     **/
    sleep_time = info_.executor->remove (cur_event);
    Event * valid = cur_event.second;

    // if we had a timeout, we'll have a non-zero Event *
    if (valid > 0)
    {
      ++valid->executions;
      Knowledge_Record result (valid->knowledge->evaluate (valid->root));

      if (valid->intended_executions >= 0 &&
          valid->intended_executions == valid->executions)
      {
        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "%s: " \
          "Timed Event has expired. Deleting.\n",
          print_prefix.c_str ()));

        delete valid;
      }
      else if (valid->cancel_on_false && result.is_false ())
      {
        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "%s: " \
          "Timed Event returned false. User requested a cancel. Deleting.\n",
          print_prefix.c_str ()));

        delete valid;
      }
      else
      {
        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "%s: " \
          "Timed Event added back to queue.\n",
          print_prefix.c_str ()));

        cur_event.first += valid->period;
        info_.executor->add (cur_event);
      }
    }
    // otherwise, wait on changes to the queue
    else
    {
      if (sleep_time > zero_time)
      {
        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "%s: " \
          "Not ready for timeout. Thread sleeping.\n",
          print_prefix.c_str ()));
        
        Wait_Settings wait_settings;
        wait_settings.poll_frequency = 0.5;
        wait_settings.max_wait_time = 5.0;
    
        // inform sleeping threads of new queued events
        info_.control_plane->wait (queued_or_terminated_, wait_settings);
      }
    }
  }

  info_.control_plane->evaluate (thread_closed_);

  return 0;
}

