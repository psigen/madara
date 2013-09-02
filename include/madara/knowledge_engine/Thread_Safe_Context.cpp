#include <iostream>
#include <sstream>

#include <string.h>

#include "madara/utility/Utility.h"

#include "madara/knowledge_engine/Thread_Safe_Context.h"
#include "madara/utility/Log_Macros.h"
#include "madara/expression_tree/Interpreter.h"
#include "madara/knowledge_engine/File_Header.h"
#include "madara/transport/Transport.h"
#include <stdio.h>
#include <time.h>


// constructor
Madara::Knowledge_Engine::Thread_Safe_Context::Thread_Safe_Context ()
  : changed_ (mutex_), clock_ (0),
  interpreter_ (new Madara::Expression_Tree::Interpreter ())
{
  expansion_splitters_.push_back ("{");
  expansion_splitters_.push_back ("}");
}

// destructor
Madara::Knowledge_Engine::Thread_Safe_Context::~Thread_Safe_Context (void)
{
  delete interpreter_;
}

/**
 * Retrieves a knowledge record from the key. This function is useful
 * for performance reasons and also for using a Knowledge_Record that
 * can be one of multiple types
 **/
Madara::Knowledge_Record *
Madara::Knowledge_Engine::Thread_Safe_Context::get_record (
  const std::string & key,
  const Knowledge_Reference_Settings & settings)
{
  std::string key_actual;
  const std::string * key_ptr;
  Context_Guard guard (mutex_);

  if (settings.expand_variables)
  {
    key_actual = expand_statement (key);
    key_ptr = &key_actual;
  }
  else
    key_ptr = &key;

  if (*key_ptr == "")
    return 0;

  // if the variable doesn't exist, hash maps create a record automatically
  // when used in this manner
  return &map_[*key_ptr];
}


Madara::Knowledge_Engine::Variable_Reference
Madara::Knowledge_Engine::Thread_Safe_Context::get_ref (
  const std::string & key,
  const Knowledge_Reference_Settings & settings)
{
  std::string key_actual;
  const std::string * key_ptr;
  Context_Guard guard (mutex_);

  Variable_Reference record;

  // expand the key if the user asked for it
  if (settings.expand_variables)
  {
    key_actual = expand_statement (key);
    key_ptr = &key_actual;
  }
  else
    key_ptr = &key;
  
  // set name to the expanded key, for debugging purposes
  record.set_name (*key_ptr);

  // if the key is possible, create a reference to the record
  if (*key_ptr != "")
  {
    record.record_ = &map_[*key_ptr];
  }
  return record;
}

// set the value of a variable
int
Madara::Knowledge_Engine::Thread_Safe_Context::set (
  const Variable_Reference & variable,
  Madara::Knowledge_Record::Integer value,
  const Knowledge_Update_Settings & settings)
{
  Context_Guard guard (mutex_);
  if (variable.record_)
  {
    // check if we have the appropriate write quality
    if (!settings.always_overwrite &&
        variable.record_->write_quality < variable.record_->quality)
      return -2;

    variable.record_->set_value (value);
    variable.record_->quality = variable.record_->write_quality;
  
    // otherwise set the value
    if (variable.name_.get_ptr ()[0] != '.')
    {
      if (!settings.treat_globals_as_locals)
      {
        mark_modified (variable.name_.get_ptr (), *(variable.record_),
          Knowledge_Engine::Knowledge_Reference_Settings (false));
      }
    }

    if (settings.signal_changes)
      changed_.signal ();
  }
  else
    return -1;

  return 0;
}

// set the value of a variable
int
Madara::Knowledge_Engine::Thread_Safe_Context::set (
  const Variable_Reference & variable,
  const Madara::Knowledge_Record & value,
  const Knowledge_Update_Settings & settings)
{
  Context_Guard guard (mutex_);
  if (variable.record_)
  {
    // check if we have the appropriate write quality
    if (!settings.always_overwrite &&
        variable.record_->write_quality < variable.record_->quality)
      return -2;

    // copy size and type from the record value
    variable.record_->size_ = value.size_;
    variable.record_->type_ = value.type_;

    // copy the value from the record value
    if (     value.type () == Knowledge_Record::INTEGER)
      variable.record_->int_value_ = value.int_value_;

    else if (value.type () == Knowledge_Record::DOUBLE)
      variable.record_->double_value_ = value.double_value_;
    
    else if (value.type () == Knowledge_Record::INTEGER_ARRAY)
      variable.record_->int_array_ = value.int_array_;

    else if (value.type () == Knowledge_Record::DOUBLE_ARRAY)
      variable.record_->double_array_ = value.double_array_;
    
    else if (value.is_string_type ())
      variable.record_->str_value_ = value.str_value_;

    else if (value.is_binary_file_type ())
      variable.record_->file_value_ = value.file_value_;

    variable.record_->quality = variable.record_->write_quality;
  
    // otherwise set the value
    if (variable.name_.get_ptr ()[0] != '.')
    {
      if (!settings.treat_globals_as_locals)
      {
        mark_modified (variable.name_.get_ptr (), *(variable.record_),
          Knowledge_Engine::Knowledge_Reference_Settings (false));
      }
    }

    if (settings.signal_changes)
      changed_.signal ();
  }
  else
    return -1;

  return 0;
}


// set the value of an array index
int
Madara::Knowledge_Engine::Thread_Safe_Context::set_index (
  const Variable_Reference & variable, size_t index,
  Madara::Knowledge_Record::Integer value,
  const Knowledge_Update_Settings & settings)
{
  Context_Guard guard (mutex_);
  if (variable.record_)
  {
    // check if we have the appropriate write quality
    if (!settings.always_overwrite &&
        variable.record_->write_quality < variable.record_->quality)
      return -2;

    variable.record_->set_index (index, value);
    variable.record_->quality = variable.record_->write_quality;
  
    // otherwise set the value
    if (variable.name_.get_ptr ()[0] != '.')
    {
      if (!settings.treat_globals_as_locals)
      {
        mark_modified (variable.name_.get_ptr (), *(variable.record_),
          Knowledge_Engine::Knowledge_Reference_Settings (false));
      }
    }

    if (settings.signal_changes)
      changed_.signal ();
  }
  else
    return -1;

  return 0;
}

// set the value of a variable
int
Madara::Knowledge_Engine::Thread_Safe_Context::set (
  const Variable_Reference & variable,
  const Knowledge_Record::Integer * value,
  uint32_t size,
  const Knowledge_Update_Settings & settings)
{
  Context_Guard guard (mutex_);
  if (variable.record_)
  {
    // check if we have the appropriate write quality
    if (!settings.always_overwrite &&
        variable.record_->write_quality < variable.record_->quality)
      return -2;

    variable.record_->set_value (value, size);
    variable.record_->quality = variable.record_->write_quality;
  
    // otherwise set the value
    if (variable.name_.get_ptr ()[0] != '.')
    {
      if (!settings.treat_globals_as_locals)
      {
        mark_modified (variable.name_.get_ptr (), *(variable.record_),
          Knowledge_Engine::Knowledge_Reference_Settings (false));
      }
    }

    if (settings.signal_changes)
      changed_.signal ();
  }
  else
    return -1;

  return 0;
}

// set the value of a variable
int
Madara::Knowledge_Engine::Thread_Safe_Context::set (
  const Variable_Reference & variable,
  const std::vector <Knowledge_Record::Integer> & value,
  const Knowledge_Update_Settings & settings)
{
  Context_Guard guard (mutex_);
  if (variable.record_)
  {
    // check if we have the appropriate write quality
    if (!settings.always_overwrite &&
        variable.record_->write_quality < variable.record_->quality)
      return -2;

    variable.record_->set_value (value);
    variable.record_->quality = variable.record_->write_quality;
  
    // otherwise set the value
    if (variable.name_.get_ptr ()[0] != '.')
    {
      if (!settings.treat_globals_as_locals)
      {
        mark_modified (variable.name_.get_ptr (), *(variable.record_),
          Knowledge_Engine::Knowledge_Reference_Settings (false));
      }
    }

    if (settings.signal_changes)
      changed_.signal ();
  }
  else
    return -1;

  return 0;
}

// set the value of a variable
int
Madara::Knowledge_Engine::Thread_Safe_Context::set (
  const Variable_Reference & variable,
  double value,
  const Knowledge_Update_Settings & settings)
{
  Context_Guard guard (mutex_);
  if (variable.record_)
  {
    // check if we have the appropriate write quality
    if (!settings.always_overwrite &&
        variable.record_->write_quality < variable.record_->quality)
      return -2;

    variable.record_->set_value (value);
    variable.record_->quality = variable.record_->write_quality;
  
    // otherwise set the value
    if (variable.name_.get_ptr ()[0] != '.')
    {
      if (!settings.treat_globals_as_locals)
      {
        mark_modified (variable.name_.get_ptr (), *(variable.record_),
          Knowledge_Engine::Knowledge_Reference_Settings (false));
      }
    }

    if (settings.signal_changes)
      changed_.signal ();
  }
  else
    return -1;

  return 0;
}

// set the value of a variable
int
Madara::Knowledge_Engine::Thread_Safe_Context::set_index (
  const Variable_Reference & variable, size_t index,
  double value,
  const Knowledge_Update_Settings & settings)
{
  Context_Guard guard (mutex_);
  if (variable.record_)
  {
    // check if we have the appropriate write quality
    if (!settings.always_overwrite &&
        variable.record_->write_quality < variable.record_->quality)
      return -2;

    variable.record_->set_index (index, value);
    variable.record_->quality = variable.record_->write_quality;
  
    // otherwise set the value
    if (variable.name_.get_ptr ()[0] != '.')
    {
      if (!settings.treat_globals_as_locals)
      {
        mark_modified (variable.name_.get_ptr (), *(variable.record_),
          Knowledge_Engine::Knowledge_Reference_Settings (false));
      }
    }

    if (settings.signal_changes)
      changed_.signal ();
  }
  else
    return -1;

  return 0;
}

// set the value of a variable
int
Madara::Knowledge_Engine::Thread_Safe_Context::set (
  const Variable_Reference & variable,
  const double * value,
  uint32_t size,
  const Knowledge_Update_Settings & settings)
{
  Context_Guard guard (mutex_);
  if (variable.record_)
  {
    // check if we have the appropriate write quality
    if (!settings.always_overwrite &&
        variable.record_->write_quality < variable.record_->quality)
      return -2;

    variable.record_->set_value (value, size);
    variable.record_->quality = variable.record_->write_quality;
  
    // otherwise set the value
    if (variable.name_.get_ptr ()[0] != '.')
    {
      if (!settings.treat_globals_as_locals)
      {
        mark_modified (variable.name_.get_ptr (), *(variable.record_),
          Knowledge_Engine::Knowledge_Reference_Settings (false));
      }
    }

    if (settings.signal_changes)
      changed_.signal ();
  }
  else
    return -1;

  return 0;
}

// set the value of a variable
int
Madara::Knowledge_Engine::Thread_Safe_Context::set (
  const Variable_Reference & variable,
  const std::vector <double> & value,
  const Knowledge_Update_Settings & settings)
{
  Context_Guard guard (mutex_);
  if (variable.record_)
  {
    // check if we have the appropriate write quality
    if (!settings.always_overwrite &&
        variable.record_->write_quality < variable.record_->quality)
      return -2;

    variable.record_->set_value (value);
    variable.record_->quality = variable.record_->write_quality;
  
    // otherwise set the value
    if (variable.name_.get_ptr ()[0] != '.')
    {
      if (!settings.treat_globals_as_locals)
      {
        mark_modified (variable.name_.get_ptr (), *(variable.record_),
          Knowledge_Engine::Knowledge_Reference_Settings (false));
      }
    }

    if (settings.signal_changes)
      changed_.signal ();
  }
  else
    return -1;

  return 0;
}

// set the value of a variable
int
Madara::Knowledge_Engine::Thread_Safe_Context::set (
  const Variable_Reference & variable,
  const std::string & value,
  const Knowledge_Update_Settings & settings)
{
  Context_Guard guard (mutex_);
  if (variable.record_)
  {
    // check if we have the appropriate write quality
    if (!settings.always_overwrite &&
        variable.record_->write_quality < variable.record_->quality)
      return -2;

    variable.record_->set_value (value);
    variable.record_->quality = variable.record_->write_quality;
  
    // otherwise set the value
    if (variable.name_.get_ptr ()[0] != '.')
    {
      if (!settings.treat_globals_as_locals)
      {
        mark_modified (variable.name_.get_ptr (), *(variable.record_),
          Knowledge_Engine::Knowledge_Reference_Settings (false));
      }
    }

    if (settings.signal_changes)
      changed_.signal ();
  }
  else
    return -1;

  return 0;
}

// set the value of a variable
int
Madara::Knowledge_Engine::Thread_Safe_Context::set_xml (
  const Variable_Reference & variable,
  const char * value, size_t size,
  const Knowledge_Update_Settings & settings)
{
  
  Context_Guard guard (mutex_);
  if (variable.record_)
  {
    // check if we have the appropriate write quality
    if (!settings.always_overwrite &&
        variable.record_->write_quality < variable.record_->quality)
      return -2;

    variable.record_->set_xml (value, size);
    variable.record_->quality = variable.record_->write_quality;
  
    // otherwise set the value
    if (variable.name_.get_ptr ()[0] != '.')
    {
      if (!settings.treat_globals_as_locals)
      {
        mark_modified (variable.name_.get_ptr (), *(variable.record_),
          Knowledge_Engine::Knowledge_Reference_Settings (false));
      }
    }

    if (settings.signal_changes)
      changed_.signal ();
  }
  else
    return -1;

  return 0;
}

// set the value of a variable
int
Madara::Knowledge_Engine::Thread_Safe_Context::set_text (
  const Variable_Reference & variable,
  const char * value, size_t size,
  const Knowledge_Update_Settings & settings)
{
  Context_Guard guard (mutex_);
  if (variable.record_)
  {
    // check if we have the appropriate write quality
    if (!settings.always_overwrite &&
        variable.record_->write_quality < variable.record_->quality)
      return -2;

    variable.record_->set_text (value, size);
    variable.record_->quality = variable.record_->write_quality;
  
    // otherwise set the value
    if (variable.name_.get_ptr ()[0] != '.')
    {
      if (!settings.treat_globals_as_locals)
      {
        mark_modified (variable.name_.get_ptr (), *(variable.record_),
          Knowledge_Engine::Knowledge_Reference_Settings (false));
      }
    }

    if (settings.signal_changes)
      changed_.signal ();
  }
  else
    return -1;

  return 0;
}

// set the value of a variable
int
Madara::Knowledge_Engine::Thread_Safe_Context::set_jpeg (
  const Variable_Reference & variable,
  const unsigned char * value, size_t size,
  const Knowledge_Update_Settings & settings)
{
  Context_Guard guard (mutex_);
  if (variable.record_)
  {
    // check if we have the appropriate write quality
    if (!settings.always_overwrite &&
        variable.record_->write_quality < variable.record_->quality)
      return -2;

    variable.record_->set_jpeg (value, size);
    variable.record_->quality = variable.record_->write_quality;
  
    // otherwise set the value
    if (variable.name_.get_ptr ()[0] != '.')
    {
      if (!settings.treat_globals_as_locals)
      {
        mark_modified (variable.name_.get_ptr (), *(variable.record_),
          Knowledge_Engine::Knowledge_Reference_Settings (false));
      }
    }

    if (settings.signal_changes)
      changed_.signal ();
  }
  else
    return -1;

  return 0;
}

// set the value of a variable
int
Madara::Knowledge_Engine::Thread_Safe_Context::set_file (
  const Variable_Reference & variable,
  const unsigned char * value, size_t size,
  const Knowledge_Update_Settings & settings)
{
  Context_Guard guard (mutex_);
  if (variable.record_)
  {
    // check if we have the appropriate write quality
    if (!settings.always_overwrite &&
        variable.record_->write_quality < variable.record_->quality)
      return -2;

    variable.record_->set_file (value, size);
    variable.record_->quality = variable.record_->write_quality;
  
    // otherwise set the value
    if (variable.name_.get_ptr ()[0] != '.')
    {
      if (!settings.treat_globals_as_locals)
      {
        mark_modified (variable.name_.get_ptr (), *(variable.record_),
          Knowledge_Engine::Knowledge_Reference_Settings (false));
      }
    }

    if (settings.signal_changes)
      changed_.signal ();
  }
  else
    return -1;

  return 0;
}

// set the value of a variable
int
Madara::Knowledge_Engine::Thread_Safe_Context::read_file (
  const Variable_Reference & variable,
  const std::string & filename,
  const Knowledge_Update_Settings & settings)
{
  int return_value = 0;
  Context_Guard guard (mutex_);
  if (variable.record_)
  {
    // check if we have the appropriate write quality
    if (!settings.always_overwrite &&
        variable.record_->write_quality < variable.record_->quality)
      return -2;

    return_value = variable.record_->read_file (filename);
    variable.record_->quality = variable.record_->write_quality;
  
    // otherwise set the value
    if (variable.name_.get_ptr ()[0] != '.')
    {
      if (!settings.treat_globals_as_locals)
      {
        mark_modified (variable.name_.get_ptr (), *(variable.record_),
          Knowledge_Engine::Knowledge_Reference_Settings (false));
      }
    }

    if (settings.signal_changes)
      changed_.signal ();
  }
  else
    return return_value = -1;

  return return_value;
}

/// get quality of last update to a variable.
/// @return    quality of the variable 
uint32_t 
Madara::Knowledge_Engine::Thread_Safe_Context::get_quality (
  const std::string & key,
  const Knowledge_Reference_Settings & settings)
{
  // enter the mutex
  std::string key_actual;
  const std::string * key_ptr;
  Context_Guard guard (mutex_);
  
  if (settings.expand_variables)
  {
    key_actual = expand_statement (key);
    key_ptr = &key_actual;
  }
  else
    key_ptr = &key;
  
  // find the key in the knowledge base
  Knowledge_Map::iterator found = map_.find (*key_ptr);

  // create the variable if it has never been written to before
  // and update its current value quality to the quality parameter

  if (found != map_.end ())
    return map_[*key_ptr].quality;

  // default quality is 0
  return 0;
}

/// get quality of last update to a variable.
/// @return    quality of the variable 
uint32_t 
Madara::Knowledge_Engine::Thread_Safe_Context::get_write_quality (
  const std::string & key,
  const Knowledge_Reference_Settings & settings)
{
  // enter the mutex
  std::string key_actual;
  const std::string * key_ptr;
  Context_Guard guard (mutex_);
  
  if (settings.expand_variables)
  {
    key_actual = expand_statement (key);
    key_ptr = &key_actual;
  }
  else
    key_ptr = &key;

  // find the key in the knowledge base
  Knowledge_Map::iterator found = map_.find (*key_ptr);

  // create the variable if it has never been written to before
  // and update its current value quality to the quality parameter

  if (found != map_.end ())
    return map_[*key_ptr].write_quality;

  // default quality is 0
  return 0;
}

/// Set quality of last update to a variable.
/// @return    quality of the variable after this call
uint32_t 
Madara::Knowledge_Engine::Thread_Safe_Context::set_quality (
  const std::string & key, uint32_t quality,
                           bool force_update,
  const Knowledge_Reference_Settings & settings)
{
  // enter the mutex
  std::string key_actual;
  const std::string * key_ptr;
  Context_Guard guard (mutex_);
  
  if (settings.expand_variables)
  {
    key_actual = expand_statement (key);
    key_ptr = &key_actual;
  }
  else
    key_ptr = &key;
  
  // check for null key
  if (*key_ptr == "")
    return 0;
  
  // find the key in the knowledge base
  Knowledge_Map::iterator found = map_.find (*key_ptr);

  // create the variable if it has never been written to before
  // and update its current value quality to the quality parameter

  if (found == map_.end () || force_update || quality > found->second.quality)
    map_[*key_ptr].quality = quality;

  // return current quality
  return map_[*key_ptr].quality;
}
      
/// Set quality of this process writing to a variable
void 
Madara::Knowledge_Engine::Thread_Safe_Context::set_write_quality (
  const std::string & key, uint32_t quality,
  const Knowledge_Reference_Settings & settings)
{
  // enter the mutex
  std::string key_actual;
  const std::string * key_ptr;
  Context_Guard guard (mutex_);
  
  if (settings.expand_variables)
  {
    key_actual = expand_statement (key);
    key_ptr = &key_actual;
  }
  else
    key_ptr = &key;

  // create the variable if it has never been written to before
  // and update its local process write quality to the quality parameter
  map_[*key_ptr].write_quality = quality;
}

/// Set if the variable value will be different. Always updates clock to
/// highest value
/// @return   1 if the value was changed. 0 if not changed. 
///           -1 if null key, -2 if quality not high enough
int
Madara::Knowledge_Engine::Thread_Safe_Context::set_if_unequal (
  const std::string & key, Madara::Knowledge_Record::Integer value,
  uint32_t quality, uint64_t clock,
  const Knowledge_Update_Settings & settings)
{
  int result = 1;

  // enter the mutex
  std::string key_actual;
  const std::string * key_ptr;
  Context_Guard guard (mutex_);
  
  if (settings.expand_variables)
  {
    key_actual = expand_statement (key);
    key_ptr = &key_actual;
  }
  else
    key_ptr = &key;
  
  // check for null key
  if (*key_ptr == "")
    return -1;
  
  // find the key in the knowledge base
  Knowledge_Map::iterator found = map_.find (*key_ptr);

  // if it's found, then compare the value
  if (!settings.always_overwrite && found != map_.end ())
  {
    // setup a rhs
    Madara::Knowledge_Record rhs;
    rhs.set_value (value);

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
    else if ((found->second == rhs).is_true ())
      result = 0;
  }

  Madara::Knowledge_Record & record = map_[*key_ptr];

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
    record.set_value (value);
  
    // otherwise set the value
    if ((*key_ptr)[0] != '.')
    {
      if (!settings.treat_globals_as_locals)
      {
        mark_modified (*key_ptr, record,
          Knowledge_Reference_Settings (false));
      }
    }
    
    if (settings.signal_changes)
      changed_.signal ();
  }

  // value was changed
  return result;
}

/// Set if the variable value will be different. Always updates clock to
/// highest value
/// @return   1 if the value was changed. 0 if not changed. 
///           -1 if null key, -2 if quality not high enough
int
Madara::Knowledge_Engine::Thread_Safe_Context::set_if_unequal (
  const std::string & key, double value,
  uint32_t quality, uint64_t clock,
  const Knowledge_Update_Settings & settings)
{
  int result = 1;

  // enter the mutex
  std::string key_actual;
  const std::string * key_ptr;
  Context_Guard guard (mutex_);
  
  if (settings.expand_variables)
  {
    key_actual = expand_statement (key);
    key_ptr = &key_actual;
  }
  else
    key_ptr = &key;
  
  // check for null key
  if (*key_ptr == "")
    return -1;
  
  // find the key in the knowledge base
  Knowledge_Map::iterator found = map_.find (*key_ptr);

  // if it's found, then compare the value
  if (!settings.always_overwrite && found != map_.end ())
  {
    // setup a rhs
    Madara::Knowledge_Record rhs;
    rhs.set_value (value);

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
    else if ((found->second == rhs).is_true ())
      result = 0;
  }

  Madara::Knowledge_Record & record = map_[*key_ptr];

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
    record.set_value (value);
  
    // otherwise set the value
    if ((*key_ptr)[0] != '.')
    {
      if (!settings.treat_globals_as_locals)
      {
        mark_modified (*key_ptr, record,
          Knowledge_Reference_Settings (false));
      }
    }
    
    if (settings.signal_changes)
      changed_.signal ();
  }

  // value was changed
  return result;
}

/// Set if the variable value will be different. Always updates clock to
/// highest value
/// @return   1 if the value was changed. 0 if not changed. 
///           -1 if null key, -2 if quality not high enough
int
Madara::Knowledge_Engine::Thread_Safe_Context::set_if_unequal (
  const std::string & key, const std::string & value,
  uint32_t quality, uint64_t clock,
  const Knowledge_Update_Settings & settings)
{
  int result = 1;

  // enter the mutex
  std::string key_actual;
  const std::string * key_ptr;
  Context_Guard guard (mutex_);
  
  if (settings.expand_variables)
  {
    key_actual = expand_statement (key);
    key_ptr = &key_actual;
  }
  else
    key_ptr = &key;
  
  // check for null key
  if (*key_ptr == "")
    return -1;
  
  // find the key in the knowledge base
  Knowledge_Map::iterator found = map_.find (*key_ptr);

  // if it's found, then compare the value
  if (!settings.always_overwrite && found != map_.end ())
  {
    // setup a rhs
    Madara::Knowledge_Record rhs;
    rhs.set_value (value);

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
    else if ((found->second == rhs).is_true ())
      result = 0;
  }

  Madara::Knowledge_Record & record = map_[*key_ptr];

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
    record.set_value (value);
  
    // otherwise set the value
    if ((*key_ptr)[0] != '.')
    {
      if (!settings.treat_globals_as_locals)
      {
        mark_modified (*key_ptr, record,
          Knowledge_Reference_Settings (false));
      }
    }
    
    if (settings.signal_changes)
      changed_.signal ();
  }

  // value was changed
  return result;
}


/// Set if the variable value will be different. Always updates clock to
/// highest value
/// @return   1 if the value was changed. 0 if not changed. 
///           -1 if null key, -2 if quality not high enough
int
Madara::Knowledge_Engine::Thread_Safe_Context::update_record_from_external (
  const std::string & key, const Knowledge_Record & rhs,
  const Knowledge_Update_Settings & settings)
{
  int result = 1;

  // enter the mutex
  std::string key_actual;
  const std::string * key_ptr;
  Context_Guard guard (mutex_);
  
  if (settings.expand_variables)
  {
    key_actual = expand_statement (key);
    key_ptr = &key_actual;
  }
  else
    key_ptr = &key;
  
  // check for null key
  if (*key_ptr == "")
    return -1;
  
  // find the key in the knowledge base
  Knowledge_Map::iterator found = map_.find (*key_ptr);

  // if it's found, then compare the value
  if (!settings.always_overwrite && found != map_.end ())
  {
    // if we do not have enough quality to update the variable
    // return -2
    if (rhs.quality < found->second.quality)
      result = -2;

    // if we have the same quality, but our clock value
    // is less than what we've already seen, then return -3
    else if (rhs.quality == found->second.quality && 
             rhs.clock < found->second.clock)
      result = -3;

    // if we reach this point, then the record is safe to copy
    found->second = rhs;
    
    // otherwise set the value
    if ((*key_ptr)[0] != '.')
    {
      if (!settings.treat_globals_as_locals)
      {
        mark_modified (*key_ptr, found->second,
          Knowledge_Reference_Settings (false));
      }
    }
  }
  else
  {
    // if we reach this point, then we have to create the record
    Knowledge_Record & current_value = map_[*key_ptr];
    current_value = rhs;

    // otherwise set the value
    if ((*key_ptr)[0] != '.')
    {
      if (!settings.treat_globals_as_locals)
      {
        mark_modified (*key_ptr, current_value,
          Knowledge_Reference_Settings (false));
      }
    }
  }
  
  // if we need to update the global clock, then update it
  if (rhs.clock > this->clock_)
    this->clock_ = rhs.clock;
  
  if (settings.signal_changes)
    changed_.signal ();

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
    MADARA_DEBUG (level, (LM_DEBUG, 
      "%s=%s\n", i->first.c_str (), i->second.to_string (", ").c_str ()));
}

/// Expand a string with variable expansions. This is a generic form of
/// the function found in Variable_Node, which is optimized to never change
/// keys.
std::string 
Madara::Knowledge_Engine::Thread_Safe_Context::expand_statement (
  const std::string & statement)
{
  // enter the mutex
  Context_Guard guard (mutex_);

  // vectors for holding parsed tokens and pivot_list
  size_t subcount = 0;
  size_t begin_exp = 0;

  std::stringstream builder;

  // iterate over the input string
  for (std::string::size_type i = 0; i < statement.size (); ++i)
  {
    // if this is an open brace, increase the subcount
    if (statement[i] == '{')
    {
      ++subcount;
      if (subcount == 1)
        begin_exp = i;
    }
    // closed brace should decrease subcount
    else if (statement[i] == '}')
    {
      if (subcount == 1)
      {
        std::string expandable = 
          statement.substr (begin_exp + 1, i - begin_exp - 1);
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
        builder << statement[i];
    }
  }

  // check to see if all brace counts are appropriate
  if (subcount != 0)
  {
    MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
      "\nKARL COMPILE ERROR: Improperly matched braces in %s\n",
      statement.c_str ()));
    exit (-1);
  }

  return builder.str ();
}

// defines a function by name
void
Madara::Knowledge_Engine::Thread_Safe_Context::define_function (
  const std::string & name,
    VALUE_TYPE (*func) (Function_Arguments &, Variables &),
        const Knowledge_Reference_Settings & settings)
{
  // enter the mutex
  std::string key_actual;
  const std::string * key_ptr;
  Context_Guard guard (mutex_);
  
  if (settings.expand_variables)
  {
    key_actual = expand_statement (name);
    key_ptr = &key_actual;
  }
  else
    key_ptr = &name;
  
  // check for null key
  if (*key_ptr == "")
    return;
  
  functions_[*key_ptr].extern_named_ = 0;
  functions_[*key_ptr].extern_unnamed_ = func;
}

// defines a function by name
void
Madara::Knowledge_Engine::Thread_Safe_Context::define_function (
  const std::string & name,
    VALUE_TYPE (*func) (const char * name, Function_Arguments &, Variables &),
        const Knowledge_Reference_Settings & settings)
{
  // enter the mutex
  std::string key_actual;
  const std::string * key_ptr;
  Context_Guard guard (mutex_);
  
  if (settings.expand_variables)
  {
    key_actual = expand_statement (name);
    key_ptr = &key_actual;
  }
  else
    key_ptr = &name;
  
  // check for null key
  if (*key_ptr == "")
    return;
  
  functions_[*key_ptr].extern_named_ = func;
  functions_[*key_ptr].extern_unnamed_ = 0;
}


/**
  * Defines a MADARA KaRL function
  * @param  name       name of the function
  * @param  expression KaRL function body       
  **/
void
Madara::Knowledge_Engine::Thread_Safe_Context::define_function (const std::string & name,
  const std::string & expression,
  const Knowledge_Reference_Settings & settings)
{
  Compiled_Expression compiled = compile (expression);
  define_function (name, compiled, settings);
}
      
/**
  * Defines a MADARA KaRL function
  * @param  name       name of the function
  * @param  expression KaRL function body       
  **/
void
Madara::Knowledge_Engine::Thread_Safe_Context::define_function (const std::string & name,
  const Compiled_Expression & expression,
  const Knowledge_Reference_Settings & settings)
{
  // enter the mutex
  std::string key_actual;
  const std::string * key_ptr;
  Context_Guard guard (mutex_);
  
  if (settings.expand_variables)
  {
    key_actual = expand_statement (name);
    key_ptr = &key_actual;
  }
  else
    key_ptr = &name;
  
  // check for null key
  if (*key_ptr == "")
    return;
  
  functions_[*key_ptr].extern_unnamed_ = 0;
  functions_[*key_ptr].extern_named_ = 0;
  functions_[*key_ptr].function_contents_ = expression.expression;
}


/**
  * Retrieves an external function
  * @param  name       name of the function to retrieve
  * @param   settings  settings for applying the update
  * @return            the mapped external function
  **/
Madara::Knowledge_Engine::Function *
Madara::Knowledge_Engine::Thread_Safe_Context::retrieve_function (
  const std::string & name,
  const Knowledge_Reference_Settings & settings)
{
  // enter the mutex
  std::string key_actual;
  const std::string * key_ptr;
  Context_Guard guard (mutex_);
  
  if (settings.expand_variables)
  {
    key_actual = expand_statement (name);
    key_ptr = &key_actual;
  }
  else
    key_ptr = &name;
  
  // check for null key
  if (*key_ptr == "")
    return 0;
  
  return &functions_[*key_ptr];
}


Madara::Knowledge_Engine::Compiled_Expression
Madara::Knowledge_Engine::Thread_Safe_Context::compile (
  const std::string & expression)
{
  MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
      DLINFO "Thread_Safe_Context::compile:" \
      " compiling %s\n", expression.c_str ()));
  
  Context_Guard guard (mutex_);
  Compiled_Expression ce;
  ce.logic = expression;
  ce.expression = interpreter_->interpret (*this, expression);

  return ce;
}

Madara::Knowledge_Record
Madara::Knowledge_Engine::Thread_Safe_Context::evaluate (
  Compiled_Expression expression,
  const Knowledge_Update_Settings & settings)
{
  Context_Guard guard (mutex_);
  return expression.expression.evaluate (settings);
}

size_t
  Madara::Knowledge_Engine::Thread_Safe_Context::to_vector (
  const std::string & subject,
  unsigned int start,
  unsigned int end,
  std::vector <Knowledge_Record> & target)
{
  target.clear ();
  
  // enter the mutex
  Context_Guard guard (mutex_);

  if (end >= start)
  {
    target.resize (end - start + 1);

    for (unsigned int i = 0; start <= end; ++start, ++i)
    {
      std::stringstream buffer;
      buffer << subject;
      buffer << start;
      target[i] = get (buffer.str ());
    }
  }

  return target.size ();
}


size_t
  Madara::Knowledge_Engine::Thread_Safe_Context::to_map (
  const std::string & expression,
  std::map <std::string, Knowledge_Record> & target)
{
  target.clear ();
  
  std::string subject (expression);
  bool matches_found (false);

  // remove the wildcard and make this into a subject
  if (subject[subject.size () - 1] == '*')
    subject.resize (subject.size () - 1);

  // just in case a string implementation does not inline
  std::string::size_type subject_size = subject.size ();
  const char * subject_ptr = subject.c_str ();

  // enter the mutex
  Context_Guard guard (mutex_);

  // if expression is blank, assume the user wants all variables
  if (expression.size () == 0)
    target = map_;
  else
  {
    for (Knowledge_Map::iterator i = map_.begin ();
      i != map_.end (); ++i)
    {
      if (i->first.size () >= subject_size)
      {
        int result = strncmp (i->first.c_str (), subject_ptr, subject_size);
        if (result == 0)
        {
          // we have a match, add this to the map
          target[i->first] = i->second;
          matches_found = true;
        }
        else if (matches_found)
        {
          // we have already found matches, and now we're not seeing matches
          break;
        }
      }
    }
  }


  return target.size ();
}

size_t
Madara::Knowledge_Engine::Thread_Safe_Context::save_context (
  const std::string & filename,
  const std::string & id)
{
  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Thread_Safe_Context::save_context:" \
      " opening file %s\n", filename.c_str ()));
  
  size_t total_written (0);
  FILE * file = fopen (filename.c_str (), "wb");
  
  File_Header meta;
  meta.states = 1;
  strncpy (meta.originator, id.c_str (),
    sizeof (meta.originator) < id.size () + 1 ?
    sizeof (meta.originator) : id.size () + 1);

  if (file)
  {
    int64_t max_buffer (102800);
    int64_t buffer_remaining (max_buffer);
    Utility::Scoped_Array <char> buffer = new char [max_buffer];

    char * current = buffer.get_ptr ();
  
    MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "Thread_Safe_Context::save_context:" \
        " generating file meta\n"));
  
    current = meta.write (current, buffer_remaining);
  
    MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "Thread_Safe_Context::save_context:" \
        " writing records\n"));
  
    // lock the context
    Context_Guard guard (mutex_);

    for (Knowledge_Map::iterator i = map_.begin ();
         i != map_.end (); ++i)
    {
      // get the encoded size of the record for checking buffer boundaries
      int64_t encoded_size = i->second.get_encoded_size (i->first);
      meta.size += encoded_size;

      if (encoded_size > buffer_remaining)
      {
        /**
         * if the record is larger than the buffer we have remaining, then
         * write the buffer to the file
         **/
        current = buffer.get_ptr ();
        fwrite (current,
          (size_t) (max_buffer - buffer_remaining), 1, file);
        total_written += (size_t) (max_buffer - buffer_remaining);
        buffer_remaining = max_buffer;

        if (encoded_size > max_buffer)
        {
          /**
           * If the record is larger than the buffer, then we must allocate a
           * buffer large enough to write to it.
           **/
          buffer = new char [encoded_size];
          max_buffer = encoded_size;
          buffer_remaining = max_buffer;
          current = buffer.get_ptr ();
        } // end if larger than buffer
      } // end if larger than buffer remaining

      current = i->second.write (current, i->first, buffer_remaining);
    }
  
    if (buffer_remaining != max_buffer)
    {
      fwrite (buffer.get_ptr (),
        (size_t) (max_buffer - buffer_remaining), 1, file);
      total_written += (size_t) (max_buffer - buffer_remaining);
    }

    fseek (file, 0, SEEK_SET);

    meta.size = Madara::Utility::endian_swap (meta.size);

    fwrite (&meta.size, sizeof (int64_t), 1, file);

    fclose (file);
  }

  return meta.size;
}

size_t
Madara::Knowledge_Engine::Thread_Safe_Context::load_context (
  const std::string & filename, std::string & id,
  const Knowledge_Update_Settings & settings)
{
  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Thread_Safe_Context::load_context:" \
      " opening file %s\n", filename.c_str ()));
  
  // using ACE for writing to the destination file
  FILE * file = fopen (filename.c_str (), "rb");

  size_t total_read (0);

  if (file)
  {
    int64_t max_buffer (102800);
    int64_t buffer_remaining (max_buffer);

    Utility::Scoped_Array <char> buffer = new char [max_buffer];
    const char * current = buffer.get_ptr ();
    
    MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "Thread_Safe_Context::load_context:" \
        " reading file meta data\n"));
  
    buffer_remaining = (int64_t) fread (buffer.get_ptr (),
      1, max_buffer, file);
    total_read += (size_t) buffer_remaining;
    
    if (buffer_remaining > 0)
    {
      File_Header meta;
      current = meta.read (current, buffer_remaining);

      if (meta.size > (uint64_t) max_buffer)
      {
        /**
         * if the size of the file is larger than our buffer
         * then allocate a new buffer of the file size and
         * copy the old buffer over
         **/
        Utility::Scoped_Array <char> new_buffer (new char [meta.size]);
        memcpy (new_buffer.get_ptr (), buffer.get_ptr (), max_buffer);
        size_t size_difference = meta.size - max_buffer;

        int offset = (int) (current - buffer.get_ptr ());

        // set current to the same offset in the new buffer
        current = new_buffer.get_ptr () + offset;
        buffer_remaining = (int64_t) meta.size - (int64_t) offset;

        // read the rest of the file into the new_buffer
        buffer_remaining += (int64_t) fread (new_buffer.get_ptr () + max_buffer,
          1, size_difference, file);
        
        max_buffer = (int64_t) meta.size;
        total_read = (size_t) max_buffer;

        buffer = new_buffer;
      }

      Knowledge_Record record;
      std::string key;
      
      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
          DLINFO "Thread_Safe_Context::load_context:" \
          " reading records\n"));
  
      // lock the context
      Context_Guard guard (mutex_);

      // read the records
      while (buffer_remaining > 0)
      {
        // read each record and apply it to the context
        current = record.read (current, key, buffer_remaining);
        update_record_from_external (key, record, settings);
      } // while buffer_remaining > 0

    }  // if buffer_remaining > 0

    fclose (file);
  }

  return total_read;
}