/* -*- C++ -*- */
#ifndef _FUNCTION_NODE_CPP_
#define _FUNCTION_NODE_CPP_

#include <iostream>
#include <sstream>

#include "madara/expression_tree/Component_Node.h"
#include "madara/expression_tree/Composite_Unary_Node.h"
#include "madara/expression_tree/Visitor.h"
#include "madara/expression_tree/Composite_Function_Node.h"
#include "madara/expression_tree/Leaf_Node.h"

#include "madara/utility/Log_Macros.h"
#include "madara/knowledge_engine/Functions.h"

#ifdef _MADARA_PYTHON_CALLBACKS_

  #include <boost/python/call.hpp> 

#endif

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

  // setup array of record pointers that point to .1, .2, .3, etc.
  if (nodes_.size () > 0)
    compiled_args_.resize (nodes_.size ());
  

  for (Component_Nodes::size_type i = 0; i < nodes_.size (); ++i)
  {
    bool arg_can_change = false;
    result = nodes_[i]->prune (arg_can_change);
    
    if (!arg_can_change && dynamic_cast <Leaf_Node *> (nodes_[i]) == 0)
    {
      delete nodes_[i];
      nodes_[i] = new Leaf_Node (result);
    }

    {
      // setup the corresponding compiled arg
      std::stringstream buffer;
      buffer << ".";
      buffer << i;
      compiled_args_[i] = context_.get_record (buffer.str ());
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
    *(compiled_args_[j]) = args[j];
  }


  Madara::Knowledge_Engine::Variables variables;
  variables.context_ = &context_;
  
  MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_DEBUG, 
    "Function %s is being called with %d args.\n",
    this->name_.c_str (), args.size ()));
  
  // if the user has defined a named function, return that
  if (function_->is_extern_named ())
    return function_->extern_named (name_.c_str (), args, variables);

  // if the user has defined an unnamed function, return that
  else if (function_->is_extern_unnamed ())
    return function_->extern_unnamed (args, variables);

#ifdef _MADARA_JAVA_
  else if (function_->is_java_callable())
    return function_->call_java(args, variables);
#endif
  
#ifdef _MADARA_PYTHON_CALLBACKS_
  else if (function_->is_python_callable ())
    return boost::python::call <Madara::Knowledge_Record> (
          function_->python_function.ptr (),
          boost::ref (args), boost::ref (variables));
#endif

  // otherwise, assume it is a MADARA function
  else
  {
    return function_->function_contents.evaluate ();
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
