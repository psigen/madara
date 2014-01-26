#include "Vector_N.h"


Madara::Knowledge_Engine::Containers::Vector_N::Vector_N (
  const Eval_Settings & settings)
: context_ (0), settings_ (settings) 
{
}
 
Madara::Knowledge_Engine::Containers::Vector_N::Vector_N (
  const std::string & name,
  Knowledge_Base & knowledge,
  const Eval_Settings & settings)
: context_ (&(knowledge.get_context ())), name_ (name), settings_ (settings) 
{
}
  
Madara::Knowledge_Engine::Containers::Vector_N::Vector_N (
  const std::string & name,
  Variables & knowledge,
  const Eval_Settings & settings)
: context_ (knowledge.get_context ()), name_ (name), settings_ (settings) 
{
}
      

Madara::Knowledge_Engine::Containers::Vector_N::Vector_N (const Vector_N & rhs)
  : context_ (rhs.context_),
    name_ (rhs.name_),
    settings_ (rhs.settings_)
{

}


Madara::Knowledge_Engine::Containers::Vector_N::~Vector_N ()
{

}

std::string
Madara::Knowledge_Engine::Containers::Vector_N::get_name (void)
{
  Guard guard (mutex_);
  return name_;
}

void
Madara::Knowledge_Engine::Containers::Vector_N::set_name (
  const std::string & var_name,
  Knowledge_Base & knowledge)
{
  context_ = &(knowledge.get_context ());
  name_ = var_name;
}

void
Madara::Knowledge_Engine::Containers::Vector_N::set_name (
  const std::string & var_name,
  Variables & knowledge)
{
  context_ = knowledge.get_context ();
  name_ = var_name;
}

Madara::Knowledge_Record
Madara::Knowledge_Engine::Containers::Vector_N::operator[] (const Index & index)
{
  Guard guard (mutex_);
  Knowledge_Record result;

  if (index.size () > 0 && context_)
  {
    std::stringstream buffer;
    buffer << name_;
    
    for (unsigned int i = 0; i < index.size (); ++i)
    {
      buffer << ".";
      buffer << index[i];
    }

    result = context_->get (buffer.str (), settings_);
  }

  return result;
}
      
      

int
Madara::Knowledge_Engine::Containers::Vector_N::read_file (
  const Index & index, 
  const std::string & filename, 
  const Eval_Settings & settings)
{
  Guard guard (mutex_);
  int result = -1;
  
  if (index.size () > 0 && context_)
  {
    std::stringstream buffer;
    buffer << name_;
    
    for (unsigned int i = 0; i < index.size (); ++i)
    {
      buffer << ".";
      buffer << index[i];
    }

    result = context_->read_file (buffer.str (), filename, settings);
  }

  return result;
}
      

int
Madara::Knowledge_Engine::Containers::Vector_N::set_file (
  const Index & index,
  const unsigned char * value, size_t size, 
  const Eval_Settings & settings)
{
  Guard guard (mutex_);
  int result = -1;
  
  if (index.size () > 0 && context_)
  {
    std::stringstream buffer;
    buffer << name_;
    
    for (unsigned int i = 0; i < index.size (); ++i)
    {
      buffer << ".";
      buffer << index[i];
    }

    result = context_->set_file (buffer.str (), value, size, settings);
  }
  
  return result;
}
      

int
Madara::Knowledge_Engine::Containers::Vector_N::set_jpeg (
  const Index & index,
  const unsigned char * value, size_t size, 
  const Eval_Settings & settings)
{
  Guard guard (mutex_);
  int result = -1;
  
  if (index.size () > 0 && context_)
  {
    std::stringstream buffer;
    buffer << name_;
    
    for (unsigned int i = 0; i < index.size (); ++i)
    {
      buffer << ".";
      buffer << index[i];
    }

    result = context_->set_jpeg (buffer.str (), value, size, settings);
  }
  
  return result;
}
      

int
Madara::Knowledge_Engine::Containers::Vector_N::set (
  const Index & index,
  Knowledge_Record::Integer value, 
  const Eval_Settings & settings)
{
  Guard guard (mutex_);
  int result = -1;
  
  if (index.size () > 0 && context_)
  {
    std::stringstream buffer;
    buffer << name_;
    
    for (unsigned int i = 0; i < index.size (); ++i)
    {
      buffer << ".";
      buffer << index[i];
    }

    result = context_->set (buffer.str (), value, settings);
  }
  
  return result;
}


int
Madara::Knowledge_Engine::Containers::Vector_N::set_index (
  const Index & index,
  size_t sub_index,
  Knowledge_Record::Integer value,
  const Eval_Settings & settings)
{
  Guard guard (mutex_);
  int result = -1;
  
  if (index.size () > 0 && context_)
  {
    std::stringstream buffer;
    buffer << name_;
    
    for (unsigned int i = 0; i < index.size (); ++i)
    {
      buffer << ".";
      buffer << index[i];
    }

    result = context_->set_index (buffer.str (), sub_index, value, settings);
  }
  
  return result;
}


int
Madara::Knowledge_Engine::Containers::Vector_N::set (
  const Index & index,
  const Madara::Knowledge_Record::Integer * value,
  uint32_t size,
  const Eval_Settings & settings)
{
  Guard guard (mutex_);
  int result = -1;
  
  if (index.size () > 0 && context_)
  {
    std::stringstream buffer;
    buffer << name_;
    
    for (unsigned int i = 0; i < index.size (); ++i)
    {
      buffer << ".";
      buffer << index[i];
    }

    result = context_->set (buffer.str (), value, size, settings);
  }
  
  return result;
}
       

int
Madara::Knowledge_Engine::Containers::Vector_N::set (
  const Index & index,
  const std::vector <Knowledge_Record::Integer> & value,
  const Eval_Settings & settings)
{
  Guard guard (mutex_);
  int result = -1;
  
  if (index.size () > 0 && context_)
  {
    std::stringstream buffer;
    buffer << name_;
    
    for (unsigned int i = 0; i < index.size (); ++i)
    {
      buffer << ".";
      buffer << index[i];
    }

    result = context_->set (buffer.str (), value, settings);
  }
  
  return result;
}
       

int
Madara::Knowledge_Engine::Containers::Vector_N::set (
  const Index & index,
  double value, 
  const Eval_Settings & settings)
{
  Guard guard (mutex_);
  int result = -1;
  
  if (index.size () > 0 && context_)
  {
    std::stringstream buffer;
    buffer << name_;
    
    for (unsigned int i = 0; i < index.size (); ++i)
    {
      buffer << ".";
      buffer << index[i];
    }

    result = context_->set (buffer.str (), value, settings);
  }
  
  return result;
}


int
Madara::Knowledge_Engine::Containers::Vector_N::set_index (
  const Index & index,
  size_t sub_index,
  double value,
  const Eval_Settings & settings)
{
  Guard guard (mutex_);
  int result = -1;
  
  if (index.size () > 0 && context_)
  {
    std::stringstream buffer;
    buffer << name_;
    
    for (unsigned int i = 0; i < index.size (); ++i)
    {
      buffer << ".";
      buffer << index[i];
    }

    result = context_->set_index (buffer.str (), sub_index, value, settings);
  }
  
  return result;
}


int
Madara::Knowledge_Engine::Containers::Vector_N::set (
  const Index & index,
  const double * value,
  uint32_t size,
  const Eval_Settings & settings)
{
  Guard guard (mutex_);
  int result = -1;
  
  if (index.size () > 0 && context_)
  {
    std::stringstream buffer;
    buffer << name_;
    
    for (unsigned int i = 0; i < index.size (); ++i)
    {
      buffer << ".";
      buffer << index[i];
    }

    result = context_->set (buffer.str (), value, size, settings);
  }
  
  return result;
}
       

int
Madara::Knowledge_Engine::Containers::Vector_N::set (
  const Index & index,
  const std::vector <double> & value,
  const Eval_Settings & settings)
{
  Guard guard (mutex_);
  int result = -1;
  
  if (index.size () > 0 && context_)
  {
    std::stringstream buffer;
    buffer << name_;
    
    for (unsigned int i = 0; i < index.size (); ++i)
    {
      buffer << ".";
      buffer << index[i];
    }

    result = context_->set (buffer.str (), value, settings);
  }
  
  return result;
}
        

int
Madara::Knowledge_Engine::Containers::Vector_N::set (
  const Index & index,
  const std::string & value, 
  const Eval_Settings & settings)
{
  Guard guard (mutex_);
  int result = -1;
  
  if (index.size () > 0 && context_)
  {
    std::stringstream buffer;
    buffer << name_;
    
    for (unsigned int i = 0; i < index.size (); ++i)
    {
      buffer << ".";
      buffer << index[i];
    }

    result = context_->set (buffer.str (), value, settings);
  }
  
  return result;
}


void
Madara::Knowledge_Engine::Containers::Vector_N::set_quality (
  const Index & index,
  uint32_t quality,
  const Knowledge_Reference_Settings & settings)
{
  Guard guard (mutex_);
  if (index.size () > 0 && context_)
  {
    std::stringstream buffer;
    buffer << name_;
    
    for (unsigned int i = 0; i < index.size (); ++i)
    {
      buffer << ".";
      buffer << index[i];
    }

    context_->set_quality (buffer.str (), quality, true, settings);
  }
}
