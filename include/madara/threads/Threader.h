

#ifndef _MADARA_THREADS_THREADER_H_
#define _MADARA_THREADS_THREADER_H_

/**
 * @file Threader.h
 * @author James Edmondson <jedmondson@gmail.com>
 *
 * This file contains a thre
 **/

#include <string>
#include <vector>
#include <map>
#include <list>
#include "madara/knowledge_engine/Knowledge_Base.h"
#include "Base_Thread.h"
#include "Worker_Thread.h"
#include "madara/MADARA_export.h"

namespace Madara
{
  namespace Threads
  {
    /**
     * Starts threads with first class support of MADARA contexts
     **/
    class MADARA_Export Threader
    {
    public:
      /**
       * Default constructor
       **/
      Threader ();

      /**
       * Constructor
       * @param  data_plane   The data plane for threads to use
       **/
      Threader (Knowledge_Engine::Knowledge_Base & data_plane);

      /**
       * Destructor
       **/
      ~Threader ();

      /**
       * Starts a new thread and executes the provided user
       * thread once. Execution ordering is init -> execute -> cleanup.
       * 
       * <br>&nbsp;<br>The thread will be managed by the
       * threader. Please do not pass the address of a thread
       * on the stack and do not delete this memory yourself
       * or your program will crash.
       * @param name    unique thread name for the thread.
       *                If possible, try to use one word or
       *                words separated by underscores (_)
       * @param thread  user-created thread implementation
       * @param paused  create thread in a paused state.
       **/
      void run (const std::string name, Base_Thread * thread,
        bool paused = false);
      
      /**
       * Starts a new thread and executes the provided user
       * thread once. Execution ordering is init -> *execute -> cleanup.
       * init and cleanup are only called once.
       * 
       * <br>&nbsp;<br>The thread will be managed by the
       * threader. Please do not pass the address of a thread
       * on the stack and do not delete this memory yourself
       * or your program will crash.
       * @param hertz   the intended hertz (frequency) that the thread's
       *                execute should be ran. Hertz is in operations per
       *                second.
       * @param name    unique thread name for the thread.
       *                If possible, try to use one word or
       *                words separated by underscores (_)
       * @param thread  user-created thread implementation
       * @param paused  create thread in a paused state.
       **/
      void run (double hertz, const std::string name, Base_Thread * thread,
        bool paused = false);

      /**
       * Wait for a specific thread to complete
       * @param name    unique thread name for the thread.
       **/
      void wait (const std::string name);

      /**
       * Wait for all threads to complete
       **/
      void wait (void);
      
      /**
       * Requests a specific thread to pause
       * @param name    unique thread name for the thread.
       **/
      void pause (const std::string name);

      /**
       * Requests all threads to pause
       **/
      void pause (void);
      
      /**
       * Requests a specific thread to resume (unpause)
       * @param name    unique thread name for the thread.
       **/
      void resume (const std::string name);

      /**
       * Requests all threads to resume (unpause)
       **/
      void resume (void);
      
      /**
       * Requests a specific thread to terminate
       * @param name    unique thread name for the thread.
       **/
      void terminate (const std::string name);

      /**
       * Requests all threads to terminate
       **/
      void terminate (void);

    private:
      
      /**
       * The data plane used by threads
       **/
      Knowledge_Engine::Knowledge_Base * data_;

      /**
       * The control plane used by threads for termination
       * and pause information. This has to be on the heap, because
       * each thread gets its own stack!
       **/
      Knowledge_Engine::Knowledge_Base * control_;

      /**
       * the threads that are still active
       **/
      Named_Worker_Threads threads_;
    };
  }
}

#endif  // _MADARA_THREADS_THREADER_H_
