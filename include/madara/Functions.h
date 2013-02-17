
#ifndef _MADARA_EXTERNAL_FUNCTIONS_H_
#define _MADARA_EXTERNAL_FUNCTIONS_H_

#include <string>
#include "madara/MADARA_export.h"
#include "madara/knowledge_engine/Knowledge_Record.h"
#include "madara/knowledge_engine/Knowledge_Update_Settings.h"
#include "madara/expression_tree/Expression_Tree.h"

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

    private:
      /**
       * Variables context that is directly used by the KaRL engine
       **/
      Thread_Safe_Context * context_;
    };
    
    typedef  std::vector <Knowledge_Record>   Function_Arguments;
     
    class Function
    {
    /**
     * @class Function
     * @brief This class stores a function definition
     */
    public:
      /**
       * Default constructor
       **/
      Function ()
        : extern_func_ (0)
      {
      }

      /**
       * Constructor for function pointer
       **/
      Function (VALUE_TYPE (*extern_func) (Function_Arguments &, Variables &))
        : extern_func_ (extern_func)
      {
      }
      
      /**
       * Constructor for function pointer
       **/
      Function (const Madara::Expression_Tree::Expression_Tree & function_contents)
        : function_contents_ (function_contents)
      {
      }

      // internal function pointer
      VALUE_TYPE (*extern_func_) (Function_Arguments &, Variables &);

      // expression tree
      Madara::Expression_Tree::Expression_Tree function_contents_;
    };
    
  }
}




#endif