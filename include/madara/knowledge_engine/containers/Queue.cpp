#include <sstream>

#include "Queue.h"
#include "madara/knowledge_engine/Context_Guard.h"

Madara::Knowledge_Engine::Containers::Queue::Queue (
  const Knowledge_Update_Settings & settings)
: context_ (0), settings_ (settings)
{
}
  
Madara::Knowledge_Engine::Containers::Queue::Queue (
  const std::string & name,
  Knowledge_Base & knowledge,
  const Knowledge_Update_Settings & settings)
: context_ (&(knowledge.get_context ())), name_ (name),
  count_ (name + ".count", knowledge),
  head_ (name + ".head", knowledge),
  tail_ (name + ".tail", knowledge),
  queue_ (name, knowledge),
  settings_ (settings) 
{
}
 
Madara::Knowledge_Engine::Containers::Queue::Queue (
  const std::string & name,
  Variables & knowledge,
  const Knowledge_Update_Settings & settings)
: context_ (knowledge.get_context ()), name_ (name),
  count_ (name + ".count", knowledge),
  head_ (name + ".head", knowledge),
  tail_ (name + ".tail", knowledge),
  queue_ (name, knowledge),
  settings_ (settings) 
{
}
 
Madara::Knowledge_Engine::Containers::Queue::Queue (
  const std::string & name,
  Knowledge_Base & knowledge,
  int size,
  const Knowledge_Update_Settings & settings)
: context_ (&(knowledge.get_context ())), name_ (name),
  count_ (name + ".count", knowledge),
  head_ (name + ".head", knowledge),
  tail_ (name + ".tail", knowledge),
  queue_ (name, knowledge, size, false, settings),
  settings_ (settings) 
{
}
 
Madara::Knowledge_Engine::Containers::Queue::Queue (
  const std::string & name,
  Variables & knowledge,
  int size,
  const Knowledge_Update_Settings & settings)
: context_ (knowledge.get_context ()), name_ (name),
  count_ (name + ".count", knowledge),
  head_ (name + ".head", knowledge),
  tail_ (name + ".tail", knowledge),
  queue_ (name, knowledge, size, false, settings),
  settings_ (settings) 
{
}


Madara::Knowledge_Engine::Containers::Queue::Queue (const Queue & rhs)
  : context_ (rhs.context_),
    name_ (rhs.name_),
    count_ (rhs.count_),
    head_ (rhs.head_),
    tail_ (rhs.tail_),
    queue_ (rhs.queue_),
    settings_ (rhs.settings_)
{

}


Madara::Knowledge_Engine::Containers::Queue::~Queue ()
{

}

void
Madara::Knowledge_Engine::Containers::Queue::operator= (const Queue & rhs)
{
  if (this != &rhs)
  {
    Guard guard (mutex_), guard2 (rhs.mutex_);

    this->context_ = rhs.context_;
    this->name_ = rhs.name_;
    this->count_ = rhs.count_;
    this->head_ = rhs.head_;
    this->tail_ = rhs.tail_;
    this->queue_ = rhs.queue_;
    this->settings_ = rhs.settings_;
  }
}


bool
Madara::Knowledge_Engine::Containers::Queue::operator== (
  const Queue & value) const
{
  return name_ == value.get_name ();
}
        
bool
Madara::Knowledge_Engine::Containers::Queue::operator!= (
  const Queue & value) const
{
  return name_ != value.get_name ();
}

bool
Madara::Knowledge_Engine::Containers::Queue::enqueue (
  const Knowledge_Record & record)
{
  bool result (false);

  // avoid double retrieval of count_ during check
  Knowledge_Record::Integer count = *count_;

  if (context_ && name_ != "" && queue_.size () > 0 &&
      count >= 0 && 
      count < (Knowledge_Record::Integer)queue_.size ())
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);

    context_->set (queue_.vector_[*tail_], record, settings_);
    tail_ = increment (*tail_, 1);
    ++count_;
  }

  return result;
}

Madara::Knowledge_Record
Madara::Knowledge_Engine::Containers::Queue::dequeue (void)
{
  Madara::Knowledge_Record result;
  
  if (context_ && name_ != "" && count_ > 0)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);

    result = context_->get (queue_.vector_[*head_], settings_);
    
    head_ = increment (*head_, 1);
    --count_;
  }

  return result;
}

void
Madara::Knowledge_Engine::Containers::Queue::clear (void)
{
  if (context_ && name_ != "" && count_ > 0)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);

    count_ = 0;
    head_ = *tail_;
  }
}

Madara::Knowledge_Record
Madara::Knowledge_Engine::Containers::Queue::inspect (size_t position)
{
  Madara::Knowledge_Record result;
  if (context_ && name_ != "" &&
      (Knowledge_Record::Integer)position < *count_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);

    size_t index = (size_t)increment (
      *head_, (Knowledge_Record::Integer)position);
    
    result = context_->get (queue_.vector_[index], settings_);
  }
  return result;
}

std::string
Madara::Knowledge_Engine::Containers::Queue::get_name (void) const
{
  Guard guard (mutex_);
  return name_;
}

void
Madara::Knowledge_Engine::Containers::Queue::set_name (
  const std::string & var_name,
  Knowledge_Base & knowledge)
{
  Knowledge_Update_Settings keep_local (true);
  context_ = &(knowledge.get_context ());

  Context_Guard context_guard (*context_);
  Guard guard (mutex_);

  name_ = var_name;
  
  this->count_.set_name (var_name + ".count", knowledge);
  this->head_.set_name (var_name + ".head", knowledge);
  this->tail_.set_name (var_name + ".tail", knowledge);
  this->queue_.set_name (var_name, knowledge);
}

void
Madara::Knowledge_Engine::Containers::Queue::set_name (
  const std::string & var_name,
  Variables & knowledge)
{
  Knowledge_Update_Settings keep_local (true);
  context_ = knowledge.get_context ();

  Context_Guard context_guard (*context_);
  Guard guard (mutex_);

  name_ = var_name;
  
  this->count_.set_name (var_name + ".count", knowledge);
  this->head_.set_name (var_name + ".head", knowledge);
  this->tail_.set_name (var_name + ".tail", knowledge);
  this->queue_.set_name (var_name, knowledge);
}

size_t
Madara::Knowledge_Engine::Containers::Queue::size (void)
{
  return queue_.size ();
}

size_t
Madara::Knowledge_Engine::Containers::Queue::count (void)
{
  return (size_t)(*count_);
}

void
Madara::Knowledge_Engine::Containers::Queue::resize (int size)
{
  if (context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);

    queue_.resize (size, false);

    if (count_ > (Knowledge_Record::Integer)queue_.size ())
    {
      count_ = (Knowledge_Record::Integer)size;
    }
  }
}

Madara::Knowledge_Engine::Knowledge_Update_Settings
Madara::Knowledge_Engine::Containers::Queue::set_settings (
  const Knowledge_Update_Settings & settings)
{
  Guard guard (mutex_);
  
  Knowledge_Update_Settings old_settings = settings_;

  settings_ = settings;

  return old_settings;
}

void
Madara::Knowledge_Engine::Containers::Queue::set_quality (
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
