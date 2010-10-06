/* -*- C++ -*- */
#ifndef _EVALUATION_VISITOR_CPP_
#define _EVALUATION_VISITOR_CPP_

#include <iostream>
#include <typeinfo>
#include <algorithm>

#include "madara/expression_tree/Leaf_Node.h"
#include "madara/expression_tree/Variable_Node.h"
#include "madara/expression_tree/Composite_Negate_Node.h"
#include "madara/expression_tree/Composite_Predecrement_Node.h"
#include "madara/expression_tree/Composite_Preincrement_Node.h"
#include "madara/expression_tree/Composite_Not_Node.h"
#include "madara/expression_tree/Composite_Add_Node.h"
#include "madara/expression_tree/Composite_Assignment_Node.h"
#include "madara/expression_tree/Composite_And_Node.h"
#include "madara/expression_tree/Composite_Or_Node.h"
#include "madara/expression_tree/Composite_Equality_Node.h"
#include "madara/expression_tree/Composite_Inequality_Node.h"
#include "madara/expression_tree/Composite_Greater_Than_Equal_Node.h"
#include "madara/expression_tree/Composite_Greater_Than_Node.h"
#include "madara/expression_tree/Composite_Less_Than_Equal_Node.h"
#include "madara/expression_tree/Composite_Less_Than_Node.h"
#include "madara/expression_tree/Composite_Subtract_Node.h"
#include "madara/expression_tree/Composite_Divide_Node.h"
#include "madara/expression_tree/Composite_Multiply_Node.h"
#include "madara/expression_tree/Composite_Modulus_Node.h"
#include "madara/expression_tree/Composite_Both_Node.h"
#include "madara/expression_tree/Composite_Implies_Node.h"

#include "madara/expression_tree/Evaluation_Visitor.h"

/// Evaluate a Leaf_Node (holds a static value)
void 
Madara::Expression_Tree::Evaluation_Visitor::visit (
  const Madara::Expression_Tree::Leaf_Node &node)
{
  stack_.push (node.item ());
}

/// Evaluate a Variable_Node (holds a dynamic value)
void 
Madara::Expression_Tree::Evaluation_Visitor::visit (
  const Madara::Expression_Tree::Variable_Node &node)
{
  stack_.push (node.item ());
}

/// evaluation of a negation (Composite_Negate_Node)
void 
Madara::Expression_Tree::Evaluation_Visitor::visit (
  const Madara::Expression_Tree::Composite_Negate_Node &node)
{
  if (stack_.size () >= 1)
    stack_.push (-stack_.pop ());
}

/// evaluation of a decrement (Composite_Predecrement_Node)
void 
Madara::Expression_Tree::Evaluation_Visitor::visit (
  const Madara::Expression_Tree::Composite_Predecrement_Node &node)
{
  if (stack_.size () >= 1)
  {
    int old_value = stack_.pop ();
    try
    {
      Variable_Node * right = dynamic_cast <Variable_Node *> (node.right ());

      int new_value = --old_value;
      if (right)
      {
        new_value = right->dec ();
      }
      stack_.push (new_value);
    }
    catch (::std::bad_cast &)
    {
      stack_.push (--old_value);
    }
  }
}

/// evaluation of a increment (Composite_Preincrement_Node)
void 
Madara::Expression_Tree::Evaluation_Visitor::visit (
  const Madara::Expression_Tree::Composite_Preincrement_Node &node)
{
  if (stack_.size () >= 1)
  {
    int old_value = stack_.pop ();
    try
    {
      Variable_Node * right = dynamic_cast <Variable_Node *> (node.right ());

      int new_value = ++old_value;
      if (right)
      {
        new_value = right->inc ();
      }
      stack_.push (new_value);
    }
    catch (::std::bad_cast &)
    {
      stack_.push (++old_value);
    }
  }
}

/// evaluation of a negation (Composite_Not_Node)
void 
Madara::Expression_Tree::Evaluation_Visitor::visit (
  const Madara::Expression_Tree::Composite_Not_Node &node)
{
  if (stack_.size () >= 1)
    stack_.push (!stack_.pop ());
}

/// evaluation of an addition (Composite_Add_Node)
void 
Madara::Expression_Tree::Evaluation_Visitor::visit (
  const Madara::Expression_Tree::Composite_Add_Node &node)
{
  if (stack_.size () >= 2)
    stack_.push (stack_.pop () + stack_.pop ());
  // ::std::cout << "add current top: " << stack_.top () << ::std::endl;
}

/// evaluation of an assignment (Composite_Assignment_Node)
void 
Madara::Expression_Tree::Evaluation_Visitor::visit (
  const Madara::Expression_Tree::Composite_Assignment_Node &node)
{
  if (stack_.size () >= 2)
  {
    // for an assignment to be valid, we need a variable to the left
    // and an expression tree to the right. 
    try
    {
      // this is really backwards logic, but it was the only way I could think of
      // to allow for a = b = c with this type of tree and post-order flow
      int right = stack_.pop ();
      int old_value = stack_.pop ();
      Variable_Node * left = dynamic_cast <Variable_Node *> (node.left ());
      left->set (right);
      stack_.push (right);
    }
    catch (::std::bad_cast &)
    {
      std::cerr << "ERROR: Bad cast: Assignment operation must have a Variable as left operand\n";
    }
  }
  // ::std::cout << "add current top: " << stack_.top () << ::std::endl;
}

/// evaluation of a logical and comparison (Composite_And_Node)
void 
Madara::Expression_Tree::Evaluation_Visitor::visit (
  const Madara::Expression_Tree::Composite_And_Node &node)
{
  if (stack_.size () >= 2)
  {
    int right = stack_.pop ();
    int left = stack_.pop ();

    stack_.push (left && right);
  }
}

/// evaluation of a logical or comparison (Composite_Or_Node)
void 
Madara::Expression_Tree::Evaluation_Visitor::visit (
  const Madara::Expression_Tree::Composite_Or_Node &node)
{
  if (stack_.size () >= 2)
  {
    int right = stack_.pop ();
    int left = stack_.pop ();

    stack_.push (left || right);
  }
}

/// evaluation of both left and right (Composite_Both_Node)
void 
Madara::Expression_Tree::Evaluation_Visitor::visit (
  const Madara::Expression_Tree::Composite_Both_Node &node)
{
  if (stack_.size () >= 2)
  {
    int right_v = stack_.pop ();
    int left_v = stack_.pop ();

    // I was trying to use std::max, but it was giving me
    // some grief, so I just implemented it as is
    stack_.push (left_v > right_v ? left_v : right_v);
  }
}

/// evaluation of an equality comparison (Composite_Equality_Node)
void 
Madara::Expression_Tree::Evaluation_Visitor::visit (
  const Madara::Expression_Tree::Composite_Equality_Node &node)
{
  if (stack_.size () >= 2)
  {
    int right = stack_.pop ();
    int left = stack_.pop ();

    stack_.push (left == right);
  }
  // ::std::cout << "add current top: " << stack_.top () << ::std::endl;
}

/// evaluation of an inequality comparison (Composite_Inequality_Node)
void 
Madara::Expression_Tree::Evaluation_Visitor::visit (
  const Madara::Expression_Tree::Composite_Inequality_Node &node)
{
  if (stack_.size () >= 2)
  {
    int right = stack_.pop ();
    int left = stack_.pop ();

    stack_.push (left != right);
  }
  // ::std::cout << "add current top: " << stack_.top () << ::std::endl;
}

/// evaluation of a greater than equal to comparison (Composite_Greater_Than_Equal_Node)
void 
Madara::Expression_Tree::Evaluation_Visitor::visit (
  const Madara::Expression_Tree::Composite_Greater_Than_Equal_Node &node)
{
  if (stack_.size () >= 2)
  {
    int right = stack_.pop ();
    int left = stack_.pop ();

    stack_.push (left >= right);
  }
  // ::std::cout << "add current top: " << stack_.top () << ::std::endl;
}

/// evaluation of a greater than comparison (Composite_Greater_Than_Node)
void 
Madara::Expression_Tree::Evaluation_Visitor::visit (
  const Madara::Expression_Tree::Composite_Greater_Than_Node &node)
{
  if (stack_.size () >= 2)
  {
    int right = stack_.pop ();
    int left = stack_.pop ();

    stack_.push (left > right);
  }
  // ::std::cout << "add current top: " << stack_.top () << ::std::endl;
}

/// evaluation of a less than equal to comparison (Composite_Less_Than_Equal_Node)
void 
Madara::Expression_Tree::Evaluation_Visitor::visit (
  const Madara::Expression_Tree::Composite_Less_Than_Equal_Node &node)
{
  if (stack_.size () >= 2)
  {
    int right = stack_.pop ();
    int left = stack_.pop ();

    stack_.push (left <= right);
  }
  // ::std::cout << "add current top: " << stack_.top () << ::std::endl;
}

/// evaluation of a less than comparison (Composite_Less_Than_Node)
void 
Madara::Expression_Tree::Evaluation_Visitor::visit (
  const Madara::Expression_Tree::Composite_Less_Than_Node &node)
{
  if (stack_.size () >= 2)
  {
    int right = stack_.pop ();
    int left = stack_.pop ();

    stack_.push (left < right);
  }
  // ::std::cout << "add current top: " << stack_.top () << ::std::endl;
}

/// evaluation of an addition (Composite_Subtract_Node)
void 
Madara::Expression_Tree::Evaluation_Visitor::visit (
  const Madara::Expression_Tree::Composite_Subtract_Node &node)
{
  if (stack_.size () >= 2)
    {
      int rhs = stack_.pop ();
      stack_.push (stack_.pop () - rhs);
    }
}

/// evaluations of a division (Composite_Divide_Node)
void 
Madara::Expression_Tree::Evaluation_Visitor::visit (
  const Madara::Expression_Tree::Composite_Divide_Node &node)
{
  if (stack_.size () >= 2 && stack_.top ())
    {
      int rhs = stack_.pop ();
      stack_.push (stack_.pop () / rhs );
    }
  else
    {
      ::std::cout << "\n\n**ERROR**: Division by zero is not allowed. ";
      ::std::cout << "Resetting evaluation visitor.\n\n";
      reset ();
    }
}

/// evaluations of a division (Composite_Multiply_Node)
void 
Madara::Expression_Tree::Evaluation_Visitor::visit (
  const Madara::Expression_Tree::Composite_Multiply_Node &node)
{
  if (stack_.size () >= 2)
    stack_.push (stack_.pop () * stack_.pop ());
}

/// evaluations of a division (Composite_Modulus_Node)
void 
Madara::Expression_Tree::Evaluation_Visitor::visit (
  const Madara::Expression_Tree::Composite_Modulus_Node &node)
{
  if (stack_.size () >= 2)
    stack_.push (stack_.pop () % stack_.pop ());
}

/// evaluations of a division (Composite_Modulus_Node)
void 
Madara::Expression_Tree::Evaluation_Visitor::visit (
  const Madara::Expression_Tree::Composite_Implies_Node &node)
{
  if (stack_.size () >= 2)
    stack_.push (stack_.pop () ? stack_.pop () : 0);
}

/// print a total for the evaluation
int 
Madara::Expression_Tree::Evaluation_Visitor::total (void)
{
  if (!stack_.is_empty ())
    return stack_.top ();
  else
    return 0;
}

/// reset the evaluation
void 
Madara::Expression_Tree::Evaluation_Visitor::reset (void)
{
  stack_.erase ();
}

#endif /* _EVALUATION_VISITOR_CPP_ */
