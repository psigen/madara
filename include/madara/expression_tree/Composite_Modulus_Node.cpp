/* -*- C++ -*- */
#ifndef _MODULUS_NODE_CPP_
#define _MODULUS_NODE_CPP_

#include <iostream>

#include "madara/expression_tree/Component_Node.h"
#include "madara/expression_tree/Composite_Binary_Node.h"
#include "madara/expression_tree/Composite_Modulus_Node.h"
#include "madara/expression_tree/Visitor.h"
#include "madara/expression_tree/Leaf_Node.h"

#include "madara/utility/Log_Macros.h"

// Ctor
Madara::Expression_Tree::Composite_Modulus_Node::Composite_Modulus_Node (Component_Node * left,
                              Component_Node * right)
  : Composite_Binary_Node (left, right)
{    
}

// Dtor
Madara::Expression_Tree::Composite_Modulus_Node::~Composite_Modulus_Node (void)
{
}

Madara::Knowledge_Record
Madara::Expression_Tree::Composite_Modulus_Node::item (void) const
{
  Madara::Knowledge_Record record;
  record.set_value ("%");
  return record;
}


/// Prune the tree of unnecessary nodes. 
/// Returns evaluation of the node and sets can_change appropriately.
/// if this node can be changed, that means it shouldn't be pruned.
Madara::Knowledge_Record
Madara::Expression_Tree::Composite_Modulus_Node::prune (bool & can_change)
{
  bool left_child_can_change = false;
  bool right_child_can_change = false;
  Madara::Knowledge_Record left_value;
  Madara::Knowledge_Record right_value;

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
      "\nKARL COMPILE ERROR: Modulus" \
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

      if (right_value == Madara::Knowledge_Record::Integer (0))
      {
        MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
          "\nKARL COMPILE ERROR: Modulus" \
          " results in permanent divide by zero\n"));
        exit (-1);
      }
    }
  }
  else
  {
    MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
      "\nKARL COMPILE ERROR: Modulus" \
      " has no right expression (divide by zero)\n"));
    exit (-1);
  }

  can_change = left_child_can_change || right_child_can_change;

  return left_value % right_value;
}

/// Evaluates the node and its children. This does not prune any of
/// the expression tree, and is much faster than the prune function
Madara::Knowledge_Record 
Madara::Expression_Tree::Composite_Modulus_Node::evaluate (void)
{
  return left_->evaluate () % right_->evaluate ();
}


// accept a visitor
void 
Madara::Expression_Tree::Composite_Modulus_Node::accept (Visitor& visitor) const
{
  visitor.visit (*this);
}

#endif /* _MODULUS_NODE_CPP_ */
