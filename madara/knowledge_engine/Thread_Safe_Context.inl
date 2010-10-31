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
#include "ace/Log_Msg.h"



// Atomically increment a stored value. This function is only used in one place
// so we will expand it into that Preincrement_Node
inline int
Madara::Knowledge_Engine::Thread_Safe_Context::inc (const ::std::string & key)
{
  // check for null key
  if (key == "")
    return -1;

  // enter the mutex
  Context_Guard guard (mutex_);

  // creat the key if it didn't exist
  map_[key];

  // find the key in the knowledge base. It will be
  // much faster to use the iterator for modifications
  // than it will be to look up the key in the map for
  // everything we want to modify (e.g. map_[key].quality = 1)
  Knowledge_Map::iterator found = map_.find (key);

  // use a reference so this is easier to read
  Knowledge_Record & record = found->second;

  // check if we have the appropriate write quality
  if (record.write_quality < record.quality)
    return -2;

  // otherwise go ahead and update
  ++record.value;

  if (key[0] != '.')
    record.status = Madara::Knowledge_Record::MODIFIED;
  else
    record.scope = Madara::Knowledge_Record::LOCAL_SCOPE;

  changed_.signal ();

  return record.value;
}

// return whether or not the key exists
inline bool
Madara::Knowledge_Engine::Thread_Safe_Context::exists (const ::std::string & key) const
{
  Context_Guard guard (mutex_);

  // if key is not null
  if (key != "")
  {
    // find the key in the knowledge base
    Knowledge_Map::const_iterator found = map_.find (key);

    // if it's found, then return the value
    if (found != map_.end ())
      return true;
  }

  // if no match, return empty (0)
  return false;
}

// Atomically decrement a stored value. Only reason we are inlining this function
// is because it is called by only one function, and we can save a bit of
// execution time via expansion into that function call.
inline int
Madara::Knowledge_Engine::Thread_Safe_Context::dec (const ::std::string & key)
{
  // check for null key
  if (key == "")
    return -1;

  // enter the mutex
  Context_Guard guard (mutex_);

  // creat the key if it didn't exist
  map_[key];

  // find the key in the knowledge base. It will be
  // much faster to use the iterator for modifications
  // than it will be to look up the key in the map for
  // everything we want to modify (e.g. map_[key].quality = 1)
  Knowledge_Map::iterator found = map_.find (key);

  // use a reference so this is easier to read
  Knowledge_Record & record = found->second;

  // check if we have the appropriate write quality
  if (record.write_quality < record.quality)
    return -2;

  // otherwise go ahead and update
  --record.value;

  if (key[0] != '.')
    record.status = Madara::Knowledge_Record::MODIFIED;
  else
    record.scope = Madara::Knowledge_Record::LOCAL_SCOPE;

  changed_.signal ();

  return record.value;
}

/// set the lamport clock (updates with lamport clocks lower
/// than our current clock get discarded)
inline unsigned long  
Madara::Knowledge_Engine::Thread_Safe_Context::set_clock (unsigned long clock)
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
inline unsigned long  
Madara::Knowledge_Engine::Thread_Safe_Context::set_clock (
  const std::string & key, unsigned long clock)
{
  Context_Guard guard (mutex_);

  // find the key in the knowledge base
  Knowledge_Map::iterator found = map_.find (key);

  // if it's found, then compare the value
  if (found != map_.end ())
  {
    // check for value already set
    if (found->second.clock < clock)
    {
      found->second.clock = clock;

      // try to update the global clock as well
      this->set_clock (clock);
    }

    return found->second.clock;
  }
  else
    // key does not exist
    return 0;
}

/// set the lamport clock for a particular variable (updates with 
/// lamport clocks lower than our current clock get discarded)
inline unsigned long  
Madara::Knowledge_Engine::Thread_Safe_Context::inc_clock (
  const std::string & key)
{
  Context_Guard guard (mutex_);

  // find the key in the knowledge base
  Knowledge_Map::iterator found = map_.find (key);

  // if it's found, then compare the value
  if (found != map_.end ())
  {
    return ++found->second.clock;
  }
  else
    // key does not exist
    return 0;
}

/// increment the process lamport clock
inline unsigned long 
Madara::Knowledge_Engine::Thread_Safe_Context::inc_clock (void)
{
  Context_Guard guard (mutex_);
  return ++clock_;
}

/// get the lamport clock (updates with lamport clocks lower
/// than our current clock get discarded)
inline unsigned long 
Madara::Knowledge_Engine::Thread_Safe_Context::get_clock (void)
{
  Context_Guard guard (mutex_);
  return clock_;
}

/// get the lamport clock for a particular variable
inline unsigned long 
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
  const std::string & statement) const
{
  // enter the mutex
  Context_Guard guard (mutex_);
  ACE_DEBUG ((LM_INFO, this->expand_statement (statement).c_str ()));
}

// clear all variables and their values
inline void
Madara::Knowledge_Engine::Thread_Safe_Context::clear (void)
{
  // enter the mutex
  Context_Guard guard (mutex_);

  map_.clear ();
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

/// Return list of variables that have been modified
inline void
Madara::Knowledge_Engine::Thread_Safe_Context::get_modified (
  Madara::String_Vector & modified) const
{
  modified.clear ();
  Context_Guard guard (mutex_);
  for (Madara::Knowledge_Map::const_iterator i = map_.begin ();
       i != map_.end (); 
       ++i)
  {
    if (Madara::Knowledge_Record::MODIFIED == i->second.status)
      modified.push_back (i->first);
  }
}

/// Reset all variables to unmodified
inline void 
Madara::Knowledge_Engine::Thread_Safe_Context::reset_modified (void)
{
  Context_Guard guard (mutex_);
  for (Madara::Knowledge_Map::iterator i = map_.begin ();
       i != map_.end (); 
       ++i)
  {
    if (Madara::Knowledge_Record::MODIFIED == i->second.status)
      i->second.status = Madara::Knowledge_Record::UNMODIFIED;
  }
}

/// Reset a variable to unmodified
inline void 
Madara::Knowledge_Engine::Thread_Safe_Context::reset_modified (
  const std::string & variable)
{
  Context_Guard guard (mutex_);
  Madara::Knowledge_Map::iterator i = map_.find (variable);
  
  if (i->second.status == Madara::Knowledge_Record::MODIFIED)
    i->second.status = Madara::Knowledge_Record::UNMODIFIED;
}


/// Signal the condition that it can wake up someone else on changed data.
inline void
Madara::Knowledge_Engine::Thread_Safe_Context::signal (void) const
{
  Context_Guard guard (mutex_);
  changed_.signal ();
}

#endif // _MADARA_THREADSAFECONTEXT_INL_
