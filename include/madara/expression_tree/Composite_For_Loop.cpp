/* -*- C++ -*- */
#ifndef _FOR_LOOP_CPP_
#define _FOR_LOOP_CPP_

#include <iostream>

#include "madara/expression_tree/Component_Node.h"
#include "madara/expression_tree/Composite_Unary_Node.h"
#include "madara/expression_tree/Visitor.h"
#include "madara/expression_tree/Composite_For_Loop.h"
#include "madara/expression_tree/Leaf_Node.h"
#include "madara/expression_tree/Composite_Assignment_Node.h"

#include "madara/utility/Log_Macros.h"

// Ctor

Madara::Expression_Tree::Composite_For_Loop::Composite_For_Loop (
       Component_Node * precondition,
        Component_Node * condition, 
        Component_Node * postcondition,
        Component_Node * body,
        Madara::Knowledge_Engine::Thread_Safe_Context & context)
  : context_(context), precondition_ (precondition), condition_ (condition), 
    postcondition_ (postcondition), body_ (body)
{
  
}

// Dtor
Madara::Expression_Tree::Composite_For_Loop::~Composite_For_Loop (void)
{
}

Madara::Knowledge_Record
Madara::Expression_Tree::Composite_For_Loop::item (void) const
{
  Madara::Knowledge_Record record;
  record.set_value ("for (;;)");
  return record;
}

/// Prune the tree of unnecessary nodes. 
/// Returns evaluation of the node and sets can_change appropriately.
/// if this node can be changed, that means it shouldn't be pruned.
Madara::Knowledge_Record
Madara::Expression_Tree::Composite_For_Loop::prune (bool & can_change)
{
  // user can always change a function, and we have no control over
  // what it does. Consequently, a function node cannot be pruned out
  // under any situation
  can_change = true;
  
  Madara::Knowledge_Record zero;
  return zero;
}

/// Evaluates the node and its children. This does not prune any of
/// the expression tree, and is much faster than the prune function
Madara::Knowledge_Record 
Madara::Expression_Tree::Composite_For_Loop::evaluate (
const Madara::Knowledge_Engine::Knowledge_Update_Settings & settings)
{
  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
    DLINFO "Composite_For_Loop::evaluate:" \
    " Executing precondition.\n"));
  precondition_->evaluate (settings);

  Madara::Knowledge_Record::Integer count = 0;
  while (condition_->evaluate (settings).is_true ())
  {
    MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
      DLINFO "Composite_For_Loop::evaluate:" \
      " Executing loop body.\n"));

    body_->evaluate (settings);

    MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
      DLINFO "Composite_For_Loop::evaluate:" \
      " Executing postcondition.\n"));

    postcondition_->evaluate (settings);
    ++count;
  }

  // return is the number of successful body executions
  
  Madara::Knowledge_Record evaluations;
  evaluations.set_value (count);
  return evaluations;
}

// accept a visitor
void 
Madara::Expression_Tree::Composite_For_Loop::accept (Visitor &visitor) const
{
  visitor.visit (*this);
}

#endif /* _FOR_LOOP_CPP_ */
