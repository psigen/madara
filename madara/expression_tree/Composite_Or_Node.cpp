/* -*- C++ -*- */

#ifndef COMPOSITE_OR_NODE_CPP
#define COMPOSITE_OR_NODE_CPP

#include <iostream>

#include "madara/expression_tree/Component_Node.h"
#include "madara/expression_tree/Visitor.h"
#include "madara/expression_tree/Composite_Or_Node.h"
#include "madara/expression_tree/Leaf_Node.h"

#include "madara/utility/Log_Macros.h"

// Ctor

Madara::Expression_Tree::Composite_Or_Node::Composite_Or_Node (
  Component_Node *left, Component_Node *right)
: Madara::Expression_Tree::Composite_Binary_Node (left, right)
{    
}

long long
Madara::Expression_Tree::Composite_Or_Node::item (void) const
{
  return '|';
}

/// Prune the tree of unnecessary nodes. 
/// Returns evaluation of the node and sets can_change appropriately.
/// if this node can be changed, that means it shouldn't be pruned.
long long
Madara::Expression_Tree::Composite_Or_Node::prune (bool & can_change)
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
      "\nKARL COMPILE ERROR: Or" \
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
      "\nKARL COMPILE ERROR: Or" \
      " has no right expression\n"));
    exit (-1);  
  }

  can_change = left_child_can_change || right_child_can_change;

  return left_value || right_value;
}

/// Evaluates the node and its children. This does not prune any of
/// the expression tree, and is much faster than the prune function
long long 
Madara::Expression_Tree::Composite_Or_Node::evaluate (void)
{
  // if left is not true, then evaluate right
  if (!left_->evaluate ())
    return right_->evaluate ();

  // if left was true, then the Or returns true
  return 1;
}

// accept a visitor
void 
Madara::Expression_Tree::Composite_Or_Node::accept (Visitor &visitor) const
{
  visitor.visit (*this);
}

#endif /* COMPOSITE_OR_NODE_CPP */
