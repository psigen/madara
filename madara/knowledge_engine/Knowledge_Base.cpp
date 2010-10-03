#include "madara/utility/Utility.h"
#include "madara/knowledge_engine/Knowledge_Base.h"
#include "madara/knowledge_engine/Knowledge_Base_Impl.h"
#include "madara/expression_tree/Interpreter.h"
#include "madara/expression_tree/Evaluation_Visitor.h"
#include "madara/expression_tree/Print_Visitor.h"
#include "madara/expression_tree/Expression_Tree.h"
#include "madara/expression_tree/Iterator.h"
#include "madara/transport/TCP_Transport.h"
#include "madara/utility/Utility.h"

#include <sstream>


#ifdef _USE_OPEN_SPLICE_
  #include "madara/transport/Splice_DDS_Transport.h"
#endif // _USE_OPEN_SPLICE_

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
Madara::Knowledge_Engine::Knowledge_Base::close_transport (void)
{
  impl_->close_transport ();
}

int
Madara::Knowledge_Engine::Knowledge_Base::get (const ::std::string & key) const
{
  return impl_->get (key);
}

void
Madara::Knowledge_Engine::Knowledge_Base::set (const ::std::string & key, 
                                               int value)
{
  impl_->set (key, value, true);
}

void
Madara::Knowledge_Engine::Knowledge_Base::set (const ::std::string & key, 
                                               int value, bool send_modifieds)
{
  impl_->set (key, value, send_modifieds);
}


int
Madara::Knowledge_Engine::Knowledge_Base::wait (const ::std::string & expression)
{
  return impl_->wait (expression, true);
}

int
Madara::Knowledge_Engine::Knowledge_Base::wait (const ::std::string & expression, 
                                                bool send_modifieds)
{
  return impl_->wait (expression, send_modifieds);
}

void
Madara::Knowledge_Engine::Knowledge_Base::add_rule (const ::std::string & expression)
{
  impl_->add_rule (expression);
}

int
Madara::Knowledge_Engine::Knowledge_Base::evaluate (
  const ::std::string & expression)
{
  return impl_->evaluate (expression, true);
}

int
Madara::Knowledge_Engine::Knowledge_Base::evaluate (
  const ::std::string & expression, bool send_modifieds)
{
  return impl_->evaluate (expression, send_modifieds);
}

void
Madara::Knowledge_Engine::Knowledge_Base::print_rules (void) const
{
  impl_->print_rules ();
}

void
Madara::Knowledge_Engine::Knowledge_Base::print_knowledge (void) const
{
  impl_->print_knowledge ();
}

void
Madara::Knowledge_Engine::Knowledge_Base::clear (void)
{
  impl_->clear ();
}

void
Madara::Knowledge_Engine::Knowledge_Base::clear_rules (void)
{
  impl_->clear_rules ();
}

void
Madara::Knowledge_Engine::Knowledge_Base::clear_map (void)
{
  impl_->clear_map ();
}
