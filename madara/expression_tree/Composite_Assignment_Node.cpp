/* -*- C++ -*- */

#ifndef _ASSIGNMENT_NODE_CPP_
#define _ASSIGNMENT_NODE_CPP_

#include <iostream>

#include "madara/expression_tree/Component_Node.h"
#include "madara/expression_tree/Visitor.h"
#include "madara/expression_tree/Composite_Assignment_Node.h"
#include "madara/expression_tree/Leaf_Node.h"
#include "madara/expression_tree/Variable_Node.h"

#include "ace/Log_Msg.h"
// Ctor

Madara::Expression_Tree::Composite_Assignment_Node::Composite_Assignment_Node (
  Component_Node *left, Component_Node *right)
  : Composite_Binary_Node (left, right)
{    
}

int
Madara::Expression_Tree::Composite_Assignment_Node::item (void) const
{
  return '=';
}


int
Madara::Expression_Tree::Composite_Assignment_Node::prune (bool & can_change)
{
  bool left_child_can_change = false;
  bool right_child_can_change = false;
  int left_value = 0;
  int right_value = 0;

  if (this->left_ && dynamic_cast <Variable_Node *> (left_) != 0)
    left_child_can_change = true;
  else
  {
    ACE_DEBUG ((LM_DEBUG, 
      "\nEXPRESSION COMPILE ERROR: Variable assignment has no variable\n"));
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
    ACE_DEBUG ((LM_DEBUG, "\nEXPRESSION COMPILE ERROR: Assignment has no right operand\n"));
    return -1;    
  }

  can_change = left_child_can_change || right_child_can_change;

  return right_value;
}

/// Evaluates the node and its children. This does not prune any of
/// the expression tree, and is much faster than the prune function
int 
Madara::Expression_Tree::Composite_Assignment_Node::evaluate (void)
{
  // get the value from the right side and set the variable's value with it
  int value = right_->evaluate ();
  dynamic_cast <Variable_Node *> (left_)->set (value);

  // return the value
  return value;
}


// accept a visitor
void 
Madara::Expression_Tree::Composite_Assignment_Node::accept (Visitor &visitor) const
{
  visitor.visit (*this);
}

#endif /* _ASSIGNMENT_NODE_CPP_ */
