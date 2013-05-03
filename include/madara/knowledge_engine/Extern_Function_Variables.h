
#ifndef _MADARA_EXTERNAL_FUNCTION_VARIABLES_H_
#define _MADARA_EXTERNAL_FUNCTION_VARIABLES_H_

#include <string>
#include "madara/MADARA_export.h"
#include "madara/knowledge_engine/Knowledge_Record.h"
#include "madara/knowledge_engine/Knowledge_Update_Settings.h"
#include "madara/expression_tree/Expression_Tree.h"
#include "madara/knowledge_engine/Compiled_Expression.h"

/**
 * @file Functions.h
 * @author James Edmondson <james.r.edmondson@vanderbilt.edu>
 *
 * This file contains declarations for declaring external functions that
 * are accessible by the MADARA KARL interpreter
 **/

namespace Madara
{
  namespace Expression_Tree
  {
    class Composite_Function_Node;
  }

  namespace Knowledge_Engine
  {
    class Thread_Safe_Context;
    
    typedef Madara::Knowledge_Record VALUE_TYPE;
   
    /**
     * @class Variables
     * @brief Provides an interface for external functions into the MADARA
     *        KaRL variable settings
     */
    class MADARA_Export Variables
    {
    public:
      friend class Madara::Expression_Tree::Composite_Function_Node;

      /**
       * Constructor
       **/
      Variables ()
        : context_ (0)
      {
      }

      /**
       * Retrieves the value of a variable.
       * @param   key       unique identifier of the variable
       * @return   the value of the keyed variable
       **/
      Knowledge_Record get (const std::string & key) const;
      
      /**
       * Sets the value of a variable.
       * @param   key       unique identifier of the variable
       * @param   value     new value of the variable
       * @param   settings  Settings for updating knowledge
       * @return   the new value of the keyed variable
       **/
      int set (const std::string & key,
        Madara::Knowledge_Record::Integer value,
        const Knowledge_Update_Settings & settings =
          DEFAULT_KNOWLEDGE_UPDATE_SETTINGS);
      
      /**
       * Sets the value of a variable.
       * @param   key       unique identifier of the variable
       * @param   value     new value of the variable
       * @param   settings  Settings for updating knowledge
       * @return   the new value of the keyed variable
       **/
      int set (const std::string & key, double value,
        const Knowledge_Update_Settings & settings =
          DEFAULT_KNOWLEDGE_UPDATE_SETTINGS);
      
      /**
       * Sets the value of a variable.
       * @param   key       unique identifier of the variable
       * @param   value     new value of the variable
       * @param   settings  Settings for updating knowledge
       * @return   the new value of the keyed variable
       **/
      int set (const std::string & key, const std::string & value,
        const Knowledge_Update_Settings & settings =
          DEFAULT_KNOWLEDGE_UPDATE_SETTINGS);
      
      /**
       * Sets the value of a variable.
       * @param   key       unique identifier of the variable
       * @param   value     new value of the variable
       * @param   settings  Settings for updating knowledge
       * @return   the new value of the keyed variable
       **/
      int set (const std::string & key,
        const Madara::Knowledge_Record & value,
        const Knowledge_Update_Settings & settings =
          DEFAULT_KNOWLEDGE_UPDATE_SETTINGS);
      
      /**
       * Atomically increments the value of the variable
       * @param   key            unique identifier of the variable
       * @param   settings  Settings for updating knowledge
       * @return                 new value of variable
       **/
      Knowledge_Record inc (const std::string & key,
        const Knowledge_Update_Settings & settings =
          DEFAULT_KNOWLEDGE_UPDATE_SETTINGS);

      /**
       * Decrements the value of the variable
       * @param   key            unique identifier of the variable
       * @param   settings  Settings for updating knowledge
       * @return                 new value of variable
       **/
      Knowledge_Record dec (const std::string & key,
        const Knowledge_Update_Settings & settings =
          DEFAULT_KNOWLEDGE_UPDATE_SETTINGS);

      /**
       * Prints all variables and values in the context
       * @param   level          log level. @see Log_Macros.h
       **/
      void print (unsigned int level) const;

      /**
       * Print a statement, similar to printf (variable expansions
       * allowed) e.g., input = "MyVar{.id} = {MyVar{.id}}\n";
       * @param   statement      templated statement to print from
       * @param   level          log level. @see Log_Macros.h
       **/
      void print (const std::string & statement, unsigned int level) const;
      
      /**
       * Expands a string with variable expansion. For instance, if
       * .id == 5, and a statement of "MyVar{.id} = {.id} * 30" then
       * then expanded statement would be "MyVar5 = 5 * 30".
       * @param  statement  statement to expand. Useful for printing.
       * @return            variable expanded statement
       **/
      std::string expand_statement (const std::string & statement) const;
      
      /**
       * Compiles a KaRL expression into an expression tree. Always do this
       * before calling evaluate because it puts the expression into an
       * optimized format. Best practice is to save the Compiled_Expression
       * in a global variable or in some kind of persistent store. Pair with
       * expand_statement if you know that variable expansion is used but
       * the variable values that are expanded never change (e.g. an id that
       * is set through the command line and thus stays the same after it is
       * initially set).
       *
       * @param expression         expression to compile
       * @return                   compiled, optimized expression tree
       **/
      Compiled_Expression
        compile (const std::string & expression);
      
      /**
       * Evaluates an expression (USE ONLY FOR PROTOTYPING; DO NOT USE IN
       * PRODUCTION SYSTEMS). Consider compiling the expression first with
       * a one-time compile call during an initialization phase for your
       * program, and then using the evaluate Compiled_Expression call
       * in any function that must be called frequently or periodically.
       * The difference in overhead between this function and the compiled
       * version is orders of magnitude (generally nanoseconds versus
       * microseconds every call).
       *
       * @param expression      KaRL expression to evaluate
       * @param settings        Settings for evaluating and printing
       * @return                value of expression
       **/
      Madara::Knowledge_Record evaluate (
        const std::string & expression,
        const Knowledge_Update_Settings & settings =
          DEFAULT_KNOWLEDGE_UPDATE_SETTINGS);

      /**
       * Evaluates an expression. Recommended best practices are to compile the
       * expression into a global variable or persistent store outside of the
       * function call and use a reference to this Compiled_Expression from
       * within the external function that you create.
       *
       * @param expression      KaRL expression to wait on (result of compile)
       * @param settings        Settings for updating knowledge
       * @return                value of expression
       **/
      Madara::Knowledge_Record evaluate (
        Compiled_Expression & expression,
        const Knowledge_Update_Settings & settings =
          DEFAULT_KNOWLEDGE_UPDATE_SETTINGS);

      /**
       * Fills a vector with Knowledge Records that begin with a common subject
       * and have a finite range of integer values.
       * @param   subject     The common subject of the variable names. For
       *                      instance, if we are looking for a range of vars
       *                      like "var0", "var1", "var2", then the common
       *                      subject would be "var".
       * @param   start       An inclusive start index
       * @param   end         An inclusive end index
       * @param   target      The vector that will be filled with
       *                      Knowledge Record instances within the subject
       *                      range.
       * @return              entries in the resulting vector
       **/
      unsigned int to_vector (const std::string & subject,
                              unsigned int start,
                              unsigned int end,
                              std::vector <Knowledge_Record> & target);
      
      /**
       * Fills a variable map with Knowledge Records that match an expression.
       * At the moment, this expression must be of the form "subject*"
       * @param   expression  An expression that matches the variable names
       *                      that are of interest. Wildcards may only be
       *                      at the end.
       * @param   target      The map that will be filled with variable names
       *                      and the Knowledge Records they correspond to
       * @return              entries in the resulting map
       **/
      unsigned int to_map    (const std::string & subject,
                       std::map <std::string, Knowledge_Record> & target);

    private:
      /**
       * Variables context that is directly used by the KaRL engine
       **/
      Thread_Safe_Context * context_;
    };
  }
}




#endif