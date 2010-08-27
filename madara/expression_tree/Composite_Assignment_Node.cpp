/* -*- C++ -*- */

#ifndef _ASSIGNMENT_NODE_CPP_
#define _ASSIGNMENT_NODE_CPP_

#include <iostream>

#include "madara/expression_tree/Component_Node.h"
#include "madara/expression_tree/Visitor.h"
#include "madara/expression_tree/Composite_Assignment_Node.h"

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

// accept a visitor

void 
Madara::Expression_Tree::Composite_Assignment_Node::accept (Visitor &visitor) const
{
  visitor.visit (*this);
}

#endif /* _ASSIGNMENT_NODE_CPP_ */
