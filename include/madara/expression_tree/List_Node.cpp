#include "madara/expression_tree/Visitor.h"
#include "madara/expression_tree/List_Node.h"
#include "madara/utility/Utility.h"

#include "madara/utility/Log_Macros.h"
#include <string>
#include <sstream>

Madara::Expression_Tree::List_Node::List_Node (
         Madara::Knowledge_Engine::Thread_Safe_Context &context)
: context_ (context), list_ ()
{

}

Madara::Expression_Tree::List_Node::~List_Node ()
{
  // do not clean up record_. Let the context clean that up.
}


void
Madara::Expression_Tree::List_Node::accept (Visitor &visitor) const
{
  visitor.visit (*this);
}

long long
Madara::Expression_Tree::List_Node::item () const
{
  return list_.size ();
}

/// Prune the tree of unnecessary nodes. 
/// Returns evaluation of the node and sets can_change appropriately.
/// if this node can be changed, that means it shouldn't be pruned.
long long
Madara::Expression_Tree::List_Node::prune (bool & can_change)
{
  // a variable is one of very few nodes that can change over time and
  // cannot be pruned
  can_change = true;

  return list_.size ();
}

/// Evaluates the node and its children. This does not prune any of
/// the expression tree, and is much faster than the prune function
long long 
Madara::Expression_Tree::List_Node::evaluate (void)
{
  return list_.size ();
}

