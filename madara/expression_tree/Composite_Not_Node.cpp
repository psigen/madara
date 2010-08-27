/* -*- C++ -*- */
#ifndef _NOT_NODE_CPP_
#define _NOT_NODE_CPP_

#include <iostream>

#include "madara/expression_tree/Component_Node.h"
#include "madara/expression_tree/Composite_Unary_Node.h"
#include "madara/expression_tree/Visitor.h"
#include "madara/expression_tree/Composite_Not_Node.h"

// Ctor

Madara::Expression_Tree::Composite_Not_Node::Composite_Not_Node (Component_Node *right)
  : Composite_Unary_Node (right)
{    
}

// Dtor
Madara::Expression_Tree::Composite_Not_Node::~Composite_Not_Node (void)
{
}

int
Madara::Expression_Tree::Composite_Not_Node::item (void) const
{
  return '!';
}

// accept a visitor
void 
Madara::Expression_Tree::Composite_Not_Node::accept (Visitor &visitor) const
{
  visitor.visit (*this);
}

#endif /* _NOT_NODE_CPP_ */
