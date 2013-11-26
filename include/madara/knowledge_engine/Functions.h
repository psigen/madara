
#ifndef _MADARA_EXTERNAL_FUNCTIONS_H_
#define _MADARA_EXTERNAL_FUNCTIONS_H_

#include <string>
#include "madara/MADARA_export.h"
#include "madara/knowledge_engine/Knowledge_Record.h"
#include "madara/knowledge_engine/Knowledge_Update_Settings.h"
#include "madara/expression_tree/Expression_Tree.h"
#include "madara/knowledge_engine/Extern_Function_Variables.h"

#ifdef _MADARA_PYTHON_CALLBACKS_
  #include <boost/python.hpp>
#endif

/**
 * @file Functions.h
 * @author James Edmondson <jedmondson@gmail.com>
 *
 * This file contains declarations for declaring external functions that
 * are accessible by the MADARA KARL interpreter
 **/

namespace Madara
{
  namespace Knowledge_Engine
  {
    class Thread_Safe_Context;
    
    typedef Madara::Knowledge_Record VALUE_TYPE;
   
    typedef  std::vector <Knowledge_Record>   Function_Arguments;
     
    /**
     * @class Function
     * @brief This class stores a function definition
     */
    class MADARA_Export Function
    {
    public:

      /**
       * Types of functions supported
       **/
      enum Types
      {
        UNINITIALIZED = 0,
        EXTERN_UNNAMED = 1,
        EXTERN_NAMED = 2,
        KARL_EXPRESSION = 3,
        PYTHON_CALLABLE = 4
      };

      /**
       * Default constructor
       **/
      Function ()
        : extern_named (0), extern_unnamed (0), type (UNINITIALIZED)
      {
      }

      /**
       * Constructor for function pointer
       **/
      Function (Knowledge_Record (*func) (Function_Arguments &, Variables &))
        : extern_named (0), extern_unnamed (func), type (EXTERN_UNNAMED)
      {
      }
      
      /**
       * Constructor for function pointer
       **/
      Function (Knowledge_Record (*func) (const char *, 
        Function_Arguments &, Variables &))
        : extern_named (func), extern_unnamed (0), type (EXTERN_NAMED)
      {
      }
      
      /**
       * Constructor for KaRL expression
       **/
      Function (const Madara::Expression_Tree::Expression_Tree & func)
        : function_contents (func), type (KARL_EXPRESSION)
      {
      }

#ifdef _MADARA_PYTHON_CALLBACKS_
      /**
       * Constructor for function pointer
       **/
      Function (boost::python::object & func)
        : python_function (func), type (PYTHON_CALLABLE)
      {
        bool invalid_callable = false;
   
        // Check to make sure its a callable object
        if (0 == PyObject_HasAttrString (func.ptr (), "__call__"))
        {
          // If not, lets throw an exception to warn the user
          PyErr_SetString (
            PyExc_TypeError,
            "Handler must be a callable object");

          boost::python::throw_error_already_set(); 
        } 
      }
      
      bool is_python_callable (void)
      {
        return type == PYTHON_CALLABLE && !python_function.is_none ();
      }

      boost::python::object python_function;
#endif
      
      bool is_extern_unnamed (void)
      {
        return type == EXTERN_UNNAMED && extern_unnamed;
      }
      
      bool is_extern_named (void)
      {
        return type == EXTERN_NAMED && extern_named;
      }
  
      bool is_karl_expression (void)
      {
        return type == KARL_EXPRESSION;
      }

      bool is_uninitialized (void)
      {
        return type == UNINITIALIZED;
      }

      // internal function pointer
      Knowledge_Record (*extern_named) (
        const char *, Function_Arguments &, Variables &);

      // internal function pointer
      Knowledge_Record (*extern_unnamed) (Function_Arguments &, Variables &);
       
      // expression tree
      Madara::Expression_Tree::Expression_Tree function_contents;

      // type of function definition
      int type;
    };
    
  }
}




#endif