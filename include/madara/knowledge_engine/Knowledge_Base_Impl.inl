#ifndef _MADARA_KNOWLEDGE_BASE_IMPL_INL_
#define _MADARA_KNOWLEDGE_BASE_IMPL_INL_

#include "madara/utility/Utility.h"
#include "madara/knowledge_engine/Knowledge_Base_Impl.h"
#include "madara/knowledge_engine/Knowledge_Record.h"
#include "madara/expression_tree/Interpreter.h"
#include "madara/expression_tree/Expression_Tree.h"
#include "madara/utility/Log_Macros.h"

/**
 * @file Knowledge_Base_Impl.inl
 * @author James Edmondson <james.r.edmondson@vanderbilt.edu>
 *
 * This file contains the inline functions for Knowledge_Base_Impl class
 */

inline Madara::Knowledge_Record
Madara::Knowledge_Engine::Knowledge_Base_Impl::get (
  const std::string & t_key,
  const Knowledge_Reference_Settings & settings)
{
  return map_.get (t_key, settings);
}

inline bool
Madara::Knowledge_Engine::Knowledge_Base_Impl::exists (
  const std::string & key,
  const Knowledge_Reference_Settings & settings) const
{
  return map_.exists (key, settings);
}

inline std::string
Madara::Knowledge_Engine::Knowledge_Base_Impl::expand_statement (
  const std::string & statement)
{
  return map_.expand_statement (statement);
}

/// Read a policy into the knowledge base
inline int
Madara::Knowledge_Engine::Knowledge_Base_Impl::read_policy (
  const std::string & knowledge_key, const std::string & filename)
{
  return files_.read_policy (knowledge_key, filename);
}

/// Write file from the knowledge base to a specified file
inline int
Madara::Knowledge_Engine::Knowledge_Base_Impl::write_file (
  const std::string & knowledge_key, const std::string & filename)
{
  return map_.get_record (knowledge_key)->to_file (filename);
}

inline int
Madara::Knowledge_Engine::Knowledge_Base_Impl::set (const std::string & key,
                                    Madara::Knowledge_Record::Integer value)
{
  Eval_Settings settings;
  return set (key, value, settings);
}

inline int
Madara::Knowledge_Engine::Knowledge_Base_Impl::set (const std::string & key,
                                               double value)
{
  Eval_Settings settings;
  return set (key, value, settings);
}

inline int
Madara::Knowledge_Engine::Knowledge_Base_Impl::set (const std::string & key,
                                               const std::string & value)
{
  Eval_Settings settings;
  return set (key, value, settings);
}

/// Set quality of writing to a variable
inline void 
Madara::Knowledge_Engine::Knowledge_Base_Impl::set_quality (
  const std::string & t_key, uint32_t quality,
  const Knowledge_Reference_Settings & settings)
{
  map_.set_write_quality (t_key, quality, settings);
}

#ifdef _USE_CID_

inline void
Madara::Knowledge_Engine::Knowledge_Base_Impl::print_all_redeployment_results (
  std::ostream & output)
{
  settings_.print_all_results (output);
}

inline void
Madara::Knowledge_Engine::Knowledge_Base_Impl::run_all (void)
{
  settings_.run_all ();
}

inline void
Madara::Knowledge_Engine::Knowledge_Base_Impl::print_my_latencies (
  std::ostream & output)
{
  settings_.print_my_latencies (output);
}

inline void
Madara::Knowledge_Engine::Knowledge_Base_Impl::print_all_latencies (
  std::ostream & output)
{
  settings_.print_all_latencies (output);
}

inline void
Madara::Knowledge_Engine::Knowledge_Base_Impl::print_all_summations (
  std::ostream & output)
{
  settings_.print_all_summations (output);
}

inline long
Madara::Knowledge_Engine::Knowledge_Base_Impl::start_latency (void)
{
  if (transport_)
    return transport_->start_latency ();
  else
    return -1;
}

inline long
Madara::Knowledge_Engine::Knowledge_Base_Impl::vote (void)
{
  if (transport_)
    return transport_->vote ();
  else
    return -1;
}

#endif // _USE_CID_

inline Madara::Transport::Settings &
Madara::Knowledge_Engine::Knowledge_Base_Impl::transport_settings (void)
{
  return settings_;
}

inline Madara::Knowledge_Record
Madara::Knowledge_Engine::Knowledge_Base_Impl::wait (
  const std::string & expression)
{
  Compiled_Expression compiled = compile (expression);
  Wait_Settings settings;
  return wait (compiled, settings);
}

inline Madara::Knowledge_Record
Madara::Knowledge_Engine::Knowledge_Base_Impl::evaluate (
  const std::string & expression)
{
  Compiled_Expression compiled = compile (expression);
  Eval_Settings settings;
  return evaluate (compiled, settings);
}

inline void
Madara::Knowledge_Engine::Knowledge_Base_Impl::print_knowledge (
  unsigned int level)
{
  MADARA_DEBUG (level, (LM_INFO, 
    "\nKnowledge in Knowledge Base:\n"));
  map_.print (level);
}

inline void
Madara::Knowledge_Engine::Knowledge_Base_Impl::print (
  const std::string & statement, unsigned int level)
{
  map_.print (statement, level);
}

inline void
Madara::Knowledge_Engine::Knowledge_Base_Impl::clear (void)
{
  map_.clear ();
}

inline void
Madara::Knowledge_Engine::Knowledge_Base_Impl::clear_map (void)
{
  map_.clear ();
}

/// lock the underlying knowledge base against any updates
/// until we release
inline void Madara::Knowledge_Engine::Knowledge_Base_Impl::acquire (void)
{
  map_.lock ();
}

/// release the lock on the underlying knowledge base
inline void Madara::Knowledge_Engine::Knowledge_Base_Impl::release (void)
{
  map_.unlock ();
}

// Defines a function
inline
void Madara::Knowledge_Engine::Knowledge_Base_Impl::define_function (
  const std::string & name, VALUE_TYPE (*func) (Function_Arguments &, Variables &))
{
  map_.define_function (name, func);
}

// Defines a function
inline
void Madara::Knowledge_Engine::Knowledge_Base_Impl::define_function (
  const std::string & name,
    VALUE_TYPE (*func) (const char *, Function_Arguments &, Variables &))
{
  map_.define_function (name, func);
}

/**
  * Defines a MADARA KaRL function      
  **/
inline
void
Madara::Knowledge_Engine::Knowledge_Base_Impl::define_function (const std::string & name,
  const std::string & expression)
{
  map_.define_function (name, expression);
}
      
/**
  * Defines a MADARA KaRL function    
  **/
inline
void
Madara::Knowledge_Engine::Knowledge_Base_Impl::define_function (
  const std::string & name,
  const Compiled_Expression & expression)
{
  map_.define_function (name, expression);
}
     
/**
  * Attaches a transport to the Knowledge Engine.   
  **/
inline
ssize_t
Madara::Knowledge_Engine::Knowledge_Base_Impl::attach_transport (
  Madara::Transport::Base * transport)
{
  transports_.push_back (transport);
  return transports_.size ();
}

/**
  * Returns the Thread_Safe_Context associated with this Knowledge
  * Base. This is necessary for creating custom transports.
  *
  * @return             the context used by the knowledge base
  **/
inline 
Madara::Knowledge_Engine::Thread_Safe_Context & 
Madara::Knowledge_Engine::Knowledge_Base_Impl::get_context (void)
{
  return map_;
}


/**
  * Returns the unique host and ephemeral binding for this Knowlede Base
  * @return             host:port identifier for this knowledge base
  **/
inline
std::string 
Madara::Knowledge_Engine::Knowledge_Base_Impl::get_id (void)
{
  return id_;
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
inline
unsigned int
Madara::Knowledge_Engine::Knowledge_Base_Impl::to_vector (
                              const std::string & subject,
                              unsigned int start,
                              unsigned int end,
                              std::vector <Knowledge_Record> & target)
{
  return map_.to_vector (subject, start, end, target);
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
inline
unsigned int
Madara::Knowledge_Engine::Knowledge_Base_Impl::to_map (
  const std::string & expression,
  std::map <std::string, Knowledge_Record> & target)
{
  return map_.to_map (expression, target);
}

#endif  // _MADARA_KNOWLEDGE_BASE_IMPL_INL_
