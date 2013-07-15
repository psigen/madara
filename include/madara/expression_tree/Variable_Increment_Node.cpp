#include "madara/expression_tree/Visitor.h"
#include "madara/expression_tree/Leaf_Node.h"
#include "madara/expression_tree/Variable_Increment_Node.h"
#include "madara/utility/Utility.h"

#include "madara/utility/Log_Macros.h"
#include <string>
#include <sstream>

Madara::Expression_Tree::Variable_Increment_Node::Variable_Increment_Node (
  Component_Node * lhs, Madara::Knowledge_Record value,
  Component_Node * rhs, 
  Madara::Knowledge_Engine::Thread_Safe_Context &context)
: var_ (0), array_ (0), value_ (value), rhs_ (rhs), context_ (context)
{
  var_ = dynamic_cast <Variable_Node *> (lhs);

  if (!var_)
    array_ = dynamic_cast <Composite_Array_Reference *> (lhs);
}

Madara::Expression_Tree::Variable_Increment_Node::~Variable_Increment_Node ()
{
  // do not clean up record_. Let the context clean that up.
}

void
Madara::Expression_Tree::Variable_Increment_Node::accept (Visitor &visitor) const
{
  visitor.visit (*this);
}

Madara::Knowledge_Record
Madara::Expression_Tree::Variable_Increment_Node::item () const
{
  Knowledge_Record value;

  if (var_)
    value = var_->item ();
  else if (array_)
    value = array_->item ();

  return value;
}

/// Prune the tree of unnecessary nodes. 
/// Returns evaluation of the node and sets can_change appropriately.
/// if this node can be changed, that means it shouldn't be pruned.
Madara::Knowledge_Record
Madara::Expression_Tree::Variable_Increment_Node::prune (bool & can_change)
{
  bool left_child_can_change = false;
  bool right_child_can_change = false;
  Madara::Knowledge_Record right_value;

  if (this->var_ != 0 || this->array_ != 0)
    left_child_can_change = true;
  else
  {
    MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
      "\nKARL COMPILE ERROR: Variable assignment has no variable\n"));
    exit (-1);    
  }

  if (this->rhs_)
  {
    right_value = this->rhs_->prune (right_child_can_change);
    if (!right_child_can_change && dynamic_cast <Leaf_Node *> (rhs_) == 0)
    {
      delete this->rhs_;
      this->rhs_ = new Leaf_Node (right_value);
    }
  }
  else
  {
    MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
      "\nKARL COMPILE ERROR: Assignment has no right expression\n"));
    exit (-1);
  }

  can_change = left_child_can_change || right_child_can_change;

  return right_value;
}

/// Evaluates the node and its children. This does not prune any of
/// the expression tree, and is much faster than the prune function
Madara::Knowledge_Record 
Madara::Expression_Tree::Variable_Increment_Node::evaluate (
  const Madara::Knowledge_Engine::Knowledge_Update_Settings & settings)
{
  Madara::Knowledge_Record rhs;

  if (rhs_)
    rhs = rhs_->evaluate (settings);
  else
    rhs = value_;

  if (var_)
  {
    MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
      "Composite_Assignment_Node::evaluate: "
      "Attempting to set variable %s to %s.\n",
      var_->expand_key ().c_str (),
      rhs.to_string ().c_str ()));
    
    Knowledge_Record result (var_->evaluate (settings) + rhs);
    var_->set (result, settings);
    return result;
  }
  else if (array_)
  {
    MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
      "Composite_Assignment_Node::evaluate: "
      "Attempting to set index of var %s to %s.\n",
      array_->expand_key ().c_str (),
      rhs.to_string ().c_str ()));

    Knowledge_Record result (array_->evaluate (settings) + rhs);
    array_->set (result, settings);
    return result;
  }
  else
  {
    MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
      "Composite_Assignment_Node::evaluate: "
      "left hand side was neither a variable nor an array reference. "
      "Check your expression for errors.\n",
      array_->expand_key ().c_str (),
      rhs.to_string ().c_str ()));

  }

  // return the value
  return rhs;
}
