#include <iostream>
#include <sstream>

#include "madara/utility/Utility.h"

#include "madara/knowledge_engine/Thread_Safe_Context.h"
#include "ace/Log_Msg.h"


// constructor
Madara::Knowledge_Engine::Thread_Safe_Context::Thread_Safe_Context ()
: changed_ (mutex_), clock_ (0)
{
  expansion_splitters_.push_back ("{");
  expansion_splitters_.push_back ("}");
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
  return Madara::Knowledge_Record::UNMODIFIED;
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
    if (key[0] != '.')
      map_[key].status = Madara::Knowledge_Record::MODIFIED;
    else
      map_[key].scope = Madara::Knowledge_Record::LOCAL_SCOPE;
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
    if (key[0] != '.')
      map_[key].status = Madara::Knowledge_Record::MODIFIED;
    else
      map_[key].scope = Madara::Knowledge_Record::LOCAL_SCOPE;
    changed_.signal ();

    return value;
  }

  // if no match, return -1 (ERROR)
  return -1;
}

// set the value of a variable
int
Madara::Knowledge_Engine::Thread_Safe_Context::set (
  const ::std::string & key, long value, bool modified)
{
  // check for null key
  if (key == "")
    return -1;

  // enter the mutex
  Context_Guard guard (mutex_);

  // check if we have the appropriate write quality
  if (this->get_write_quality (key) < this->get_quality (key))
    return -2;

  map_[key].value = value;
  map_[key].quality = map_[key].write_quality;
  
  // otherwise set the value
  if (key[0] != '.')
  {
    if (modified)
      map_[key].status = Madara::Knowledge_Record::MODIFIED;
  }
  else
    map_[key].scope = Madara::Knowledge_Record::LOCAL_SCOPE;

  changed_.signal ();

  return 0;
}

/// get quality of last update to a variable.
/// @return    quality of the variable 
unsigned long 
Madara::Knowledge_Engine::Thread_Safe_Context::get_quality (const ::std::string & key)
{
  // enter the mutex
  Context_Guard guard (mutex_);

  // find the key in the knowledge base
  Knowledge_Map::iterator found = map_.find (key);

  // create the variable if it has never been written to before
  // and update its current value quality to the quality parameter

  if (found != map_.end ())
    return map_[key].quality;

  // default quality is 0
  return 0;
}

/// get quality of last update to a variable.
/// @return    quality of the variable 
unsigned long 
Madara::Knowledge_Engine::Thread_Safe_Context::get_write_quality (const ::std::string & key)
{
  // enter the mutex
  Context_Guard guard (mutex_);

  // find the key in the knowledge base
  Knowledge_Map::iterator found = map_.find (key);

  // create the variable if it has never been written to before
  // and update its current value quality to the quality parameter

  if (found != map_.end ())
    return map_[key].write_quality;

  // default quality is 0
  return 0;
}

/// Set quality of last update to a variable.
/// @return    quality of the variable after this call
unsigned long 
Madara::Knowledge_Engine::Thread_Safe_Context::set_quality (
  const ::std::string & key, unsigned long quality,
                           bool force_update)
{
  // enter the mutex
  Context_Guard guard (mutex_);

  // find the key in the knowledge base
  Knowledge_Map::iterator found = map_.find (key);

  // create the variable if it has never been written to before
  // and update its current value quality to the quality parameter

  if (found == map_.end () || force_update || quality > found->second.quality)
    map_[key].quality = quality;

  // return current quality
  return map_[key].quality;
}
      
/// Set quality of this process writing to a variable
void 
Madara::Knowledge_Engine::Thread_Safe_Context::set_write_quality (
  const ::std::string & key, unsigned long quality)
{
  // enter the mutex
  Context_Guard guard (mutex_);

  // create the variable if it has never been written to before
  // and update its local process write quality to the quality parameter
  map_[key].write_quality = quality;
}

/// Set if the variable value will be different. Always updates clock to
/// highest value
/// @return   1 if the value was changed. 0 if not changed. 
///           -1 if null key, -2 if quality not high enough
int
Madara::Knowledge_Engine::Thread_Safe_Context::set_if_unequal (
  const ::std::string & key, long value,
  unsigned long quality, unsigned long clock,
  bool modified)
{
  int result = 1;

  // check for null key
  if (key == "")
    return -1;

  // enter the mutex
  Context_Guard guard (mutex_);

  // find the key in the knowledge base
  Knowledge_Map::iterator found = map_.find (key);

  // if it's found, then compare the value
  if (found != map_.end ())
  {
    // if we do not have enough quality to update the variable
    // return -2
    if (quality < found->second.quality)
      result = -2;

    // if we have the same quality, but our clock value
    // is less than what we've already seen, then return -3
    else if (quality == found->second.quality && 
             clock < found->second.clock)
      result = -3;

    // check for value already set
    else if (found->second.value == value)
      result = 0;
  }

  // if we need to update quality, then update it
  if (result != -2 && map_[key].quality != quality)
    map_[key].quality = quality;

  // if we need to update the variable clock, then update it
  if (clock > map_[key].clock)
    map_[key].clock = clock;

  // if we need to update the global clock, then update it
  if (clock > this->clock_)
    this->clock_ = clock;

  if (result == 1)
  {
    // we have a situation where the value needs to be changed
    map_[key].value = value;
  
    // otherwise set the value
    if (key[0] != '.')
    {
      if (modified)
        map_[key].status = Madara::Knowledge_Record::MODIFIED;
    }
    else
      map_[key].scope = Madara::Knowledge_Record::LOCAL_SCOPE;

    changed_.signal ();
  }

  // value was changed
  return result;
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

/// Expand a string with variable expansions. This is a generic form of
/// the function found in Variable_Node, which is optimized to never change
/// keys.
std::string 
Madara::Knowledge_Engine::Thread_Safe_Context::expand_statement (
  const std::string & key) const
{
  // enter the mutex
  Context_Guard guard (mutex_);

  // vectors for holding parsed tokens and pivot_list
  std::vector< std::string> tokens;
  std::vector< std::string> pivot_list;
  unsigned int count = 1;
  unsigned int subcount = 0;
  unsigned int begin_exp = 0;

  std::stringstream builder;

  // iterate over the input string
  for (unsigned int i = 0; i < key.size (); ++i)
  {
    // if this is an open brace, increase the subcount
    if (key[i] == '{')
    {
      ++subcount;
      if (subcount == 1)
        begin_exp = i;
    }
    // closed brace should decrease subcount
    else if (key[i] == '}')
    {
      if (subcount == 1)
      {
        std::string expandable = key.substr (begin_exp + 1, i - begin_exp - 1);
        std::string results = this->expand_statement (expandable);
        builder << this->get (results);
      }
      --subcount;
    }
    // otherwise, if this subcount is 0, then we need to add it to our output
    // we allow anything not in subcount == 0 to be handled through recursion
    else
    {
      if (subcount == 0)
        builder << key[i];
    }
  }

  // check to see if all brace counts are appropriate
  if (subcount != 0)
  {
    ACE_DEBUG ((LM_DEBUG, 
      "EXPAND_ERROR 1: Improperly matched braces in %s\n", key.c_str ()));
  }

  return builder.str ();
}

/// set the lamport clock (updates with lamport clocks lower
/// than our current clock get discarded)
unsigned long  
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
unsigned long  
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
unsigned long  
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
unsigned long 
Madara::Knowledge_Engine::Thread_Safe_Context::inc_clock (void)
{
  Context_Guard guard (mutex_);
  return ++clock_;
}

/// get the lamport clock (updates with lamport clocks lower
/// than our current clock get discarded)
unsigned long 
Madara::Knowledge_Engine::Thread_Safe_Context::get_clock (void)
{
  Context_Guard guard (mutex_);
  return clock_;
}

/// get the lamport clock for a particular variable
unsigned long 
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

/// Print a statement, similar to printf (variable expressions allowed)
/// e.g. input = "MyVar{.id} = {MyVar{.id}}\n";
void 
Madara::Knowledge_Engine::Thread_Safe_Context::print (
  const std::string & statement) const
{
  // enter the mutex
  Context_Guard guard (mutex_);
  ACE_DEBUG ((LM_INFO, this->expand_statement (statement).c_str ()));
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

/// Return list of variables that have been modified
void
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
void 
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
void 
Madara::Knowledge_Engine::Thread_Safe_Context::reset_modified (
  const std::string & variable)
{
  Context_Guard guard (mutex_);
  Madara::Knowledge_Map::iterator i = map_.find (variable);
  
  if (i->second.status == Madara::Knowledge_Record::MODIFIED)
    i->second.status = Madara::Knowledge_Record::UNMODIFIED;
}


/// Signal the condition that it can wake up someone else on changed data.
void
Madara::Knowledge_Engine::Thread_Safe_Context::signal (void) const
{
  Context_Guard guard (mutex_);
  changed_.signal ();
}