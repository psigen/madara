#include "Vector.h"
#include "madara/knowledge_engine/Context_Guard.h"


Madara::Knowledge_Engine::Containers::Vector::Vector (
  const Knowledge_Update_Settings & settings)
: context_ (0), settings_ (settings) 
{
}
   
Madara::Knowledge_Engine::Containers::Vector::Vector (
  const std::string & name,
  Knowledge_Base & knowledge,
  int size,
  bool delete_vars,
  const Knowledge_Update_Settings & settings)
: context_ (&(knowledge.get_context ())), name_ (name), settings_ (true)
{
  size_ = get_size_ref ();
  resize (size, delete_vars);
  settings_ = settings;
}
  
Madara::Knowledge_Engine::Containers::Vector::Vector (
  const std::string & name,
  Variables & knowledge,
  int size,
  bool delete_vars,
  const Knowledge_Update_Settings & settings)
: context_ (knowledge.get_context ()), name_ (name), settings_ (true)
{
  size_ = get_size_ref ();
  resize (size, delete_vars);
  settings_ = settings;
}

Madara::Knowledge_Engine::Containers::Vector::Vector (const Vector & rhs)
  : context_ (rhs.context_),
    name_ (rhs.name_),
    vector_ (rhs.vector_),
    settings_ (rhs.settings_)
{

}


Madara::Knowledge_Engine::Containers::Vector::~Vector ()
{

}


void
Madara::Knowledge_Engine::Containers::Vector::operator= (
  const Vector & rhs)
{
  if (this != &rhs)
  {
    Guard guard (mutex_), guard2 (rhs.mutex_);

    this->context_ = rhs.context_;
    this->name_ = rhs.name_;
    this->settings_ = rhs.settings_;
    this->size_ = rhs.size_;
    this->vector_ = rhs.vector_;
  }
}


Madara::Knowledge_Engine::Variable_Reference
Madara::Knowledge_Engine::Containers::Vector::get_size_ref (void)
{
  Variable_Reference ref;
  
  if (context_ && name_ != "")
  {
    Knowledge_Update_Settings keep_local (true);
    std::stringstream buffer;
    buffer << name_;
    buffer << ".size";

    ref = context_->get_ref (buffer.str (), keep_local);
  }

  return ref;
}

void
Madara::Knowledge_Engine::Containers::Vector::resize (
  int size, bool delete_vars)
{
  if (context_ && name_ != "")
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);

    if (size >= 0)
    {
      size_t old_size = vector_.size ();

      if (old_size != size)
      {
        vector_.resize (size);
        
        context_->set (size_, Knowledge_Record::Integer (size), settings_);

        if ((size_t)size > old_size)
        {
          for (; old_size < (size_t)size; ++old_size)
          {
            std::stringstream buffer;
            buffer << name_;
            buffer << '.';
            buffer << old_size;
            vector_[old_size] = context_->get_ref (buffer.str (), settings_);
          }
        }
        else if (delete_vars)
        {
          for (; (size_t)size < old_size; ++size)
          {
            std::stringstream buffer;
            buffer << name_;
            buffer << '.';
            buffer << size;

            context_->delete_variable (buffer.str (), settings_);
          }
        }
      }
    }
    else
    {
      // dynamically allocate size from the context
      size_t cur_size =
        context_->get (size_, settings_).to_integer ();

      size_t old_size = vector_.size ();

      if (old_size != cur_size)
      {
        vector_.resize (cur_size);

        if (cur_size > old_size)
        {
          for (; old_size < cur_size; ++old_size)
          {
            std::stringstream buffer;
            buffer << name_;
            buffer << '.';
            buffer << old_size;
            vector_[old_size] = context_->get_ref (buffer.str (), settings_);
          }
        }
        else if (delete_vars)
        {
          for (; cur_size < old_size; ++cur_size)
          {
            std::stringstream buffer;
            buffer << name_;
            buffer << '.';
            buffer << cur_size;

            context_->delete_variable (buffer.str (), settings_);
          }
        }
      }
    }
  }
}

size_t
Madara::Knowledge_Engine::Containers::Vector::size (void) const
{
  Guard guard (mutex_);
  return vector_.size ();
}

std::string
Madara::Knowledge_Engine::Containers::Vector::get_name (void) const
{
  Guard guard (mutex_);
  return name_;
}

void
Madara::Knowledge_Engine::Containers::Vector::set_name (
  const std::string & var_name,
  Knowledge_Base & knowledge, int size)
{
  if (context_ != &(knowledge.get_context ()) || name_ != var_name)
  {
    context_ = &(knowledge.get_context ());

    Context_Guard context_guard (*context_);
    Guard guard (mutex_);

    name_ = var_name;

    vector_.clear ();

    size_ = get_size_ref ();

    if (size > 0)
      resize (size_t (size));
  }
}

void
Madara::Knowledge_Engine::Containers::Vector::set_name (
  const std::string & var_name,
  Variables & knowledge, int size)
{
  if (context_ != knowledge.get_context () || name_ != var_name)
  {
    context_ = knowledge.get_context ();

    Context_Guard context_guard (*context_);
    Guard guard (mutex_);

    name_ = var_name;

    vector_.clear ();
    if (size > 0)
      resize (size_t (size));
  }
}

void
Madara::Knowledge_Engine::Containers::Vector::exchange (
  Vector & other, bool refresh_keys, bool delete_keys)
{
  if (context_ && other.context_)
  {
    Context_Guard context_guard (*context_);
    Context_Guard other_context_guard (*other.context_);
    Guard guard (mutex_), guard2 (other.mutex_);

    if (refresh_keys)
    {
      other.resize ();
      this->resize ();
    }

    size_t other_size = other.vector_.size ();
    size_t this_size = this->vector_.size ();

    for (size_t i = 0; i < this_size; ++i)
    {
      // temp = this[i];
      Knowledge_Record temp = context_->get (this->vector_[i], settings_);
    
      if (i < other_size)
      {
        // this[i] = other[i];
        context_->set (this->vector_[i],
          context_->get (other.vector_[i], other.settings_),
          settings_);

        // other[i] = temp;
        other.context_->set (other.vector_[i], temp, other.settings_);
      }
      else
      {
        if (delete_keys)
        {
          std::stringstream buffer;
          buffer << this->name_;
          buffer << '.';
          buffer << i;
          this->context_->delete_variable (buffer.str (), other.settings_);
        }
        else
        {
          Knowledge_Record zero;
          this->context_->set (this->vector_[i], zero, this->settings_);
        }

        {
          std::stringstream buffer;
          buffer << other.name_;
          buffer << '.';
          buffer << i;

          // other[i] = temp;
          other.context_->set (buffer.str (), temp, other.settings_);
        }
      }

    }

    // copy the other vector's elements to this vector's location
    for (size_t i = this_size; i < other_size; ++i)
    {
      std::stringstream buffer;
      buffer << this->name_;
      buffer << '.';
      buffer << i;
      context_->set (buffer.str (),
        other.context_->get (other.vector_[i], other.settings_), this->settings_);
    }

    // set the size appropriately
    this->context_->set (this->size_,
      Knowledge_Record::Integer (other_size), this->settings_);
    other.context_->set (other.size_,
      Knowledge_Record::Integer (this_size), other.settings_);

    if (refresh_keys)
    {
      this->resize (-1, true);
      other.resize (-1, true);
    }
  }
}

void
Madara::Knowledge_Engine::Containers::Vector::transfer_to (Vector & other)
{
  if (context_ && other.context_)
  {
    Context_Guard context_guard (*context_);
    Context_Guard other_context_guard (*other.context_);
    Guard guard (mutex_);
    Guard guard2 (other.mutex_);

    size_t other_size = other.vector_.size ();
    size_t this_size = this->vector_.size ();

    size_t size = other_size + this_size;
    other.resize ((int)size);

    for (size_t i = 0, j = other_size; i < this_size; ++i, ++j)
    {
      other.context_->set (other.vector_[j], (*this)[i], other.settings_);
    }

    this->resize (0, true);
  }
}

void
Madara::Knowledge_Engine::Containers::Vector::copy_to (
  Knowledge_Vector & target) const
{
  if (context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);

    target.resize (vector_.size ());
    
    for (size_t i = 0; i < vector_.size (); ++i)
    {
      target[i].deep_copy ((*this)[i]);
    }
  }
}

Madara::Knowledge_Record
Madara::Knowledge_Engine::Containers::Vector::operator[] (
  size_t index) const
{
  Knowledge_Record result;
  Knowledge_Update_Settings keep_local (true);

  if (index < vector_.size () && context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    result = context_->get (vector_[index], keep_local);
  }

  return result;
}

Madara::Knowledge_Record
Madara::Knowledge_Engine::Containers::Vector::to_record (
  size_t index) const
{
  Knowledge_Record result;

  if (index < vector_.size () && context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    result = context_->get (vector_[index], settings_);
  }

  return result;
}

bool
Madara::Knowledge_Engine::Containers::Vector::exists (
  size_t index) const
{
  bool result (false);

  if (index < vector_.size () && context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    result = context_->exists (vector_[index]);
  }

  return result;
}

int
Madara::Knowledge_Engine::Containers::Vector::read_file (
  unsigned int index,
  const std::string & filename)
{
  int result = -1;

  if (index < vector_.size () && context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    result = context_->read_file (vector_[index], filename, settings_);
  }

  return result;
}

int
Madara::Knowledge_Engine::Containers::Vector::read_file (
  unsigned int index,
  const std::string & filename, 
  const Knowledge_Update_Settings & settings)
{
  int result = -1;

  if (index < vector_.size () && context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    result = context_->read_file (vector_[index], filename, settings);
  }

  return result;
}

int
Madara::Knowledge_Engine::Containers::Vector::set_file (
  unsigned int index,
  const unsigned char * value, size_t size)
{
  int result = -1;
  
  if (index < vector_.size () && context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    result = context_->set_file (vector_[index], value, size, settings_);
  }

  return result;
}
  
int
Madara::Knowledge_Engine::Containers::Vector::set_file (
  unsigned int index,
  const unsigned char * value, size_t size, 
  const Knowledge_Update_Settings & settings)
{
  int result = -1;
  
  if (index < vector_.size () && context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    result = context_->set_file (vector_[index], value, size, settings);
  }

  return result;
}

int
Madara::Knowledge_Engine::Containers::Vector::set_jpeg (
  unsigned int index,
  const unsigned char * value, size_t size)
{
  int result = -1;
  
  if (index < vector_.size () && context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    result = context_->set_jpeg (vector_[index], value, size, settings_);
  }
  
  return result;
}
  
int
Madara::Knowledge_Engine::Containers::Vector::set_jpeg (
  unsigned int index,
  const unsigned char * value, size_t size, 
  const Knowledge_Update_Settings & settings)
{
  int result = -1;
  
  if (index < vector_.size () && context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    result = context_->set_jpeg (vector_[index], value, size, settings);
  }
  return result;
}

int
Madara::Knowledge_Engine::Containers::Vector::set (
  unsigned int index,
  Knowledge_Record::Integer value)
{
  int result = -1;
  
  if (index < vector_.size () && context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    result = context_->set (vector_[index], value, settings_);
  }

  return result;
}

int
Madara::Knowledge_Engine::Containers::Vector::set (
  unsigned int index,
  Knowledge_Record::Integer value, 
  const Knowledge_Update_Settings & settings)
{
  int result = -1;
  
  if (index < vector_.size () && context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    result = context_->set (vector_[index], value, settings);
  }

  return result;
}

int
Madara::Knowledge_Engine::Containers::Vector::set_index (
  unsigned int index,
  size_t sub_index,
  Knowledge_Record::Integer value)
{
  int result = -1;
  
  if (index < vector_.size () && context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    result = context_->set_index (vector_[index], sub_index, value, settings_);
  }

  return result;
}

int
Madara::Knowledge_Engine::Containers::Vector::set_index (
  unsigned int index,
  size_t sub_index,
  Knowledge_Record::Integer value,
  const Knowledge_Update_Settings & settings)
{
  int result = -1;
  
  if (index < vector_.size () && context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    result = context_->set_index (vector_[index], sub_index, value, settings);
  }

  return result;
}

int
Madara::Knowledge_Engine::Containers::Vector::set (
  unsigned int index,
  const Madara::Knowledge_Record::Integer * value,
  uint32_t size)
{
  int result = -1;
  
  if (index < vector_.size () && context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    result = context_->set (vector_[index], value, size, settings_);
  }

  return result;
}

int
Madara::Knowledge_Engine::Containers::Vector::set (
  unsigned int index,
  const Madara::Knowledge_Record::Integer * value,
  uint32_t size,
  const Knowledge_Update_Settings & settings)
{
  int result = -1;
  
  if (index < vector_.size () && context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    result = context_->set (vector_[index], value, size, settings);
  }
  
  return result;
}

int
Madara::Knowledge_Engine::Containers::Vector::set (
  unsigned int index,
  const std::vector <Knowledge_Record::Integer> & value)
{
  int result = -1;
  
  if (index < vector_.size () && context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    result = context_->set (vector_[index], value, settings_);
  }

  return result;
}
 
int
Madara::Knowledge_Engine::Containers::Vector::set (
  unsigned int index,
  const std::vector <Knowledge_Record::Integer> & value,
  const Knowledge_Update_Settings & settings)
{
  int result = -1;
  
  if (index < vector_.size () && context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    result = context_->set (vector_[index], value, settings);
  }

  return result;
}

int
Madara::Knowledge_Engine::Containers::Vector::set (
  unsigned int index,
  double value)
{
  int result = -1;
  
  if (index < vector_.size () && context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    result = context_->set (vector_[index], value, settings_);
  }

  return result;
}

int
Madara::Knowledge_Engine::Containers::Vector::set (
  unsigned int index,
  double value, 
  const Knowledge_Update_Settings & settings)
{
  int result = -1;
  
  if (index < vector_.size () && context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    result = context_->set (vector_[index], value, settings);
  }

  return result;
}

int
Madara::Knowledge_Engine::Containers::Vector::set_index (
  unsigned int index,
  size_t sub_index,
  double value)
{
  int result = -1;
  
  if (index < vector_.size () && context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    result = context_->set_index (vector_[index], sub_index, value, settings_);
  }

  return result;
}

int
Madara::Knowledge_Engine::Containers::Vector::set_index (
  unsigned int index,
  size_t sub_index,
  double value,
  const Knowledge_Update_Settings & settings)
{
  int result = -1;
  
  if (index < vector_.size () && context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    result = context_->set_index (vector_[index], sub_index, value, settings);
  }

  return result;
}

int
Madara::Knowledge_Engine::Containers::Vector::set (
  unsigned int index,
  const double * value,
  uint32_t size)
{
  int result = -1;
  
  if (index < vector_.size () && context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    result = context_->set (vector_[index], value, size, settings_);
  }

  return result;
}

int
Madara::Knowledge_Engine::Containers::Vector::set (
  unsigned int index,
  const double * value,
  uint32_t size,
  const Knowledge_Update_Settings & settings)
{
  int result = -1;
  
  if (index < vector_.size () && context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    result = context_->set (vector_[index], value, size, settings);
  }

  return result;
}

int
Madara::Knowledge_Engine::Containers::Vector::set (
  unsigned int index,
  const std::vector <double> & value)
{
  int result = -1;
  
  if (index < vector_.size () && context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    result = context_->set (vector_[index], value, settings_);
  }

  return result;
}

int
Madara::Knowledge_Engine::Containers::Vector::set (
  unsigned int index,
  const std::vector <double> & value,
  const Knowledge_Update_Settings & settings)
{
  int result = -1;
  
  if (index < vector_.size () && context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    result = context_->set (vector_[index], value, settings);
  }

  return result;
}

int
Madara::Knowledge_Engine::Containers::Vector::set (
  unsigned int index,
  const std::string & value)
{
  int result = -1;
  
  if (index < vector_.size () && context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    result = context_->set (vector_[index], value, settings_);
  }

  return result;
}

int
Madara::Knowledge_Engine::Containers::Vector::set (
  unsigned int index,
  const std::string & value, 
  const Knowledge_Update_Settings & settings)
{
  int result = -1;
  
  if (index < vector_.size () && context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    result = context_->set (vector_[index], value, settings);
  }

  return result;
}

Madara::Knowledge_Engine::Knowledge_Update_Settings
Madara::Knowledge_Engine::Containers::Vector::set_settings (
  const Knowledge_Update_Settings & settings)
{
  Guard guard (mutex_);
  
  Knowledge_Update_Settings old_settings = settings_;

  settings_ = settings;

  return old_settings;
}

void
Madara::Knowledge_Engine::Containers::Vector::set_quality (
  unsigned int index,
  uint32_t quality,
  const Knowledge_Reference_Settings & settings)
{
  if (index < vector_.size () && context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    context_->set_quality (vector_[index].get_name (), quality,
      true, settings);
  }
}
