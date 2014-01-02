
#ifndef _MADARA_TIMED_EXECUTOR_H_
#define _MADARA_TIMED_EXECUTOR_H_

#include <vector>
#include <map>
#include <string>
#include <utility>
#include <queue>
#include "madara/knowledge_engine/Knowledge_Base.h"
#include "madara/knowledge_engine/Eval_Settings.h"

#include "ace/High_Res_Timer.h"
#include "ace/Barrier.h"
#include "ace/Mutex.h"
#include "ace/Thread_Mutex.h"

/**
 * @file Timed_Executor.h
 * @author James Edmondson <jedmondson@gmail.com>
 *
 * This file contains C++ objects that facilitate timed event
 * execution.
 **/

namespace Madara
{
  namespace Knowledge_Engine
  {
    /**
     * @class Event
     * @brief Expression container for timed event
     */
    struct Event
    {
      /// time between executions in seconds
      ACE_Time_Value period;

      /// holder of last result
      Knowledge_Record last_result;

      /// expression to be executed
      Compiled_Expression expression;

      /**
       * knowledge base for executing the expression
       * (should be same as compiled)
       **/
      Knowledge_Base * knowledge;

      /// executions so far
      unsigned int executions;

      /// intended executions
      int intended_executions;
    };

    /// forward declare the struct
    struct Timed_Event_Thread_Info;

    /// forward declare the event threads
    class Timed_Event_Thread;

    typedef std::pair <ACE_Time_Value, Event *> Timed_Event;

    /**
     * @class Timed_Executor
     * @brief This class executes expression events at specified times
     */
    class MADARA_Export Timed_Executor
    {
    public:
      /**
       * Default constructor
       **/
      Timed_Executor ();
      
      /**
       * Destructor
       **/
      ~Timed_Executor ();

      /**
       * Adds a timed event to the queue
       * @param  new_event   new event to add to the queue
       **/
      void add (const Timed_Event & new_event);
      
      /**
       * Adds an event to the queue
       * @param  new_event   new event to add to the queue.
       **/
      void add (const Event & new_event);

      /**
       * Removes a timed event from the queue
       * @param  a placeholder for the current highest priority event
       * @return time until next event time out (useful when the cur_event
       *         has a zeroed event (the 2nd part of the cur_event pair)
       **/
      ACE_Time_Value remove (Timed_Event & cur_event);

      /**
       * Informs threads of a shutdown and waits for threads to exit
       **/
      void shutdown (void);

      /**
       * Checks if the thread pool has been shutdown
       * @return true if shutdown has been called. False otherwise.
       **/
      bool is_shutdown (void);

      /**
       * Returns the time 
       **/
      ACE_Time_Value time_until_next (void);
      
      /**
       * Launches threads
       * @param threads  number of threads
       **/
      void launch_threads (unsigned int threads);

      /**
       * Enter barrier
       **/
      void enter_barrier (void);
      
    private:
      
      /// condition for threads that enter with nothing to do
      typedef ACE_Condition <ACE_Recursive_Thread_Mutex> Condition;

      /// guard for access and changes
      typedef ACE_Guard<ACE_Recursive_Thread_Mutex> Guard;
      

      /**
       * Locks the context
       **/
      void lock (void);

      /**
       * Unlocks the event queue
       **/
      void unlock (void);

      /**
       * Mutex for local changes
       **/
      mutable ACE_Recursive_Thread_Mutex mutex_;
      
      /**
       * Condition for situations with no work available
       **/
      mutable Condition changed_;

      /**
       * Event queue
       **/
      std::priority_queue <Timed_Event, 
        std::vector <Timed_Event> > events_;

      /**
       * Flag for threads to read shutdown information
       **/
      volatile bool terminated_;

      /**
       * Vector of thread info
       **/
      Timed_Event_Thread_Info * thread_info_;

      /**
       * Timed Event Threads
       **/
      Timed_Event_Thread ** threads_;

      /**
       * Number of thread
       **/
      Knowledge_Record::Integer num_threads_;

      /**
       * For barrier and other information
       **/
      Knowledge_Base control_plane_;

      /**
       * Reference to the number of threads in the control plane
       **/
      Variable_Reference threads_ref_;
    };

    /**
     * A struct that is passed to Timed Event Thread Executors
     * to control job handling, shutdowns, etc.
     **/
    struct Timed_Event_Thread_Info
    {
      /**
       * pointer to Timed_Executor caller, which provides thread-safe callbacks
       * for shutdown information
       **/
      Timed_Executor * executor;
      
      /**
       * For barrier and other information
       **/
      Knowledge_Base * control_plane;

      /**
       * thread identifier
       **/
      unsigned int id;
    };

    /**
     * Fills an event struct. It is highly recommended that you use this
     * function.
     * @param knowledge  the knowledge base to reference and execute in
     * @param expression KaRL expression to execute
     * @param period     time, in seconds, between executions
     * @param executions number of executions. -1 means infinite executions
     *                   until a shutdown is called on the timed executor
     **/
    MADARA_Export Event fill_event (
      Knowledge_Base & knowledge,
      const std::string & expression,
      double period, int executions = -1);
  }
}

/**
  * Comparison for higher event priorities
  * @param lhs  left hand side of the check
  * @param rhs  right hand side of the check
  * @return true if lhs timer is greater than rhs, false otherwise.
  **/
bool operator< (
  const Madara::Knowledge_Engine::Timed_Event & lhs,
  const Madara::Knowledge_Engine::Timed_Event & rhs);



#endif // _MADARA_TIMED_EXECUTOR_H_