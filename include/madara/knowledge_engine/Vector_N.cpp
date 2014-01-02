#include "Vector_N.h"


Madara::Knowledge_Engine::Vector_N::Vector_N (const std::string & name,
  Knowledge_Base & knowledge,
  const Eval_Settings & settings)
: knowledge_ (knowledge), name_ (name), settings_ (settings) 
{
}
      

Madara::Knowledge_Engine::Vector_N::Vector_N (const Vector_N & rhs)
  : knowledge_ (rhs.knowledge_),
    name_ (rhs.name_),
    vector_ (rhs.vector_),
    settings_ (rhs.settings_)
{

}


Madara::Knowledge_Engine::Vector_N::~Vector_N ()
{

}

std::string
Madara::Knowledge_Engine::Vector_N::get_name (void)
{
  Guard guard (mutex_);
  return name_;
}

Madara::Knowledge_Record
Madara::Knowledge_Engine::Vector_N::operator[] (const Index & index)
{
  Guard guard (mutex_);
  Knowledge_Record result;

  if (index.size () > 0)
  {
    std::stringstream buffer;
    buffer << name_;
    
    for (unsigned int i = 0; i < index.size (); ++i)
    {
      buffer << ".";
      buffer << index[i];
    }

    result = knowledge_.get (buffer.str (), settings_);
  }

  return result;
}
      
      

int Madara::Knowledge_Engine::Vector_N::read_file (const Index & index, 
                const std::string & filename, 
  const Eval_Settings & settings)
{
  Guard guard (mutex_);
  int result = -1;
  
  if (index.size () > 0)
  {
    std::stringstream buffer;
    buffer << name_;
    
    for (unsigned int i = 0; i < index.size (); ++i)
    {
      buffer << ".";
      buffer << index[i];
    }

    result = knowledge_.read_file (buffer.str (), filename, settings);
  }

  return result;
}
      

int Madara::Knowledge_Engine::Vector_N::set_file (const Index & index,
  const unsigned char * value, size_t size, 
  const Eval_Settings & settings)
{
  Guard guard (mutex_);
  int result = -1;
  
  if (index.size () > 0)
  {
    std::stringstream buffer;
    buffer << name_;
    
    for (unsigned int i = 0; i < index.size (); ++i)
    {
      buffer << ".";
      buffer << index[i];
    }

    result = knowledge_.set_file (buffer.str (), value, size, settings);
  }
  
  return result;
}
      

int Madara::Knowledge_Engine::Vector_N::set_jpeg (const Index & index,
  const unsigned char * value, size_t size, 
  const Eval_Settings & settings)
{
  Guard guard (mutex_);
  int result = -1;
  
  if (index.size () > 0)
  {
    std::stringstream buffer;
    buffer << name_;
    
    for (unsigned int i = 0; i < index.size (); ++i)
    {
      buffer << ".";
      buffer << index[i];
    }

    result = knowledge_.set_jpeg (buffer.str (), value, size, settings);
  }
  
  return result;
}
      

int Madara::Knowledge_Engine::Vector_N::set (const Index & index,
  Knowledge_Record::Integer value, 
  const Eval_Settings & settings)
{
  Guard guard (mutex_);
  int result = -1;
  
  if (index.size () > 0)
  {
    std::stringstream buffer;
    buffer << name_;
    
    for (unsigned int i = 0; i < index.size (); ++i)
    {
      buffer << ".";
      buffer << index[i];
    }

    result = knowledge_.set (buffer.str (), value, settings);
  }
  
  return result;
}


int Madara::Knowledge_Engine::Vector_N::set_index (const Index & index,
  size_t sub_index,
  Knowledge_Record::Integer value,
  const Eval_Settings & settings)
{
  Guard guard (mutex_);
  int result = -1;
  
  if (index.size () > 0)
  {
    std::stringstream buffer;
    buffer << name_;
    
    for (unsigned int i = 0; i < index.size (); ++i)
    {
      buffer << ".";
      buffer << index[i];
    }

    result = knowledge_.set_index (buffer.str (), sub_index, value, settings);
  }
  
  return result;
}


int Madara::Knowledge_Engine::Vector_N::set (const Index & index,
  const Madara::Knowledge_Record::Integer * value,
  uint32_t size,
  const Eval_Settings & settings)
{
  Guard guard (mutex_);
  int result = -1;
  
  if (index.size () > 0)
  {
    std::stringstream buffer;
    buffer << name_;
    
    for (unsigned int i = 0; i < index.size (); ++i)
    {
      buffer << ".";
      buffer << index[i];
    }

    result = knowledge_.set (buffer.str (), value, size, settings);
  }
  
  return result;
}
       

int Madara::Knowledge_Engine::Vector_N::set (const Index & index,
  const std::vector <Knowledge_Record::Integer> & value,
  const Eval_Settings & settings)
{
  Guard guard (mutex_);
  int result = -1;
  
  if (index.size () > 0)
  {
    std::stringstream buffer;
    buffer << name_;
    
    for (unsigned int i = 0; i < index.size (); ++i)
    {
      buffer << ".";
      buffer << index[i];
    }

    result = knowledge_.set (buffer.str (), value, settings);
  }
  
  return result;
}
       

int Madara::Knowledge_Engine::Vector_N::set (const Index & index, double value, 
  const Eval_Settings & settings)
{
  Guard guard (mutex_);
  int result = -1;
  
  if (index.size () > 0)
  {
    std::stringstream buffer;
    buffer << name_;
    
    for (unsigned int i = 0; i < index.size (); ++i)
    {
      buffer << ".";
      buffer << index[i];
    }

    result = knowledge_.set (buffer.str (), value, settings);
  }
  
  return result;
}


int Madara::Knowledge_Engine::Vector_N::set_index (const Index & index,
  size_t sub_index,
  double value,
  const Eval_Settings & settings)
{
  Guard guard (mutex_);
  int result = -1;
  
  if (index.size () > 0)
  {
    std::stringstream buffer;
    buffer << name_;
    
    for (unsigned int i = 0; i < index.size (); ++i)
    {
      buffer << ".";
      buffer << index[i];
    }

    result = knowledge_.set_index (buffer.str (), sub_index, value, settings);
  }
  
  return result;
}


int Madara::Knowledge_Engine::Vector_N::set (const Index & index,
  const double * value,
  uint32_t size,
  const Eval_Settings & settings)
{
  Guard guard (mutex_);
  int result = -1;
  
  if (index.size () > 0)
  {
    std::stringstream buffer;
    buffer << name_;
    
    for (unsigned int i = 0; i < index.size (); ++i)
    {
      buffer << ".";
      buffer << index[i];
    }

    result = knowledge_.set (buffer.str (), value, size, settings);
  }
  
  return result;
}
       

int Madara::Knowledge_Engine::Vector_N::set (const Index & index,
  const std::vector <double> & value,
  const Eval_Settings & settings)
{
  Guard guard (mutex_);
  int result = -1;
  
  if (index.size () > 0)
  {
    std::stringstream buffer;
    buffer << name_;
    
    for (unsigned int i = 0; i < index.size (); ++i)
    {
      buffer << ".";
      buffer << index[i];
    }

    result = knowledge_.set (buffer.str (), value, settings);
  }
  
  return result;
}
        

int Madara::Knowledge_Engine::Vector_N::set (const Index & index,
  const std::string & value, 
  const Eval_Settings & settings)
{
  Guard guard (mutex_);
  int result = -1;
  
  if (index.size () > 0)
  {
    std::stringstream buffer;
    buffer << name_;
    
    for (unsigned int i = 0; i < index.size (); ++i)
    {
      buffer << ".";
      buffer << index[i];
    }

    result = knowledge_.set (buffer.str (), value, settings);
  }
  
  return result;
}


void Madara::Knowledge_Engine::Vector_N::set_quality (
  const Index & index, uint32_t quality,
  const Knowledge_Reference_Settings & settings)
{
  Guard guard (mutex_);
  if (index.size () > 0)
  {
    std::stringstream buffer;
    buffer << name_;
    
    for (unsigned int i = 0; i < index.size (); ++i)
    {
      buffer << ".";
      buffer << index[i];
    }

    knowledge_.set_quality (buffer.str (), quality, settings);
  }
}
