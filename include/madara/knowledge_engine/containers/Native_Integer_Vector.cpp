#include "Native_Integer_Vector.h"
#include "madara/knowledge_engine/Context_Guard.h"

Madara::Knowledge_Engine::Containers::Native_Integer_Vector::Native_Integer_Vector (
  const Knowledge_Update_Settings & settings)
: context_ (0), settings_ (settings) 
{
}
   
Madara::Knowledge_Engine::Containers::Native_Integer_Vector::Native_Integer_Vector (
  const std::string & name,
  Knowledge_Base & knowledge,
  int size,
  const Knowledge_Update_Settings & settings)
: context_ (&(knowledge.get_context ())), name_ (name), settings_ (true, false)
{
  vector_ = knowledge.get_ref (name, settings_);
  resize (size);
  settings_ = settings;
}
  
Madara::Knowledge_Engine::Containers::Native_Integer_Vector::Native_Integer_Vector (
  const std::string & name,
  Variables & knowledge,
  int size,
  const Knowledge_Update_Settings & settings)
: context_ (knowledge.get_context ()), name_ (name), settings_ (true, false)
{
  vector_ = knowledge.get_ref (name, settings);
  resize (size);
  settings_ = settings;
}

Madara::Knowledge_Engine::Containers::Native_Integer_Vector::Native_Integer_Vector (
  const Native_Integer_Vector & rhs)
: context_ (rhs.context_),
    name_ (rhs.name_),
    vector_ (rhs.vector_),
    settings_ (rhs.settings_)
{

}


Madara::Knowledge_Engine::Containers::Native_Integer_Vector::~Native_Integer_Vector ()
{

}
  
void
Madara::Knowledge_Engine::Containers::Native_Integer_Vector::modify (void)
{
  Context_Guard context_guard (*context_);
  if (context_ && name_ != "")
  {
    context_->mark_modified (vector_);
  }
}
 
void
Madara::Knowledge_Engine::Containers::Native_Integer_Vector::operator= (
  const Native_Integer_Vector & rhs)
{
  if (this != &rhs)
  {
    Guard guard (mutex_), guard2 (rhs.mutex_);

    this->context_ = rhs.context_;
    this->name_ = rhs.name_;
    this->settings_ = rhs.settings_;
    this->vector_ = rhs.vector_;
  }
}

void
Madara::Knowledge_Engine::Containers::Native_Integer_Vector::resize (
  size_t size)
{
  if (context_ && name_ != "")
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);

    Knowledge_Record value = context_->get (vector_, settings_);
    if (value.type () != Knowledge_Record::DOUBLE_ARRAY)
    {
      std::vector <double> new_array (size, 0.0);
      value.set_value (new_array);
    }
    else
    {
      value.resize (size);
    }
    context_->set (vector_, value, settings_);
  }
}

size_t
Madara::Knowledge_Engine::Containers::Native_Integer_Vector::size (void) const
{
  Context_Guard context_guard (*context_);
  Guard guard (mutex_);

  return context_->get (vector_, settings_).size ();
}

std::string
Madara::Knowledge_Engine::Containers::Native_Integer_Vector::get_name (
  void) const
{
  Guard guard (mutex_);
  return name_;
}

void
Madara::Knowledge_Engine::Containers::Native_Integer_Vector::set_name (
  const std::string & var_name,
  Knowledge_Base & knowledge, int size)
{
  if (context_ != &(knowledge.get_context ()) || name_ != var_name)
  {
    context_ = &(knowledge.get_context ());

    Context_Guard context_guard (*context_);
    Guard guard (mutex_);

    name_ = var_name;

    vector_ = knowledge.get_ref (var_name, settings_);

    if (size > 0)
      resize (size_t (size));
  }
}

void
Madara::Knowledge_Engine::Containers::Native_Integer_Vector::set_name (
  const std::string & var_name,
  Variables & knowledge, int size)
{
  if (context_ != knowledge.get_context () || name_ != var_name)
  {
    context_ = knowledge.get_context ();

    Context_Guard context_guard (*context_);
    Guard guard (mutex_);

    name_ = var_name;
    
    vector_ = knowledge.get_ref (var_name, settings_);

    if (size > 0)
      resize (size_t (size));
  }
}

void
Madara::Knowledge_Engine::Containers::Native_Integer_Vector::set_name (
  const std::string & var_name,
  Thread_Safe_Context & knowledge, int size)
{
  if (context_ != &knowledge || name_ != var_name)
  {
    context_ = &knowledge;

    Context_Guard context_guard (*context_);
    Guard guard (mutex_);

    name_ = var_name;
    
    vector_ = knowledge.get_ref (var_name, settings_);

    if (size > 0)
      resize (size_t (size));
  }
}

void
Madara::Knowledge_Engine::Containers::Native_Integer_Vector::exchange (
  Native_Integer_Vector & other)
{
  if (context_ && other.context_)
  {
    Context_Guard context_guard (*context_);
    Context_Guard other_context_guard (*other.context_);
    Guard guard (mutex_), guard2 (other.mutex_);

    Knowledge_Record temp (context_->get (other.vector_));

    context_->set (other.vector_, context_->get (vector_), other.settings_);
    context_->set (vector_, temp, settings_);
  }
}

void
Madara::Knowledge_Engine::Containers::Native_Integer_Vector::transfer_to (
  Native_Integer_Vector & other)
{
  if (context_ && other.context_)
  {
    Context_Guard context_guard (*context_);
    Context_Guard other_context_guard (*other.context_);
    Guard guard (mutex_);
    Guard guard2 (other.mutex_);

    size_t other_size = other.size ();
    size_t this_size = this->size ();

    if (this_size > 0)
    {
      size_t size = other_size + this_size;
      other.resize ((int)size);

      Knowledge_Record rhs (other.context_->get (other.vector_));
      Knowledge_Record lhs (context_->get (vector_));

      rhs.set_index (size - 1, lhs.retrieve_index (this_size - 1).to_double ());

      for (size_t i = 0, j = other_size; i < this_size - 1; ++i, ++j)
      {
        rhs.set_index (other_size, lhs.retrieve_index (i).to_double ());
      }

      other.context_->set (other.vector_, rhs, other.settings_);

      this->resize (0);
    }
  }
}

void
Madara::Knowledge_Engine::Containers::Native_Integer_Vector::copy_to (
  Knowledge_Vector & target) const
{
  if (context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);

    target.resize (size ());

    for (size_t i = 0; i < target.size (); ++i)
    {
      target[i].deep_copy ((*this)[i]);
    }
  }
}

Madara::Knowledge_Engine::Containers::Native_Integer_Vector::type
Madara::Knowledge_Engine::Containers::Native_Integer_Vector::operator[] (
  size_t index) const
{
  Knowledge_Record result;

  if (context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    result = context_->get (vector_, settings_);

    if (index < result.size ())
      result = result.retrieve_index (index);
  }

  return result.to_double ();
}

int
Madara::Knowledge_Engine::Containers::Native_Integer_Vector::set (
  unsigned int index,
  type value)
{
  int result = -1;

  if (context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    result = context_->set_index (vector_, index, value, settings_);
  }
  
  return result;
}

int
Madara::Knowledge_Engine::Containers::Native_Integer_Vector::set (
  unsigned int index,
  type value, 
  const Knowledge_Update_Settings & settings)
{
  int result = -1;

  if (context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    result = context_->set_index (vector_, index, value, settings);
  }
  
  return result;
}

int
Madara::Knowledge_Engine::Containers::Native_Integer_Vector::set (
  const std::vector <type> & value)
{
  int result = -1;
  
  if (context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    context_->set (vector_, value, settings_);
  }
  
  return result;
}
 
int
Madara::Knowledge_Engine::Containers::Native_Integer_Vector::set (
  const std::vector <type> & value,
  const Knowledge_Update_Settings & settings)
{
  int result = -1;
  
  if (context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    context_->set (vector_, value, settings);
  }
  
  return result;
}     

Madara::Knowledge_Engine::Knowledge_Update_Settings
Madara::Knowledge_Engine::Containers::Native_Integer_Vector::set_settings (
  const Knowledge_Update_Settings & settings)
{
  Guard guard (mutex_);
  
  Knowledge_Update_Settings old_settings = settings_;

  settings_ = settings;

  return old_settings;
}

void
Madara::Knowledge_Engine::Containers::Native_Integer_Vector::set_quality (
  unsigned int index,
  uint32_t quality,
  const Knowledge_Reference_Settings & settings)
{
  if (context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    context_->set_quality (name_, quality, true, settings);
  }
}

Madara::Knowledge_Record
Madara::Knowledge_Engine::Containers::Native_Integer_Vector::to_record (
  size_t index) const
{
  Madara::Knowledge_Record result = context_->get (this->vector_, settings_);
  result = result.retrieve_index (index);
  return result;
}

Madara::Knowledge_Record
Madara::Knowledge_Engine::Containers::Native_Integer_Vector::to_record (
  void) const
{
  return context_->get (this->vector_, settings_);
}
