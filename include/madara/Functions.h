
#ifndef _MADARA_EXTERNAL_FUNCTIONS_H_
#define _MADARA_EXTERNAL_FUNCTIONS_H_

#include <string>
#include "madara/MADARA_export.h"
#include "madara/Globals.h"

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
    
    typedef Madara::Knowledge_Record::VALUE_TYPE VALUE_TYPE;
   
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
      VALUE_TYPE get (const std::string & key) const;
      
      /**
       * Sets the value of a variable.
       * @param   key       unique identifier of the variable
       * @param   value     new value of the variable
       * @return   the new value of the keyed variable
       **/
      VALUE_TYPE set (const std::string & key, VALUE_TYPE value);
      
      /**
       * Atomically increments the value of the variable
       * @param   key            unique identifier of the variable
       * @return                 new value of variable
       **/
      VALUE_TYPE inc (const ::std::string & key);

      /**
       * Decrements the value of the variable
       * @param   key            unique identifier of the variable
       * @return                 new value of variable
       **/
      VALUE_TYPE dec (const ::std::string & key);

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
      Function (VALUE_TYPE (*extern_func) (Variables *))
        : extern_func_ (extern_func)
      {
      }

      // internal function pointer
      VALUE_TYPE (*extern_func_) (Variables *);
    };
    
  }
}




#endif