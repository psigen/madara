/* -*- C++ -*- */

#ifndef COMPOSITE_OR_NODE_CPP
#define COMPOSITE_OR_NODE_CPP

#include <iostream>

#include "madara/expression_tree/Component_Node.h"
#include "madara/expression_tree/Visitor.h"
#include "madara/expression_tree/Composite_Or_Node.h"

// Ctor

Madara::Expression_Tree::Composite_Or_Node::Composite_Or_Node (
  Component_Node *left, Component_Node *right)
: Madara::Expression_Tree::Composite_Binary_Node (left, right)
{    
}

int
Madara::Expression_Tree::Composite_Or_Node::item (void) const
{
  return '|';
}

// accept a visitor

void 
Madara::Expression_Tree::Composite_Or_Node::accept (Visitor &visitor) const
{
  visitor.visit (*this);
}

#endif /* COMPOSITE_OR_NODE_CPP */
