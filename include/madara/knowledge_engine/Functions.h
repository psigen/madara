
#ifndef _MADARA_EXTERNAL_FUNCTIONS_H_
#define _MADARA_EXTERNAL_FUNCTIONS_H_

#include <string>
#include "madara/MADARA_export.h"
#include "madara/knowledge_engine/Knowledge_Record.h"
#include "madara/knowledge_engine/Knowledge_Update_Settings.h"
#include "madara/expression_tree/Expression_Tree.h"
#include "madara/knowledge_engine/Extern_Function_Variables.h"

/**
 * @file Functions.h
 * @author James Edmondson <james.r.edmondson@vanderbilt.edu>
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
        : extern_named_ (0), extern_unnamed_ (0)
      {
      }

      /**
       * Constructor for function pointer
       **/
      Function (VALUE_TYPE (*extern_func) (Function_Arguments &, Variables &))
        : extern_named_ (0), extern_unnamed_ (extern_func)
      {
      }
      
      /**
       * Constructor for function pointer
       **/
      Function (VALUE_TYPE (*extern_func) (const char *, 
        Function_Arguments &, Variables &))
        : extern_named_ (extern_func), extern_unnamed_ (0)
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
      VALUE_TYPE (*extern_named_) (
        const char *, Function_Arguments &, Variables &);

      // internal function pointer
      VALUE_TYPE (*extern_unnamed_) (Function_Arguments &, Variables &);

      // expression tree
      Madara::Expression_Tree::Expression_Tree function_contents_;
    };
    
  }
}




#endif