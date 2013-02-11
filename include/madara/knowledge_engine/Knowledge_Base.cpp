
#include "madara/knowledge_engine/Knowledge_Base.h"
#include "madara/knowledge_engine/Knowledge_Base_Impl.h"

#include <sstream>
#include <iostream>

Madara::Knowledge_Engine::Knowledge_Base::Knowledge_Base ()
: impl_ (new Knowledge_Base_Impl ())
{
}

Madara::Knowledge_Engine::Knowledge_Base::Knowledge_Base (
  const std::string & host, int transport)
: impl_ (new Knowledge_Base_Impl (host, transport))
{
}

Madara::Knowledge_Engine::Knowledge_Base::Knowledge_Base (
  const std::string & host, int transport, 
  const std::string & knowledge_domain)
: impl_ (new Knowledge_Base_Impl (host, transport, knowledge_domain))
{
}

Madara::Knowledge_Engine::Knowledge_Base::Knowledge_Base (
  const std::string & host, const Madara::Transport::Settings & settings)
: impl_ (new Knowledge_Base_Impl (host, settings))
{
}

Madara::Knowledge_Engine::Knowledge_Base::Knowledge_Base (
  const Knowledge_Base & original)
: impl_ (original.impl_)
{
}

Madara::Knowledge_Engine::Knowledge_Base::~Knowledge_Base ()
{
  // taken care of by Refcounter class
}

/// Assignment operator.
void 
Madara::Knowledge_Engine::Knowledge_Base::operator= (
  const Knowledge_Base &original)
{
  // Refcounter class takes care of the internal decrements and
  // increments.
  if (this != &original)
    impl_ = original.impl_;   
}

void
Madara::Knowledge_Engine::Knowledge_Base::activate_transport (void)
{
  impl_->activate_transport ();
}

/// Applies current time and modified to all global variables and tries
/// to send them.
int
Madara::Knowledge_Engine::Knowledge_Base::apply_modified (void)
{
  return impl_->apply_modified ();
}

void
Madara::Knowledge_Engine::Knowledge_Base::close_transport (void)
{
  impl_->close_transport ();
}

Madara::Knowledge_Record
Madara::Knowledge_Engine::Knowledge_Base::get (const std::string & key)
{
  return impl_->get (key);
}


int
Madara::Knowledge_Engine::Knowledge_Base::read_file (const std::string & knowledge_key, 
                     const std::string & file_name)
{
  return impl_->read_file (knowledge_key, file_name);
}

int
Madara::Knowledge_Engine::Knowledge_Base::read_policy (const std::string & knowledge_key, 
                     const std::string & file_name)
{
  return impl_->read_policy (knowledge_key, file_name);
}

int
Madara::Knowledge_Engine::Knowledge_Base::write_file (const std::string & knowledge_key, 
                     const std::string & file_name)
{
  return impl_->write_file (knowledge_key, file_name);
}


std::string
Madara::Knowledge_Engine::Knowledge_Base::expand_statement (
  const std::string & statement)
{
  return impl_->expand_statement (statement);
}

Madara::Knowledge_Engine::Compiled_Expression
Madara::Knowledge_Engine::Knowledge_Base::compile (
  const std::string & expression)
{
  return impl_->compile (expression);
}

int
Madara::Knowledge_Engine::Knowledge_Base::set (
  const std::string & key, 
  Madara::Knowledge_Record::Integer value)
{
  Eval_Settings settings;
  return impl_->set (key, value, settings);
}

int
Madara::Knowledge_Engine::Knowledge_Base::set (
  const std::string & key, 
  double value)
{
  Eval_Settings settings;
  return impl_->set (key, value, settings);
}

int
Madara::Knowledge_Engine::Knowledge_Base::set (
  const std::string & key, 
  const std::string & value)
{
  Eval_Settings settings;
  return impl_->set (key, value, settings);
}

int
Madara::Knowledge_Engine::Knowledge_Base::set (const std::string & key,
  Madara::Knowledge_Record::Integer value,
  const Eval_Settings & settings)
{
  return impl_->set (key, value, settings);
}

int
Madara::Knowledge_Engine::Knowledge_Base::set (const std::string & key,
  double value,
  const Eval_Settings & settings)
{
  return impl_->set (key, value, settings);
}

int
Madara::Knowledge_Engine::Knowledge_Base::set (const std::string & key,
  const std::string & value,
  const Eval_Settings & settings)
{
  return impl_->set (key, value, settings);
}


/// Set quality of writing to a variable
void 
Madara::Knowledge_Engine::Knowledge_Base::set_quality (
  const std::string & key, uint32_t quality)
{
  impl_->set_quality (key, quality);
}

#ifdef _USE_CID_

void
Madara::Knowledge_Engine::Knowledge_Base::print_all_redeployment_results (
  std::ostream & output)
{
  return impl_->print_all_redeployment_results (output);
}

void
Madara::Knowledge_Engine::Knowledge_Base::run_all (void)
{
  return impl_->run_all ();
}

long
Madara::Knowledge_Engine::Knowledge_Base::start_latency (void)
{
  return impl_->start_latency ();
}

void
Madara::Knowledge_Engine::Knowledge_Base::print_my_latencies (
  std::ostream & output)
{
  impl_->print_my_latencies (output);
}

void
Madara::Knowledge_Engine::Knowledge_Base::print_all_latencies (
  std::ostream & output)
{
  impl_->print_all_latencies (output);
}

void
Madara::Knowledge_Engine::Knowledge_Base::print_all_summations (
  std::ostream & output)
{
  impl_->print_all_summations (output);
}

long
Madara::Knowledge_Engine::Knowledge_Base::vote (void)
{
  return impl_->vote ();
}

#endif // _USE_CID_

Madara::Transport::Settings &
Madara::Knowledge_Engine::Knowledge_Base::transport_settings (void)
{
  return impl_->transport_settings ();
}

Madara::Knowledge_Record
Madara::Knowledge_Engine::Knowledge_Base::wait (const std::string & expression)
{
  Wait_Settings settings;
  return impl_->wait (expression, settings);
}

Madara::Knowledge_Record
Madara::Knowledge_Engine::Knowledge_Base::wait (
  Compiled_Expression & expression, 
  const Wait_Settings & settings)
{
  return impl_->wait (expression, settings);
}

void
Madara::Knowledge_Engine::Knowledge_Base::print_knowledge (
  unsigned int level)
{
  impl_->print_knowledge (level);
}

void
Madara::Knowledge_Engine::Knowledge_Base::print (
  const std::string & statement, unsigned int level)
{
  impl_->print (statement, level);
}

void
Madara::Knowledge_Engine::Knowledge_Base::clear (void)
{
  impl_->clear ();
}

void
Madara::Knowledge_Engine::Knowledge_Base::clear_map (void)
{
  impl_->clear_map ();
}

void
Madara::Knowledge_Engine::Knowledge_Base::acquire (void)
{
  impl_->acquire ();
}

void
Madara::Knowledge_Engine::Knowledge_Base::release (void)
{
  impl_->release ();
}

// evaluate a knowledge expression and send any modifications
Madara::Knowledge_Record
Madara::Knowledge_Engine::Knowledge_Base::evaluate (
  const std::string & expression)
{
  Eval_Settings settings;
  return impl_->evaluate (expression, settings);
}

// evaluate a knowledge expression and choose to send any modifications
Madara::Knowledge_Record
Madara::Knowledge_Engine::Knowledge_Base::evaluate (
  const std::string & expression,
  const Eval_Settings & settings)
{
  return impl_->evaluate (expression, settings);
}

// evaluate a knowledge expression and choose to send any modifications
Madara::Knowledge_Record
Madara::Knowledge_Engine::Knowledge_Base::evaluate (
  Compiled_Expression & expression,
  const Eval_Settings & settings)
{
  return impl_->evaluate (expression, settings);
}


// Defines a function
void Madara::Knowledge_Engine::Knowledge_Base::define_function (
  const std::string & name, VALUE_TYPE (*func) (Function_Arguments &, Variables &))
{
  impl_->define_function (name, func);
}
