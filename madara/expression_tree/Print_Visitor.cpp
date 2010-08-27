/* -*- C++ -*- */
#ifndef _PRINT_VISITOR_CPP_
#define _PRINT_VISITOR_CPP_

#include <iostream>

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
#include "madara/expression_tree/Print_Visitor.h"

/// visit function - prints Leaf_Node contents to std::cout

void 
Madara::Expression_Tree::Print_Visitor::visit (const Leaf_Node &node)
{
  std::cout << node.item () << " ";
}

/// visit function - prints Variable_Node value to std::cout

void 
Madara::Expression_Tree::Print_Visitor::visit (const Variable_Node &node)
{
  std::cout << node.key () << " ";
}

/// visit function - prints Composite_Negate_Node contents to std::cout

void 
Madara::Expression_Tree::Print_Visitor::visit (const Composite_Negate_Node &node)
{
  std::cout.put ('-');
}

/// visit function - prints Composite_Negate_Node contents to std::cout

void 
Madara::Expression_Tree::Print_Visitor::visit (const Composite_Predecrement_Node &node)
{
  std::cout << "--";
}

/// visit function - prints Composite_Negate_Node contents to std::cout

void 
Madara::Expression_Tree::Print_Visitor::visit (const Composite_Preincrement_Node &node)
{
  std::cout << "++";
}

/// visit function - prints Composite_Not_Node contents to std::cout

void 
Madara::Expression_Tree::Print_Visitor::visit (const Composite_Not_Node &node)
{
  std::cout.put ('!');
}

/// visit function - prints Composite_Add_Node contents to std::cout

void 
Madara::Expression_Tree::Print_Visitor::visit (const Composite_Add_Node &node)
{
  std::cout << "+ ";
}

/// visit function - prints Composite_And_Node contents to std::cout

void 
Madara::Expression_Tree::Print_Visitor::visit (const Composite_And_Node &node)
{
  std::cout << "&& ";
}

/// visit function - prints Composite_Or_Node contents to std::cout

void 
Madara::Expression_Tree::Print_Visitor::visit (const Composite_Or_Node &node)
{
  std::cout << "|| ";
}

/// visit function - prints Composite_Equality_Node contents to std::cout
void 
Madara::Expression_Tree::Print_Visitor::visit (const Composite_Equality_Node &node)
{
  std::cout << "== ";
}

/// visit function - prints Composite_Inequality_Node contents to std::cout
void 
Madara::Expression_Tree::Print_Visitor::visit (const Composite_Inequality_Node &node)
{
  std::cout << "!= ";
}

/// visit function - prints Composite_Greater_Than_Equal_Node contents to std::cout
void 
Madara::Expression_Tree::Print_Visitor::visit (const Composite_Greater_Than_Equal_Node &node)
{
  std::cout << ">= ";
}

/// visit function - prints Composite_Greater_Than_Node contents to std::cout
void 
Madara::Expression_Tree::Print_Visitor::visit (const Composite_Greater_Than_Node &node)
{
  std::cout << "> ";
}

/// visit function - prints Composite_Less_Than_Equal_Node contents to std::cout
void 
Madara::Expression_Tree::Print_Visitor::visit (const Composite_Less_Than_Equal_Node &node)
{
  std::cout << "<= ";
}

/// visit function - prints Composite_Less_Than_Node contents to std::cout
void 
Madara::Expression_Tree::Print_Visitor::visit (const Composite_Less_Than_Node &node)
{
  std::cout << "< ";
}

/// visit function - prints Composite_Assignment_Node contents to std::cout

void 
Madara::Expression_Tree::Print_Visitor::visit (const Composite_Assignment_Node &node)
{
  std::cout << "= ";
}

/// visit function - prints Composite_Subtract_Node contents to std::cout

void 
Madara::Expression_Tree::Print_Visitor::visit (const Composite_Subtract_Node &node)
{
  std::cout << "- ";
}

/// visit function - prints Composite_Divide_Node contents to std::cout

void 
Madara::Expression_Tree::Print_Visitor::visit (const Composite_Divide_Node &node)
{
  std::cout << "/ ";
}

/// visit function - prints Composite_Multiply_Node contents to std::cout

void 
Madara::Expression_Tree::Print_Visitor::visit (const Composite_Multiply_Node &node)
{
  std::cout << "* ";
}

#endif /* _PRINT_VISITOR_CPP_ */
