/* -*- C++ -*- */

#ifndef _ASSIGNMENT_NODE_CPP_
#define _ASSIGNMENT_NODE_CPP_

#include <iostream>

#include "madara/expression_tree/Component_Node.h"
#include "madara/expression_tree/Visitor.h"
#include "madara/expression_tree/Composite_Assignment_Node.h"
#include "madara/expression_tree/Leaf_Node.h"

#include "madara/utility/Log_Macros.h"
// Ctor

Madara::Expression_Tree::Composite_Assignment_Node::Composite_Assignment_Node (
  Component_Node *left, Component_Node *right)
  : Composite_Unary_Node (right)
{
  left_ = dynamic_cast <Variable_Node *> (left);
}

Madara::Knowledge_Record
Madara::Expression_Tree::Composite_Assignment_Node::item (void) const
{
  return "=";
}


Madara::Knowledge_Record
Madara::Expression_Tree::Composite_Assignment_Node::prune (bool & can_change)
{
  bool left_child_can_change = false;
  bool right_child_can_change = false;
  Madara::Knowledge_Record right_value;

  if (this->left_ != 0)
    left_child_can_change = true;
  else
  {
    MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
      "\nKARL COMPILE ERROR: Variable assignment has no variable\n"));
    exit (-1);    
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
    MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
      "\nKARL COMPILE ERROR: Assignment has no right expression\n"));
    exit (-1);
  }

  can_change = left_child_can_change || right_child_can_change;

  return right_value;
}

/// Evaluates the node and its children. This does not prune any of
/// the expression tree, and is much faster than the prune function
Madara::Knowledge_Record 
Madara::Expression_Tree::Composite_Assignment_Node::evaluate (
  const Madara::Knowledge_Engine::Knowledge_Update_Settings & settings)
{
  // get the value from the right side and set the variable's value with it
  //Madara::Knowledge_Record value = right_->evaluate ();
  left_->set (right_->evaluate (settings));

  // return the value
  return left_->evaluate (settings);
}


// accept a visitor
void 
Madara::Expression_Tree::Composite_Assignment_Node::accept (Visitor &visitor) const
{
  visitor.visit (*this);
}

#endif /* _ASSIGNMENT_NODE_CPP_ */
