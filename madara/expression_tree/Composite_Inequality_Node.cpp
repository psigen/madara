/* -*- C++ -*- */

#ifndef _INEQUALITY_NODE_CPP_
#define _INEQUALITY_NODE_CPP_

#include <iostream>

#include "madara/expression_tree/Component_Node.h"
#include "madara/expression_tree/Visitor.h"
#include "madara/expression_tree/Composite_Inequality_Node.h"

// Ctor

Madara::Expression_Tree::Composite_Inequality_Node::Composite_Inequality_Node (
  Component_Node *left, Component_Node *right)
: Madara::Expression_Tree::Composite_Binary_Node (left, right)
{    
}

int
Madara::Expression_Tree::Composite_Inequality_Node::item (void) const
{
  return '!';
}

// accept a visitor

void 
Madara::Expression_Tree::Composite_Inequality_Node::accept (Visitor &visitor) const
{
  visitor.visit (*this);
}

#endif /* _INEQUALITY_NODE_CPP_ */
