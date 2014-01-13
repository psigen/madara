#include "Vector.h"


Madara::Knowledge_Engine::Containers::Vector::Vector (
  const Eval_Settings & settings)
: knowledge_ (0), settings_ (settings) 
{
}
   
Madara::Knowledge_Engine::Containers::Vector::Vector (
  const std::string & name,
  size_t size,
  Knowledge_Base & knowledge,
  const Eval_Settings & settings)
: knowledge_ (&knowledge), name_ (name), settings_ (settings) 
{
  resize (size);
}
      

Madara::Knowledge_Engine::Containers::Vector::Vector (const Vector & rhs)
  : knowledge_ (rhs.knowledge_),
    name_ (rhs.name_),
    vector_ (rhs.vector_),
    settings_ (rhs.settings_)
{

}


Madara::Knowledge_Engine::Containers::Vector::~Vector ()
{

}


void
Madara::Knowledge_Engine::Containers::Vector::resize (size_t size)
{
  if (knowledge_ && name_ != "")
  {
    Guard guard (mutex_);
    size_t old_size = vector_.size ();

    if (old_size != size)
    {
      vector_.resize (size);

      if (size > old_size)
      {
        for (; old_size < size; ++old_size)
        {
          std::stringstream buffer;
          buffer << name_;
          buffer << '.';
          buffer << old_size;
          vector_[old_size] = knowledge_->get_ref (buffer.str (), settings_);
        }
      }
    }
  }
}

size_t
Madara::Knowledge_Engine::Containers::Vector::size (void)
{
  Guard guard (mutex_);
  return vector_.size ();
}

std::string
Madara::Knowledge_Engine::Containers::Vector::get_name (void)
{
  Guard guard (mutex_);
  return name_;
}

void
Madara::Knowledge_Engine::Containers::Vector::set_name (
  const std::string & var_name,
  Knowledge_Base & knowledge, int size)
{
  if (knowledge_ != &knowledge || name_ != var_name)
  {
    knowledge_ = &knowledge;
    name_ = var_name;

    vector_.clear ();
    if (size > 0)
      resize (size_t (size));
  }
}

Madara::Knowledge_Record
Madara::Knowledge_Engine::Containers::Vector::operator[] (unsigned int index)
{
  Guard guard (mutex_);
  Knowledge_Record result;

  if (index < vector_.size () && knowledge_)
    result = knowledge_->get (vector_[index], settings_);

  return result;
}
      
      

int
Madara::Knowledge_Engine::Containers::Vector::read_file (
  unsigned int index,
  const std::string & filename, 
  const Eval_Settings & settings)
{
  Guard guard (mutex_);
  int result = -1;

  if (index < vector_.size () && knowledge_)
    result = knowledge_->read_file (vector_[index], filename, settings);

  return result;
}
      

int
Madara::Knowledge_Engine::Containers::Vector::set_file (
  unsigned int index,
  const unsigned char * value, size_t size, 
  const Eval_Settings & settings)
{
  Guard guard (mutex_);
  int result = -1;
  
  if (index < vector_.size () && knowledge_)
    result = knowledge_->set_file (vector_[index], value, size, settings);
  
  return result;
}
      

int
Madara::Knowledge_Engine::Containers::Vector::set_jpeg (
  unsigned int index,
  const unsigned char * value, size_t size, 
  const Eval_Settings & settings)
{
  Guard guard (mutex_);
  int result = -1;
  
  if (index < vector_.size () && knowledge_)
    result = knowledge_->set_jpeg (vector_[index], value, size, settings);
  
  return result;
}
      

int
Madara::Knowledge_Engine::Containers::Vector::set (
  unsigned int index,
  Knowledge_Record::Integer value, 
  const Eval_Settings & settings)
{
  Guard guard (mutex_);
  int result = -1;
  
  if (index < vector_.size () && knowledge_)
    result = knowledge_->set (vector_[index], value, settings);
  
  return result;
}


int
Madara::Knowledge_Engine::Containers::Vector::set_index (
  unsigned int index,
  size_t sub_index,
  Knowledge_Record::Integer value,
  const Eval_Settings & settings)
{
  Guard guard (mutex_);
  int result = -1;
  
  if (index < vector_.size () && knowledge_)
    result = knowledge_->set_index (vector_[index], sub_index, value, settings);
  
  return result;
}


int
Madara::Knowledge_Engine::Containers::Vector::set (
  unsigned int index,
  const Madara::Knowledge_Record::Integer * value,
  uint32_t size,
  const Eval_Settings & settings)
{
  Guard guard (mutex_);
  int result = -1;
  
  if (index < vector_.size () && knowledge_)
    result = knowledge_->set (vector_[index], value, size, settings);
  
  return result;
}
       

int
Madara::Knowledge_Engine::Containers::Vector::set (
  unsigned int index,
  const std::vector <Knowledge_Record::Integer> & value,
  const Eval_Settings & settings)
{
  Guard guard (mutex_);
  int result = -1;
  
  if (index < vector_.size () && knowledge_)
    result = knowledge_->set (vector_[index], value, settings);
  
  return result;
}
       

int
Madara::Knowledge_Engine::Containers::Vector::set (
  unsigned int index,
  double value, 
  const Eval_Settings & settings)
{
  Guard guard (mutex_);
  int result = -1;
  
  if (index < vector_.size () && knowledge_)
    result = knowledge_->set (vector_[index], value, settings);
  
  return result;
}


int
Madara::Knowledge_Engine::Containers::Vector::set_index (
  unsigned int index,
  size_t sub_index,
  double value,
  const Eval_Settings & settings)
{
  Guard guard (mutex_);
  int result = -1;
  
  if (index < vector_.size () && knowledge_)
    result = knowledge_->set_index (vector_[index], sub_index, value, settings);
  
  return result;
}


int
Madara::Knowledge_Engine::Containers::Vector::set (
  unsigned int index,
  const double * value,
  uint32_t size,
  const Eval_Settings & settings)
{
  Guard guard (mutex_);
  int result = -1;
  
  if (index < vector_.size () && knowledge_)
    result = knowledge_->set (vector_[index], value, size, settings);
  
  return result;
}
       

int
Madara::Knowledge_Engine::Containers::Vector::set (
  unsigned int index,
  const std::vector <double> & value,
  const Eval_Settings & settings)
{
  Guard guard (mutex_);
  int result = -1;
  
  if (index < vector_.size () && knowledge_)
    result = knowledge_->set (vector_[index], value, settings);
  
  return result;
}
        

int
Madara::Knowledge_Engine::Containers::Vector::set (
  unsigned int index,
  const std::string & value, 
  const Eval_Settings & settings)
{
  Guard guard (mutex_);
  int result = -1;
  
  if (index < vector_.size () && knowledge_)
    result = knowledge_->set (vector_[index], value, settings);
  
  return result;
}


void
Madara::Knowledge_Engine::Containers::Vector::set_quality (
  unsigned int index,
  uint32_t quality,
  const Knowledge_Reference_Settings & settings)
{
  Guard guard (mutex_);

  if (index < vector_.size () && knowledge_)
    knowledge_->set_quality (vector_[index].get_name (), quality, settings);
}
