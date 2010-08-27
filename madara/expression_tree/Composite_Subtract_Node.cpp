/* -*- C++ -*- */
#ifndef _SUBTRACT_NODE_CPP_
#define _SUBTRACT_NODE_CPP_

#include "madara/expression_tree/Component_Node.h"
#include "madara/expression_tree/Visitor.h"
#include "madara/expression_tree/Composite_Binary_Node.h"
#include "madara/expression_tree/Composite_Subtract_Node.h"

// Ctor
Madara::Expression_Tree::Composite_Subtract_Node::Composite_Subtract_Node (
  Component_Node *left, Component_Node *right)
: Madara::Expression_Tree::Composite_Binary_Node (left, right)
{    
}

// Dtor
Madara::Expression_Tree::Composite_Subtract_Node::~Composite_Subtract_Node (void)
{
}

int
Madara::Expression_Tree::Composite_Subtract_Node::item (void) const
{
  return '-';
}

// accept a visitor
void 
Madara::Expression_Tree::Composite_Subtract_Node::accept (Visitor &visitor) const
{
  visitor.visit (*this);
}


#endif /* _SUBTRACT_NODE_CPP_ */
