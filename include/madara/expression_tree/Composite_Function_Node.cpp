/* -*- C++ -*- */
#ifndef _FUNCTION_NODE_CPP_
#define _FUNCTION_NODE_CPP_

#include <iostream>

#include "madara/expression_tree/Component_Node.h"
#include "madara/expression_tree/Composite_Unary_Node.h"
#include "madara/expression_tree/Visitor.h"
#include "madara/expression_tree/Composite_Function_Node.h"
#include "madara/expression_tree/Leaf_Node.h"

#include "madara/utility/Log_Macros.h"
#include "madara/Functions.h"

// Ctor

Madara::Expression_Tree::Composite_Function_Node::Composite_Function_Node (
        const std::string & name, 
        Madara::Knowledge_Engine::Thread_Safe_Context & context,
        const Component_Nodes & nodes)
  : Madara::Expression_Tree::Composite_Ternary_Node (nodes),
    context_ (context), name_ (name), 
    function_ (context.retrieve_function (name))
{
  
}

// Dtor
Madara::Expression_Tree::Composite_Function_Node::~Composite_Function_Node (void)
{
}

Madara::Knowledge_Record
Madara::Expression_Tree::Composite_Function_Node::item (void) const
{
  Madara::Knowledge_Record record;
  record.set_value (name_ + "()");
  return record;
}

/// Prune the tree of unnecessary nodes. 
/// Returns evaluation of the node and sets can_change appropriately.
/// if this node can be changed, that means it shouldn't be pruned.
Madara::Knowledge_Record
Madara::Expression_Tree::Composite_Function_Node::prune (bool & can_change)
{
  // user can always change a function, and we have no control over
  // what it does. Consequently, a function node cannot be pruned out
  // under any situation
  can_change = true;
  
  Madara::Knowledge_Record result;

  for (Component_Nodes::iterator i = nodes_.begin (); i != nodes_.end ();
       ++i)
  {
    bool arg_can_change = false;
    result = (*i)->prune (arg_can_change);
    
    if (!arg_can_change && dynamic_cast <Leaf_Node *> (*i) == 0)
    {
      delete *i;
      *i = new Leaf_Node (result);
    }
  }

  return result;
}

/// Evaluates the node and its children. This does not prune any of
/// the expression tree, and is much faster than the prune function
Madara::Knowledge_Record 
Madara::Expression_Tree::Composite_Function_Node::evaluate (
const Madara::Knowledge_Engine::Knowledge_Update_Settings & settings)
{
  Madara::Knowledge_Engine::Function_Arguments args;
  args.resize (nodes_.size ());

  int j = 0;

  for (Component_Nodes::iterator i = nodes_.begin (); i != nodes_.end ();
       ++i, ++j)
  {
    args[j] = (*i)->evaluate (settings);
  }


  Madara::Knowledge_Engine::Variables variables;
  variables.context_ = &context_;

  if (function_->extern_func_)
    return function_->extern_func_ (args, variables);
  else
  {
    return function_->function_contents_.evaluate ();
    //Madara::Knowledge_Record zero;
    //MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_ERROR, 
    //  "Function %s has not been defined.\n", name_.c_str ()));
    //return zero;
  }
}

// accept a visitor
void 
Madara::Expression_Tree::Composite_Function_Node::accept (Visitor &visitor) const
{
  visitor.visit (*this);
}

#endif /* _FUNCTION_NODE_CPP_ */
