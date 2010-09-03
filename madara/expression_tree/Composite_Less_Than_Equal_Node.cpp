/* -*- C++ -*- */

#ifndef _COMPOSITE_LESS_THAN_EQUAL_NODE_CPP_
#define _COMPOSITE_LESS_THAN_EQUAL_NODE_CPP_

#include <iostream>

#include "madara/expression_tree/Component_Node.h"
#include "madara/expression_tree/Visitor.h"
#include "madara/expression_tree/Composite_Less_Than_Equal_Node.h"
#include "madara/expression_tree/Leaf_Node.h"

#include "ace/Log_Msg.h"

// Ctor

Madara::Expression_Tree::Composite_Less_Than_Equal_Node::Composite_Less_Than_Equal_Node (
  Component_Node *left, Component_Node *right)
: Madara::Expression_Tree::Composite_Binary_Node (left, right)
{    
}

int
Madara::Expression_Tree::Composite_Less_Than_Equal_Node::item (void) const
{
  return '<';
}


/// Prune the tree of unnecessary nodes. 
/// Returns evaluation of the node and sets can_change appropriately.
/// if this node can be changed, that means it shouldn't be pruned.
int
Madara::Expression_Tree::Composite_Less_Than_Equal_Node::prune (bool & can_change)
{
  bool left_child_can_change = false;
  bool right_child_can_change = false;
  int left_value = 0;
  int right_value = 0;

  if (this->left_)
  {
    left_value = this->left_->prune (left_child_can_change);
    if (!left_child_can_change && dynamic_cast <Leaf_Node *> (left_) == 0)
    {
      delete this->left_;
      this->left_ = new Leaf_Node (left_value);
    }
  }
  else
  {
    ACE_DEBUG ((LM_DEBUG, "\nEXPRESSION COMPILE ERROR: <= has no left operand\n"));
    return -1;    
  }

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
    ACE_DEBUG ((LM_DEBUG, "\nEXPRESSION COMPILE ERROR: <= has no right operand\n"));
    return -1;    
  }

  can_change = left_child_can_change || right_child_can_change;

  return left_value <= right_value;
}

/// Evaluates the node and its children. This does not prune any of
/// the expression tree, and is much faster than the prune function
int 
Madara::Expression_Tree::Composite_Less_Than_Equal_Node::evaluate (void)
{
  return left_->evaluate () <= right_->evaluate ();
}


// accept a visitor
void 
Madara::Expression_Tree::Composite_Less_Than_Equal_Node::accept (Visitor &visitor) const
{
  visitor.visit (*this);
}

#endif /* _COMPOSITE_LESS_THAN_EQUAL_NODE_CPP_ */
