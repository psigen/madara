
#include "madara/knowledge_engine/Knowledge_Base.h"
#include "madara/knowledge_engine/Knowledge_Base_Impl.h"
#include "madara/utility/Log_Macros.h"

#include <sstream>
#include <iostream>

void Madara::Knowledge_Engine::Knowledge_Base::log_to_stderr (
  bool clear_flags)
{
  ACE_LOG_MSG->set_flags (ACE_Log_Msg::STDERR);
  if (clear_flags)
  {
    ACE_LOG_MSG->clr_flags (ACE_Log_Msg::OSTREAM);
    ACE_LOG_MSG->clr_flags (ACE_Log_Msg::SYSLOG);
  }
}

void Madara::Knowledge_Engine::Knowledge_Base::log_to_file (
  const char * filename, bool clear_flags)
{
  // get the old message output stream and delete it
  ACE_OSTREAM_TYPE * output = new std::ofstream (filename);
  ACE_LOG_MSG->msg_ostream (output, true);
  ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);

  if (clear_flags)
  {
    ACE_LOG_MSG->clr_flags (ACE_Log_Msg::STDERR);
    ACE_LOG_MSG->clr_flags (ACE_Log_Msg::SYSLOG);
  }
}

void Madara::Knowledge_Engine::Knowledge_Base::log_to_system_log (
  const char * prog_name, bool clear_flags)
{
  // open a new socket to the SYSLOG with madara set as logging agent
  ACE_LOG_MSG->open (prog_name, ACE_Log_Msg::SYSLOG, prog_name);

  if (clear_flags)
  {
    ACE_LOG_MSG->clr_flags (ACE_Log_Msg::STDERR);
    ACE_LOG_MSG->clr_flags (ACE_Log_Msg::OSTREAM);
  }
}

int
Madara::Knowledge_Engine::Knowledge_Base::log_level (int level)
{
  if (level >= 0)
    MADARA_debug_level = level;

  return MADARA_debug_level;
}

int
Madara::Knowledge_Engine::Knowledge_Base::log_level (void)
{
  return MADARA_debug_level;
}


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
Madara::Knowledge_Engine::Knowledge_Base::get (const std::string & key,
             const Knowledge_Reference_Settings & settings)
{
  return impl_->get (key, settings);
}

Madara::Knowledge_Engine::Variable_Reference
Madara::Knowledge_Engine::Knowledge_Base::get_ref (const std::string & key,
             const Knowledge_Reference_Settings & settings)
{
  return impl_->get_ref (key, settings);
}

Madara::Knowledge_Record
Madara::Knowledge_Engine::Knowledge_Base::get (
  const Variable_Reference & variable,
  const Knowledge_Reference_Settings & settings)
{
  return impl_->get (variable, settings);
}

Madara::Knowledge_Record
Madara::Knowledge_Engine::Knowledge_Base::retrieve_index (
  const std::string & key, size_t index,
  const Knowledge_Reference_Settings & settings)
{
  return impl_->retrieve_index (key, index, settings);
}

Madara::Knowledge_Record
Madara::Knowledge_Engine::Knowledge_Base::retrieve_index (
  const Variable_Reference & variable, size_t index,
  const Knowledge_Reference_Settings & settings)
{
  return impl_->retrieve_index (variable, index, settings);
}


int
Madara::Knowledge_Engine::Knowledge_Base::read_file (
  const std::string & knowledge_key, 
  const std::string & filename, 
  const Eval_Settings & settings)
{
  return impl_->read_file (knowledge_key, filename, settings);
}

int
Madara::Knowledge_Engine::Knowledge_Base::read_file (
  const Variable_Reference & variable, 
  const std::string & filename, 
  const Eval_Settings & settings)
{
  return impl_->read_file (variable, filename, settings);
}

int
Madara::Knowledge_Engine::Knowledge_Base::read_policy (
  const std::string & knowledge_key, 
  const std::string & filename)
{
  return impl_->read_policy (knowledge_key, filename);
}

ssize_t
Madara::Knowledge_Engine::Knowledge_Base::write_file (const std::string & knowledge_key, 
                     const std::string & filename)
{
  return impl_->write_file (knowledge_key, filename);
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
  Madara::Knowledge_Record::Integer value,
  const Eval_Settings & settings)
{
  return impl_->set (key, value, settings);
}

int
Madara::Knowledge_Engine::Knowledge_Base::set (
  const Variable_Reference & variable,
  Madara::Knowledge_Record::Integer value,
  const Eval_Settings & settings)
{
  return impl_->set (variable, value, settings);
}

int
Madara::Knowledge_Engine::Knowledge_Base::set_index (
  const std::string & key,
  size_t index,
  Madara::Knowledge_Record::Integer value,
  const Eval_Settings & settings)
{
  return impl_->set_index (key, index, value, settings);
}

int
Madara::Knowledge_Engine::Knowledge_Base::set_index (
  const Variable_Reference & variable,
  size_t index,
  Madara::Knowledge_Record::Integer value,
  const Eval_Settings & settings)
{
  return impl_->set_index (variable, index, value, settings);
}

int
Madara::Knowledge_Engine::Knowledge_Base::set (
  const std::string & key,
  const Knowledge_Record::Integer * value,
  uint32_t size,
  const Eval_Settings & settings)
{
  return impl_->set (key, value, size, settings);
}

int
Madara::Knowledge_Engine::Knowledge_Base::set (
  const Variable_Reference & variable,
  const Knowledge_Record::Integer * value,
  uint32_t size,
  const Eval_Settings & settings)
{
  return impl_->set (variable, value, size, settings);
}

int
Madara::Knowledge_Engine::Knowledge_Base::set (
  const std::string & key,
  const std::vector <Knowledge_Record::Integer> & value,
  const Eval_Settings & settings)
{
  return impl_->set (key, value, settings);
}

int
Madara::Knowledge_Engine::Knowledge_Base::set (
  const Variable_Reference & variable,
  const std::vector <Knowledge_Record::Integer> & value,
  const Eval_Settings & settings)
{
  return impl_->set (variable, value, settings);
}

int
Madara::Knowledge_Engine::Knowledge_Base::set (
  const std::string & key,
  double value,
  const Eval_Settings & settings)
{
  return impl_->set (key, value, settings);
}

int
Madara::Knowledge_Engine::Knowledge_Base::set (
  const Variable_Reference & variable,
  double value,
  const Eval_Settings & settings)
{
  return impl_->set (variable, value, settings);
}

int
Madara::Knowledge_Engine::Knowledge_Base::set_index (
  const std::string & key,
  size_t index,
  double value,
  const Eval_Settings & settings)
{
  return impl_->set_index (key, index, value, settings);
}

int
Madara::Knowledge_Engine::Knowledge_Base::set_index (
  const Variable_Reference & variable,
  size_t index,
  double value,
  const Eval_Settings & settings)
{
  return impl_->set_index (variable, index, value, settings);
}

int
Madara::Knowledge_Engine::Knowledge_Base::set (
  const std::string & key,
  const double * value,
  uint32_t size,
  const Eval_Settings & settings)
{
  return impl_->set (key, value, size, settings);
}

int
Madara::Knowledge_Engine::Knowledge_Base::set (
  const Variable_Reference & variable,
  const double * value,
  uint32_t size,
  const Eval_Settings & settings)
{
  return impl_->set (variable, value, size, settings);
}

int
Madara::Knowledge_Engine::Knowledge_Base::set (
  const std::string & key,
  const std::vector <double> & value,
  const Eval_Settings & settings)
{
  return impl_->set (key, value, settings);
}

int
Madara::Knowledge_Engine::Knowledge_Base::set (
  const Variable_Reference & variable,
  const std::vector <double> & value,
  const Eval_Settings & settings)
{
  return impl_->set (variable, value, settings);
}

int
Madara::Knowledge_Engine::Knowledge_Base::set (const std::string & key,
  const std::string & value,
  const Eval_Settings & settings)
{
  return impl_->set (key, value, settings);
}

int
Madara::Knowledge_Engine::Knowledge_Base::set (
  const Variable_Reference & variable,
  const std::string & value,
  const Eval_Settings & settings)
{
  return impl_->set (variable, value, settings);
}


/// Set quality of writing to a variable
void 
Madara::Knowledge_Engine::Knowledge_Base::set_quality (
  const std::string & key, uint32_t quality,
  const Knowledge_Reference_Settings & settings)
{
  impl_->set_quality (key, quality, settings);
}


bool
Madara::Knowledge_Engine::Knowledge_Base::exists (
  const std::string & key,
  const Knowledge_Reference_Settings & settings)

{
  return impl_->exists (key, settings);
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
Madara::Knowledge_Engine::Knowledge_Base::wait (
  Compiled_Expression & expression, 
  const Wait_Settings & settings)
{
  return impl_->wait (expression, settings);
}

Madara::Knowledge_Record
Madara::Knowledge_Engine::Knowledge_Base::wait (
  const std::string & expression, 
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
  const std::string & name,
    VALUE_TYPE (*func) (Function_Arguments &, Variables &))
{
  impl_->define_function (name, func);
}

// Defines a function
void Madara::Knowledge_Engine::Knowledge_Base::define_function (
  const std::string & name,
    VALUE_TYPE (*func) (const char *, Function_Arguments &, Variables &))
{
  impl_->define_function (name, func);
}


/**
  * Defines a MADARA KaRL function
  * @param  name       name of the function
  * @param  expression KaRL function body       
  **/
void
Madara::Knowledge_Engine::Knowledge_Base::define_function (const std::string & name,
  const std::string & expression)
{
  impl_->define_function (name, expression);
}
      
/**
  * Defines a MADARA KaRL function
  * @param  name       name of the function
  * @param  expression KaRL function body       
  **/
void
Madara::Knowledge_Engine::Knowledge_Base::define_function (const std::string & name,
  const Compiled_Expression & expression)
{
  impl_->define_function (name, expression);
}
    

/**
  * Attaches a transport to the Knowledge Engine. Note that the
  * transport should use the same Thread_Safe_Context as the
  * Knowledge Engine.
  * @param  transport   a new transport to attach tot he Knowledge Base
  * @return             the number of transports now attached
  **/
ssize_t
Madara::Knowledge_Engine::Knowledge_Base::attach_transport (
  Madara::Transport::Base * transport)
{
  return impl_->attach_transport (transport);
}

/**
  * Returns the Thread_Safe_Context associated with this Knowledge
  * Base. This is necessary for creating custom transports.
  *
  * @return             the context used by the knowledge base
  **/
Madara::Knowledge_Engine::Thread_Safe_Context &
Madara::Knowledge_Engine::Knowledge_Base::get_context (void)
{
  return impl_->get_context ();
}

/**
  * Returns the unique host and ephemeral binding for this Knowlede Base
  * @return             host:port identifier for this knowledge base
  **/
std::string
Madara::Knowledge_Engine::Knowledge_Base::get_id (void)
{
  return impl_->get_id ();
}

/**
  * Fills a vector with Knowledge Records that begin with a common subject
  * and have a finite range of integer values.
  * @param   subject     The common subject of the variable names. For
  *                      instance, if we are looking for a range of vars
  *                      like "var0", "var1", "var2", then the common
  *                      subject would be "var".
  * @param   start       An inclusive start index
  * @param   end         An inclusive end index
  * @param   target      The vector that will be filled with
  *                      Knowledge Record instances within the subject
  *                      range.
  * @return              entries in the resulting vector
  **/
size_t
Madara::Knowledge_Engine::Knowledge_Base::to_vector (
                              const std::string & subject,
                              unsigned int start,
                              unsigned int end,
                              std::vector <Knowledge_Record> & target)
{
  return impl_->to_vector (subject, start, end, target);
}

/**
  * Fills a variable map with Knowledge Records that match an expression.
  * At the moment, this expression must be of the form "subject*"
  * @param   expression  An expression that matches the variable names
  *                      that are of interest. Wildcards may only be
  *                      at the end.
  * @param   target      The map that will be filled with variable names
  *                      and the Knowledge Records they correspond to
  * @return              entries in the resulting map
  **/
size_t
Madara::Knowledge_Engine::Knowledge_Base::to_map (
  const std::string & expression,
  std::map <std::string, Knowledge_Record> & target)
{
  return impl_->to_map (expression, target);
}

size_t
Madara::Knowledge_Engine::Knowledge_Base::save_context (
  const std::string & filename)
{
  return impl_->save_context (filename);
}

size_t
  Madara::Knowledge_Engine::Knowledge_Base::load_context (
  const std::string & filename,
  bool use_id,
  const Knowledge_Update_Settings & settings)
{
  return impl_->load_context (filename, use_id, settings);
}
