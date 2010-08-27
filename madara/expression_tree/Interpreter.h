

#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_

/**
 * @file Interpreter.h
 * @author James Edmondson <james.r.edmondson@vanderbilt.edu>
 *
 * This file contains classes and methods specific to the numerical
 * Madara::Expression_Tree::Interpreter class
 */

#include <string>
#include <list>
#include <map>

#include "madara/Globals.h"
#include "madara/expression_tree/Expression_Tree.h"
#include "madara/Thread_Safe_Context.h"

namespace Madara
{
  namespace Expression_Tree
  {
    // Forward declaration.
    class Symbol;

    /**
     * @class Interpreter
     * @brief Parses incoming expression strings into a parse tree and
     *        generates an expression tree from the parse tree.
     *
     *        This class plays the role of the "interpreter" in the
     *        Intepreter pattern.  It also uses the Builder pattern to
     *        generate the nodes in the expression tree.
     */
    class Interpreter
    {
    public:
      /// Constructor.
      Interpreter (void);

      /// destructor
      virtual ~Interpreter (void);

      /// Converts a string and context into a parse tree, and builds an
      /// expression tree out of the parse tree.
      Expression_Tree interpret (Madara::Thread_Safe_Context &context, 
                                 const ::std::string &input);

      /// Method for checking if a character is a valid operator.
      static bool is_operator (char input);

      /// Method for checking if a character is a number.
      static bool is_number (char input);
      
      /// Method for checking if a character is a candidate for a part of
      /// a variable name.
      static bool is_alphanumeric (char input);

    private:
      /// Inserts a terminal into the parse tree.
      void terminal_insert (Symbol *op,
                           ::std::list<Symbol *>& list);

      /// Inserts a variable (leaf node / number) into the parse tree.
      void variable_insert (Madara::Thread_Safe_Context &context,
                            const ::std::string &input,
                            ::std::string::size_type &i,
                          int & accumulated_precedence,
                          ::std::list<Symbol *>& list,
                           Symbol *& lastValidInput);

      /// Inserts a leaf node / number into the parse tree.
      void number_insert (const ::std::string &input, 
                          ::std::string::size_type &i,
                          int & accumulated_precedence,
                          ::std::list<Symbol *>& list,
                           Symbol *& lastValidInput);

      /// Inserts a multiplication or division into the parse tree.
      void precedence_insert (Symbol *op, ::std::list<Symbol *>& list);

      /// Inserts an assignment into the parse tree.
      void assignment_insert (Symbol *op, ::std::list<Symbol *>& list);

      void main_loop (Madara::Thread_Safe_Context & context,
                           const ::std::string &input,
                           ::std::string::size_type &i,
                           Symbol *& lastValidInput,
                           bool & handled,
                           int & accumulated_precedence,
                           ::std::list<Symbol *>& list);

      void handle_parenthesis (Madara::Thread_Safe_Context & context,
                           const ::std::string &input,
                           ::std::string::size_type &i,
                           Symbol *& lastValidInput,
                           bool & handled,
                           int & accumulated_precedence,
                           ::std::list<Symbol *>& list);

      /// Stores the parse tree. isn't entirely necessary right now, but
      /// is kept as a list for future expansion (parentheses).
      //::std::list<Symbol *> list_;
    };
  }
}

#endif /* _INTERPRETER_H_ */
