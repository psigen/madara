#ifndef _MADARA_KNOWLEDGE_BASE_INL_
#define _MADARA_KNOWLEDGE_BASE_INL_

/**
 * @file Knowledge_Base.inl
 * @author James Edmondson <james.r.edmondson@vanderbilt.edu>
 *
 * This file contains the inline functions for the Knowledge_Base_Impl class
 */

#include "madara/knowledge_engine/Knowledge_Base_Impl.h"

/// Assignment operator.
inline void 
Madara::Knowledge_Engine::Knowledge_Base::operator= (
  const Knowledge_Base &original)
{
  // Refcounter class takes care of the internal decrements and
  // increments.
  if (this != &original)
    impl_ = original.impl_;   
}

inline void
Madara::Knowledge_Engine::Knowledge_Base::close_transport (void)
{
  impl_->close_transport ();
}

inline long
Madara::Knowledge_Engine::Knowledge_Base::get (const ::std::string & key) const
{
  return impl_->get (key);
}


inline int
Madara::Knowledge_Engine::Knowledge_Base::read_file (const std::string & knowledge_key, 
                     const std::string & file_name)
{
  return impl_->read_file (knowledge_key, file_name);
}

inline int
Madara::Knowledge_Engine::Knowledge_Base::read_policy (const std::string & knowledge_key, 
                     const std::string & file_name)
{
  return impl_->read_policy (knowledge_key, file_name);
}

inline int
Madara::Knowledge_Engine::Knowledge_Base::write_file (const std::string & knowledge_key, 
                     const std::string & file_name)
{
  return impl_->write_file (knowledge_key, file_name);
}


inline std::string
Madara::Knowledge_Engine::Knowledge_Base::expand_statement (
  const ::std::string & statement) const
{
  return impl_->expand_statement (statement);
}

inline int
Madara::Knowledge_Engine::Knowledge_Base::set (const ::std::string & key, 
                                               long value)
{
  return impl_->set (key, value, true);
}

inline int
Madara::Knowledge_Engine::Knowledge_Base::set (const ::std::string & key, 
                                               long value, bool send_modifieds)
{
  return impl_->set (key, value, send_modifieds);
}


/// Set quality of writing to a variable
inline void 
Madara::Knowledge_Engine::Knowledge_Base::set_quality (
  const ::std::string & key, unsigned long quality)
{
  impl_->set_quality (key, quality);
}

inline int
Madara::Knowledge_Engine::Knowledge_Base::wait (const ::std::string & expression)
{
  return impl_->wait (expression, true);
}

inline int
Madara::Knowledge_Engine::Knowledge_Base::wait (const ::std::string & expression, 
                                                bool send_modifieds)
{
  return impl_->wait (expression, send_modifieds);
}

inline void
Madara::Knowledge_Engine::Knowledge_Base::add_rule (const ::std::string & expression)
{
  impl_->add_rule (expression);
}


inline void
Madara::Knowledge_Engine::Knowledge_Base::print_rules (void) const
{
  impl_->print_rules ();
}

inline void
Madara::Knowledge_Engine::Knowledge_Base::print_knowledge (void) const
{
  impl_->print_knowledge ();
}

inline void
Madara::Knowledge_Engine::Knowledge_Base::print (
  const std::string & statement) const
{
  impl_->print (statement);
}

inline void
Madara::Knowledge_Engine::Knowledge_Base::clear (void)
{
  impl_->clear ();
}

inline void
Madara::Knowledge_Engine::Knowledge_Base::clear_rules (void)
{
  impl_->clear_rules ();
}

inline void
Madara::Knowledge_Engine::Knowledge_Base::clear_map (void)
{
  impl_->clear_map ();
}

inline void
Madara::Knowledge_Engine::Knowledge_Base::acquire (void)
{
  impl_->acquire ();
}

inline void
Madara::Knowledge_Engine::Knowledge_Base::release (void)
{
  impl_->release ();
}

// evaluate a knowledge expression and send any modifications
inline int
Madara::Knowledge_Engine::Knowledge_Base::evaluate (
  const ::std::string & expression)
{
  return impl_->evaluate (expression, true);
}

// evaluate a knowledge expression and choose to send any modifications
inline int
Madara::Knowledge_Engine::Knowledge_Base::evaluate (
  const ::std::string & expression, bool send_modifieds)
{
  return impl_->evaluate (expression, send_modifieds);
}



#endif   // _MADARA_KNOWLEDGE_BASE_INL_
