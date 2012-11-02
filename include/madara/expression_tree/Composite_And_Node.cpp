/* -*- C++ -*- */

#ifndef COMPOSITE_AND_NODE_CPP
#define COMPOSITE_AND_NODE_CPP

#include <iostream>

#include "madara/expression_tree/Component_Node.h"
#include "madara/expression_tree/Visitor.h"
#include "madara/expression_tree/Composite_And_Node.h"
#include "madara/expression_tree/Leaf_Node.h"

#include "madara/utility/Log_Macros.h"
// Ctor

Madara::Expression_Tree::Composite_And_Node::Composite_And_Node (
  Component_Node *left, Component_Node *right)
: Madara::Expression_Tree::Composite_Binary_Node (left, right)
{    
}

int64_t
Madara::Expression_Tree::Composite_And_Node::item (void) const
{
  return '&';
}


/// Prune the tree of unnecessary nodes. 
/// Returns evaluation of the node and sets can_change appropriately.
/// if this node can be changed, that means it shouldn't be pruned.
Madara::Knowledge_Record::VALUE_TYPE
Madara::Expression_Tree::Composite_And_Node::prune (bool & can_change)
{
  bool left_child_can_change = false;
  bool right_child_can_change = false;
  Madara::Knowledge_Record::VALUE_TYPE left_value = 0;
  Madara::Knowledge_Record::VALUE_TYPE right_value = 0;

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
      "\nKARL COMPILE ERROR: And has no left expression\n"));
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
      "\nKARL COMPILE ERROR: And has no right expression\n"));
    exit (-1);
  }

  can_change = left_child_can_change || right_child_can_change;

  return left_value && right_value;
}

/// Evaluates the node and its children. This does not prune any of
/// the expression tree, and is much faster than the prune function
Madara::Knowledge_Record::VALUE_TYPE 
Madara::Expression_Tree::Composite_And_Node::evaluate (void)
{
  // only evaluate right if left evaluates to non-zero
  if (left_->evaluate ())
    return right_->evaluate ();

  return 0;
}


// accept a visitor
void 
Madara::Expression_Tree::Composite_And_Node::accept (Visitor &visitor) const
{
  visitor.visit (*this);
}

#endif /* COMPOSITE_AND_NODE_CPP */
