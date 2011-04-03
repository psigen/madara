/* -*- C++ -*- */
#ifndef _COMPOSITE_PREDECREMENT_NODE_CPP_
#define _COMPOSITE_PREDECREMENT_NODE_CPP_

#include <iostream>

#include "madara/expression_tree/Component_Node.h"
#include "madara/expression_tree/Composite_Unary_Node.h"
#include "madara/expression_tree/Visitor.h"
#include "madara/expression_tree/Composite_Predecrement_Node.h"
#include "madara/expression_tree/Leaf_Node.h"
#include "madara/expression_tree/Variable_Node.h"

#include "ace/Log_Msg.h"

// Ctor

Madara::Expression_Tree::Composite_Predecrement_Node::Composite_Predecrement_Node (Component_Node *right)
  : Composite_Unary_Node (right)
{    
}

// Dtor
Madara::Expression_Tree::Composite_Predecrement_Node::~Composite_Predecrement_Node (void)
{
}

long long
Madara::Expression_Tree::Composite_Predecrement_Node::item (void) const
{
  return '-';
}

/// Prune the tree of unnecessary nodes. 
/// Returns evaluation of the node and sets can_change appropriately.
/// if this node can be changed, that means it shouldn't be pruned.
long long
Madara::Expression_Tree::Composite_Predecrement_Node::prune (bool & can_change)
{
  bool right_child_can_change = false;
  long long right_value = 0;

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
    ACE_DEBUG ((LM_DEBUG, "\nEXPRESSION COMPILE ERROR: ! has no right operand\n"));
    return -1;    
  }

  can_change = right_child_can_change;

  return --right_value;
}

/// Evaluates the node and its children. This does not prune any of
/// the expression tree, and is much faster than the prune function
long long 
Madara::Expression_Tree::Composite_Predecrement_Node::evaluate (void)
{
  Variable_Node * right = dynamic_cast <Variable_Node *> (this->right_);
  if (right)
    return right->dec ();

  return this->right_->evaluate () - 1;
}

// accept a visitor
void 
Madara::Expression_Tree::Composite_Predecrement_Node::accept (Visitor &visitor) const
{
  visitor.visit (*this);
}

#endif /* _COMPOSITE_PREDECREMENT_NODE_CPP_ */
