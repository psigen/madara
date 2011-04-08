/* -*- C++ -*- */

#ifndef _INEQUALITY_NODE_CPP_
#define _INEQUALITY_NODE_CPP_

#include <iostream>

#include "madara/expression_tree/Component_Node.h"
#include "madara/expression_tree/Visitor.h"
#include "madara/expression_tree/Composite_Inequality_Node.h"
#include "madara/expression_tree/Leaf_Node.h"

#include "madara/utility/Log_Macros.h"

// Ctor
Madara::Expression_Tree::Composite_Inequality_Node::Composite_Inequality_Node (
  Component_Node *left, Component_Node *right)
: Madara::Expression_Tree::Composite_Binary_Node (left, right)
{    
}

long long
Madara::Expression_Tree::Composite_Inequality_Node::item (void) const
{
  return '!';
}


/// Prune the tree of unnecessary nodes. 
/// Returns evaluation of the node and sets can_change appropriately.
/// if this node can be changed, that means it shouldn't be pruned.
long long
Madara::Expression_Tree::Composite_Inequality_Node::prune (bool & can_change)
{
  bool left_child_can_change = false;
  bool right_child_can_change = false;
  long long left_value = 0;
  long long right_value = 0;

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
    MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
      "\nKARL COMPILE ERROR: Inequality" \
      " has no left expression\n"));
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
      "\nKARL COMPILE ERROR: Inequality" \
      " has no right expression\n"));
    exit (-1); 
  }

  can_change = left_child_can_change || right_child_can_change;

  return left_value != right_value;
}

/// Evaluates the node and its children. This does not prune any of
/// the expression tree, and is much faster than the prune function
long long 
Madara::Expression_Tree::Composite_Inequality_Node::evaluate (void)
{
  return left_->evaluate () != right_->evaluate ();
}


// accept a visitor
void 
Madara::Expression_Tree::Composite_Inequality_Node::accept (Visitor &visitor) const
{
  visitor.visit (*this);
}

#endif /* _INEQUALITY_NODE_CPP_ */
