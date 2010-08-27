/* -*- C++ -*- */

#ifndef _ADD_NODE_CPP_
#define _ADD_NODE_CPP_

#include <iostream>

#include "madara/expression_tree/Component_Node.h"
#include "madara/expression_tree/Visitor.h"
#include "madara/expression_tree/Composite_Add_Node.h"

// Ctor

Madara::Expression_Tree::Composite_Add_Node::Composite_Add_Node (Component_Node *left,
                    Component_Node *right)
  : Composite_Binary_Node (left, right)
{    
}

int
Madara::Expression_Tree::Composite_Add_Node::item (void) const
{
  return '+';
}

// accept a visitor

void 
Madara::Expression_Tree::Composite_Add_Node::accept (Visitor &visitor) const
{
  visitor.visit (*this);
}

#endif /* _ADD_NODE_CPP_ */
