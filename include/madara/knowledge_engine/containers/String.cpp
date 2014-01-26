#include "String.h"

Madara::Knowledge_Engine::Containers::String::String (
  const Eval_Settings & settings)
: context_ (0), settings_ (settings) 
{
}
   

Madara::Knowledge_Engine::Containers::String::String (
  const std::string & name,
  Knowledge_Base & knowledge,
  const Eval_Settings & settings)
: context_ (&(knowledge.get_context ())), name_ (name), settings_ (settings)
{
  variable_ = knowledge.get_ref (name);
}
   
Madara::Knowledge_Engine::Containers::String::String (
  const std::string & name,
  Knowledge_Base & knowledge,
  const std::string & value,
  const Eval_Settings & settings)
  : context_ (&(knowledge.get_context ())), name_ (name), settings_ (settings)
{
  variable_ = knowledge.get_ref (name);
  knowledge.set (variable_, value);
}
     
Madara::Knowledge_Engine::Containers::String::String (
  const std::string & name,
  Variables & knowledge,
  const Eval_Settings & settings)
: context_ (knowledge.get_context ()), name_ (name), settings_ (settings)
{
  variable_ = knowledge.get_ref (name);
}
   
Madara::Knowledge_Engine::Containers::String::String (
  const std::string & name,
  Variables & knowledge,
  const std::string & value,
  const Eval_Settings & settings)
  : context_ (knowledge.get_context ()), name_ (name), settings_ (settings)
{
  variable_ = knowledge.get_ref (name);
  knowledge.set (variable_, value);
}
      

Madara::Knowledge_Engine::Containers::String::String (const String & rhs)
  : context_ (rhs.context_),
    name_ (rhs.name_),
    variable_ (rhs.variable_),
    settings_ (rhs.settings_)
{

}


Madara::Knowledge_Engine::Containers::String::~String ()
{

}

void
Madara::Knowledge_Engine::Containers::String::exchange (String & other)
{
  Guard guard (mutex_), guard2 (other.mutex_);

  type temp = *other;
  other = **this;
  *this = temp;
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
  context_ = &(knowledge.get_context ());
  name_ = var_name;
  variable_ = context_->get_ref (name_, settings_);
}

void
Madara::Knowledge_Engine::Containers::String::set_name (
  const std::string & var_name,
  Variables & knowledge)
{
  context_ = knowledge.get_context ();
  name_ = var_name;
  variable_ = context_->get_ref (name_, settings_);
}

Madara::Knowledge_Engine::Containers::String::type
Madara::Knowledge_Engine::Containers::String::operator= (const type & value)
{
  Guard guard (mutex_);

  if (context_)
  {
    context_->set (variable_, value, settings_);
  }

  return value;
}
    
Madara::Knowledge_Engine::Containers::String::type
Madara::Knowledge_Engine::Containers::String::operator* (void)
{
  Guard guard (mutex_);
  
  if (context_)
  {
    return context_->get (variable_, settings_).to_string ();
  }
  else
    return "";
}
   
double
Madara::Knowledge_Engine::Containers::String::to_double (void)
{
  Guard guard (mutex_);
  
  if (context_)
  {
    return context_->get (variable_, settings_).to_double ();
  }
  else
    return 0.0;
}
   
Madara::Knowledge_Record::Integer
Madara::Knowledge_Engine::Containers::String::to_integer (void)
{
  Guard guard (mutex_);
  
  if (context_)
  {
    return context_->get (variable_, settings_).to_integer ();
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

  if (context_)
    context_->set_quality (name_, quality, true, settings);
}
