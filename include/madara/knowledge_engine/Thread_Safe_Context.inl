#ifndef _MADARA_THREADSAFECONTEXT_INL_
#define _MADARA_THREADSAFECONTEXT_INL_

/**
 * @file Thread_Safe_Context.h
 * @author James Edmondson <james.r.edmondson@vanderbilt.edu>
 *
 * This file contains inline functions for a thread safe context
 */

#include "ace/Guard_T.h"
#include "ace/Recursive_Thread_Mutex.h"
#include "ace/Condition_Recursive_Thread_Mutex.h"
#include "ace/Synch.h"
#include "madara/utility/Log_Macros.h"
#include "madara/expression_tree/Interpreter.h"

#include <sstream>



// Atomically increment a stored value. This function is only used in one place
// so we will expand it into that Preincrement_Node
inline  Madara::Knowledge_Record
Madara::Knowledge_Engine::Thread_Safe_Context::inc (const std::string & key,
  const Knowledge_Update_Settings & settings)
{
  // check for null key
  if (key == "")
    return Knowledge_Record ();
  
  // enter the mutex
  Context_Guard guard (mutex_);
  
  // create the key if it didn't exist
  Knowledge_Record & record = map_[key];

  if (settings.always_overwrite || record.write_quality > record.quality)
  {
    ++record;
    record.quality = record.write_quality;
  }
  
  // otherwise set the value
  if (key[0] != '.')
  {
    if (!settings.treat_globals_as_locals)
    {
      mark_modified (key, record);
    }
  }

  changed_.signal ();

  return record;
}

// return whether or not the key exists
inline bool
Madara::Knowledge_Engine::Thread_Safe_Context::delete_expression (
  const std::string & expression)
{
  Context_Guard guard (mutex_);

  return interpreter_->delete_expression (expression);
}

// return whether or not the key exists
inline bool
Madara::Knowledge_Engine::Thread_Safe_Context::delete_variable (
  const std::string & key)
{
  Context_Guard guard (mutex_);

  return map_.erase (key) == 1;
}

// return whether or not the key exists
inline bool
Madara::Knowledge_Engine::Thread_Safe_Context::exists (const std::string & key) const
{
  Context_Guard guard (mutex_);

  // if key is not null
  if (key != "")
  {
    // find the key in the knowledge base
    Knowledge_Map::const_iterator found = map_.find (key);

    // if it's found, then return the value
    if (found != map_.end ())
      return found->second.status != Knowledge_Record::UNCREATED;
  }

  // if no match, return empty (0)
  return false;
}

// Atomically decrement a stored value. Only reason we are inlining this function
// is because it is called by only one function, and we can save a bit of
// execution time via expansion into that function call.
inline Madara::Knowledge_Record
Madara::Knowledge_Engine::Thread_Safe_Context::dec (const std::string & key,
  const Knowledge_Update_Settings & settings)
{
  // check for null key
  if (key == "")
    return Knowledge_Record ();
  
  // enter the mutex
  Context_Guard guard (mutex_);
  
  // create the key if it didn't exist
  Knowledge_Record & record = map_[key];

  if (settings.always_overwrite || record.write_quality > record.quality)
  {
    --record;
    record.quality = record.write_quality;
  }
  
  // otherwise set the value
  if (key[0] != '.')
  {
    if (!settings.treat_globals_as_locals)
    {
      mark_modified (key, record);
    }
  }

  changed_.signal ();

  return record;
}

/// set the lamport clock (updates with lamport clocks lower
/// than our current clock get discarded)
inline uint64_t
Madara::Knowledge_Engine::Thread_Safe_Context::set_clock (
  uint64_t clock)
{
  Context_Guard guard (mutex_);

  // clock_ is always increasing. We never reset it to a lower clock value
  // user can check return value to see if the clock was set.
  if (clock_ < clock)
    clock_ = clock;

  return clock_;
}

/// set the lamport clock (updates with lamport clocks lower
/// than our current clock get discarded)
inline uint64_t 
Madara::Knowledge_Engine::Thread_Safe_Context::set_clock (
  const std::string & key, uint64_t clock)
{
  Context_Guard guard (mutex_);
  
  // create the key if it didn't exist
  Knowledge_Record & record = map_[key];

  // check for value already set
  if (record.clock < clock)
  {
    record.clock = clock;

    // try to update the global clock as well
    this->set_clock (clock);
  }

  return record.clock;
}

/// set the lamport clock for a particular variable (updates with 
/// lamport clocks lower than our current clock get discarded)
inline uint64_t
Madara::Knowledge_Engine::Thread_Safe_Context::inc_clock (
  const std::string & key,
  const Knowledge_Update_Settings & settings)
{
  Context_Guard guard (mutex_);
  
  // create the key if it didn't exist
  Knowledge_Record & record = map_[key];

  return record.clock += settings.clock_increment;
}

/// increment the process lamport clock
inline uint64_t
Madara::Knowledge_Engine::Thread_Safe_Context::inc_clock (
  const Knowledge_Update_Settings & settings)
{
  Context_Guard guard (mutex_);
  return clock_ += settings.clock_increment;
}

/// get the lamport clock (updates with lamport clocks lower
/// than our current clock get discarded)
inline uint64_t
Madara::Knowledge_Engine::Thread_Safe_Context::get_clock (void)
{
  Context_Guard guard (mutex_);
  return clock_;
}

/// get the lamport clock for a particular variable
inline uint64_t
Madara::Knowledge_Engine::Thread_Safe_Context::get_clock (
  const std::string & key)
{
  Context_Guard guard (mutex_);

  // find the key in the knowledge base
  Knowledge_Map::iterator found = map_.find (key);

  // if it's found, then compare the value
  if (found != map_.end ())
  {
    return found->second.clock;
  }
  else
    // key does not exist
    return 0;
}

/// Lock the mutex on this context. Warning: this will cause
/// all operations to block until the unlock call is made.
inline void
Madara::Knowledge_Engine::Thread_Safe_Context::lock (void) const
{
  mutex_.acquire ();
}

/// Unlock the mutex on this context.
inline void
Madara::Knowledge_Engine::Thread_Safe_Context::unlock (void) const
{
  mutex_.release ();
}

/// Print a statement, similar to printf (variable expressions allowed)
/// e.g. input = "MyVar{.id} = {MyVar{.id}}\n";
inline void 
Madara::Knowledge_Engine::Thread_Safe_Context::print (
  const std::string & statement, unsigned int level) const
{
  // enter the mutex
  Context_Guard guard (mutex_);
  MADARA_DEBUG (level, (LM_INFO, 
    this->expand_statement (statement).c_str ()));
}

// clear all variables and their values
inline void
Madara::Knowledge_Engine::Thread_Safe_Context::clear (void)
{
  // enter the mutex
  Context_Guard guard (mutex_);

  // we no longer blow everything away to allow for cached variables
  // in the expression tree. If we allow this clear, there will be
  // segmentation faults if the tree is used later.
  //map_.clear ();

  for (Madara::Knowledge_Map::iterator i = map_.begin (); 
       i != map_.end (); ++i)
  {
    i->second.reset_value ();
  }

  changed_map_.clear ();
  changed_.signal ();
}


/// Make the current thread of execution wait for a change on the
/// context
inline void 
Madara::Knowledge_Engine::Thread_Safe_Context::wait_for_change (
  bool extra_release)
{
  // enter the mutex
  Context_Guard guard (mutex_);

  // if the caller is relying on a recursive call (e.g. Knowlege_Base::wait),
  // we'll need to call an extra release for this to work. Otherwise, the 
  // context would remain locked to the calling thread - even though it will
  // now be put to sleep
  if (extra_release)
    mutex_.release ();

  changed_.wait ();
}

inline void
Madara::Knowledge_Engine::Thread_Safe_Context::mark_modified (
  const std::string & key, Madara::Knowledge_Record & record)
{
  changed_map_[key] = &record;

  if (record.status != Madara::Knowledge_Record::MODIFIED)
    record.status = Madara::Knowledge_Record::MODIFIED;
}

/// Return list of variables that have been modified
inline const Madara::Knowledge_Records &
Madara::Knowledge_Engine::Thread_Safe_Context::get_modified (void) const
{
  Context_Guard guard (mutex_);

  return changed_map_;
}

/// Reset all variables to unmodified
inline void 
Madara::Knowledge_Engine::Thread_Safe_Context::reset_modified (void)
{
  Context_Guard guard (mutex_);
  changed_map_.clear ();
}

/// Changes all global variables to modified at current time
inline void 
Madara::Knowledge_Engine::Thread_Safe_Context::apply_modified (void)
{
  Context_Guard guard (mutex_);

  // each synchronization counts as an event, since this is a
  // pretty important networking event

  //++this->clock_;

  for (Madara::Knowledge_Map::iterator i = map_.begin ();
       i != map_.end (); 
       ++i)
  {
    if (i->first.size () > 0 && i->first[0] != '.')
    {
      // local or global doesn't matter. Clock and modification
      // aren't really a part of local variable checking anyway
      //i->second.status = Madara::Knowledge_Record::MODIFIED;

      mark_modified (i->first, i->second);

      //i->second.clock = this->clock_;
    }
  }
}

/// Reset a variable to unmodified
inline void 
Madara::Knowledge_Engine::Thread_Safe_Context::reset_modified (
  const std::string & variable)
{
  Context_Guard guard (mutex_);
  changed_map_.erase (variable);
}


/// Signal the condition that it can wake up someone else on changed data.
inline void
Madara::Knowledge_Engine::Thread_Safe_Context::signal (void) const
{
  Context_Guard guard (mutex_);
  changed_.signal ();
}

#endif // _MADARA_THREADSAFECONTEXT_INL_
