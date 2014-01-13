#include "Integer.h"


Madara::Knowledge_Engine::Containers::Integer::Integer (
  const Eval_Settings & settings)
: knowledge_ (0), settings_ (settings) 
{
}
  
Madara::Knowledge_Engine::Containers::Integer::Integer (
  const std::string & name,
  Knowledge_Base & knowledge,
  const Eval_Settings & settings)
: knowledge_ (&knowledge), name_ (name), settings_ (settings) 
{
  variable_ = knowledge.get_ref (name);
}
      

Madara::Knowledge_Engine::Containers::Integer::Integer (const Integer & rhs)
  : knowledge_ (rhs.knowledge_),
    name_ (rhs.name_),
    variable_ (rhs.variable_),
    settings_ (rhs.settings_)
{

}


Madara::Knowledge_Engine::Containers::Integer::~Integer ()
{

}

std::string
Madara::Knowledge_Engine::Containers::Integer::get_name (void)
{
  Guard guard (mutex_);
  return name_;
}

void
Madara::Knowledge_Engine::Containers::Integer::set_name (
  const std::string & var_name,
  Knowledge_Base & knowledge)
{
  knowledge_ = &knowledge;
  name_ = var_name;
  variable_ = knowledge_->get_ref (name_, settings_);
}

Madara::Knowledge_Engine::Containers::Integer::type
Madara::Knowledge_Engine::Containers::Integer::operator= (type value)
{
  Guard guard (mutex_);
  
  if (knowledge_)
  {
    knowledge_->set (variable_, value, settings_);
  }

  return value;
}
    
Madara::Knowledge_Engine::Containers::Integer::type
Madara::Knowledge_Engine::Containers::Integer::operator* (void)
{
  Guard guard (mutex_);
  
  if (knowledge_)
  {
    return knowledge_->get (variable_, settings_).to_integer ();
  }
  else
    return 0;
}
   
double
Madara::Knowledge_Engine::Containers::Integer::to_double (void)
{
  Guard guard (mutex_);
  
  if (knowledge_)
  {
    return knowledge_->get (variable_, settings_).to_double ();
  }
  else
    return 0.0;
}
   
std::string
Madara::Knowledge_Engine::Containers::Integer::to_string (void)
{
  Guard guard (mutex_);
  
  if (knowledge_)
  {
    return knowledge_->get (variable_, settings_).to_string ();
  }
  else
    return "";
}

void
Madara::Knowledge_Engine::Containers::Integer::set_quality (
  uint32_t quality,
  const Knowledge_Reference_Settings & settings)
{
  Guard guard (mutex_);
  
  if (knowledge_)
  {
    knowledge_->set_quality (name_, quality, settings);
  }
}
