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

#include "madara/knowledge_engine/Knowledge_Record.h"
#include "madara/knowledge_engine/Function_Map.h"

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

      /**
       * Constructor.
       **/
      Thread_Safe_Context ();

      /**
       * Destructor.
       **/
      ~Thread_Safe_Context (void);

      /**
       * Atomically returns the value of a variable.
       * @param   key    unique identifier of the variable
       * @return         the Madara::Knowledge_Record::Integer value for the variable
       **/
      Madara::Knowledge_Record
        get (const std::string & key) const;

      /**
       * Retrieves a knowledge record from the key. This function is useful
       * for performance reasons and also for using a Knowledge_Record that
       * can be one of multiple types
       * @param   key    unique identifier of the variable. Allows variable 
       *                 expansion.
       * @return         the knowledge record for the variable
       **/
      Madara::Knowledge_Record * get_record (const std::string & key);

      /**
       * Atomically sets the value of a variable to an integer.
       * @param   key       unique identifier of the variable
       * @param   value     new value of the variable
       * @param   modified  send modifications after applying the update
       * @return   0 if the value was set. -1 if null key
       **/
      int set (const std::string & key,
        Madara::Knowledge_Record::Integer value, 
        bool modified = true);
      
      /**
       * Atomically sets the value of a variable to a double.
       * @param   key       unique identifier of the variable
       * @param   value     new value of the variable
       * @param   modified  send modifications after applying the update
       * @return   0 if the value was set. -1 if null key
       **/
      int set (const std::string & key,
        double value, 
        bool modified = true);
      
      /**
       * Atomically sets the value of a variable to a string
       * @param   value     new value of the variable
       * @param   modified  send modifications after applying the update
       * @return   0 if the value was set. -1 if null key
       **/
      int set (const std::string & key,
        const std::string & value, 
        bool modified = true);

      /**
       * Atomically sets if the variable value will be different
       * @param   key       unique identifier of the variable
       * @param   value     new value of the variable
       * @param   modified  send modifications after applying the update
       * @return   1 if the value was changed. 0 if not changed.
       *          -1 if null key
       **/
      int set_if_unequal (const std::string & key,
        Madara::Knowledge_Record::Integer value, 
        uint32_t quality, uint64_t clock, 
        bool modified = true);
      
      /**
       * Atomically sets if the variable value will be different
       * @param   key       unique identifier of the variable
       * @param   value     new value of the variable
       * @param   modified  send modifications after applying the update
       * @return   1 if the value was changed. 0 if not changed.
       *          -1 if null key
       **/
      int set_if_unequal (const std::string & key,
        double value, 
        uint32_t quality, uint64_t clock, 
        bool modified = true);
      
      /**
       * Atomically sets if the variable value will be different
       * @param   key       unique identifier of the variable
       * @param   value     new value of the variable
       * @param   modified  send modifications after applying the update
       * @return   1 if the value was changed. 0 if not changed.
       *          -1 if null key
       **/
      int set_if_unequal (const std::string & key,
        const std::string & value, 
        uint32_t quality, uint64_t clock, 
        bool modified = true);

      /**
       * Atomically gets quality of a variable
       * @param   key       unique identifier of the variable
       * @return   quality associated with the variable
       **/
      uint32_t get_quality (const std::string & key);
      
      /**
       * Atomically gets write quality of this process for a variable
       * @param   key       unique identifier of the variable
       * @return   write quality associated with the variable
       **/
      uint32_t get_write_quality (const std::string & key);
      
      /**
       * Atomically sets quality of this process for a variable
       * @param   key            unique identifier of the variable
       * @param   quality        quality of this process
       * @param   force_update   force an update to variable, even if lower
       * @return   write quality associated with the variable
       **/
      uint32_t set_quality (const std::string & key, 
        uint32_t quality, bool force_update);

       /**
       * Force a change to be registered, waking up anyone waiting on entry
       **/
      void set_changed (void);

      /**
       * Atomically sets write quality of this process for a variable
       * @param   key            unique identifier of the variable
       * @param   quality        write quality of this process
       **/
      void set_write_quality (const std::string & key, uint32_t quality);

      /**
       * Retrieves a list of modified variables. Useful for building a
       * disseminatable knowledge update.
       * @param   modified       string of modified variables (key=value;)
       * @param   quality        write quality of this process
       **/
      const Knowledge_Records & get_modified (void) const;

      /**
       * Reset all variables to be unmodified. This will clear all global
       * knowledge updates. Use with caution.
       **/
      void reset_modified (void);

      /**
       * Changes all global variables to modified at current clock.
       **/
      void apply_modified (void);

      /**
       * Changes all global variables to modified at current clock.
       **/
      void mark_modified (const std::string & key,
        Madara::Knowledge_Record & record);

      /**
       * Resets a variable to unmodified
       * @param   key            unique identifier of the variable
       **/
      void reset_modified (const std::string & key);

      /**
       * Atomically increments the value of the variable
       * @param   key            unique identifier of the variable
       * @return                 new value of variable
       **/
      Madara::Knowledge_Record inc (const std::string & key);

      /**
       * Wait for a change to happen to the context.
       * @param   extra release  perform extra release of lock for nested locks
       **/
      void wait_for_change (bool extra_release = false);

      /**
       * Atomically decrements the value of the variable
       * @param   key            unique identifier of the variable
       * @return                 new value of variable
       **/
      Madara::Knowledge_Record dec (const std::string & key);

      /**
       * Atomically checks to see if a variable already exists
       * @param   key            unique identifier of the variable
       * @return                 true if variable exists
       **/
      bool exists (const std::string & key) const;

      /**
       * Atomically prints all variables and values in the context
       * @param   level          log level. @see Log_Macros.h
       **/
      void print (unsigned int level) const;

      /**
       * Atomically Print a statement, similar to printf (variable expansions
       * allowed) e.g., input = "MyVar{.id} = {MyVar{.id}}\n";
       * @param   statement      templated statement to print from
       * @param   level          log level. @see Log_Macros.h
       **/
      void print (const std::string & statement, unsigned int level) const;

      /**
       * Clears all variables and their values
       **/
      void clear (void);

      /**
       * Locks the mutex on this context. Warning: this will cause
       * all operations to block until the unlock call is made.
       **/
      void lock (void) const;

      /**
       * Unlocks the mutex on this context.
       **/
      void unlock (void) const;
      
      /**
       * Atomically increments the Lamport clock and returns the new
       * clock time (intended for sending knowledge updates).
       * @return        new clock time
       **/
      uint64_t inc_clock (void);

      /**
       * Atomically increments the Lamport clock of a variable and returns the
       * new clock time (intended for sending knowledge updates).
       * @param   key   unique identifier of the variable
       * @return        new clock time for variable
       **/
      uint64_t inc_clock (const std::string & key);

      /**
       * Atomically sets the lamport clock.
       * @param  clock  new global clock
       * @return        new clock time
       **/
      uint64_t set_clock (uint64_t clock);

      /**
       * Atomically sets the Lamport clock of a variable and returns the
       * new clock time (intended for sending knowledge updates).
       * @param    key     unique identifier of the variable
       * @param    clock   new variable clock
       * @return           new clock time for variable
       **/
      uint64_t set_clock (const std::string & key, 
        uint64_t clock);

      /**
       * Atomically gets the Lamport clock
       * @return           current global clock
       **/
      uint64_t get_clock (void);

      /**
       * Atomically gets the Lamport clock of a variable
       * @param    key     unique identifier of the variable
       * @return           current variable clock
       **/
      uint64_t get_clock (
        const std::string & key);

      /**
       * Signals that this thread is done with the context. Anyone
       * waiting on the underlying condition is awoken.
       **/
      void signal (void) const;

      /**
       * Expands a string with variable expansion. For instance, if
       * .id == 5, and a statement of "MyVar{.id} = {.id} * 30" then
       * then expanded statement would be "MyVar5 = 5 * 30".
       * @param  statement  statement to expand. Useful for printing.
       * @return            variable expanded statement
       **/
      std::string expand_statement (const std::string & statement) const;


      /**
       * Defines a function
       * @param  name       name of the function
       * @param  func       external function to call with this name
       **/
      void define_function (const std::string & name,
        VALUE_TYPE (*func) (Function_Arguments &, Variables &));

      /**
       * Retrieves an external function
       * @param  name       name of the function to retrieve
       * @return            the mapped external function
       **/
      Function * retrieve_function (const std::string & name);

    private:
      typedef ACE_Guard<ACE_Recursive_Thread_Mutex> Context_Guard;

      /// Hash table containing variable names and values.
      Madara::Knowledge_Map map_;
      mutable ACE_Recursive_Thread_Mutex mutex_;
      mutable Condition changed_;
      std::vector< std::string> expansion_splitters_;
      mutable uint64_t clock_;
      Knowledge_Records changed_map_;

      /// map of function names to functions
      Function_Map functions_;
    };
  }
}

#include "Thread_Safe_Context.inl"

#endif
