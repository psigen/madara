/* -*- C++ -*- */

#ifndef _COMPOSITE_GREATER_THAN_NODE_CPP_
#define _COMPOSITE_GREATER_THAN_NODE_CPP_

#include <iostream>

#include "madara/expression_tree/Component_Node.h"
#include "madara/expression_tree/Visitor.h"
#include "madara/expression_tree/Composite_Greater_Than_Node.h"

// Ctor

Madara::Expression_Tree::Composite_Greater_Than_Node::Composite_Greater_Than_Node (
  Component_Node *left, Component_Node *right)
: Madara::Expression_Tree::Composite_Binary_Node (left, right)
{    
}

int
Madara::Expression_Tree::Composite_Greater_Than_Node::item (void) const
{
  return '>';
}

// accept a visitor

void 
Madara::Expression_Tree::Composite_Greater_Than_Node::accept (Visitor &visitor) const
{
  visitor.visit (*this);
}

#endif /* _COMPOSITE_GREATER_THAN_NODE_CPP_ */
