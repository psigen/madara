/* -*- C++ -*- */

#ifndef COMPOSITE_AND_NODE_CPP
#define COMPOSITE_AND_NODE_CPP

#include <iostream>

#include "madara/expression_tree/Component_Node.h"
#include "madara/expression_tree/Visitor.h"
#include "madara/expression_tree/Composite_And_Node.h"

// Ctor

Madara::Expression_Tree::Composite_And_Node::Composite_And_Node (
  Component_Node *left, Component_Node *right)
: Madara::Expression_Tree::Composite_Binary_Node (left, right)
{    
}

int
Madara::Expression_Tree::Composite_And_Node::item (void) const
{
  return '&';
}

// accept a visitor

void 
Madara::Expression_Tree::Composite_And_Node::accept (Visitor &visitor) const
{
  visitor.visit (*this);
}

#endif /* COMPOSITE_AND_NODE_CPP */
