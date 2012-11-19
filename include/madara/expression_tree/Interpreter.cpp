#ifndef _INTERPRETER_CPP_
#define _INTERPRETER_CPP_

#include <iostream>
#include <sstream>

#include "madara/expression_tree/Component_Node.h"
#include "madara/expression_tree/Leaf_Node.h"
#include "madara/expression_tree/Variable_Node.h"
#include "madara/expression_tree/Variable_Compare_Node.h"
#include "madara/expression_tree/Variable_Decrement_Node.h"
#include "madara/expression_tree/Variable_Divide_Node.h"
#include "madara/expression_tree/Variable_Increment_Node.h"
#include "madara/expression_tree/Variable_Multiply_Node.h"
#include "madara/expression_tree/List_Node.h"
#include "madara/expression_tree/Composite_Negate_Node.h"
#include "madara/expression_tree/Composite_Predecrement_Node.h"
#include "madara/expression_tree/Composite_Preincrement_Node.h"
#include "madara/expression_tree/Composite_Not_Node.h"
#include "madara/expression_tree/Composite_Add_Node.h"
#include "madara/expression_tree/Composite_And_Node.h"
#include "madara/expression_tree/Composite_Or_Node.h"
#include "madara/expression_tree/Composite_Assignment_Node.h"
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
#include "madara/expression_tree/Composite_Return_Right_Node.h"
#include "madara/expression_tree/Composite_Function_Node.h"
#include "madara/expression_tree/Composite_For_Loop.h"
#include "madara/expression_tree/Composite_Sequential_Node.h"
#include "madara/expression_tree/Composite_Implies_Node.h"
#include "madara/expression_tree/Interpreter.h"
#include "madara/utility/Log_Macros.h"

namespace Madara
{
  namespace Expression_Tree
  {
    enum
    {
      BOTH_PRECEDENCE = 0,
      IMPLIES_PRECEDENCE = 1,
      ASSIGNMENT_PRECEDENCE = 2,
      LOGICAL_PRECEDENCE = 3,
      CONDITIONAL_PRECEDENCE = 4,
      ADD_PRECEDENCE = 5,
      SUBTRACT_PRECEDENCE = 5,
      MULTIPLY_PRECEDENCE = 6,
      MODULUS_PRECEDENCE = 6,
      DIVIDE_PRECEDENCE = 6,
      NEGATE_PRECEDENCE = 7,
      PARENTHESIS_PRECEDENCE = 8,
      NUMBER_PRECEDENCE = 8,
      VARIABLE_PRECEDENCE = 8,
      FUNCTION_PRECEDENCE = 8,
      FOR_LOOP_PRECEDENCE = 8
    };

    /**
    * @class Symbol
    * @brief Abstract base class of all parse tree nodes.
    */
    class Symbol
    {
    public:
      /// constructor
      Symbol (Symbol *left, Symbol *right, int precedence_ = 0);

      /// destructor
      virtual ~Symbol (void);

      /// abstract method for returning precedence level (higher
      /// value means higher precedence
      virtual int precedence (void)
      {
        return precedence_;
      }

      virtual int add_precedence (int accumulated_precedence) = 0;

      /// abstract method for building an Expression Expression_Tree Node

      virtual Component_Node *build (void) = 0;

      /// left and right pointers

      Symbol *left_;
      Symbol *right_;
      int precedence_;
    };

    /**
    * @class Operator
    * @brief Abstract base class for all parse tree node operators
    * @see   Add, Subtract, Multiply, Divide
    */
    class Operator : public Symbol
    {
    public:
      /// constructor
      Operator (Symbol *left, Symbol *right, int precedence_ = 1);

      /// destructor
      ~Operator (void);
    };

    /**
    * @class Unary_Operator
    * @brief Abstract base class for all parse tree node operators
    * @see   Add, Subtract, Multiply, Divide
    */
    class Unary_Operator : public Symbol
    {
    public:
      /// constructor
      Unary_Operator (Symbol *right, int precedence_ = 1);

      /// destructor
      ~Unary_Operator (void);
    };

    /**
    * @class Number
    * @brief Leaf node of parse tree
    */

    class Number : public Symbol
    {
    public:
      /// constructors
      Number (std::string input);
      Number (Madara::Knowledge_Record::Integer input);
      Number (double input);

      /// destructor
      virtual ~Number (void);

      /// returns the precedence level
      //virtual int precedence (void);
      virtual int add_precedence (int accumulated_precedence);

      /// builds an equivalent Expression_Tree node
      virtual Component_Node *build (void);
 //   private:
      /// contains the value of the leaf node
      Madara::Knowledge_Record item_;
    };

    /**
    * @class Variable
    * @brief Leaf node of parse tree
    */

    class Variable : public Symbol
    {
    public:
      /// constructors
      Variable (const std::string & key, 
        Madara::Knowledge_Engine::Thread_Safe_Context & context);

      /// destructor
      virtual ~Variable (void);

      /// returns the precedence level
      //virtual int precedence (void);
      virtual int add_precedence (int accumulated_precedence);

      /// builds an equivalent Expression_Tree node
      virtual Component_Node *build (void);
    //private:
      /// Key for retrieving value of this variable.
      const std::string key_;

      /// Context for variables
      Madara::Knowledge_Engine::Thread_Safe_Context & context_;
    };
    
    /**
    * @class Variable_Decrement
    * @brief Decrement a variable by a certain amount
    */

    class Variable_Decrement : public Operator
    {
    public:
      /// constructors
      Variable_Decrement (const std::string & key,
        Madara::Knowledge_Record value,
        Symbol * rhs,
        Madara::Knowledge_Engine::Thread_Safe_Context & context);

      /// destructor
      virtual ~Variable_Decrement (void);

      /// returns the precedence level
      //virtual int precedence (void);
      virtual int add_precedence (int accumulated_precedence);

      /// builds an equivalent Expression_Tree node
      virtual Component_Node *build (void);
    //private:
      /// Key for retrieving value of this variable.
      const std::string key_;

      /// value can be faster than rhs_, so use it if possible
      Madara::Knowledge_Record value_;

      /// Context for variables
      Madara::Knowledge_Engine::Thread_Safe_Context & context_;
    };

    /**
    * @class Variable_Divide
    * @brief Divide a variable by a certain amount
    */

    class Variable_Divide : public Operator
    {
    public:
      /// constructors
      Variable_Divide (const std::string & key,
        Madara::Knowledge_Record value,
        Symbol * rhs,
        Madara::Knowledge_Engine::Thread_Safe_Context & context);

      /// destructor
      virtual ~Variable_Divide (void);

      /// returns the precedence level
      //virtual int precedence (void);
      virtual int add_precedence (int accumulated_precedence);

      /// builds an equivalent Expression_Tree node
      virtual Component_Node *build (void);
    //private:
      /// Key for retrieving value of this variable.
      const std::string key_;

      /// value can be faster than rhs_, so use it if possible
      Madara::Knowledge_Record value_;

      /// Context for variables
      Madara::Knowledge_Engine::Thread_Safe_Context & context_;
    };

    /**
    * @class Variable_Increment
    * @brief Increment a variable by a certain amount
    */

    class Variable_Increment : public Operator
    {
    public:
      /// constructors
      Variable_Increment (const std::string & key,
        Madara::Knowledge_Record value,
        Symbol * rhs,
        Madara::Knowledge_Engine::Thread_Safe_Context & context);

      /// destructor
      virtual ~Variable_Increment (void);

      /// returns the precedence level
      //virtual int precedence (void);
      virtual int add_precedence (int accumulated_precedence);

      /// builds an equivalent Expression_Tree node
      virtual Component_Node *build (void);
    //private:
      /// Key for retrieving value of this variable.
      const std::string key_;

      /// value can be faster than rhs_, so use it if possible
      Madara::Knowledge_Record value_;

      /// Context for variables
      Madara::Knowledge_Engine::Thread_Safe_Context & context_;
    };
    
    /**
    * @class Variable_Multiply
    * @brief Multiply a variable by a certain amount
    */

    class Variable_Multiply : public Operator
    {
    public:
      /// constructors
      Variable_Multiply (const std::string & key,
        Madara::Knowledge_Record value,
        Symbol * rhs,
        Madara::Knowledge_Engine::Thread_Safe_Context & context);

      /// destructor
      virtual ~Variable_Multiply (void);

      /// returns the precedence level
      //virtual int precedence (void);
      virtual int add_precedence (int accumulated_precedence);

      /// builds an equivalent Expression_Tree node
      virtual Component_Node *build (void);
    //private:
      /// Key for retrieving value of this variable.
      const std::string key_;

      /// value can be faster than rhs_, so use it if possible
      Madara::Knowledge_Record value_;

      /// Context for variables
      Madara::Knowledge_Engine::Thread_Safe_Context & context_;
    };

    /**
    * @class Variable_Compare
    * @brief Increment a variable by a certain amount
    */

    class Variable_Compare : public Symbol
    {
    public:
      /// constructors
      Variable_Compare (const std::string & key,
        Madara::Knowledge_Record value,
        Symbol * rhs, int compare_type,
        Madara::Knowledge_Engine::Thread_Safe_Context & context);

      /// destructor
      virtual ~Variable_Compare (void);

      /// returns the precedence level
      //virtual int precedence (void);
      virtual int add_precedence (int accumulated_precedence);

      /// builds an equivalent Expression_Tree node
      virtual Component_Node *build (void);
    //private:
      /// Key for retrieving value of this variable.
      const std::string key_;
      
      /// value can be faster than rhs_, so use it if possible
      Madara::Knowledge_Record value_;

      /// rhs is used for complex rhs types (not a simple number)
      Symbol * rhs_;

      /// type of comparison. See madara/Globals.h
      int compare_type_;

      /// Context for variables
      Madara::Knowledge_Engine::Thread_Safe_Context & context_;
    };

    
    /**
    * @class List
    * @brief Parameter List
    */

    class List : public Symbol
    {
    public:
      /// constructors
      List ( 
        Madara::Knowledge_Engine::Thread_Safe_Context & context);

      /// destructor
      virtual ~List (void);

      /// returns the precedence level
      //virtual int precedence (void);
      virtual int add_precedence (int accumulated_precedence);

      /// builds an equivalent Expression_Tree node
      virtual Component_Node *build (void);
    private:

      /// Context for variables
      Madara::Knowledge_Engine::Thread_Safe_Context & context_;
    };

    /**
    * @class Subtract
    * @brief Subtraction node of the parse tree
    */

    class Subtract : public Operator
    {
    public:
      /// constructor
      Subtract (void);

      /// destructor
      virtual ~Subtract (void);

      /// returns the precedence level
      //virtual int precedence (void);
      virtual int add_precedence (int accumulated_precedence);

      /// builds an equivalent Expression_Tree node
      virtual Component_Node *build (void);
    };

    /**
    * @class Add
    * @brief Addition node of the parse tree
    */

    class Add : public Operator
    {
    public:
      /// constructor
      Add (void);

      /// destructor
      virtual ~Add (void);

      /// returns the precedence level
      //virtual int precedence (void);
      virtual int add_precedence (int accumulated_precedence);

      /// builds an equivalent Expression_Tree node
      virtual Component_Node *build (void);
    };

    /**
    * @class And
    * @brief Logically and node of the parse tree
    */

    class And : public Operator
    {
    public:
      /// constructor
      And (void);

      /// destructor
      virtual ~And (void);

      /// returns the precedence level
      //virtual int precedence (void);
      virtual int add_precedence (int accumulated_precedence);

      /// builds an equivalent Expression_Tree node
      virtual Component_Node *build (void);
    };

    /**
    * @class Or
    * @brief Logically or node of the parse tree
    */

    class Or : public Operator
    {
    public:
      /// constructor
      Or (void);

      /// destructor
      virtual ~Or (void);

      /// returns the precedence level
      //virtual int precedence (void);
      virtual int add_precedence (int accumulated_precedence);

      /// builds an equivalent Expression_Tree node
      virtual Component_Node *build (void);
    };

    /**
    * @class Both
    * @brief Evaluates both left and right children, regardless of values
    */

    class Both : public Operator
    {
    public:
      /// constructor
      Both (void);

      /// destructor
      virtual ~Both (void);

      /// returns the precedence level
      //virtual int precedence (void);
      virtual int add_precedence (int accumulated_precedence);

      /// builds an equivalent Expression_Tree node
      virtual Component_Node *build (void);
    };
    
    /**
    * @class Return_Right
    * @brief Evaluates both left and right children and returns right value
    */

    class Return_Right : public Operator
    {
    public:
      /// constructor
      Return_Right (void);

      /// destructor
      virtual ~Return_Right (void);

      /// returns the precedence level
      //virtual int precedence (void);
      virtual int add_precedence (int accumulated_precedence);

      /// builds an equivalent Expression_Tree node
      virtual Component_Node *build (void);
    };
    
    /**
    * @class Sequence
    * @brief Evaluates both left and right children, regardless of values
    */

    class Sequence : public Operator
    {
    public:
      /// constructor
      Sequence (void);

      /// destructor
      virtual ~Sequence (void);

      /// returns the precedence level
      //virtual int precedence (void);
      virtual int add_precedence (int accumulated_precedence);

      /// builds an equivalent Expression_Tree node
      virtual Component_Node *build (void);
    };

    /**
    * @class Implies
    * @brief Assign the value of an expression to a variable
    */

    class Implies : public Operator
    {
    public:
      /// constructor
      Implies (void);

      /// destructor
      virtual ~Implies (void);

      /// returns the precedence level
      //virtual int precedence (void);
      virtual int add_precedence (int accumulated_precedence);

      /// builds an equivalent Expression_Tree node
      virtual Component_Node *build (void);
    };

    /**
    * @class Assignment
    * @brief Assign the value of an expression to a variable
    */

    class Assignment : public Operator
    {
    public:
      /// constructor
      Assignment (void);

      /// destructor
      virtual ~Assignment (void);

      /// returns the precedence level
      //virtual int precedence (void);
      virtual int add_precedence (int accumulated_precedence);

      /// builds an equivalent Expression_Tree node
      virtual Component_Node *build (void);
    };

    /**
    * @class Equality
    * @brief Check and left and right arguments for equality
    */

    class Equality : public Operator
    {
    public:
      /// constructor
      Equality (void);

      /// destructor
      virtual ~Equality (void);

      /// returns the precedence level
      //virtual int precedence (void);
      virtual int add_precedence (int accumulated_precedence);

      /// builds an equivalent Expression_Tree node
      virtual Component_Node *build (void);
    };

    /**
    * @class Inequality
    * @brief Check and left and right arguments for inequality
    */

    class Inequality : public Operator
    {
    public:
      /// constructor
      Inequality (void);

      /// destructor
      virtual ~Inequality (void);

      /// returns the precedence level
      //virtual int precedence (void);
      virtual int add_precedence (int accumulated_precedence);

      /// builds an equivalent Expression_Tree node
      virtual Component_Node *build (void);
    };

    /**
    * @class Greater_Than_Equal
    * @brief Check and left and right arguments for greater than or equal to
    */

    class Greater_Than_Equal : public Operator
    {
    public:
      /// constructor
      Greater_Than_Equal (void);

      /// destructor
      virtual ~Greater_Than_Equal (void);

      /// returns the precedence level
      //virtual int precedence (void);
      virtual int add_precedence (int accumulated_precedence);

      /// builds an equivalent Expression_Tree node
      virtual Component_Node *build (void);
    };

    /**
    * @class Greater_Than
    * @brief Check and left and right arguments for greater than
    */

    class Greater_Than : public Operator
    {
    public:
      /// constructor
      Greater_Than (void);

      /// destructor
      virtual ~Greater_Than (void);

      /// returns the precedence level
      //virtual int precedence (void);
      virtual int add_precedence (int accumulated_precedence);

      /// builds an equivalent Expression_Tree node
      virtual Component_Node *build (void);
    };

    /**
    * @class Less_Than_Equal
    * @brief Check and left and right arguments for less than or equal to
    */

    class Less_Than_Equal : public Operator
    {
    public:
      /// constructor
      Less_Than_Equal (void);

      /// destructor
      virtual ~Less_Than_Equal (void);

      /// returns the precedence level
      //virtual int precedence (void);
      virtual int add_precedence (int accumulated_precedence);

      /// builds an equivalent Expression_Tree node
      virtual Component_Node *build (void);
    };

    /**
    * @class Less_Than
    * @brief Check and left and right arguments for less than
    */

    class Less_Than : public Operator
    {
    public:
      /// constructor
      Less_Than (void);

      /// destructor
      virtual ~Less_Than (void);

      /// returns the precedence level
      //virtual int precedence (void);
      virtual int add_precedence (int accumulated_precedence);

      /// builds an equivalent Expression_Tree node
      virtual Component_Node *build (void);
    };
    
    /**
    * @class Function
    * @brief Function node of the parse tree
    */

    class Function : public Unary_Operator
    {
    public:
      /// constructor
      Function (const std::string & name,
        Madara::Knowledge_Engine::Thread_Safe_Context & context);

      /// destructor
      virtual ~Function (void);

      /// returns the precedence level
      //virtual int precedence (void);
      virtual int add_precedence (int accumulated_precedence);

      /// builds an equivalent Expression_Tree node
      virtual Component_Node *build (void);

      std::string name_;
      Madara::Knowledge_Engine::Thread_Safe_Context & context_;
    };
    
    /**
    * @class For_Loop
    * @brief Iterative looping node of the parse tree
    */

    class For_Loop : public Unary_Operator
    {
    public:
      /// constructor
      For_Loop (Symbol * precondition, Symbol * condition,
        Symbol * postcondition, Symbol * body,
        Madara::Knowledge_Engine::Thread_Safe_Context & context);

      /// destructor
      virtual ~For_Loop (void);

      /// returns the precedence level
      //virtual int precedence (void);
      virtual int add_precedence (int accumulated_precedence);

      /// builds an equivalent Expression_Tree node
      virtual Component_Node *build (void);

      Symbol * precondition_;
      Symbol * condition_;
      Symbol * postcondition_;
      Symbol * body_;
      Madara::Knowledge_Engine::Thread_Safe_Context & context_;
    };

    /**
    * @class Negate
    * @brief Negate node of the parse tree
    */

    class Negate : public Unary_Operator
    {
    public:
      /// constructor
      Negate (void);

      /// destructor
      virtual ~Negate (void);

      /// returns the precedence level
      //virtual int precedence (void);
      virtual int add_precedence (int accumulated_precedence);

      /// builds an equivalent Expression_Tree node
      virtual Component_Node *build (void);
    };

    /**
    * @class Predecrement
    * @brief Predecrement node of the parse tree
    */

    class Predecrement : public Unary_Operator
    {
    public:
      /// constructor
      Predecrement (void);

      /// destructor
      virtual ~Predecrement (void);

      /// returns the precedence level
      //virtual int precedence (void);
      virtual int add_precedence (int accumulated_precedence);

      /// builds an equivalent Expression_Tree node
      virtual Component_Node *build (void);
    };

    /**
    * @class Preincrement
    * @brief Preincrement node of the parse tree
    */

    class Preincrement : public Unary_Operator
    {
    public:
      /// constructor
      Preincrement (void);

      /// destructor
      virtual ~Preincrement (void);

      /// returns the precedence level
      //virtual int precedence (void);
      virtual int add_precedence (int accumulated_precedence);

      /// builds an equivalent Expression_Tree node
      virtual Component_Node *build (void);
    };

    /**
    * @class Not
    * @brief Logically not the right node
    */

    class Not : public Unary_Operator
    {
    public:
      /// constructor
      Not (void);

      /// destructor
      virtual ~Not (void);

      /// returns the precedence level
      //virtual int precedence (void);
      virtual int add_precedence (int accumulated_precedence);

      /// builds an equivalent Expression_Tree node
      virtual Component_Node *build (void);
    };

    /**
    * @class Multiply
    * @brief Multiplication node of the parse tree
    */

    class Multiply : public Operator
    {
    public:
      /// constructor
      Multiply (void);

      /// destructor
      virtual ~Multiply (void);

      /// returns the precedence level
      //virtual int precedence (void);
      virtual int add_precedence (int accumulated_precedence);

      /// builds an equivalent Expression_Tree node
      virtual Component_Node *build (void);
    };

    /**
    * @class Modulus
    * @brief Modulus node of the parse tree (10 % 4 == 2)
    */

    class Modulus : public Operator
    {
    public:
      /// constructor
      Modulus (void);

      /// destructorm
      virtual ~Modulus (void);

      /// returns the precedence level
      //virtual int precedence (void);
      virtual int add_precedence (int accumulated_precedence);

      /// builds an equivalent Expression_Tree node
      virtual Component_Node *build (void);
    };

    /**
    * @class Divide
    * @brief Division node of the parse tree
    */

    class Divide : public Operator
    {
    public:
      /// constructor
      Divide (void);

      /// destructor
      virtual ~Divide (void);

      /// returns the precedence level
      //virtual int precedence (void);
      virtual int add_precedence (int accumulated_precedence);

      /// builds an equivalent Expression_Tree node
      virtual Component_Node *build (void);
    };

  }
}

// constructor
Madara::Expression_Tree::Symbol::Symbol (
  Madara::Expression_Tree::Symbol *left, 
  Madara::Expression_Tree::Symbol *right, int precedence)
: left_ (left), right_ (right), precedence_ (precedence)
{
}

// destructor
Madara::Expression_Tree::Symbol::~Symbol (void)
{
  delete left_;
  delete right_;
}

// constructor
Madara::Expression_Tree::Operator::Operator (Symbol *left, Symbol *right, int precedence)
: Symbol (left, right, precedence)
{
}

// destructor
Madara::Expression_Tree::Operator::~Operator (void)
{
}

// constructor
Madara::Expression_Tree::Unary_Operator::Unary_Operator (
  Madara::Expression_Tree::Symbol *right, int precedence)
: Madara::Expression_Tree::Symbol (0, right, precedence)
{
}

// destructor
Madara::Expression_Tree::Unary_Operator::~Unary_Operator (void)
{
}

// constructor
Madara::Expression_Tree::Number::Number (Madara::Knowledge_Record::Integer input)
: Symbol (0, 0, NUMBER_PRECEDENCE)
{
  item_.set_value (input);
}

// constructor
Madara::Expression_Tree::Number::Number (double input)
: Symbol (0, 0, NUMBER_PRECEDENCE)
{
  item_.set_value (input);
}

// constructor
Madara::Expression_Tree::Number::Number (std::string input)
: Symbol (0, 0, NUMBER_PRECEDENCE)
{
  item_.set_value (input);
}

// destructor
Madara::Expression_Tree::Number::~Number (void)
{
}

// returns the precedence level
int 
Madara::Expression_Tree::Number::add_precedence (int precedence)
{
  return this->precedence_ = NUMBER_PRECEDENCE + precedence;
}

// builds an equivalent Expression_Tree node
Madara::Expression_Tree::Component_Node *
Madara::Expression_Tree::Number::build (void)
{
  return new Leaf_Node (item_);
}

// constructor
Madara::Expression_Tree::Negate::Negate (void)
: Unary_Operator (0, NEGATE_PRECEDENCE)
{
}

// destructor
Madara::Expression_Tree::Negate::~Negate (void)
{
}

// returns the precedence level
int 
Madara::Expression_Tree::Negate::add_precedence (int precedence)
{
  return this->precedence_ = NEGATE_PRECEDENCE + precedence;
}

// builds an equivalent Expression_Tree node
Madara::Expression_Tree::Component_Node *
Madara::Expression_Tree::Negate::build ()
{
  return new Composite_Negate_Node (right_->build ());
}

// constructor
Madara::Expression_Tree::Function::Function (const std::string & name,
        Madara::Knowledge_Engine::Thread_Safe_Context & context)
: name_ (name), context_ (context), Unary_Operator (0, VARIABLE_PRECEDENCE)
{
}

// destructor
Madara::Expression_Tree::Function::~Function (void)
{
}

// returns the precedence level
int 
Madara::Expression_Tree::Function::add_precedence (int precedence)
{
  return this->precedence_ = VARIABLE_PRECEDENCE + precedence;
}

// builds an equivalent Expression_Tree node
Madara::Expression_Tree::Component_Node *
Madara::Expression_Tree::Function::build ()
{
  if (right_)
    return new Composite_Function_Node (name_, context_, right_->build ());
  else
  {
      MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_ERROR,
        "KARL COMPILE: Function %s contains no arguments.\n",
        name_.c_str ()));
    return new Composite_Function_Node (name_, context_, 0);
  }
}

// constructor
Madara::Expression_Tree::For_Loop::For_Loop (Symbol * precondition,
        Symbol * condition, Symbol * postcondition,
        Symbol * body,
        Madara::Knowledge_Engine::Thread_Safe_Context & context)
: precondition_ (precondition), condition_ (condition),
  postcondition_ (postcondition), body_ (body), context_ (context),
  Unary_Operator (0, VARIABLE_PRECEDENCE)
{
}

// destructor
Madara::Expression_Tree::For_Loop::~For_Loop (void)
{
  delete precondition_;
  delete postcondition_;
  delete condition_;
  delete body_;
}

// returns the precedence level
int 
Madara::Expression_Tree::For_Loop::add_precedence (int precedence)
{
  return this->precedence_ = VARIABLE_PRECEDENCE + precedence;
}

// builds an equivalent Expression_Tree node
Madara::Expression_Tree::Component_Node *
Madara::Expression_Tree::For_Loop::build ()
{
  if (body_)
    return new Composite_For_Loop (
         precondition_->build (), condition_->build (), 
         postcondition_->build (), body_->build (), context_);
  else
  return new Composite_Assignment_Node (precondition_->left_->build (),
                                          condition_->right_->build ());
}

// constructor
Madara::Expression_Tree::Predecrement::Predecrement (void)
: Unary_Operator (0, NEGATE_PRECEDENCE)
{
}

// destructor
Madara::Expression_Tree::Predecrement::~Predecrement (void)
{
}

// returns the precedence level
int 
Madara::Expression_Tree::Predecrement::add_precedence (int precedence)
{
  return this->precedence_ = NEGATE_PRECEDENCE + precedence;
}

// builds an equivalent Expression_Tree node
Madara::Expression_Tree::Component_Node *
Madara::Expression_Tree::Predecrement::build ()
{
  return new Composite_Predecrement_Node (right_->build ());
}

// constructor
Madara::Expression_Tree::Preincrement::Preincrement (void)
: Unary_Operator (0, NEGATE_PRECEDENCE)
{
}

// destructor
Madara::Expression_Tree::Preincrement::~Preincrement (void)
{
}

// returns the precedence level
int 
Madara::Expression_Tree::Preincrement::add_precedence (int precedence)
{
  return this->precedence_ = NEGATE_PRECEDENCE + precedence;
}

// builds an equivalent Expression_Tree node
Madara::Expression_Tree::Component_Node *
Madara::Expression_Tree::Preincrement::build ()
{
  return new Composite_Preincrement_Node (right_->build ());
}

// constructor
Madara::Expression_Tree::Not::Not (void)
: Unary_Operator (0, NEGATE_PRECEDENCE)
{
}

// destructor
Madara::Expression_Tree::Not::~Not (void)
{
}

// returns the precedence level
int 
Madara::Expression_Tree::Not::add_precedence (int precedence)
{
  return this->precedence_ = NEGATE_PRECEDENCE + precedence;
}

// builds an equivalent Expression_Tree node
Madara::Expression_Tree::Component_Node *
Madara::Expression_Tree::Not::build ()
{
  return new Composite_Not_Node (right_->build ());
}

// constructor
Madara::Expression_Tree::Variable::Variable (const std::string & key, 
                    Madara::Knowledge_Engine::Thread_Safe_Context & context)
: Symbol (0, 0, VARIABLE_PRECEDENCE), key_(key), context_ (context)
{
}

// destructor
Madara::Expression_Tree::Variable::~Variable (void)
{
}

// returns the precedence level
int 
Madara::Expression_Tree::Variable::add_precedence (int precedence)
{
  return this->precedence_ = VARIABLE_PRECEDENCE + precedence;
}

// builds an equivalent Expression_Tree node
Madara::Expression_Tree::Component_Node *
Madara::Expression_Tree::Variable::build (void)
{
  return new Variable_Node (key_, context_);
}


// constructor
Madara::Expression_Tree::Variable_Decrement::Variable_Decrement (const std::string & key, 
  Madara::Knowledge_Record value, Symbol * rhs,
                    Madara::Knowledge_Engine::Thread_Safe_Context & context)
: Operator (0, rhs, ASSIGNMENT_PRECEDENCE), key_ (key), value_(value),
  context_ (context)
{
}

// destructor
Madara::Expression_Tree::Variable_Decrement::~Variable_Decrement (void)
{
}

// returns the precedence level
int 
Madara::Expression_Tree::Variable_Decrement::add_precedence (int precedence)
{
  return this->precedence_ = ASSIGNMENT_PRECEDENCE + precedence;
}

// builds an equivalent Expression_Tree node
Madara::Expression_Tree::Component_Node *
Madara::Expression_Tree::Variable_Decrement::build (void)
{
  if (this->right_)
    return new Variable_Decrement_Node (key_, value_, this->right_->build (), context_);
  else
    return new Variable_Decrement_Node (key_, value_, 0, context_);
}


// constructor
Madara::Expression_Tree::Variable_Divide::Variable_Divide (const std::string & key, 
  Knowledge_Record value, Symbol * rhs,
                    Madara::Knowledge_Engine::Thread_Safe_Context & context)
: Operator (0, rhs, ASSIGNMENT_PRECEDENCE), key_ (key), value_ (value),
  context_ (context)
{
}

// destructor
Madara::Expression_Tree::Variable_Divide::~Variable_Divide (void)
{
}

// returns the precedence level
int 
Madara::Expression_Tree::Variable_Divide::add_precedence (int precedence)
{
  return this->precedence_ = ASSIGNMENT_PRECEDENCE + precedence;
}

// builds an equivalent Expression_Tree node
Madara::Expression_Tree::Component_Node *
Madara::Expression_Tree::Variable_Divide::build (void)
{
  if (this->right_)
    return new Variable_Divide_Node (key_, value_, this->right_->build (), context_);
  else
    return new Variable_Divide_Node (key_, value_, 0, context_);
}

// constructor
Madara::Expression_Tree::Variable_Increment::Variable_Increment (const std::string & key, 
  Madara::Knowledge_Record value, Symbol * rhs,
                    Madara::Knowledge_Engine::Thread_Safe_Context & context)
: Operator (0, rhs, ASSIGNMENT_PRECEDENCE), key_ (key), value_(value),
  context_ (context)
{
}

// destructor
Madara::Expression_Tree::Variable_Increment::~Variable_Increment (void)
{
}

// returns the precedence level
int 
Madara::Expression_Tree::Variable_Increment::add_precedence (int precedence)
{
  return this->precedence_ = ASSIGNMENT_PRECEDENCE + precedence;
}

// builds an equivalent Expression_Tree node
Madara::Expression_Tree::Component_Node *
Madara::Expression_Tree::Variable_Increment::build (void)
{
  if (this->right_)
    return new Variable_Increment_Node (key_, value_, this->right_->build (), context_);
  else
    return new Variable_Increment_Node (key_, value_, 0, context_);
}

// constructor
Madara::Expression_Tree::Variable_Multiply::Variable_Multiply (const std::string & key, 
  Madara::Knowledge_Record value, Symbol * rhs,
                    Madara::Knowledge_Engine::Thread_Safe_Context & context)
: Operator (0, rhs, ASSIGNMENT_PRECEDENCE), key_ (key), value_(value),
  context_ (context)
{
}

// destructor
Madara::Expression_Tree::Variable_Multiply::~Variable_Multiply (void)
{
}

// returns the precedence level
int 
Madara::Expression_Tree::Variable_Multiply::add_precedence (int precedence)
{
  return this->precedence_ = ASSIGNMENT_PRECEDENCE + precedence;
}

// builds an equivalent Expression_Tree node
Madara::Expression_Tree::Component_Node *
Madara::Expression_Tree::Variable_Multiply::build (void)
{
  if (this->right_)
    return new Variable_Multiply_Node (key_, value_, this->right_->build (), context_);
  else
    return new Variable_Multiply_Node (key_, value_, 0, context_);
}


// constructor
Madara::Expression_Tree::Variable_Compare::Variable_Compare (const std::string & key, 
  Madara::Knowledge_Record value, Symbol * rhs, int compare_type,
                    Madara::Knowledge_Engine::Thread_Safe_Context & context)
: Symbol (0, 0, VARIABLE_PRECEDENCE), key_ (key), value_(value), rhs_ (rhs),
  compare_type_ (compare_type), context_ (context)
{
}

// destructor
Madara::Expression_Tree::Variable_Compare::~Variable_Compare (void)
{
}

// returns the precedence level
int 
Madara::Expression_Tree::Variable_Compare::add_precedence (int precedence)
{
  return this->precedence_ = VARIABLE_PRECEDENCE + precedence;
}

// builds an equivalent Expression_Tree node
Madara::Expression_Tree::Component_Node *
Madara::Expression_Tree::Variable_Compare::build (void)
{
  if (rhs_)
    return new Variable_Compare_Node (key_, value_, compare_type_,
    rhs_->build (), context_);
  else
    return new Variable_Compare_Node (key_, value_, compare_type_,
    0, context_);
}



// constructor
Madara::Expression_Tree::List::List (
                    Madara::Knowledge_Engine::Thread_Safe_Context & context)
: Symbol (0, 0, VARIABLE_PRECEDENCE), context_ (context)
{
}

// destructor
Madara::Expression_Tree::List::~List (void)
{
}

// returns the precedence level
int 
Madara::Expression_Tree::List::add_precedence (int precedence)
{
  return this->precedence_ = VARIABLE_PRECEDENCE + precedence;
}

// builds an equivalent Expression_Tree node
Madara::Expression_Tree::Component_Node *
Madara::Expression_Tree::List::build (void)
{
  return new List_Node (context_);
}

// constructor
Madara::Expression_Tree::Add::Add (void)
: Operator (0, 0, ADD_PRECEDENCE)
{
}

// destructor
Madara::Expression_Tree::Add::~Add (void)
{
}

// returns the precedence level
int 
Madara::Expression_Tree::Add::add_precedence (int precedence)
{
  return this->precedence_ = ADD_PRECEDENCE + precedence;
}

// builds an equivalent Expression_Tree node
Madara::Expression_Tree::Component_Node *
Madara::Expression_Tree::Add::build (void)
{
  return new Composite_Add_Node (left_->build (), right_->build ());
}


// constructor
Madara::Expression_Tree::And::And (void)
: Operator (0, 0, LOGICAL_PRECEDENCE)
{
}

// destructor
Madara::Expression_Tree::And::~And (void)
{
}

// returns the precedence level
int 
Madara::Expression_Tree::And::add_precedence (int precedence)
{
  return this->precedence_ = LOGICAL_PRECEDENCE + precedence;
}

// builds an equivalent Expression_Tree node
Madara::Expression_Tree::Component_Node *
Madara::Expression_Tree::And::build (void)
{
  return new Composite_And_Node (left_->build (), right_->build ());
}

// constructor
Madara::Expression_Tree::Or::Or (void)
: Operator (0, 0, LOGICAL_PRECEDENCE)
{
}

// destructor
Madara::Expression_Tree::Or::~Or (void)
{
}

// returns the precedence level
int 
Madara::Expression_Tree::Or::add_precedence (int precedence)
{
  return this->precedence_ = LOGICAL_PRECEDENCE + precedence;
}

// builds an equivalent Expression_Tree node
Madara::Expression_Tree::Component_Node *
Madara::Expression_Tree::Or::build (void)
{
  return new Composite_Or_Node (left_->build (), right_->build ());
}

// constructor
Madara::Expression_Tree::Both::Both (void)
: Operator (0, 0, BOTH_PRECEDENCE)
{
}

// destructor
Madara::Expression_Tree::Both::~Both (void)
{
}

// returns the precedence level
int 
Madara::Expression_Tree::Both::add_precedence (int precedence)
{
  return this->precedence_ = BOTH_PRECEDENCE + precedence;
}

// builds an equivalent Expression_Tree node
Madara::Expression_Tree::Component_Node *
Madara::Expression_Tree::Both::build (void)
{
  // Since users can say something like ";;;;;;;;", it is very possible
  // that a both operation contains no valid children. So, we need
  // to check whether or not we have a valid child.
  if (left_ && right_)
    return new Composite_Both_Node (left_->build (), right_->build ());
  else if (left_)
    // all we have is a valid left child, so there is no reason to build
    // a Both operator
    return left_->build ();
  else if (right_)
    // all we have is a valid right child, so there is no reason to build
    // a Both operator
    return right_->build ();
  else
    // we've got nothing. This node should eventually be pruned out of the
    // picture if at all possible.
    return new Leaf_Node ((Madara::Knowledge_Record::Integer)0);
}


// constructor
Madara::Expression_Tree::Return_Right::Return_Right (void)
: Operator (0, 0, BOTH_PRECEDENCE)
{
}

// destructor
Madara::Expression_Tree::Return_Right::~Return_Right (void)
{
}

// returns the precedence level
int 
Madara::Expression_Tree::Return_Right::add_precedence (int precedence)
{
  return this->precedence_ = BOTH_PRECEDENCE + precedence;
}

// builds an equivalent Expression_Tree node
Madara::Expression_Tree::Component_Node *
Madara::Expression_Tree::Return_Right::build (void)
{
  // Since users can say something like ";;;;;;;;", it is very possible
  // that a both operation contains no valid children. So, we need
  // to check whether or not we have a valid child.
  if (left_ && right_)
    return new Composite_Return_Right_Node (left_->build (), right_->build ());
  else if (left_)
    // all we have is a valid left child, so there is no reason to build
    // a Both operator
    return left_->build ();
  else if (right_)
    // all we have is a valid right child, so there is no reason to build
    // a Both operator
    return right_->build ();
  else
    // we've got nothing. This node should eventually be pruned out of the
    // picture if at all possible.
    return new Leaf_Node ((Madara::Knowledge_Record::Integer)0);
}

// constructor
Madara::Expression_Tree::Sequence::Sequence (void)
: Operator (0, 0, BOTH_PRECEDENCE)
{
}

// destructor
Madara::Expression_Tree::Sequence::~Sequence (void)
{
}

// returns the precedence level
int 
Madara::Expression_Tree::Sequence::add_precedence (int precedence)
{
  return this->precedence_ = BOTH_PRECEDENCE + precedence;
}

// builds an equivalent Expression_Tree node
Madara::Expression_Tree::Component_Node *
Madara::Expression_Tree::Sequence::build (void)
{
  // Since users can say something like ";;;;;;;;", it is very possible
  // that a both operation contains no valid children. So, we need
  // to check whether or not we have a valid child.
  if (left_ && right_)
    return new Composite_Sequential_Node (left_->build (), right_->build ());
  else if (left_)
    // all we have is a valid left child, so there is no reason to build
    // a Both operator
    return left_->build ();
  else if (right_)
    // all we have is a valid right child, so there is no reason to build
    // a Both operator
    return right_->build ();
  else
    // we've got nothing. This node should eventually be pruned out of the
    // picture if at all possible.
    return new Leaf_Node ((Madara::Knowledge_Record::Integer)0);
}

// constructor
Madara::Expression_Tree::Implies::Implies (void)
: Operator (0, 0, IMPLIES_PRECEDENCE)
{
}

// destructor
Madara::Expression_Tree::Implies::~Implies (void)
{
}

// returns the precedence level
int 
Madara::Expression_Tree::Implies::add_precedence (int precedence)
{
  return this->precedence_ = IMPLIES_PRECEDENCE + precedence;
}

// builds an equivalent Expression_Tree node
Madara::Expression_Tree::Component_Node *
Madara::Expression_Tree::Implies::build (void)
{
  return new Composite_Implies_Node (left_->build (), right_->build ());
}


// constructor
Madara::Expression_Tree::Assignment::Assignment (void)
: Operator (0, 0, ASSIGNMENT_PRECEDENCE)
{
}

// destructor
Madara::Expression_Tree::Assignment::~Assignment (void)
{
}

// returns the precedence level
int 
Madara::Expression_Tree::Assignment::add_precedence (int precedence)
{
  return this->precedence_ = ASSIGNMENT_PRECEDENCE + precedence;
}

// builds an equivalent Expression_Tree node
Madara::Expression_Tree::Component_Node *
Madara::Expression_Tree::Assignment::build (void)
{
  // because of the way post order is evaluated and the fact that we want
  // to support statements like a = b = c, we reverse the order of the arguments
  // so the left side is built first. This looks very, very odd when printing
  // but it is the only way I know of to make this work with the order that
  // the Evaluation Visitor will visit the tree.
  return new Composite_Assignment_Node (left_->build (), right_->build ());
}


// constructor
Madara::Expression_Tree::Equality::Equality (void)
: Operator (0, 0, CONDITIONAL_PRECEDENCE)
{
}

// destructor
Madara::Expression_Tree::Equality::~Equality (void)
{
}

// returns the precedence level
int 
Madara::Expression_Tree::Equality::add_precedence (int precedence)
{
  return this->precedence_ = CONDITIONAL_PRECEDENCE + precedence;
}

// builds an equivalent Expression_Tree node
Madara::Expression_Tree::Component_Node *
Madara::Expression_Tree::Equality::build (void)
{
  return new Composite_Equality_Node (left_->build (), right_->build ());
}

// constructor
Madara::Expression_Tree::Inequality::Inequality (void)
: Operator (0, 0, CONDITIONAL_PRECEDENCE)
{
}

// destructor
Madara::Expression_Tree::Inequality::~Inequality (void)
{
}

// returns the precedence level
int 
Madara::Expression_Tree::Inequality::add_precedence (int precedence)
{
  return this->precedence_ = CONDITIONAL_PRECEDENCE + precedence;
}

// builds an equivalent Expression_Tree node
Madara::Expression_Tree::Component_Node *
Madara::Expression_Tree::Inequality::build (void)
{
  return new Composite_Inequality_Node (left_->build (), right_->build ());
}

// constructor
Madara::Expression_Tree::Greater_Than_Equal::Greater_Than_Equal (void)
: Operator (0, 0, CONDITIONAL_PRECEDENCE)
{
}

// destructor
Madara::Expression_Tree::Greater_Than_Equal::~Greater_Than_Equal (void)
{
}

// returns the precedence level
int 
Madara::Expression_Tree::Greater_Than_Equal::add_precedence (int precedence)
{
  return this->precedence_ = CONDITIONAL_PRECEDENCE + precedence;
}

// builds an equivalent Expression_Tree node
Madara::Expression_Tree::Component_Node *
Madara::Expression_Tree::Greater_Than_Equal::build (void)
{
  return new Composite_Greater_Than_Equal_Node (left_->build (), right_->build ());
}

// constructor
Madara::Expression_Tree::Greater_Than::Greater_Than (void)
: Operator (0, 0, CONDITIONAL_PRECEDENCE)
{
}

// destructor
Madara::Expression_Tree::Greater_Than::~Greater_Than (void)
{
}

// returns the precedence level
int 
Madara::Expression_Tree::Greater_Than::add_precedence (int precedence)
{
  return this->precedence_ = CONDITIONAL_PRECEDENCE + precedence;
}

// builds an equivalent Expression_Tree node
Madara::Expression_Tree::Component_Node *
Madara::Expression_Tree::Greater_Than::build (void)
{
  return new Composite_Greater_Than_Node (left_->build (), right_->build ());
}

// constructor
Madara::Expression_Tree::Less_Than_Equal::Less_Than_Equal (void)
: Operator (0, 0, CONDITIONAL_PRECEDENCE)
{
}

// destructor
Madara::Expression_Tree::Less_Than_Equal::~Less_Than_Equal (void)
{
}

// returns the precedence level
int 
Madara::Expression_Tree::Less_Than_Equal::add_precedence (int precedence)
{
  return this->precedence_ = CONDITIONAL_PRECEDENCE + precedence;
}

// builds an equivalent Expression_Tree node
Madara::Expression_Tree::Component_Node *
Madara::Expression_Tree::Less_Than_Equal::build (void)
{
  return new Composite_Less_Than_Equal_Node (left_->build (), right_->build ());
}

// constructor
Madara::Expression_Tree::Less_Than::Less_Than (void)
: Operator (0, 0, CONDITIONAL_PRECEDENCE)
{
}

// destructor
Madara::Expression_Tree::Less_Than::~Less_Than (void)
{
}

// returns the precedence level
int 
Madara::Expression_Tree::Less_Than::add_precedence (int precedence)
{
  return this->precedence_ = CONDITIONAL_PRECEDENCE + precedence;
}

// builds an equivalent Expression_Tree node
Madara::Expression_Tree::Component_Node *
Madara::Expression_Tree::Less_Than::build (void)
{
  return new Composite_Less_Than_Node (left_->build (), right_->build ());
}


// constructor
Madara::Expression_Tree::Subtract::Subtract (void)
: Operator (0, 0, SUBTRACT_PRECEDENCE)
{
}

// destructor
Madara::Expression_Tree::Subtract::~Subtract (void)
{
}

// returns the precedence level
int 
Madara::Expression_Tree::Subtract::add_precedence (int precedence)
{
  return this->precedence_ = ADD_PRECEDENCE + precedence;
}

// builds an equivalent Expression_Tree node
Madara::Expression_Tree::Component_Node *
Madara::Expression_Tree::Subtract::build (void)
{
  return new Composite_Subtract_Node (left_->build (), right_->build ());
}

// constructor
Madara::Expression_Tree::Multiply::Multiply (void)
: Operator (0, 0, MULTIPLY_PRECEDENCE)
{
}

// destructor
Madara::Expression_Tree::Multiply::~Multiply (void)
{
}

// returns the precedence level
int 
Madara::Expression_Tree::Multiply::add_precedence (int precedence)
{
  return this->precedence_ = MULTIPLY_PRECEDENCE + precedence;
}

// builds an equivalent Expression_Tree node
Madara::Expression_Tree::Component_Node *
Madara::Expression_Tree::Multiply::build (void)
{
  return new Composite_Multiply_Node (left_->build (), right_->build ());
}

// constructor
Madara::Expression_Tree::Modulus::Modulus (void)
: Operator (0, 0, MODULUS_PRECEDENCE)
{
}

// destructor
Madara::Expression_Tree::Modulus::~Modulus (void)
{
}

// returns the precedence level
int 
Madara::Expression_Tree::Modulus::add_precedence (int precedence)
{
  return this->precedence_ = MODULUS_PRECEDENCE + precedence;
}

// builds an equivalent Expression_Tree node
Madara::Expression_Tree::Component_Node *
Madara::Expression_Tree::Modulus::build (void)
{
  return new Composite_Modulus_Node (left_->build (), right_->build ());
}

// constructor
Madara::Expression_Tree::Divide::Divide (void)
: Operator (0, 0, DIVIDE_PRECEDENCE)
{
}

// destructor
Madara::Expression_Tree::Divide::~Divide (void)
{
}

// returns the precedence level
int 
Madara::Expression_Tree::Divide::add_precedence (int precedence)
{
  return this->precedence_ = DIVIDE_PRECEDENCE + precedence;
}

// builds an equivalent Expression_Tree node
Madara::Expression_Tree::Component_Node *
Madara::Expression_Tree::Divide::build (void)
{
  return new Composite_Divide_Node (left_->build (), right_->build ());
}

// constructor
Madara::Expression_Tree::Interpreter::Interpreter ()
{
}

// destructor
Madara::Expression_Tree::Interpreter::~Interpreter ()
{
}

// method for checking if a character is a valid operator
bool
Madara::Expression_Tree::Interpreter::is_operator (char input)
{
  return input == '+' 
    || input == '-' 
    || input == '*' 
    || input == '/' || input == '%';
}

// method for checking if a character is a number
bool
Madara::Expression_Tree::Interpreter::is_number (char input)
{
  return input >= '0' && input <= '9';
}

// method for checking if a character is a number
bool
Madara::Expression_Tree::Interpreter::is_string_literal (char input)
{
  return input == '"' || input == '\'';
}

// method for checking if a character is a candidate
// for a part of a variable name
bool
Madara::Expression_Tree::Interpreter::is_alphanumeric (char input)
{
  return (input >= 'a' && input <= 'z') 
    || (input >= 'A' && input <= 'Z') 
    || (input == '_') 
    || (input >= '0' && input <= '9') || input == '.' 
    || input == '{' || input == '}';
}

// method for checking if input is whitespace
bool
Madara::Expression_Tree::Interpreter::is_whitespace (char input)
{
  return input == ' ' || input == '\t' || input == '\r' || input == '\n';
}

// extracts precondition, condition, postcondition, and body from input 
void
  Madara::Expression_Tree::Interpreter::handle_for_loop (
                Madara::Knowledge_Engine::Thread_Safe_Context &context,
                                         std::string &variable,
                                            const std::string &input,
                                           std::string::size_type &i,
                                          int & accumulated_precedence,
                                           ::std::list<Symbol *>& list //,
                                               // Symbol *& precondition,
                                               //    Symbol *& condition,
                                               //Symbol *& postcondition,
                                               //        Symbol *& body
                                                       )
{
  ::std::list <Symbol *> substr_list;
  Symbol * lastValidInput (0);
  std::string::size_type begin = i;
  Operator * precondition (0); //, * condition (0), * postcondition (0);
  Symbol * body (0), * user_pre (0), * user_cond (0), * user_post (0);

  // for extracting and using substrings of input
  std::string::size_type count (0);
  std::string substr;

  if (variable == "")
    variable = ".MADARA_I";

  bool delimiter_found = false, handled = false, equal_to = false;
  std::string::size_type delimiter_begin = 0;
  std::string::size_type delimiter_end = 0;

  // search for end of for_loop conditions. Be on lookout for delimiter.
  for (; i < input.length () && input[i] != ']' && input[i] != ')'; ++i)
  {
    if (input[i] == '-')
    {
      delimiter_found = true;
      delimiter_begin = i;
    }
    if (delimiter_found && input[i] == '>')
    {
      delimiter_end = i;
    }
  }

  // What did we end with? Less than? Greater than?
  if (input[i] == ']')
    equal_to = true;
  else if (input[i] != ')')
  {
    // this is an error. Essentially, it means the user did not close the
    // for loop.
  }
    
  // if at all possible, don't touch i
  std::string::size_type end = i;

  // get the precondition, postcondition and condition ready
  precondition = new Assignment ();
  precondition->left_ = new Variable (variable, context);
  
  //// setup postcondition
  //postcondition = new Assignment ();
  //postcondition->left_ = new Variable (variable, context);

  //postcondition->right_ = new Add ();
  //postcondition->right_->left_ = new Variable (variable, context);
  //    
  //// setup loop condition
  //if (equal_to)
  //  condition = new Less_Than_Equal ();
  //else
  //  condition = new Less_Than ();

  //condition->left_ = new Variable (variable, context);
  //

  // this is the non-short-hand way of specifying, e.g., var[0,30] {}
  if (delimiter_found)
  {
    // setup precondition
    if (delimiter_begin - begin > 0)
    {
      // run main_loop on the precondition substring
      substr = input.substr (begin, delimiter_begin - begin);

      for (count = 0;
        count < substr.length (); )
      {
        main_loop (context, substr, count, lastValidInput,
                  handled, accumulated_precedence, substr_list);
      }
      
      MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_ERROR, DLINFO
        "Precondition is set to %s\n",
        substr.c_str ()));

      // we have a precondition
      if (!substr_list.empty ())
      {
        user_pre = substr_list.back ();
        substr_list.clear ();
      }
    }
    
    // check for special increment
    if (delimiter_end - delimiter_begin > 1)
    {
      count = 0;
      lastValidInput = 0;
      substr = input.substr (delimiter_begin + 1, delimiter_end - (delimiter_begin + 1));

      for (count = 0;
        count < substr.length (); )
      {
        main_loop (context, substr, count, lastValidInput,
                  handled, accumulated_precedence, substr_list);
      }
      
      MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_ERROR, DLINFO
        "Postcondition is set to %s\n",
        substr.c_str ()));

      // we have a precondition
      if (!substr_list.empty ())
      {
        user_post = substr_list.back ();

        substr_list.clear ();
      }
    }
    
    // set condition
    if (i - (delimiter_end + 1) > 1)
    {
      lastValidInput = 0;
      substr = input.substr (delimiter_end+1, i - (delimiter_end + 1));

      for (count = 0;
        count < substr.length (); )
      {
        main_loop (context, substr, count, lastValidInput,
                  handled, accumulated_precedence, substr_list);
      }
      
      // we have a condition
      if (!substr_list.empty ())
      {
        MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_ERROR, DLINFO
          "Condition is set to %s\n",
          substr.c_str ()));

        user_cond = substr_list.back ();
        substr_list.clear ();
      }
      else
      {
        MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_ERROR, DLINFO
          "Condition was not set to %s\n",
          substr.c_str ()));

      }
    }
  }
  // if no delimiter found, this is the shorthand
  else
  {
    lastValidInput = 0;
    substr = input.substr (begin, i - begin);
    
    for (count = 0;
      count < substr.length (); )
    {
      main_loop (context, substr, count, lastValidInput,
                handled, accumulated_precedence, substr_list);
    }
      
    MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_ERROR, DLINFO
      "Condition only is set to %s\n",
      substr.c_str ()));

    // we have a condition
    if (!substr_list.empty ())
    {
      user_cond = substr_list.back ();
      substr_list.clear ();
    }
  }
  
  // if precondition not set, set to default
  if (!user_pre)
    user_pre = new Number ((Madara::Knowledge_Record::Integer)0);

  // set condition to default if not yet set
  if (!user_cond)
    user_cond = new Number ((Madara::Knowledge_Record::Integer)-1);
    
  // set postcondition to default if not yet set
  if (!user_post)
  {
    user_post = new Number ((Madara::Knowledge_Record::Integer)1);
    MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_ERROR, DLINFO
      "Postcondition is set to 1 (def)\n"));
  }

  // eat up whitespace so we can check for a parenthesis (function)
  for (++i; i < input.length () && is_whitespace (input[i]); ++i);
  
  // can't have a body without a parenthesis or brace
  if (i < input.length () && input[i] == '(')
  {
    ++i;
    lastValidInput = 0;
    
      MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_ERROR, DLINFO
        "Body is reading from %s\n",
        input.substr (i, input.size () - i).c_str ()));

    // we have a function instead of a variable
    handle_parenthesis (context, input, i, lastValidInput, handled,
      accumulated_precedence, substr_list);

    if (!substr_list.empty ())
    {
      body = substr_list.back ();
      substr_list.clear ();
    }
  }

  // now, see if we can locate a body for the for loop
  if (body)
  {
    if (MADARA_debug_level >= MADARA_LOG_DETAILED_TRACE) //MADARA_LOG_DETAILED_TRACE)
    {
      Assignment * assignment = dynamic_cast <Assignment *> (body);
      if (assignment)
      {
        Variable * variable_node = dynamic_cast <Variable *> (assignment->left_);
        Number * number = dynamic_cast <Number *> (assignment->right_);

        if (variable_node && number)
        {
          MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_ERROR, DLINFO
            "For_Loop: Body is a simple assignment of variable %s to %q\n",
            variable_node->key_.c_str (), number->item_));
        }
        else
        {
          MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_ERROR, DLINFO
            "For loop has a complex body\n"));
        }
      }
    }

    precondition->right_ = user_pre;
    Madara::Knowledge_Record post_val;
    Madara::Knowledge_Record cond_val;

    // optimize postcondition
    Number * number = dynamic_cast <Number *> (user_post);
    if (number)
    {
      post_val = number->item_;
      delete number;
      user_post = 0;
    }
    
    // optimize condition
    number = dynamic_cast <Number *> (user_cond);
    if (number)
    {
      cond_val = number->item_;
      delete number;
      user_cond = 0;
    }

    int compare_type (0);

    if (equal_to)
      compare_type = 1;

    Variable_Increment * postcondition = new Variable_Increment (variable, post_val,
      user_post, context);
    postcondition->add_precedence (accumulated_precedence + FOR_LOOP_PRECEDENCE);

    Variable_Compare * condition = new Variable_Compare (variable, cond_val,
      user_cond, compare_type, context);
    condition->add_precedence (accumulated_precedence + FOR_LOOP_PRECEDENCE);

    Symbol * op = new For_Loop (precondition, condition, postcondition, body, context);
    op->add_precedence (accumulated_precedence);

    precedence_insert (op, list);

    lastValidInput = 0;
  }
  else
  {
    // user forgot to specify a for loop body, so they apparently just want us to loop
    // the variable from the precondition to the condition. Since we assume the user
    // doesn't want a busy loop, which this is, we instead create an assignment for
    // the variable to equal the condition and clean up the pointers

    // if they specified a ']', they actually want something 1 greater than the condition
    if (equal_to)
    {
      Number * number = dynamic_cast <Number *> (user_cond);

      if (number)
        ++number->item_;
      else
      {
        // if it wasn't already a number, then it must be something more complex. We'll
        // just add one to it and see if the prune () method can optimize it a bit.
        Add * add = new Add ();
        add->left_ = new Number ((Madara::Knowledge_Record::Integer)1);
        add->right_ = user_cond;
        user_cond = add;
      }
    }

    delete precondition->right_;
    precondition->right_ = user_cond;
    precondition->add_precedence (accumulated_precedence);

    precedence_insert (precondition, list);

    lastValidInput = 0;
  }
}

// inserts a variable (leaf node / number) into the parse tree
void
Madara::Expression_Tree::Interpreter::variable_insert (
                           Madara::Knowledge_Engine::Thread_Safe_Context &context,
                                                       const std::string &input,
                                                       std::string::size_type &i,
                                                       int & accumulated_precedence,
                                                       ::std::list<Symbol *>& list,
                                                       Symbol *& lastValidInput)
{
  // build a potential variable name (this could also be a function)
  std::string::size_type j = 1;

  for (; i + j < input.length () && is_alphanumeric (input[i + j]); ++j)
    continue;

  // the variable or function name is stored in input.substr (i,j)
  // is the next char a parenthesis?

  std::string name = input.substr (i, j);
  
  i += j;
  
  //MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_ERROR, DLINFO
  //    "Checking %s, before is_whitespace. i=%d, j=%Q\n",
  //    name.c_str (), i, j));

  // eat up whitespace so we can check for a parenthesis (function)
  for (; i < input.length () && is_whitespace (input[i]); ++i);
  
  //MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_ERROR, DLINFO
  //    "Checking %s, after is_whitespace. i=%d, j=%Q\n",
  //    name.c_str (), i, j));

  if (i < input.length () && input[i] == '(')
  {
    // save the function name and update i
    Function * function = new Function (name, context);
    function->add_precedence (accumulated_precedence);

    bool handled = false;

    ::std::list<Symbol *> param_list;
    
    ++i;

    // we have a function instead of a variable
    handle_parenthesis (context, input, i, lastValidInput, handled,
      accumulated_precedence, param_list);

    //if (param_list.size () > 0)
    //  function->right_ = param_list.back ();

    function->right_ = new List (context);

    precedence_insert (function, list);
    lastValidInput = 0;
  }
  else if (i < input.length () && input[i] == '[')
  {
    ++i;
    handle_for_loop (context, name, input, i, accumulated_precedence, list);
  }
  else
  {
    Variable * variable = new Variable (name, context);
    variable->add_precedence (accumulated_precedence);

    lastValidInput = variable;

    precedence_insert (variable, list);
  }

}

// inserts a leaf node / number into the parse tree
void
Madara::Expression_Tree::Interpreter::string_insert (const std::string &input,
        std::string::size_type &i, int & accumulated_precedence,
        ::std::list<Madara::Expression_Tree::Symbol *>& list,
        Madara::Expression_Tree::Symbol *& lastValidInput)
{
  std::string::size_type j = 0;
  Number * number = 0;

  for (; i + j <= input.length () && !is_string_literal (input[i + j]); ++j)
    continue;

  number = new Number (input.substr (i, j));

  number->add_precedence (accumulated_precedence);

  lastValidInput = number;

  // update i to next char for main loop or handle parenthesis.

  i += j + 1;

  precedence_insert (number, list);
}

// inserts a leaf node / number into the parse tree
void
Madara::Expression_Tree::Interpreter::number_insert (const std::string &input,
        std::string::size_type &i, int & accumulated_precedence,
        ::std::list<Madara::Expression_Tree::Symbol *>& list,
        Madara::Expression_Tree::Symbol *& lastValidInput)
{
  // merge all consecutive number chars into a single Number symbol,
  // eg '123' = int (123). Scope of j needs to be outside of the for
  // loop.

  std::string::size_type j = 1;
  bool is_float = false;
  Number * number = 0;

  for (; i + j <= input.length () && is_number (input[i + j]); ++j)
    continue;

  // do we have a float?
  if (i + j <= input.length () && input[i + j] == '.')
  {
    ++j;
    for (; i + j <= input.length () && is_number (input[i + j]); ++j)
      continue;

    double new_number;

    std::stringstream buffer;
    buffer << input.substr (i, j);
    buffer >> new_number;

    number = new Number (new_number);
  }
  else
  {
    // we have an integer
    
    Madara::Knowledge_Record::Integer new_number;

    std::stringstream buffer;
    buffer << input.substr (i, j);
    buffer >> new_number;

    number = new Number (new_number);
  }

  number->add_precedence (accumulated_precedence);

  lastValidInput = number;

  // update i to next char for main loop or handle parenthesis.

  i += j;

  precedence_insert (number, list);
}


// inserts a multiplication or division into the parse tree
void 
Madara::Expression_Tree::Interpreter::precedence_insert (
  Madara::Expression_Tree::Symbol *op, ::std::list<Madara::Expression_Tree::Symbol *>& list)
{
  if (!list.empty ())
  {
    // if last element was a number, then make that our left_

    Symbol *parent = list.back ();
    Symbol *child = 0;

    // check to see if op is an assignment or implication, which are treated
    // uniquely
    Assignment * op_assignment = dynamic_cast <Assignment *> (op);
    Implies * op_implies = dynamic_cast <Implies *> (op);
    Unary_Operator * op_unary = dynamic_cast <Unary_Operator *> (op);
    
    // move down the right side of the tree until we find either a null or
    // a precedence that is >= this operation's precedence. This puts us
    // in the situation that we know our op should be performed after child
    // or child should be null (assignment or implication not withstanding)
    for ( child = parent->right_; 
        child && child->precedence () < op->precedence ();
        child = child->right_)
        parent = child;

    // parent->precedence is < op->precedence at this point

    if (op_assignment || op_implies || op_unary)
    {
      // if we are an assignment, implies, or unary op, we actually
      // need this op to have slightly higher precedence (it needs to be
      // evaluated first). This situation is signified by something like this
      // var1 = var2 = var3 = 1. In the list, will be var1 = var2 = var3, so parent will be
      // and assignment, and parent left will be var1, right and child will be assignment
      // and it will have a left of var2
      // We need to push our current assignment a bit further down, to make our pointers look
      // like this:
      //                  =                <== current parent
      //               /      \
      //             var1        =               <== current child
      //                      /      \
      //                    var2     null
      // What we want
      //                  =               
      //               /      \
      //             var1        =               <== parent
      //                      /      \
      //                    var2     null          <== child
      //

      for ( child = parent->right_; 
        child && child->precedence () <= op->precedence ();
        child = child->right_)
        parent = child;
    }

    // Now that we have our parent and child setup appropriately according
    // to precedence relationships, we should be able to modify or replace
    // the tree in the list

    if (parent->precedence() <= op->precedence ())
    {
      // this op needs to be evaluated before the parent

      // first let's do some error checking, so we can give helpful compile errors
      Operator * parent_binary = dynamic_cast <Operator *> (parent);
      Unary_Operator * parent_unary = dynamic_cast <Unary_Operator *> (parent);

      // if the parent is a binary (like addition or &&), then it needs a left hand side
      // if it doesn't have this, let's report a warning and insert a Leaf Node with a value
      // of zero
      if (parent_binary && !parent->left_)
      {
        // try to give as specific message as possible
        Both * parent_both = dynamic_cast <Both *> (parent);
        if (parent_both)
        {
          MADARA_DEBUG (MADARA_LOG_WARNING, (LM_TRACE, 
            DLINFO "KARL COMPILE WARNING: " \
            "Empty statements between ';' may cause slower execution.\n" \
            "  Attempting to prune away the extra statement.\n"));
        }
        else
        {
          MADARA_DEBUG (MADARA_LOG_WARNING, (LM_TRACE, 
            DLINFO "KARL COMPILE WARNING: " \
            "Binary operation has no left child. Inserting a zero.\n"));
          parent->left_ = new Number ((Madara::Knowledge_Record::Integer)0);
        }
      }

      // if the parent is a unary operator (like negate or not), then it should
      // NOT have a left child. This would only happen if someone input 
      // something like 5 ! 3, which has no meaning. This is a compile error.
      if (parent_unary && parent->left_)
      {
        MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
          "\nKARL COMPILE ERROR: " \
            "Unary operation shouldn't have a left child.\n"));
        exit (-1);
      }

      // if we've gotten to this point, then we need to 
      // replace the child with ourself in the tree
      if (child)
      {
        if (op_unary)
        {
          // This is a compile error. Unary cannot have a left
          // child, and that is the only way that being at this
          // point would make sense.
          MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
            "\nKARL COMPILE ERROR: " \
            " unary operation shouldn't have a left child.\n"));
          exit (-1);
        }
        else
          op->left_ = child;
      }

      parent->right_ = op;
    }
    else
    {
      // we are a lower precedence than our parent, so we need to replace
      // the tree in the list. This typically happens with assignment, implies,
      // logical operations (&&, ||) and equality checks

      op->left_ = parent;
      list.pop_back ();
      list.push_back (op);      
    }
  }
  else
  {
    list.push_back (op);
  }
}

void
Madara::Expression_Tree::Interpreter::main_loop (
       Madara::Knowledge_Engine::Thread_Safe_Context & context,
       const std::string &input, std::string::size_type &i,
       Madara::Expression_Tree::Symbol *& lastValidInput,
       bool & handled, int & accumulated_precedence,
       ::std::list<Madara::Expression_Tree::Symbol *>& list)
{
  handled = false;
  if (is_number (input[i]))
  {
    handled = true;
    // leaf node
    number_insert (input, i, accumulated_precedence, 
      list, lastValidInput);
  }
  else if (is_alphanumeric (input[i]))
  {
    handled = true;
    // variable leaf node
    variable_insert (context, input, i, accumulated_precedence,
      list, lastValidInput);
  }
  else if (is_string_literal (input[i]))
  {
    ++i;
    handled = true;
    // variable leaf node
    string_insert (input, i, accumulated_precedence, 
      list, lastValidInput);
  }
  else if (input[i] == '+')
  {
    handled = true;
    Symbol * op = 0;

    // is this a predecrement?
    if (i+1 < input.size () && input[i+1] == '+')
    {
      op = new Preincrement ();
      ++i;
    }
    // is this an atomic increment?
    else if (i+1 < input.size () && input[i+1] == '=')
    {
      Variable * lhs = dynamic_cast <Variable *> (lastValidInput);
      if (lhs)
        op = new Variable_Increment (lhs->key_, Madara::Knowledge_Record (), 0, context);
      else
      {
        // major error here. The left hand side must be a variable
        op = new Variable_Increment (".MADARA_I", Madara::Knowledge_Record (), 0, context);
        
        MADARA_DEBUG (MADARA_LOG_WARNING, (LM_TRACE, 
          DLINFO "KARL COMPILE WARNING (+=): " \
          "Assignments must have a variable left hand side. Using .MADARA_I by " \
          "default, but this is likely a major error in the expression.\n"));
      }
      ++i;
    }
    else
      op = new Add ();

    // insert the op according to left-to-right relationships
    op->add_precedence (accumulated_precedence);
    lastValidInput = 0;
    precedence_insert (op, list);
    ++i;
  }
  else if (input[i] == '-')
  {
    handled = true;
    Symbol * op = 0;

    // is this a predecrement?
    if (i+1 < input.size () && input[i+1] == '-')
    {
      op = new Predecrement ();
      ++i;
    }
    // is this an atomic decrement?
    else if (i+1 < input.size () && input[i+1] == '=')
    {
      Variable * lhs = dynamic_cast <Variable *> (lastValidInput);
      if (lhs)
        op = new Variable_Decrement (lhs->key_, Madara::Knowledge_Record (),
                   0, context);
      else
      {
        // major error here. The left hand side must be a variable
        op = new Variable_Decrement (".MADARA_I", Madara::Knowledge_Record (),
                   0, context);
        
        MADARA_DEBUG (MADARA_LOG_WARNING, (LM_TRACE, 
          DLINFO "KARL COMPILE WARNING (-=): " \
          "Assignments must have a variable left hand side. Using .MADARA_I by " \
          "default, but this is likely a major error in the expression.\n"));
      }
      ++i;
    }
    // Negate
    else if (!lastValidInput)
      op = new Negate ();
    // Subtract
    else
      op = new Subtract ();

    // insert the op according to left-to-right relationships
    lastValidInput = 0;
    op->add_precedence (accumulated_precedence);
    precedence_insert (op, list);
    ++i;
  }
  else if (input[i] == '*')
  {
    handled = true;
    Symbol * op = 0;

    // is this an atomic multiply?
    if (i+1 < input.size () && input[i+1] == '=')
    {
      Variable * lhs = dynamic_cast <Variable *> (lastValidInput);
      if (lhs)
        op = new Variable_Multiply (lhs->key_, Madara::Knowledge_Record (),
                   0, context);
      else
      {
        // major error here. The left hand side must be a variable
        op = new Variable_Multiply (".MADARA_I", Madara::Knowledge_Record (),
                   0, context);
        
        MADARA_DEBUG (MADARA_LOG_WARNING, (LM_TRACE, 
          DLINFO "KARL COMPILE WARNING (*=): " \
          "Assignments must have a variable left hand side. Using .MADARA_I by " \
          "default, but this is likely a major error in the expression.\n"));
      }
      ++i;
    }
    // multiplication operation
    else
      op = new Multiply ();

    // insert the op according to precedence relationships
    op->add_precedence (accumulated_precedence);
    lastValidInput = 0;
    precedence_insert (op, list);
    ++i;
  }
  else if (input[i] == '%')
  {
    // multiplication operation
    handled = true;
    Modulus *op = new Modulus ();

    // insert the op according to precedence relationships
    op->add_precedence (accumulated_precedence);
    lastValidInput = 0;
    precedence_insert (op, list);
    ++i;
  }
  else if (input[i] == '/')
  {
    // is this a one line comment?
    if (i+1 < input.size () && input[i+1] == '/')
    {
      // we have a one line comment
      for (; i < input.size () && input[i] != '\n'; ++i);
    }
    // is this a multi-line comment?
    else if (i+1 < input.size () && input[i+1] == '*')
    {
      // find the matching close
      std::string::size_type found = input.find ("*/",i+1);

      // if we were able to find the matching close,
      // then set i to the '/' in '*/'
      if (found != std::string::npos)
        i = found + 1;

      // otherwise, the user apparently wanted to
      // comment out the rest of the file
      else
        i = input.size ();
    }
    // is this an atomic decrement?
    else
    {
      handled = true;
      // division operation
      Symbol * op = 0;

      // atomic division?
      if (i+1 < input.size () && input[i+1] == '=')
      {
        Variable * lhs = dynamic_cast <Variable *> (lastValidInput);
        if (lhs)
          op = new Variable_Divide (lhs->key_, Madara::Knowledge_Record (), 0, context);
        else
        {
          // major error here. The left hand side must be a variable
          op = new Variable_Divide (".MADARA_I", Madara::Knowledge_Record (), 0, context);
        
          MADARA_DEBUG (MADARA_LOG_WARNING, (LM_TRACE, 
            DLINFO "KARL COMPILE WARNING (-=): " \
            "Assignments must have a variable left hand side. Using .MADARA_I by " \
            "default, but this is likely a major error in the expression.\n"));
        }
        ++i;
      }
      else
        op = new Divide ();

      op->add_precedence (accumulated_precedence);
      lastValidInput = 0;
      precedence_insert (op, list);
    }
    ++i;
  }
  else if (input[i] == '=')
  {
    handled = true;
    Symbol * op = 0;

    // is this an equality?
    if (i+1 < input.size () && input[i+1] == '=')
    {
      op = new Equality ();
      op->add_precedence (accumulated_precedence);

      lastValidInput = 0;
      ++i;

      // insert the op according to precedence relationships
      precedence_insert (op, list);
    }
    // is this an implication?
    else if (i+1 < input.size () && input[i+1] == '>')
    {
      op = new Implies ();
      op->add_precedence (accumulated_precedence);

      lastValidInput = 0;
      ++i;

      // insert the op according to precedence relationships
      precedence_insert (op, list);
    }
    // must be an assignment then
    else
    {
      op = new Assignment ();
      op->add_precedence (accumulated_precedence);

      lastValidInput = 0;

      // insert the op according to precedence relationships
      // assignment_insert (op, list);
      precedence_insert (op, list);
    }
    ++i;
  }
  else if (input[i] == '!')
  {
    handled = true;
    Symbol * op = 0;

    // is this an inequality?
    if (i+1 < input.size () && input[i+1] == '=')
    {
      op = new Inequality ();
      ++i;
    }
    // must be a logical not then
    else
    {
      op = new Not ();
    }

    // insert the op according to precedence relationships
    op->add_precedence (accumulated_precedence);
    lastValidInput = 0;
    precedence_insert (op, list);
    ++i;
  }
  else if (input[i] == '&')
  {
    // is this a logical and?
    if (i+1 < input.size () && input[i+1] == '&')
    {
      handled = true;
      Symbol * op = new And ();
      ++i;

      // insert the op according to precedence relationships
      op->add_precedence (accumulated_precedence);
      lastValidInput = 0;
      precedence_insert (op, list);
    }
    else
    {
      // error. We currently don't allow logical and (A & B)
      MADARA_DEBUG (MADARA_LOG_NONFATAL_ERROR, (LM_ERROR, DLINFO
        "\nKARL COMPILE ERROR: " \
        " Logical And (&) not available\n." \
        " You may want to use && instead in %s.\n", input.c_str ()));
    }
    ++i;
  }
  else if (input[i] == '|')
  {
    // is this a logical and?
    if (i+1 < input.size () && input[i+1] == '|')
    {
      handled = true;
      Symbol * op = new Or ();
      ++i;

      // insert the op according to precedence relationships
      op->add_precedence (accumulated_precedence);
      lastValidInput = 0;
      precedence_insert (op, list);
    }
    else
    {
      // error. We don't currently support logical or
      MADARA_DEBUG (MADARA_LOG_NONFATAL_ERROR, (LM_ERROR, DLINFO
        "\nKARL COMPILE ERROR: " \
        " Logical And (|) not available\n." \
        " You may want to use || instead in %s.\n", input.c_str ()));
    }
    ++i;
  }
  else if (input[i] == ';')
  {
    handled = true;
    Symbol * op = 0;

    // is this a logical and?
    if (i+1 < input.size () && input[i+1] == '>')
    {
      op = new Return_Right ();
      ++i;
    }
    else
    {
      op = new Both ();
    }

    // insert the op according to precedence relationships
    op->add_precedence (accumulated_precedence);
    lastValidInput = 0;
    precedence_insert (op, list);
    ++i;
  }
  else if (input[i] == ',')
  {
    handled = true;
    Symbol * op = new Sequence ();

    // insert the op according to precedence relationships
    op->add_precedence (accumulated_precedence);
    lastValidInput = 0;
    precedence_insert (op, list);
    ++i;
  }
  else if (input[i] == '<')
  {
    handled = true;
    Symbol * op = 0;

    // is this a less than or equal to operator?
    if (i+1 < input.size () && input[i+1] == '=')
    {
      op = new Less_Than_Equal ();
      ++i;
    }
    // must be a less than operator
    else
      op = new Less_Than ();

    // insert the op according to precedence relationships
    op->add_precedence (accumulated_precedence);
    lastValidInput = 0;
    precedence_insert (op, list);
    ++i;
  }
  else if (input[i] == '>')
  {
    handled = true;
    Symbol * op = 0;

    // is this a less than or equal to operator?
    if (i+1 < input.size () && input[i+1] == '=')
    {
      op = new Greater_Than_Equal ();
      ++i;
    }
    // must be a less than operator
    else
      op = new Greater_Than ();

    // insert the op according to precedence relationships
    op->add_precedence (accumulated_precedence);
    lastValidInput = 0;
    precedence_insert (op, list);
    ++i;
  }
  else if (input[i] == '(')
  {
    handled = true;
    ++i;
    handle_parenthesis (context, input, i, lastValidInput, 
      handled, accumulated_precedence, list);
  }
  else if (input[i] == '\t' || input[i] == ' ' 
        || input[i] == '\r' || input[i] == '\n')
  {
    handled = true;
    ++i;
    // skip whitespace
  }
}

void 
Madara::Expression_Tree::Interpreter::handle_parenthesis (
  Madara::Knowledge_Engine::Thread_Safe_Context & context,
  const std::string &input, std::string::size_type &i,
  Madara::Expression_Tree::Symbol *& lastValidInput,
  bool & handled, int & accumulated_precedence,
  ::std::list<Madara::Expression_Tree::Symbol *>& master_list)
{
  /* handle parenthesis is a lot like handling a new interpret.
  the difference is that we have to worry about how the calling
  function has its list setup */

  accumulated_precedence += PARENTHESIS_PRECEDENCE;

  ::std::list<Symbol *> list;

  handled = false;
  bool closed = false;
  while (i < input.length ())
  {
    main_loop (context, input, i, lastValidInput, 
      handled, accumulated_precedence, list);

    if (input[i] == ')')
    {
      handled = true;
      closed = true;
      ++i;
      accumulated_precedence -= PARENTHESIS_PRECEDENCE;
      break;
    }
  }

  if (!closed)
  {
    MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
      "\nKARL COMPILE ERROR: " \
      "Forgot to close parenthesis in %s.\n", input.c_str ()));
    exit (-1);
  }

  if (master_list.size () > 0 && list.size () > 0)
  {
    Symbol * lastSymbol = master_list.back ();
    Operator * op = dynamic_cast <Operator *> (lastSymbol);
    Unary_Operator * unary = dynamic_cast <Unary_Operator *>
      (lastSymbol);


    // is it a node with 2 children?
    if (op || unary)
    {
      precedence_insert (list.back (), master_list);
    }
    else
    {
      // is it a terminal node (Number)
      // error
    }
  }
  else if (list.size () > 0)
    master_list = list;

  list.clear ();
}

// converts a string and context into a parse tree, and builds an
// expression tree out of the parse tree
Madara::Expression_Tree::Expression_Tree 
Madara::Expression_Tree::Interpreter::interpret (Madara::Knowledge_Engine::Thread_Safe_Context &context, 
                                                 const std::string &input)
{
  // return the cached expression tree if it exists
  ExpressionTreeMap::const_iterator found = cache_.find (input);
  if (found != cache_.end ())
    return found->second;

  ::std::list<Symbol *> list;
  //list.clear ();
  Symbol * lastValidInput = 0;
  bool handled = false;
  int accumulated_precedence = 0;

  for (std::string::size_type i = 0;
    i < input.length (); )
  {
    // we took out the loop update from the for loop
    // and the main_loop or handle_parenthesis call
    // should now take care of this.
    main_loop (context, input, i, lastValidInput, 
      handled, accumulated_precedence, list);


    // store last valid input symbol. this is useful to the '-' operator
    // and will help us determine if '-' is a subtraction or a negation
    //if (input[i] != ' ' && input[i] != '\n')
    // lastValidInput = input[i];
  }

  // if the list has an element in it, then return the back of the list.
  if (!list.empty ())
  {
    // Invoke a recursive Expression_Tree build starting with the root
    // symbol. This is an example of the builder pattern. See pg 97
    // in GoF book.

    Expression_Tree tree = Expression_Tree (list.back ()->build ());

    // optimize the tree
    tree.prune ();
    delete list.back ();

    // store this optimized tree into cached memory
    if (cache_.find (input) == cache_.end ())
      cache_[input] = tree;

    return tree;
  }

  // If we reach this, we didn't have any symbols.
  return Expression_Tree ();
}

#endif // _INTERPRETER_CPP_
