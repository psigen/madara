/* -*- C++ -*- */
#ifndef _MULTIPLY_NODE_CPP_
#define _MULTIPLY_NODE_CPP_

#include <iostream>

#include "madara/expression_tree/Component_Node.h"
#include "madara/expression_tree/Composite_Binary_Node.h"
#include "madara/expression_tree/Composite_Multiply_Node.h"
#include "madara/expression_tree/Visitor.h"

// Ctor
Madara::Expression_Tree::Composite_Multiply_Node::Composite_Multiply_Node (Component_Node * left,
                              Component_Node * right)
  : Composite_Binary_Node (left, right)
{    
}

// Dtor
Madara::Expression_Tree::Composite_Multiply_Node::~Composite_Multiply_Node (void)
{
}

int
Madara::Expression_Tree::Composite_Multiply_Node::item (void) const
{
  return '*';
}

// accept a visitor
void 
Madara::Expression_Tree::Composite_Multiply_Node::accept (Visitor& visitor) const
{
  visitor.visit (*this);
}

#endif /* _MULTIPLY_NODE_CPP_ */
