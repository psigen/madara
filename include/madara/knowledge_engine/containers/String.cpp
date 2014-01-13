#include "String.h"

Madara::Knowledge_Engine::Containers::String::String (
  const Eval_Settings & settings)
: knowledge_ (0), settings_ (settings) 
{
}
   

Madara::Knowledge_Engine::Containers::String::String (
  const std::string & name,
  Knowledge_Base & knowledge,
  const Eval_Settings & settings)
: knowledge_ (&knowledge), name_ (name), settings_ (settings) 
{
  variable_ = knowledge.get_ref (name);
}
   
Madara::Knowledge_Engine::Containers::String::String (
  const std::string & name,
  Knowledge_Base & knowledge,
  const std::string & value,
  const Eval_Settings & settings)
: knowledge_ (&knowledge), name_ (name), settings_ (settings) 
{
  variable_ = knowledge.get_ref (name);
  knowledge.set (variable_, value);
}
       

Madara::Knowledge_Engine::Containers::String::String (const String & rhs)
  : knowledge_ (rhs.knowledge_),
    name_ (rhs.name_),
    variable_ (rhs.variable_),
    settings_ (rhs.settings_)
{

}


Madara::Knowledge_Engine::Containers::String::~String ()
{

}

std::string
Madara::Knowledge_Engine::Containers::String::get_name (void)
{
  Guard guard (mutex_);
  return name_;
}

void
Madara::Knowledge_Engine::Containers::String::set_name (
  const std::string & var_name,
  Knowledge_Base & knowledge)
{
  knowledge_ = &knowledge;
  name_ = var_name;
  variable_ = knowledge_->get_ref (name_, settings_);
}

Madara::Knowledge_Engine::Containers::String::type
Madara::Knowledge_Engine::Containers::String::operator= (const type & value)
{
  Guard guard (mutex_);

  if (knowledge_)
  {
    knowledge_->set (variable_, value, settings_);
  }

  return value;
}
    
Madara::Knowledge_Engine::Containers::String::type
Madara::Knowledge_Engine::Containers::String::operator* (void)
{
  Guard guard (mutex_);
  
  if (knowledge_)
  {
    return knowledge_->get (variable_, settings_).to_string ();
  }
  else
    return "";
}
   
double
Madara::Knowledge_Engine::Containers::String::to_double (void)
{
  Guard guard (mutex_);
  
  if (knowledge_)
  {
    return knowledge_->get (variable_, settings_).to_double ();
  }
  else
    return 0.0;
}
   
Madara::Knowledge_Record::Integer
Madara::Knowledge_Engine::Containers::String::to_integer (void)
{
  Guard guard (mutex_);
  
  if (knowledge_)
  {
    return knowledge_->get (variable_, settings_).to_integer ();
  }
  else
    return 0;
}

void
Madara::Knowledge_Engine::Containers::String::set_quality (
  uint32_t quality,
  const Knowledge_Reference_Settings & settings)
{
  Guard guard (mutex_);

  if (knowledge_)
    knowledge_->set_quality (name_, quality, settings);
}
