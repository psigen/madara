#ifndef MADARA_THREADSAFECONTEXT_H
#define MADARA_THREADSAFECONTEXT_H

/**
 * @file Thread_Safe_Context.h
 * @author James Edmondson <james.r.edmondson@vanderbilt.edu>
 *
 * This file contains a thread safe context for state information
 */

#include "ace/Guard_T.h"
#include "ace/Recursive_Thread_Mutex.h"
#include "ace/Synch_T.h"
#include "ace/Synch.h"

#ifndef ACE_LACKS_PRAGMA_ONCE
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */



#include "madara/Globals.h"

namespace Madara
{
  namespace Knowledge_Engine
  {
    /**
     * @class Thread_Safe_Context
     * @brief This class stores variables and their values for use by any entity
     *        needing state information in a thread safe way
     */
    class Thread_Safe_Context
    {
    public:
      typedef ACE_Condition <ACE_Recursive_Thread_Mutex> Condition;

      /// Constructor.
      Thread_Safe_Context ();

      /// Destructor.
      ~Thread_Safe_Context (void);

      /// Atomically return the value of a variable.
      int get (const ::std::string & key) const;

      /// Atomically set the value of a variable.
      /// @return   0 if the value was set. -1 if null key
      int set (const ::std::string & key, long value);

      /// Atomically set if the variable value will be different
      /// @return   1 if the value was changed. 0 if not changed.
      ///           -1 if null key
      int set_if_unequal (const ::std::string & key, long value);

      /// Return list of variables that have been modified
      void get_modified (Madara::String_Vector & modified) const;

      /// Reset all variables to unmodified
      void reset_modified (void);

      /// Atomically increment the value of the variable
      int inc (const ::std::string & key);

      /// Make the current thread of execution wait for a change on the
      /// context
      void wait_for_change (bool extra_release = false);

      /// Atomically decrement the value of the variable
      int dec (const ::std::string & key);

      /// Atomically check to see if a key exists
      bool exists (const ::std::string & key) const;

      /// Print all variables and their values.
      void print (void) const;

      /// Clear all variables and their values.
      void clear (void);

      /// Lock the mutex on this context. Warning: this will cause
      /// all operations to block until the unlock call is made.
      void lock (void) const;

      /// Unlock the mutex on this context.
      void unlock (void) const;
      
      /// Signal the condition that it can wake up someone else 
      /// on changed data
      void signal (void) const;

    private:
      typedef ACE_Guard<ACE_Recursive_Thread_Mutex> Context_Guard;

      /// Hash table containing variable names and values.
      Madara::Knowledge_Map map_;
      mutable ACE_Recursive_Thread_Mutex mutex_;
      mutable Condition changed_;
    };
  }
}

#endif