#include <iostream>
#include "madara/knowledge_engine/Thread_Safe_Context.h"
#include "ace/Log_Msg.h"


// constructor
Madara::Knowledge_Engine::Thread_Safe_Context::Thread_Safe_Context ()
: changed_ (mutex_)
{
}

// destructor
Madara::Knowledge_Engine::Thread_Safe_Context::~Thread_Safe_Context (void)
{
}

// return the value of a variable
int
Madara::Knowledge_Engine::Thread_Safe_Context::get (const ::std::string & key) const
{
  Context_Guard guard (mutex_);

  // if key is not null
  if (key != "")
  {
    // find the key in the knowledge base
    Knowledge_Map::const_iterator found = map_.find (key);

    // if it's found, then return the value
    if (found != map_.end ())
      return found->second.value;
  }

  // if no match, return empty (0)
  return Madara::Knowledge_Record::UNSET;
}

// return whether or not the key exists
bool
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

// Atomically increment a stored value
int
Madara::Knowledge_Engine::Thread_Safe_Context::inc (const ::std::string & key)
{
  Context_Guard guard (mutex_);

  // if key is not null
  if (key != "")
  {
    // because someone might be waiting on a context change, we
    // can't just return the value

    int value = ++(map_[key].value);
    changed_.signal ();

    return value;
  }

  // if no match, return -1 (ERROR)
  return -1;
}

// Atomically decrement a stored value
int
Madara::Knowledge_Engine::Thread_Safe_Context::dec (const ::std::string & key)
{
  Context_Guard guard (mutex_);

  // if key is not null
  if (key != "")
  {
    // because someone might be waiting on a context change, we
    // can't just return the value

    int value = --(map_[key].value);
    changed_.signal ();

    return value;
  }

  // if no match, return -1 (ERROR)
  return -1;
}

// set the value of a variable
int
Madara::Knowledge_Engine::Thread_Safe_Context::set (const ::std::string & key, long value)
{
  // check for null key
  if (key == "")
    return -1;

  // enter the mutex
  Context_Guard guard (mutex_);

  map_[key].value = value;
  
  // otherwise set the value
  map_[key].status = map_[key].status | Madara::Knowledge_Record::SET;

  changed_.signal ();

  return 0;
}

/// Set if the variable value will be different
/// @return   1 if the value was changed. 0 if not changed. -1 if null key
int
Madara::Knowledge_Engine::Thread_Safe_Context::set_if_unequal (const ::std::string & key, 
                                             long value)
{
  // check for null key
  if (key == "")
    return -1;

  // enter the mutex
  Context_Guard guard (mutex_);

  // find the key in the knowledge base
  Knowledge_Map::iterator found = map_.find (key);

  // if it's found, then compare the value
  if (found != map_.end ())
    // check for value already set
    if (found->second.value == value)
      return 0;

  // we have a situation where the value needs to be changed
  map_[key].value = value;
  
  // otherwise set the value
  map_[key].status = map_[key].status | Madara::Knowledge_Record::SET;

  changed_.signal ();

  // value was changed
  return 1;
}

// print all variables and their values
void
Madara::Knowledge_Engine::Thread_Safe_Context::print (void) const
{
  Context_Guard guard (mutex_);
  for (Madara::Knowledge_Map::const_iterator i = map_.begin ();
       i != map_.end (); 
       ++i)
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s=%d\n", 
              i->first.c_str (), i->second.value));
}

// clear all variables and their values
void
Madara::Knowledge_Engine::Thread_Safe_Context::clear (void)
{
  // enter the mutex
  Context_Guard guard (mutex_);

  map_.clear ();
  changed_.signal ();
}

/// Lock the mutex on this context. Warning: this will cause
/// all operations to block until the unlock call is made.
void
Madara::Knowledge_Engine::Thread_Safe_Context::lock (void) const
{
  mutex_.acquire ();
}

/// Unlock the mutex on this context.
void
Madara::Knowledge_Engine::Thread_Safe_Context::unlock (void) const
{
  mutex_.release ();
}


/// Make the current thread of execution wait for a change on the
/// context
void 
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

/// Signal the condition that it can wake up someone else on changed data.
void
Madara::Knowledge_Engine::Thread_Safe_Context::signal (void) const
{
  changed_.signal ();
}