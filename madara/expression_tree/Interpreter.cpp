#ifndef _INTERPRETER_CPP_
#define _INTERPRETER_CPP_

#include <iostream>

#include "madara/expression_tree/Component_Node.h"
#include "madara/expression_tree/Leaf_Node.h"
#include "madara/expression_tree/Variable_Node.h"
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
#include "madara/expression_tree/Interpreter.h"

namespace Madara
{
  namespace Expression_Tree
  {
    enum
    {
      IMPLIES_PRECEDENCE = 1,
      ASSIGNMENT_PRECEDENCE = 2,
      LOGICAL_PRECEDENCE = 3,
      CONDITIONAL_PRECEDENCE = 4,
      ADD_PRECEDENCE = 5,
      SUBTRACT_PRECEDENCE = 5,
      MULTIPLY_PRECEDENCE = 6,
      DIVIDE_PRECEDENCE = 6,
      NEGATE_PRECEDENCE = 7,
      NUMBER_PRECEDENCE = 8,
      VARIABLE_PRECEDENCE = 8
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
      Number (::std::string input);
      Number (int input);

      /// destructor
      virtual ~Number (void);

      /// returns the precedence level
      //virtual int precedence (void);
      virtual int add_precedence (int accumulated_precedence);

      /// builds an equivalent Expression_Tree node
      virtual Component_Node *build (void);
    private:
      /// contains the value of the leaf node
      int item_;
    };

    /**
    * @class Variable
    * @brief Leaf node of parse tree
    */

    class Variable : public Symbol
    {
    public:
      /// constructors
      Variable (const ::std::string & key, Madara::Thread_Safe_Context & context);

      /// destructor
      virtual ~Variable (void);

      /// returns the precedence level
      //virtual int precedence (void);
      virtual int add_precedence (int accumulated_precedence);

      /// builds an equivalent Expression_Tree node
      virtual Component_Node *build (void);
    private:
      /// Key for retrieving value of this variable.
      const ::std::string key_;

      /// Context for variables
      Madara::Thread_Safe_Context & context_;
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
Madara::Expression_Tree::Number::Number (::std::string input)
: Symbol (0, 0, NUMBER_PRECEDENCE)
{
  item_ = atoi (input.c_str ());
}

// constructor
Madara::Expression_Tree::Number::Number (int input)
: Symbol (0, 0, NUMBER_PRECEDENCE), 
item_ (input)
{
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
Madara::Expression_Tree::Variable::Variable (const ::std::string & key, 
                    Madara::Thread_Safe_Context & context)
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
Madara::Expression_Tree::Interpreter::Interpreter (void)
{
}

// destructor
Madara::Expression_Tree::Interpreter::~Interpreter (void)
{
}

// method for checking if a character is a valid operator
bool
Madara::Expression_Tree::Interpreter::is_operator (char input)
{
  return input == '+' 
    || input == '-' 
    || input == '*' 
    || input == '/';
}

// method for checking if a character is a number
bool
Madara::Expression_Tree::Interpreter::is_number (char input)
{
  return input >= '0' && input <= '9';
}

// method for checking if a character is a candidate
// for a part of a variable name
bool
Madara::Expression_Tree::Interpreter::is_alphanumeric (char input)
{
  return (input >= 'a' && input <= 'z') 
    || (input >= 'A' && input <= 'Z') 
    || (input == '_') 
    || (input >= '0' && input <= '9');
}

// inserts a terminal into the parse tree
void
Madara::Expression_Tree::Interpreter::terminal_insert (Madara::Expression_Tree::Symbol *terminal,
                                                       ::std::list<Symbol *>& list)
{
  if (!list.empty ())
  {
    // Something exists in the list, so make this number the rightmost child

    Symbol *symbol = list.back ();

    if (symbol)
    {
      // while there is a right child, continue down
      for (; symbol->right_; symbol = symbol->right_)
        continue;

      // symbol right will be this terminal and that's all we have
      // to do.
      symbol->right_ = terminal;
    }
  }
  else
    // a number appeared first
    list.push_back (terminal);
}

// inserts a variable (leaf node / number) into the parse tree
void
Madara::Expression_Tree::Interpreter::variable_insert (Madara::Thread_Safe_Context &context,
                                                       const ::std::string &input,
                                                       ::std::string::size_type &i,
                                                       int & accumulated_precedence,
                                                       ::std::list<Symbol *>& list,
                                                       Symbol *& lastValidInput)
{
  // merge all consecutive number chars into a single
  // Number symbol, eg '123' = int (123). Scope of j needs
  // to be outside of the for loop.

  ::std::string::size_type j = 1;

  for (; i + j <= input.length () && is_alphanumeric (input[i + j]); ++j)
    continue;

  // make a Number out of the integer

  Variable * variable = new Variable (input.substr (i,j), context);
  variable->add_precedence (accumulated_precedence);

  lastValidInput = variable;

  // update i to the last character that was a number. the ++i will
  // update the i at the end of the loop to the next check.

  i += j - 1;

  precedence_insert (variable, list);
}

// inserts a leaf node / number into the parse tree
void
Madara::Expression_Tree::Interpreter::number_insert (const ::std::string &input,
        ::std::string::size_type &i, int & accumulated_precedence,
        ::std::list<Madara::Expression_Tree::Symbol *>& list,
        Madara::Expression_Tree::Symbol *& lastValidInput)
{
  // merge all consecutive number chars into a single Number symbol,
  // eg '123' = int (123). Scope of j needs to be outside of the for
  // loop.

  ::std::string::size_type j = 1;

  for (; i + j <= input.length () && is_number (input[i + j]); ++j)
    continue;

  Number *number = new Number (input.substr (i,j));
  number->add_precedence (accumulated_precedence);

  lastValidInput = number;

  // update i to the last character that was a number. the ++i will
  // update the i at the end of the loop to the next check.

  i += j - 1;

  precedence_insert (number, list);
}

// inserts an assignment operation into the parse tree
void 
Madara::Expression_Tree::Interpreter::assignment_insert (
  Madara::Expression_Tree::Symbol *op, ::std::list<Madara::Expression_Tree::Symbol *>& list)
{
  if (!list.empty ())
  {
    // if last element was a number, then make that our left_

    Symbol *parent = list.back ();
    Symbol *child = parent->right_;

    if (child)
    {
      // while there is a child of parent, keep going down the right side
      for (; 
        child && child->precedence () <= op->precedence ();
        child = child->right_)
        parent = child;
    }

    if (parent->precedence () <= op->precedence ())
    {

      // op left will be the old child. new parent child will be
      // the op. To allow infinite negations, we have to check for unary_operator.

      // **************** This is the culprit

      if (!op->left_)
        op->left_ = child;

      parent->right_ = op;
    }
    else
    {
      // this can be one of two things, either we are the same
      // precedence or we are less precedence than the parent.
      // this also means different things for unary ops. The
      // most recent unary op (negate) has a higher precedence

      if (dynamic_cast <Unary_Operator *> (op))
      {
        for (; 
          child && child->precedence () == op->precedence ();
          child = child->right_)
          parent = child;

        // I can't think of a valid reason that parent->right_ would
        // be possible !0

        parent->right_ = op;
      }
      else
      {
        // everything else is evaluated the same. For instance, if
        // this is 5 * 4 / 2, and we currently have Mult (5,4) in the
        // list, we need to make parent our left child.

        op->left_ = parent;
        list.pop_back ();
        list.push_back (op);
      }
    }
  }
  else
  {
    list.push_back (op);
  }
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
    Symbol *child = parent->right_;

    if (child)
    {
      // while there is a child of parent, keep going down the right side
      for (; 
        child && child->precedence () < op->precedence ();
        child = child->right_)
        parent = child;
    }

    if (parent->precedence () < op->precedence ())
    {

      // op left will be the old child. new parent child will be
      // the op. To allow infinite negations, we have to check for unary_operator.

      // **************** This is the culprit

      if (!op->left_)
        op->left_ = child;

      parent->right_ = op;
    }
    else
    {
      // this can be one of two things, either we are the same
      // precedence or we are less precedence than the parent.
      // this also means different things for unary ops. The
      // most recent unary op (negate) has a higher precedence

      if (dynamic_cast <Unary_Operator *> (op) || dynamic_cast <Assignment *> (op))
      {
        for (; 
          child && child->precedence () == op->precedence ();
          child = child->right_)
          parent = child;

        // I can't think of a valid reason that parent->right_ would
        // be possible !0

        parent->right_ = op;
      }
      else
      {
        // everything else is evaluated the same. For instance, if
        // this is 5 * 4 / 2, and we currently have Mult (5,4) in the
        // list, we need to make parent our left child.

        op->left_ = parent;
        list.pop_back ();
        list.push_back (op);
      }
    }
  }
  else
  {
    list.push_back (op);
  }
}

void
Madara::Expression_Tree::Interpreter::main_loop (Madara::Thread_Safe_Context & context,
       const ::std::string &input, ::std::string::size_type &i,
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
    else
      op = new Add ();

    // insert the op according to left-to-right relationships
    op->add_precedence (accumulated_precedence);
    lastValidInput = 0;
    precedence_insert (op, list);
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
  }
  else if (input[i] == '*')
  {
    // multiplication operation
    handled = true;
    Multiply *op = new Multiply ();

    // insert the op according to precedence relationships
    op->add_precedence (accumulated_precedence);
    lastValidInput = 0;
    precedence_insert (op, list);
  }
  else if (input[i] == '/')
  {
    // division operation
    handled = true;
    Divide *op = new Divide ();
    op->add_precedence (accumulated_precedence);

    lastValidInput = 0;

    // insert the op according to precedence relationships
    precedence_insert (op, list);
  }
  else if (input[i] == '=')
  {
    handled = true;
    // division operation
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
    // must be an assignment then
    else
    {
      op = new Assignment ();
      op->add_precedence (accumulated_precedence);

      lastValidInput = 0;

      // insert the op according to precedence relationships
      assignment_insert (op, list);
    }
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
  }
  else if (input[i] == '(')
  {
    handled = true;
    handle_parenthesis (context, input, i, lastValidInput, 
      handled, accumulated_precedence, list);
  }
  else if (input[i] == ' ' || input[i] == '\n')
  {
    handled = true;
    // skip whitespace
  }
}

void 
Madara::Expression_Tree::Interpreter::handle_parenthesis (
  Madara::Thread_Safe_Context & context,
  const ::std::string &input, ::std::string::size_type &i,
  Madara::Expression_Tree::Symbol *& lastValidInput,
  bool & handled, int & accumulated_precedence,
  ::std::list<Madara::Expression_Tree::Symbol *>& master_list)
{
  /* handle parenthesis is a lot like handling a new interpret.
  the difference is that we have to worry about how the calling
  function has its list setup */

  //::std::cerr << "Handling an opening parenthesis.\n";

  accumulated_precedence += 5;

  ::std::list<Symbol *> list;

  handled = false;
  for (++i; i < input.length (); ++i)
  {
    main_loop (context, input, i, lastValidInput, 
      handled, accumulated_precedence, list);

    if (input[i] == ')')
    {
      //::std::cerr << "Handling a closing parenthesis.\n";

      handled = true;
      //++i;
      accumulated_precedence -= 5;
      break;
    }
  }

  if (master_list.size () > 0 && list.size () > 0)
  {
    Symbol * lastSymbol = master_list.back ();
    Operator * op = dynamic_cast <Operator *> (lastSymbol);
    Unary_Operator * unary = dynamic_cast <Unary_Operator *>
      (lastSymbol);


    // is it a node with 2 children?
    if (op)
    {
      precedence_insert (list.back (), master_list);
    }
    else if (unary)
      // is it a unary node (like negate)
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
Madara::Expression_Tree::Interpreter::interpret (Madara::Thread_Safe_Context &context, 
                                                 const ::std::string &input)
{
  ::std::list<Symbol *> list;
  //list.clear ();
  Symbol * lastValidInput = 0;
  bool handled = false;
  int accumulated_precedence = 0;

  for (::std::string::size_type i = 0;
    i < input.length (); ++i)
  {
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
    delete list.back ();
    return tree;
  }

  // If we reach this, we didn't have any symbols.
  return Expression_Tree ();
}

#endif // _INTERPRETER_CPP_
