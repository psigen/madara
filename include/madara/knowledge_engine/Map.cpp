#include "Map.h"



Madara::Knowledge_Engine::Map::Map (
  const std::string & name,
  Knowledge_Base & knowledge,
  const Eval_Settings & settings)
: knowledge_ (knowledge), name_ (name), settings_ (settings)
{
  std::map <std::string, Knowledge_Record> contents;
  std::string common = name + ".";
  knowledge_.to_map (common, contents);

  if (contents.size () > 0)
  {
    for (std::map <std::string, Knowledge_Record>::iterator i =
      contents.begin (); i != contents.end (); ++i)
    {
      map_[i->first.substr (common.size ())] =
        knowledge.get_ref (i->first, settings);
    }
  }
}
      

Madara::Knowledge_Engine::Map::Map (const Map & rhs)
  : knowledge_ (rhs.knowledge_), name_ (rhs.name_),
   map_ (rhs.map_), settings_ (rhs.settings_)
{

}

Madara::Knowledge_Engine::Map::~Map ()
{

}


Madara::Knowledge_Record
Madara::Knowledge_Engine::Map::operator[] (const std::string & key)
{
  std::stringstream buffer;
  buffer << name_;
  buffer << '.';
  buffer << key;

  std::string final_key = buffer.str ();
  std::map <std::string, Variable_Reference>::iterator entry =
    map_.find (final_key);

  if (entry == map_.end ())
  {
    Variable_Reference ref = knowledge_.get_ref (final_key, settings_);
    map_[key] = ref;
    return knowledge_.get (ref, settings_);
  }

  return knowledge_.get (entry->second, settings_);
}

unsigned int
Madara::Knowledge_Engine::Map::size (void)
{
  return map_.size ();
}

void
Madara::Knowledge_Engine::Map::keys (std::vector <std::string> & curkeys)
{
  curkeys.resize (map_.size ());
  unsigned int j = 0;

  for (std::map <std::string, Variable_Reference>::iterator i = map_.begin ();
       i != map_.end (); ++i, ++j)
  {
    curkeys[j] = i->first;
  }
}

int
Madara::Knowledge_Engine::Map::read_file (
  const std::string & key,
  const std::string & filename, 
  const Eval_Settings & settings)
{
  std::stringstream buffer;
  buffer << name_;
  buffer << '.';
  buffer << key;

  std::string final_key = buffer.str ();
  std::map <std::string, Variable_Reference>::iterator entry =
    map_.find (final_key);

  if (entry == map_.end ())
  {
    Variable_Reference ref = knowledge_.get_ref (final_key, settings_);
    map_[key] = ref;
    return knowledge_.read_file (ref, filename, settings_);
  }
  
  return knowledge_.read_file (entry->second, filename, settings_);
}
      
int Madara::Knowledge_Engine::Map::set (const std::string & key,
  Madara::Knowledge_Record::Integer value, 
  const Eval_Settings & settings)
{
  std::stringstream buffer;
  buffer << name_;
  buffer << '.';
  buffer << key;

  std::string final_key = buffer.str ();
  std::map <std::string, Variable_Reference>::iterator entry =
    map_.find (final_key);

  if (entry == map_.end ())
  {
    Variable_Reference ref = knowledge_.get_ref (final_key, settings_);
    map_[key] = ref;
    return knowledge_.set (ref, value, settings_);
  }
  
  return knowledge_.set (entry->second, value, settings_);
}


int Madara::Knowledge_Engine::Map::set_index (const std::string & key,
  size_t index,
  Madara::Knowledge_Record::Integer value,
  const Eval_Settings & settings)
{
  std::stringstream buffer;
  buffer << name_;
  buffer << '.';
  buffer << key;

  std::string final_key = buffer.str ();
  std::map <std::string, Variable_Reference>::iterator entry =
    map_.find (final_key);

  if (entry == map_.end ())
  {
    Variable_Reference ref = knowledge_.get_ref (final_key, settings_);
    map_[key] = ref;
    return knowledge_.set_index (ref, index, value, settings_);
  }
  
  return knowledge_.set_index (entry->second, index, value, settings_);
}


int Madara::Knowledge_Engine::Map::set (const std::string & key,
  const Madara::Knowledge_Record::Integer * value,
  uint32_t size,
  const Eval_Settings & settings)
{
  std::stringstream buffer;
  buffer << name_;
  buffer << '.';
  buffer << key;

  std::string final_key = buffer.str ();
  std::map <std::string, Variable_Reference>::iterator entry =
    map_.find (final_key);

  if (entry == map_.end ())
  {
    Variable_Reference ref = knowledge_.get_ref (final_key, settings_);
    map_[key] = ref;
    return knowledge_.set (ref, value, size, settings_);
  }
  
  return knowledge_.set (entry->second, value, size, settings_);
}
       
int Madara::Knowledge_Engine::Map::set (const std::string & key,
  const std::vector <Knowledge_Record::Integer> & value,
  const Eval_Settings & settings)
{
  std::stringstream buffer;
  buffer << name_;
  buffer << '.';
  buffer << key;

  std::string final_key = buffer.str ();
  std::map <std::string, Variable_Reference>::iterator entry =
    map_.find (final_key);

  if (entry == map_.end ())
  {
    Variable_Reference ref = knowledge_.get_ref (final_key, settings_);
    map_[key] = ref;
    return knowledge_.set (ref, value, settings_);
  }
  
  return knowledge_.set (entry->second, value, settings_);
}
       
int Madara::Knowledge_Engine::Map::set (const std::string & key,
  double value, 
  const Eval_Settings & settings)
{
  std::stringstream buffer;
  buffer << name_;
  buffer << '.';
  buffer << key;

  std::string final_key = buffer.str ();
  std::map <std::string, Variable_Reference>::iterator entry =
    map_.find (final_key);

  if (entry == map_.end ())
  {
    Variable_Reference ref = knowledge_.get_ref (final_key, settings_);
    map_[key] = ref;
    return knowledge_.set (ref, value, settings_);
  }
  
  return knowledge_.set (entry->second, value, settings_);
}


int Madara::Knowledge_Engine::Map::set_index (const std::string & key,
  size_t index,
  double value,
  const Eval_Settings & settings)
{
  std::stringstream buffer;
  buffer << name_;
  buffer << '.';
  buffer << key;

  std::string final_key = buffer.str ();
  std::map <std::string, Variable_Reference>::iterator entry =
    map_.find (final_key);

  if (entry == map_.end ())
  {
    Variable_Reference ref = knowledge_.get_ref (final_key, settings_);
    map_[key] = ref;
    return knowledge_.set_index (ref, index, value, settings_);
  }
  
  return knowledge_.set_index (entry->second, index, value, settings_);
}


int Madara::Knowledge_Engine::Map::set (const std::string & key,
  const double * value,
  uint32_t size,
  const Eval_Settings & settings)
{
  std::stringstream buffer;
  buffer << name_;
  buffer << '.';
  buffer << key;

  std::string final_key = buffer.str ();
  std::map <std::string, Variable_Reference>::iterator entry =
    map_.find (final_key);

  if (entry == map_.end ())
  {
    Variable_Reference ref = knowledge_.get_ref (final_key, settings_);
    map_[key] = ref;
    return knowledge_.set (ref, value, size, settings_);
  }
  
  return knowledge_.set (entry->second, value, size, settings_);
}
       

int Madara::Knowledge_Engine::Map::set (const std::string & key,
  const std::vector <double> & value,
  const Eval_Settings & settings)
{
  std::stringstream buffer;
  buffer << name_;
  buffer << '.';
  buffer << key;

  std::string final_key = buffer.str ();
  std::map <std::string, Variable_Reference>::iterator entry =
    map_.find (final_key);

  if (entry == map_.end ())
  {
    Variable_Reference ref = knowledge_.get_ref (final_key, settings_);
    map_[key] = ref;
    return knowledge_.set (ref, value, settings_);
  }
  
  return knowledge_.set (entry->second, value, settings_);
}
        

int Madara::Knowledge_Engine::Map::set (const std::string & key,
  const std::string & value, 
  const Eval_Settings & settings)
{
  std::stringstream buffer;
  buffer << name_;
  buffer << '.';
  buffer << key;

  std::string final_key = buffer.str ();
  std::map <std::string, Variable_Reference>::iterator entry =
    map_.find (final_key);

  if (entry == map_.end ())
  {
    Variable_Reference ref = knowledge_.get_ref (final_key, settings_);
    map_[key] = ref;
    return knowledge_.set (ref, value, settings_);
  }
  
  return knowledge_.set (entry->second, value, settings_);
}


void Madara::Knowledge_Engine::Map::set_quality (
  const std::string & key,
  uint32_t quality,
  const Knowledge_Reference_Settings & settings)
{
  std::stringstream buffer;
  buffer << name_;
  buffer << '.';
  buffer << key;

  knowledge_.set_quality (buffer.str (), quality, settings);
}
      

int Madara::Knowledge_Engine::Map::set_file (const std::string & key,
  const unsigned char * value, size_t size, 
  const Eval_Settings & settings)
{
  std::stringstream buffer;
  buffer << name_;
  buffer << '.';
  buffer << key;

  std::string final_key = buffer.str ();
  std::map <std::string, Variable_Reference>::iterator entry =
    map_.find (final_key);

  if (entry == map_.end ())
  {
    Variable_Reference ref = knowledge_.get_ref (final_key, settings_);
    map_[key] = ref;
    return knowledge_.set_file (ref, value, size, settings_);
  }
  
  return knowledge_.set_file (entry->second, value, size, settings_);
}
      

int Madara::Knowledge_Engine::Map::set_jpeg (const std::string & key,
  const unsigned char * value, size_t size, 
  const Eval_Settings & settings)
{
  std::stringstream buffer;
  buffer << name_;
  buffer << '.';
  buffer << key;

  std::string final_key = buffer.str ();
  std::map <std::string, Variable_Reference>::iterator entry =
    map_.find (final_key);

  if (entry == map_.end ())
  {
    Variable_Reference ref = knowledge_.get_ref (final_key, settings_);
    map_[key] = ref;
    return knowledge_.set_jpeg (ref, value, size, settings_);
  }
  
  return knowledge_.set_jpeg (entry->second, value, size, settings_);
}
