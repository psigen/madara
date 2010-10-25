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
#include "ace/Condition_Recursive_Thread_Mutex.h"
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
      int set (const ::std::string & key, long value, bool modified = true);

      /// Atomically set if the variable value will be different
      /// @return   1 if the value was changed. 0 if not changed.
      ///           -1 if null key
      int set_if_unequal (const ::std::string & key, long value, 
        unsigned long quality, unsigned long clock, bool modified = true);

      /// Get quality of last update to a variable
      unsigned long get_quality (const ::std::string & key);
      
      /// Get quality of local process updates to a variable
      unsigned long get_write_quality (const ::std::string & key);
      
      /// Set quality of last update to a variable
      unsigned long set_quality (const ::std::string & key, 
        unsigned long quality, bool force_update);
      
      /// Set quality of this process writing to a variable
      void set_write_quality (const ::std::string & key, unsigned long quality);

      /// Return list of variables that have been modified
      void get_modified (Madara::String_Vector & modified) const;

      /// Reset all variables to unmodified
      void reset_modified (void);

      /// Reset a variable to unmodified
      void reset_modified (const ::std::string & key);

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

      /// Print a statement, similar to printf (variable expansions allowed)
      /// e.g. input = "MyVar{.id} = {MyVar{.id}}\n";
      void print (const std::string & statement) const;

      /// Clear all variables and their values.
      void clear (void);

      /// Lock the mutex on this context. Warning: this will cause
      /// all operations to block until the unlock call is made.
      void lock (void) const;

      /// Unlock the mutex on this context.
      void unlock (void) const;
      
      /// atomically increment the lamport clock and return the new
      /// clock time (intended for sending knowledge updates)
      unsigned long inc_clock (void);

      /// atomically increment the lamport clock of a particular variable
      /// and return the new clock time (for sending knowledge updates)
      unsigned long inc_clock (const std::string & key);

      /// set the lamport clock (updates with lamport clocks lower
      /// than our current clock get discarded)
      unsigned long set_clock (unsigned long clock);

      /// set the lamport clock for a particular variable (updates with 
      /// lamport clocks lower than our current clock get discarded)
      unsigned long set_clock (const std::string & key, unsigned long clock);

      /// get the lamport clock (updates with lamport clocks lower
      /// than our current clock get discarded)
      unsigned long get_clock (void);

      /// get the lamport clock for a particular variable (updates with
      /// lamport clocks lower than our current clock get discarded)
      unsigned long get_clock (const std::string & key);

      /// Signal the condition that it can wake up someone else 
      /// on changed data
      void signal (void) const;

      /// Expand a string with variable expansions
      std::string expand_statement (const std::string & statement) const;

    private:
      typedef ACE_Guard<ACE_Recursive_Thread_Mutex> Context_Guard;

      /// Hash table containing variable names and values.
      Madara::Knowledge_Map map_;
      mutable ACE_Recursive_Thread_Mutex mutex_;
      mutable Condition changed_;
      std::vector< std::string> expansion_splitters_;
      unsigned long clock_;
    };
  }
}

#endif