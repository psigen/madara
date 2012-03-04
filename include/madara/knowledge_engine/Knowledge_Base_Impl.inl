#ifndef _MADARA_KNOWLEDGE_BASE_IMPL_INL_
#define _MADARA_KNOWLEDGE_BASE_IMPL_INL_

#include "madara/utility/Utility.h"
#include "madara/knowledge_engine/Knowledge_Base_Impl.h"
#include "madara/expression_tree/Interpreter.h"
#include "madara/expression_tree/Expression_Tree.h"
#include "madara/utility/Log_Macros.h"
/**
 * @file Knowledge_Base_Impl.inl
 * @author James Edmondson <james.r.edmondson@vanderbilt.edu>
 *
 * This file contains the inline functions for Knowledge_Base_Impl class
 */

inline long long
Madara::Knowledge_Engine::Knowledge_Base_Impl::get (
  const ::std::string & t_key)
{
  std::string key = map_.expand_statement (t_key);

  return map_.get (key);
}

inline std::string
Madara::Knowledge_Engine::Knowledge_Base_Impl::expand_statement (
  const ::std::string & statement)
{
  return map_.expand_statement (statement);
}

/// Read a file into the knowledge base
inline int
Madara::Knowledge_Engine::Knowledge_Base_Impl::read_file (
  const std::string & knowledge_key, const std::string & file_name)
{
  return files_.read_file (knowledge_key, file_name);
}

/// Read a policy into the knowledge base
inline int
Madara::Knowledge_Engine::Knowledge_Base_Impl::read_policy (
  const std::string & knowledge_key, const std::string & file_name)
{
  return files_.read_policy (knowledge_key, file_name);
}

/// Write file from the knowledge base to a specified file
inline int
Madara::Knowledge_Engine::Knowledge_Base_Impl::write_file (
  const std::string & knowledge_key, const std::string & file_name)
{
  return files_.write_file (knowledge_key, file_name);
}

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

inline int
Madara::Knowledge_Engine::Knowledge_Base_Impl::set (const ::std::string & key,
                                               long long value)
{
  return set (key, value, true);
}

/// Set quality of writing to a variable
inline void 
Madara::Knowledge_Engine::Knowledge_Base_Impl::set_quality (
  const ::std::string & t_key, unsigned long quality)
{
  std::string key = map_.expand_statement (t_key);

  map_.set_write_quality (key, quality);
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

inline Madara::Transport::Settings &
Madara::Knowledge_Engine::Knowledge_Base_Impl::transport_settings (void)
{
  return settings_;
}


inline long long
Madara::Knowledge_Engine::Knowledge_Base_Impl::wait (
  const ::std::string & expression)
{
  return wait (expression, true);
}

inline long long
Madara::Knowledge_Engine::Knowledge_Base_Impl::evaluate (
  const ::std::string & expression)
{
  return evaluate (expression, true);
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
  rules_.clear ();
}

inline void
Madara::Knowledge_Engine::Knowledge_Base_Impl::clear_rules (void)
{
  rules_.clear ();
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


#endif  // _MADARA_KNOWLEDGE_BASE_IMPL_INL_
