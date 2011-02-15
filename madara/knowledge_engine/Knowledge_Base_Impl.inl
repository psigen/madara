#ifndef _MADARA_KNOWLEDGE_BASE_IMPL_INL_
#define _MADARA_KNOWLEDGE_BASE_IMPL_INL_

#include "madara/utility/Utility.h"
#include "madara/knowledge_engine/Knowledge_Base_Impl.h"
#include "madara/expression_tree/Interpreter.h"
#include "madara/expression_tree/Expression_Tree.h"
#include "ace/Log_Msg.h"
/**
 * @file Knowledge_Base_Impl.inl
 * @author James Edmondson <james.r.edmondson@vanderbilt.edu>
 *
 * This file contains the inline functions for Knowledge_Base_Impl class
 */

inline long
Madara::Knowledge_Engine::Knowledge_Base_Impl::get (const ::std::string & key) const
{
  return map_.get (key);
}

inline std::string
Madara::Knowledge_Engine::Knowledge_Base_Impl::expand_statement (
  const ::std::string & statement) const
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

inline int
Madara::Knowledge_Engine::Knowledge_Base_Impl::set (const ::std::string & key, 
                                               long value)
{
  return set (key, value, true);
}

/// Set quality of writing to a variable
inline void 
Madara::Knowledge_Engine::Knowledge_Base_Impl::set_quality (
  const ::std::string & key, unsigned long quality)
{
  map_.set_write_quality (key, quality);
}


inline int
Madara::Knowledge_Engine::Knowledge_Base_Impl::wait (const ::std::string & expression)
{
  return wait (expression, true);
}

inline int
Madara::Knowledge_Engine::Knowledge_Base_Impl::evaluate (
  const ::std::string & expression)
{
  return evaluate (expression, true);
}

inline void
Madara::Knowledge_Engine::Knowledge_Base_Impl::print_knowledge (void) const
{
  ACE_DEBUG ((LM_INFO, "\nKnowledge in Knowledge Base:\n"));
  map_.print ();
}

inline void
Madara::Knowledge_Engine::Knowledge_Base_Impl::print (
  const std::string & statement) const
{
  map_.print (statement);
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
