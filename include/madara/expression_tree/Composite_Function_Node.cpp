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

// Ctor

Madara::Expression_Tree::Composite_Function_Node::Composite_Function_Node (
        const std::string & name,
        Madara::Knowledge_Engine::Thread_Safe_Context & context,
        Component_Node *right)
  : Composite_Unary_Node (right), context_(context), name_ (name), 
    function_(context_.retrieve_function (name))
{
  
}

// Dtor
Madara::Expression_Tree::Composite_Function_Node::~Composite_Function_Node (void)
{
}

long long
Madara::Expression_Tree::Composite_Function_Node::item (void) const
{
  return '-';
}

/// Prune the tree of unnecessary nodes. 
/// Returns evaluation of the node and sets can_change appropriately.
/// if this node can be changed, that means it shouldn't be pruned.
long long
Madara::Expression_Tree::Composite_Function_Node::prune (bool & can_change)
{
  // user can always change a function, and we have no control over
  // what it does. Consequently, a function node cannot be pruned out
  // under any situation
  can_change = true;
  
  Madara::Knowledge_Engine::Variables variables;
  variables.context_ = &context_;

  if (function_->extern_func_)
    return function_->extern_func_ (&variables);
  else
  {
    
    MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_ERROR, 
      "Function %s has not been defined.\n", name_.c_str ()));
    return 0;
  }
}

/// Evaluates the node and its children. This does not prune any of
/// the expression tree, and is much faster than the prune function
long long 
Madara::Expression_Tree::Composite_Function_Node::evaluate (void)
{

  //if (right_)
  //{
  //  MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_ERROR,
  //    "Function %s parameter is %q\n", name_.c_str (), right_value));
  //}
  //else
  //{
  //  MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_ERROR, 
  //    "Function %s has no parameters\n", name_.c_str ()));
  //}
  
  Madara::Knowledge_Engine::Variables variables;
  variables.context_ = &context_;

  if (function_->extern_func_)
    return function_->extern_func_ (&variables);
  else
  {
    
    MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_ERROR, 
      "Function %s has not been defined.\n", name_.c_str ()));
    return 0;
  }
}

// accept a visitor
void 
Madara::Expression_Tree::Composite_Function_Node::accept (Visitor &visitor) const
{
  visitor.visit (*this);
}

#endif /* _FUNCTION_NODE_CPP_ */
