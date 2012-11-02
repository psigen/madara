#include <iostream>
#include <sstream>

#include "madara/utility/Utility.h"

#include "madara/knowledge_engine/Thread_Safe_Context.h"
#include "madara/utility/Log_Macros.h"


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
Madara::Knowledge_Record::VALUE_TYPE
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
  return Madara::Knowledge_Record::UNCREATED;
}

/**
 * Retrieves a knowledge record from the key. This function is useful
 * for performance reasons and also for using a Knowledge_Record that
 * can be one of multiple types
 **/
Madara::Knowledge_Record *
Madara::Knowledge_Engine::Thread_Safe_Context::get_record (
  const ::std::string & key)
{
  if (key == "")
    return 0;

  // enter the mutex
  Context_Guard guard (mutex_);

  // if the variable doesn't exist, hash maps create a record automatically
  // when used in this manner
  return &map_[expand_statement(key)];
}

// set the value of a variable
int
Madara::Knowledge_Engine::Thread_Safe_Context::set (
  const ::std::string & key,
  Madara::Knowledge_Record::VALUE_TYPE value,
  bool modified)
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

  record.value = value;
  record.quality = record.write_quality;
  
  // otherwise set the value
  if (key[0] != '.')
  {
    if (modified)
    {
      mark_modified (key, record);
    }
      //record.status = Madara::Knowledge_Record::MODIFIED;
  }
  //else
  //  record.scope = Madara::Knowledge_Record::LOCAL_SCOPE;

  changed_.signal ();

  return 0;
}

/// get quality of last update to a variable.
/// @return    quality of the variable 
uint32_t 
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
uint32_t 
Madara::Knowledge_Engine::Thread_Safe_Context::get_write_quality (
  const ::std::string & key)
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
uint32_t 
Madara::Knowledge_Engine::Thread_Safe_Context::set_quality (
  const ::std::string & key, uint32_t quality,
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
  const ::std::string & key, uint32_t quality)
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
  const ::std::string & key, Madara::Knowledge_Record::VALUE_TYPE value,
  uint32_t quality, uint64_t clock,
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

  Madara::Knowledge_Record & record = map_[key];

  // if we need to update quality, then update it
  if (result != -2 && record.quality != quality)
    record.quality = quality;

  // if we need to update the variable clock, then update it
  if (clock > record.clock)
    record.clock = clock;

  // if we need to update the global clock, then update it
  if (clock > this->clock_)
    this->clock_ = clock;

  if (result == 1)
  {
    // we have a situation where the value needs to be changed
    record.value = value;
  
    // otherwise set the value
    if (key[0] != '.')
    {
      if (modified)
      {
        mark_modified (key, record);
      }
    }
    //else
    //  map_[key].scope = Madara::Knowledge_Record::LOCAL_SCOPE;

    changed_.signal ();
  }

  // value was changed
  return result;
}

/// Indicate that a status change has occurred. This could be a message
/// from the transport to let the knowledge engine know that new agents
/// are available to send knowledge to.
void 
Madara::Knowledge_Engine::Thread_Safe_Context::set_changed (void)
{
  changed_.signal ();
}

// print all variables and their values
void
Madara::Knowledge_Engine::Thread_Safe_Context::print (
  unsigned int level) const
{
  Context_Guard guard (mutex_);
  for (Madara::Knowledge_Map::const_iterator i = map_.begin ();
       i != map_.end (); 
       ++i)
    MADARA_DEBUG (level, (LM_INFO, 
      "%s=%q\n", i->first.c_str (), i->second.value));
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
  unsigned int subcount = 0;
  unsigned int begin_exp = 0;

  std::stringstream builder;

  // iterate over the input string
  for (std::string::size_type i = 0; i < key.size (); ++i)
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
    MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
      "\nKARL COMPILE ERROR: Improperly matched braces in %s\n",
      key.c_str ()));
    exit (-1);
  }

  return builder.str ();
}

// defines a function by name
void
Madara::Knowledge_Engine::Thread_Safe_Context::define_function (
  const std::string & name, VALUE_TYPE (*func) (Variables *))
{
  functions_[name] = func;
}


/**
  * Retrieves an external function
  * @param  name       name of the function to retrieve
  * @param  func       pointer to the function pointer
  * @return            the mapped external function
  **/
Madara::Knowledge_Engine::Function *
  Madara::Knowledge_Engine::Thread_Safe_Context::retrieve_function (
       const std::string & name)
{
  return &functions_[name];
}
