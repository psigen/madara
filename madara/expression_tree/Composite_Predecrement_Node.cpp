/* -*- C++ -*- */
#ifndef _COMPOSITE_PREDECREMENT_NODE_CPP_
#define _COMPOSITE_PREDECREMENT_NODE_CPP_

#include <iostream>

#include "madara/expression_tree/Component_Node.h"
#include "madara/expression_tree/Composite_Unary_Node.h"
#include "madara/expression_tree/Visitor.h"
#include "madara/expression_tree/Composite_Predecrement_Node.h"

// Ctor

Madara::Expression_Tree::Composite_Predecrement_Node::Composite_Predecrement_Node (Component_Node *right)
  : Composite_Unary_Node (right)
{    
}

// Dtor
Madara::Expression_Tree::Composite_Predecrement_Node::~Composite_Predecrement_Node (void)
{
}

int
Madara::Expression_Tree::Composite_Predecrement_Node::item (void) const
{
  return '-';
}

// accept a visitor
void 
Madara::Expression_Tree::Composite_Predecrement_Node::accept (Visitor &visitor) const
{
  visitor.visit (*this);
}

#endif /* _COMPOSITE_PREDECREMENT_NODE_CPP_ */
