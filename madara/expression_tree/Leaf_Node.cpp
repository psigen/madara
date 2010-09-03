/* -*- C++ -*- */
#ifndef _LEAF_NODE_CPP_
#define _LEAF_NODE_CPP_

#include <iostream>

#include "madara/expression_tree/Component_Node.h"
#include "madara/expression_tree/Visitor.h"
#include "madara/expression_tree/Leaf_Node.h"

#include "ace/Log_Msg.h"

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


/// Prune the tree of unnecessary nodes. 
/// Returns evaluation of the node and sets can_change appropriately.
/// if this node can be changed, that means it shouldn't be pruned.
int
Madara::Expression_Tree::Leaf_Node::prune (bool & can_change)
{
  can_change = false;

  // we could call item(), but since it is virtual, it incurs unnecessary
  // overhead.
  return item_;
}

/// Evaluates the node and its children. This does not prune any of
/// the expression tree, and is much faster than the prune function
int 
Madara::Expression_Tree::Leaf_Node::evaluate (void)
{
  // we could call item(), but since it is virtual, it incurs unnecessary
  // overhead.
  return item_;
}


void 
Madara::Expression_Tree::Leaf_Node::accept (Visitor &visitor) const
{
  visitor.visit (*this);
}

#endif /* _LEAF_NODE_CPP_ */
