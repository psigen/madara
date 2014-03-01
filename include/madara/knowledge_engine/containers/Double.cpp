#include "Double.h"


Madara::Knowledge_Engine::Containers::Double::Double (
  const Knowledge_Update_Settings & settings)
: context_ (0), settings_ (settings) 
{
}
  
Madara::Knowledge_Engine::Containers::Double::Double (
  const std::string & name,
  Knowledge_Base & knowledge,
  const Knowledge_Update_Settings & settings)
: context_ (&(knowledge.get_context ())), name_ (name), settings_ (settings) 
{
  variable_ = knowledge.get_ref (name);
}
 
Madara::Knowledge_Engine::Containers::Double::Double (
  const std::string & name,
  Variables & knowledge,
  const Knowledge_Update_Settings & settings)
: context_ (knowledge.get_context ()), name_ (name), settings_ (settings) 
{
  variable_ = knowledge.get_ref (name);
}
 
Madara::Knowledge_Engine::Containers::Double::Double (
  const std::string & name,
  Knowledge_Base & knowledge,
  type value,
  const Knowledge_Update_Settings & settings)
: context_ (&(knowledge.get_context ())), name_ (name), settings_ (settings) 
{
  variable_ = knowledge.get_ref (name);
  context_->set (variable_, value, settings);
}
 
Madara::Knowledge_Engine::Containers::Double::Double (
  const std::string & name,
  Variables & knowledge,
  type value,
  const Knowledge_Update_Settings & settings)
: context_ (knowledge.get_context ()), name_ (name), settings_ (settings) 
{
  variable_ = knowledge.get_ref (name);
  context_->set (variable_, value, settings);
}


Madara::Knowledge_Engine::Containers::Double::Double (const Double & rhs)
  : context_ (rhs.context_),
    name_ (rhs.name_),
    variable_ (rhs.variable_),
    settings_ (rhs.settings_)
{

}


Madara::Knowledge_Engine::Containers::Double::~Double ()
{

}

void
Madara::Knowledge_Engine::Containers::Double::exchange (
  Containers::Double & other)
{
  Guard guard (mutex_), guard2 (other.mutex_);

  type temp = *other;
  other = **this;
  *this = temp;
}

std::string
Madara::Knowledge_Engine::Containers::Double::get_name (void)
{
  Guard guard (mutex_);
  return name_;
}

void
Madara::Knowledge_Engine::Containers::Double::set_name (
  const std::string & var_name,
  Knowledge_Base & knowledge)
{
  context_ = &(knowledge.get_context ());
  name_ = var_name;
  variable_ = context_->get_ref (name_, settings_);
}

void
Madara::Knowledge_Engine::Containers::Double::set_name (
  const std::string & var_name,
  Variables & knowledge)
{
  context_ = knowledge.get_context ();
  name_ = var_name;
  variable_ = context_->get_ref (name_, settings_);
}

Madara::Knowledge_Engine::Containers::Double::type
Madara::Knowledge_Engine::Containers::Double::operator= (type value)
{
  Guard guard (mutex_);
  
  if (context_)
  {
    context_->set (variable_, value, settings_);
  }

  return value;
}
  
bool
Madara::Knowledge_Engine::Containers::Double::operator== (type value)
{
  Guard guard (mutex_);
  
  if (context_)
  {
    return context_->get (variable_, settings_) == value;
  }

  return false;
}

bool
Madara::Knowledge_Engine::Containers::Double::operator != (type value)
{
  Guard guard (mutex_);
  
  if (context_)
  {
    return context_->get (variable_, settings_) != value;
  }

  return true;
}

bool
Madara::Knowledge_Engine::Containers::Double::operator== (const Double & value)
{
  Guard guard (mutex_);
  
  if (context_)
  {
    return
      context_->get (variable_, settings_) ==
        value.context_->get (value.variable_, value.settings_);
  }

  return false;
}

bool
Madara::Knowledge_Engine::Containers::Double::operator != (const Double & value)
{
  Guard guard (mutex_);
  
  if (context_)
  {
    return
      context_->get (variable_, settings_) !=
        value.context_->get (value.variable_, value.settings_);
  }

  return true;
}

bool
Madara::Knowledge_Engine::Containers::Double::operator< (type value)
{
  Guard guard (mutex_);
  
  if (context_)
  {
    return context_->get (variable_, settings_) < value;
  }

  return false;
}

bool
Madara::Knowledge_Engine::Containers::Double::operator<= (type value)
{
  Guard guard (mutex_);
  
  if (context_)
  {
    return context_->get (variable_, settings_) <= value;
  }

  return false;
}

bool
Madara::Knowledge_Engine::Containers::Double::operator> (type value)
{
  Guard guard (mutex_);
  
  if (context_)
  {
    return context_->get (variable_, settings_) > value;
  }

  return false;
}

bool
Madara::Knowledge_Engine::Containers::Double::operator>= (type value)
{
  Guard guard (mutex_);
  
  if (context_)
  {
    return context_->get (variable_, settings_) >= value;
  }

  return false;
}

Madara::Knowledge_Engine::Containers::Double::type
Madara::Knowledge_Engine::Containers::Double::operator* (void)
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
Madara::Knowledge_Engine::Containers::Double::to_integer (void)
{
  Guard guard (mutex_);
  
  if (context_)
  {
    return context_->get (variable_, settings_).to_integer ();
  }
  else
    return 0;
}
   
std::string
Madara::Knowledge_Engine::Containers::Double::to_string (void)
{
  Guard guard (mutex_);
  
  if (context_)
  {
    return context_->get (variable_, settings_).to_string ();
  }
  else
    return "";
}

void
Madara::Knowledge_Engine::Containers::Double::set_quality (
  uint32_t quality,
  const Knowledge_Reference_Settings & settings)
{
  Guard guard (mutex_);
  
  if (context_)
  {
    context_->set_quality (name_, quality, true, settings);
  }
}
