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

long long
Madara::Expression_Tree::Composite_For_Loop::item (void) const
{
  return '-';
}

/// Prune the tree of unnecessary nodes. 
/// Returns evaluation of the node and sets can_change appropriately.
/// if this node can be changed, that means it shouldn't be pruned.
long long
Madara::Expression_Tree::Composite_For_Loop::prune (bool & can_change)
{
  // user can always change a function, and we have no control over
  // what it does. Consequently, a function node cannot be pruned out
  // under any situation
  can_change = true;
  
  return 0;
}

/// Evaluates the node and its children. This does not prune any of
/// the expression tree, and is much faster than the prune function
long long 
Madara::Expression_Tree::Composite_For_Loop::evaluate (void)
{
  long long count = 0;
  for (precondition_->evaluate ();
       condition_->evaluate ();
       postcondition_->evaluate ())
  {
    body_->evaluate ();
    ++count;
  }

  // return is the number of successful body executions
  return count;
}

// accept a visitor
void 
Madara::Expression_Tree::Composite_For_Loop::accept (Visitor &visitor) const
{
  visitor.visit (*this);
}

#endif /* _FOR_LOOP_CPP_ */
