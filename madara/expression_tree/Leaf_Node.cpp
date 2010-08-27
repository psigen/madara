/* -*- C++ -*- */
#ifndef _LEAF_NODE_CPP_
#define _LEAF_NODE_CPP_

#include <iostream>

#include "madara/expression_tree/Component_Node.h"
#include "madara/expression_tree/Visitor.h"
#include "madara/expression_tree/Leaf_Node.h"

// Ctor
Madara::Expression_Tree::Leaf_Node::Leaf_Node (int item)
  : Component_Node (),
    item_ (item)
{    
}

// Ctor
Madara::Expression_Tree::Leaf_Node::Leaf_Node (const ::std::string &item)
  : Component_Node ()
{    
  item_ = atoi (item.c_str ());
}

// Ctor
Madara::Expression_Tree::Leaf_Node::Leaf_Node (const char *item)
  : Component_Node ()
{    
  item_ = atoi (item);
}

// Dtor
Madara::Expression_Tree::Leaf_Node::~Leaf_Node (void)
{
}

// return the item
int
Madara::Expression_Tree::Leaf_Node::item (void) const
{
  return item_;
}

void 
Madara::Expression_Tree::Leaf_Node::accept (Visitor &visitor) const
{
  visitor.visit (*this);
}

#endif /* _LEAF_NODE_CPP_ */
