/* -*- C++ -*- */
#ifndef _COMPOSITE_PREINCREMENT_NODE_CPP_
#define _COMPOSITE_PREINCREMENT_NODE_CPP_

#include <iostream>

#include "madara/expression_tree/Component_Node.h"
#include "madara/expression_tree/Composite_Unary_Node.h"
#include "madara/expression_tree/Visitor.h"
#include "madara/expression_tree/Composite_Preincrement_Node.h"
#include "madara/expression_tree/Leaf_Node.h"
#include "madara/expression_tree/Variable_Node.h"

#include "madara/utility/Log_Macros.h"

// Ctor

Madara::Expression_Tree::Composite_Preincrement_Node::Composite_Preincrement_Node (
  Component_Node *right)
  : Composite_Unary_Node (right)
{    
}

// Dtor
Madara::Expression_Tree::Composite_Preincrement_Node::~Composite_Preincrement_Node (void)
{
}

Madara::Knowledge_Record
Madara::Expression_Tree::Composite_Preincrement_Node::item (void) const
{
  Madara::Knowledge_Record record;
  record.set_value ("++");
  return record;
}

/// Prune the tree of unnecessary nodes. 
/// Returns evaluation of the node and sets can_change appropriately.
/// if this node can be changed, that means it shouldn't be pruned.
Madara::Knowledge_Record
Madara::Expression_Tree::Composite_Preincrement_Node::prune (bool & can_change)
{
  bool right_child_can_change = false;
  Madara::Knowledge_Record right_value;

  if (this->right_)
  {
    right_value = this->right_->prune (right_child_can_change);
    if (!right_child_can_change && dynamic_cast <Leaf_Node *> (right_) == 0)
    {
      delete this->right_;
      this->right_ = new Leaf_Node (right_value);
    }
  }
  else
  {
    MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
      "\nKARL COMPILE ERROR: Preincrement" \
      " has no right expression\n"));
    exit (-1);
  }

  can_change = right_child_can_change;

  return ++right_value;
}

/// Evaluates the node and its children. This does not prune any of
/// the expression tree, and is much faster than the prune function
Madara::Knowledge_Record 
Madara::Expression_Tree::Composite_Preincrement_Node::evaluate (
  const Madara::Knowledge_Engine::Knowledge_Update_Settings & settings)
{
  Variable_Node * right = dynamic_cast <Variable_Node *> (this->right_);
  if (right)
  {
    return right->inc (settings);
  }
  
  // if we get to this point, we've somehow got a predecrement of a non-var.
  // We go along with this with a temporary record, but this is highly unusual.
  Madara::Knowledge_Record record (this->right_->evaluate (settings));
  ++record;
  
  return record;
}

// accept a visitor
void 
Madara::Expression_Tree::Composite_Preincrement_Node::accept (Visitor &visitor) const
{
  visitor.visit (*this);
}

#endif /* _COMPOSITE_PREINCREMENT_NODE_CPP_ */
