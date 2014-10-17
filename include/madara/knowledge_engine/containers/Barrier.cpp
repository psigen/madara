#include <sstream>

#include "Barrier.h"
#include "madara/knowledge_engine/Context_Guard.h"

Madara::Knowledge_Engine::Containers::Barrier::Barrier (
  const Knowledge_Update_Settings & settings)
: context_ (0), id_ (0), participants_ (1), settings_ (settings)
{
  init_noharm ();
}
  
Madara::Knowledge_Engine::Containers::Barrier::Barrier (
  const std::string & name,
  Knowledge_Base & knowledge,
  const Knowledge_Update_Settings & settings)
: context_ (&(knowledge.get_context ())), name_ (name),
  id_ (0), participants_ (1), settings_ (settings) 
{
  init_noharm ();
  build_var ();
  build_aggregate_barrier ();
}
 
Madara::Knowledge_Engine::Containers::Barrier::Barrier (
  const std::string & name,
  Variables & knowledge,
  const Knowledge_Update_Settings & settings)
: context_ (knowledge.get_context ()), name_ (name),
  id_ (0), participants_ (1), settings_ (settings) 
{
  init_noharm ();
  build_var ();
  build_aggregate_barrier ();
}
 
Madara::Knowledge_Engine::Containers::Barrier::Barrier (
  const std::string & name,
  Knowledge_Base & knowledge,
  int id,
  int participants,
  const Knowledge_Update_Settings & settings)
: context_ (&(knowledge.get_context ())), name_ (name),
  id_ (id), participants_ (participants), settings_ (settings) 
{
  init_noharm ();
  build_var ();
  build_aggregate_barrier ();
}
 
Madara::Knowledge_Engine::Containers::Barrier::Barrier (
  const std::string & name,
  Variables & knowledge,
  int id,
  int participants,
  const Knowledge_Update_Settings & settings)
: context_ (knowledge.get_context ()), name_ (name),
  id_ (id), participants_ (participants), settings_ (settings) 
{
  init_noharm ();
  build_var ();
  build_aggregate_barrier ();
}


Madara::Knowledge_Engine::Containers::Barrier::Barrier (const Barrier & rhs)
  : context_ (rhs.context_),
    name_ (rhs.name_),
    id_ (rhs.id_),
    participants_ (rhs.participants_),
    variable_ (rhs.variable_),
    settings_ (rhs.settings_),
    aggregate_barrier_ (rhs.aggregate_barrier_),
    variable_name_ (rhs.variable_name_)
{

}


Madara::Knowledge_Engine::Containers::Barrier::~Barrier ()
{

}

void
Madara::Knowledge_Engine::Containers::Barrier::operator= (const Barrier & rhs)
{
  if (this != &rhs)
  {
    Guard guard (mutex_), guard2 (rhs.mutex_);

    this->context_ = rhs.context_;
    this->name_ = rhs.name_;
    this->id_ = rhs.id_;
    this->participants_ = rhs.participants_;
    this->settings_ = rhs.settings_;
    this->variable_ = rhs.variable_;
    this->aggregate_barrier_ = rhs.aggregate_barrier_;
    this->variable_name_ = rhs.variable_name_;
  }
}

void
Madara::Knowledge_Engine::Containers::Barrier::build_aggregate_barrier (void)
{
  if (context_ && name_ != "")
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);

    std::stringstream buffer;
    if (participants_ > 0)
    {
      // barrier logic is that everyone else is at least to your barrier
      buffer << name_;
      buffer << ".0 >= ";
      buffer << name_;
      buffer << ".";
      buffer << id_;

      // add all other barrierer variables
      for (int i = 1; i < participants_; ++i)
      {
        buffer << " && ";
        buffer << name_;
        buffer << ".";
        buffer << i;
        buffer << " >= ";
        buffer << name_;
        buffer << ".";
        buffer << id_;
      }
    }

    aggregate_barrier_ = context_->compile (buffer.str ());
    std::cerr << "Barrier string: ";
    std::cerr << buffer.str ();
    std::cerr << "\n";
  }
  else if (name_ == "")
  {
    context_->print ("ERROR: Container::Barrier needs a name.\n", 0);
  }
  else if (!context_)
  {
    context_->print ("ERROR: Container::Barrier needs a context.\n", 0);
  }
}

void
Madara::Knowledge_Engine::Containers::Barrier::build_var (void)
{
  if (context_ && name_ != "")
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);

    std::stringstream buffer;

    buffer << name_;
    buffer << ".";
    buffer << id_;

    variable_name_ = buffer.str ();
    variable_ = context_->get_ref (buffer.str (), no_harm);
  }
  else if (name_ == "")
  {
    context_->print ("ERROR: Container::Barrier needs a name.\n", 0);
  }
  else if (!context_)
  {
    context_->print ("ERROR: Container::Barrier needs a context.\n", 0);
  }
}

void
Madara::Knowledge_Engine::Containers::Barrier::init_noharm (void)
{
  no_harm.always_overwrite = false;
  no_harm.delay_sending_modifieds = true;
  no_harm.expand_variables = false;
  no_harm.signal_changes = false;
  no_harm.track_local_changes = false;
  no_harm.treat_globals_as_locals = true;
}

std::string
Madara::Knowledge_Engine::Containers::Barrier::get_name (void) const
{
  Guard guard (mutex_);
  return name_;
}

size_t
Madara::Knowledge_Engine::Containers::Barrier::get_id (void) const
{
  Guard guard (mutex_);
  return id_;
}

size_t
Madara::Knowledge_Engine::Containers::Barrier::get_participants (void) const
{
  Guard guard (mutex_);
  return participants_;
}

void
Madara::Knowledge_Engine::Containers::Barrier::set_name (
  const std::string & var_name,
  Knowledge_Base & knowledge)
{
  Knowledge_Update_Settings keep_local (true);
  context_ = &(knowledge.get_context ());

  Context_Guard context_guard (*context_);
  Guard guard (mutex_);

  name_ = var_name;

  this->build_var ();
  this->build_aggregate_barrier ();
}

void
Madara::Knowledge_Engine::Containers::Barrier::set_name (
  const std::string & var_name,
  Variables & knowledge)
{
  Knowledge_Update_Settings keep_local (true);
  context_ = knowledge.get_context ();

  Context_Guard context_guard (*context_);
  Guard guard (mutex_);

  name_ = var_name;

  this->build_var ();
  this->build_aggregate_barrier ();
}

void
Madara::Knowledge_Engine::Containers::Barrier::set_name (
  const std::string & var_name,
  Thread_Safe_Context & knowledge)
{
  Knowledge_Update_Settings keep_local (true);
  context_ = &knowledge;

  Context_Guard context_guard (*context_);
  Guard guard (mutex_);

  name_ = var_name;

  this->build_var ();
  this->build_aggregate_barrier ();
}

void
Madara::Knowledge_Engine::Containers::Barrier::resize (size_t id, size_t participants)
{
  if (context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);

    id_ = id;
    participants_ = participants;

    this->build_var ();
    this->build_aggregate_barrier ();
  }
}

Madara::Knowledge_Engine::Containers::Barrier::type
Madara::Knowledge_Engine::Containers::Barrier::operator= (type value)
{
  if (context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    context_->set (variable_, value, settings_);
  }

  return value;
}

Madara::Knowledge_Record
Madara::Knowledge_Engine::Containers::Barrier::to_record (void) const
{
  Madara::Knowledge_Record result;
  
  if (context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    result = context_->get (variable_, no_harm);
  }
  
  return result;
}
 
Madara::Knowledge_Record::Integer
Madara::Knowledge_Engine::Containers::Barrier::to_integer (void) const
{
  Madara::Knowledge_Record::Integer result (0);
  
  if (context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    result = context_->get (variable_, no_harm).to_integer ();
  }

  return result;
}


void
Madara::Knowledge_Engine::Containers::Barrier::next (void)
{
  if (context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    context_->inc (variable_, settings_);
  }
}
 
bool
Madara::Knowledge_Engine::Containers::Barrier::is_done (void)
{
  bool result = false;

  if (context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    
    result = barrier_result () == 1;

    if (!result)
    {
      context_->set (variable_, context_->get (variable_, no_harm).to_integer ());
    }
  }

  return result;
}
  
void
Madara::Knowledge_Engine::Containers::Barrier::modify (void)
{
  Context_Guard context_guard (*context_);
  if (context_ && name_ != "")
  {
    context_->mark_modified (variable_);
  }
}
 

double
Madara::Knowledge_Engine::Containers::Barrier::to_double (void) const
{
  double result (0.0);

  if (context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    result = context_->get (variable_, no_harm).to_double ();
  }

  return result;
}
   
std::string
Madara::Knowledge_Engine::Containers::Barrier::to_string (void) const
{
  std::string result;

  if (context_)
  {
    Context_Guard context_guard (*context_);
    Guard guard (mutex_);
    result = context_->get (variable_, no_harm).to_string ();
  }

  return result;
}

Madara::Knowledge_Engine::Knowledge_Update_Settings
Madara::Knowledge_Engine::Containers::Barrier::set_settings (
  const Knowledge_Update_Settings & settings)
{
  Guard guard (mutex_);
  
  Knowledge_Update_Settings old_settings = settings_;

  settings_ = settings;

  return old_settings;
}

void
Madara::Knowledge_Engine::Containers::Barrier::set_quality (
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
