#include "madara/expression_tree/Visitor.h"
#include "madara/expression_tree/Variable_Node.h"

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

