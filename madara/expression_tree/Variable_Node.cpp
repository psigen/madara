#include "madara/expression_tree/Visitor.h"
#include "madara/expression_tree/Variable_Node.h"

#include "ace/Log_Msg.h"

Madara::Expression_Tree::Variable_Node::Variable_Node (const ::std::string &key, 
                              Madara::Knowledge_Engine::Thread_Safe_Context &context)
                              : key_ (key), context_ (context)
{
}

Madara::Expression_Tree::Variable_Node::~Variable_Node ()
{
}

void
Madara::Expression_Tree::Variable_Node::accept (Visitor &visitor) const
{
  visitor.visit (*this);
}

int
Madara::Expression_Tree::Variable_Node::item () const
{
  return context_.get (key_);
}

/// Prune the tree of unnecessary nodes. 
/// Returns evaluation of the node and sets can_change appropriately.
/// if this node can be changed, that means it shouldn't be pruned.
int
Madara::Expression_Tree::Variable_Node::prune (bool & can_change)
{
  // a variable is one of very few nodes that can change over time and
  // cannot be pruned
  can_change = true;

  // we could call item(), but since it is virtual, it incurs unnecessary
  // overhead.
  return context_.get (key_);
}

/// Evaluates the node and its children. This does not prune any of
/// the expression tree, and is much faster than the prune function
int 
Madara::Expression_Tree::Variable_Node::evaluate (void)
{
  // we could call item(), but since it is virtual, it incurs unnecessary
  // overhead.
  return context_.get (key_);
}

const std::string &
Madara::Expression_Tree::Variable_Node::key () const
{
  return key_;
}

int
Madara::Expression_Tree::Variable_Node::set (const int & value)
{
  return context_.set (key_, value);
}

int
Madara::Expression_Tree::Variable_Node::dec (void)
{
  return context_.dec (key_);
}

int
Madara::Expression_Tree::Variable_Node::inc (void)
{
  return context_.inc (key_);
}

