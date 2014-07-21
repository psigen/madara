
#ifndef _MADARA_EXTERNAL_FUNCTIONS_H_
#define _MADARA_EXTERNAL_FUNCTIONS_H_

#include <string>
#include "madara/MADARA_export.h"
#include "madara/knowledge_engine/Knowledge_Record.h"
#include "madara/knowledge_engine/Knowledge_Update_Settings.h"
#include "madara/expression_tree/Expression_Tree.h"

#ifdef _MADARA_JAVA_
#include <jni.h>
#include "madara_jni.h"
#endif

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
    class Variables;
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
        PYTHON_CALLABLE = 4,
        JAVA_CALLABLE = 5
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
      
#ifdef _MADARA_JAVA_
      jobject java_object;
      
      /**
       * Constructor for java
       **/
      Function (jobject& object)
      : type (JAVA_CALLABLE)
      {
        //We have to create a globla ref to the object or we cant call it
        JNIEnv* env = madara_jni_get_env();
        java_object = (jobject) env->NewGlobalRef(object);
      }
      
      bool is_java_callable (void) const
      {
        return type == JAVA_CALLABLE;
      }
      
      Knowledge_Record call_java(Function_Arguments & args, Variables & vars)
      {
        JNIEnv* env = madara_jni_get_env();
        
        //Change the vector to a java array to let MadaraJNI handle it
        jlong * argsArrayNative = new jlong [args.size()];
        for (unsigned int x = 0; x < args.size(); x++)
        {
          argsArrayNative[x] = (jlong)&(args[x]);
        }
        jlongArray argsArray = env->NewLongArray((jsize)args.size());
        env->SetLongArrayRegion(argsArray, 0, (jsize)args.size(), argsArrayNative);
        
        //Attach the tread and make the call
        jlong ret = env->CallStaticLongMethod(madara_jni_class(), madara_jni_function_callback(), java_object, argsArray, &vars);
        
        bool do_delete = true;
        //We need to see if they returned an arg we sent them, or a new value
        for (unsigned int x = 0; x < args.size(); x++)
        {
          if (ret == (jlong)&(args[x]))
          {
            do_delete = false;
            break;
          }
        }
        
        //The returned value is a pointer to a knowledge record, so we must free it
        Knowledge_Record record(*(Knowledge_Record*)ret);
        if (do_delete)
          delete (Knowledge_Record*)ret;
        
        return record;
      }
      
      Knowledge_Record call_java_filter(Function_Arguments & args, Variables & vars) const
      {
        JNIEnv* env = madara_jni_get_env();
        
        //Change the vector to a java array to let MadaraJNI handle it
        jlong * argsArrayNative = new jlong [args.size()];
        for (unsigned int x = 0; x < args.size(); x++)
        {
          argsArrayNative[x] = (jlong)&(args[x]);
        }
        jlongArray argsArray = env->NewLongArray((jsize)args.size());
        env->SetLongArrayRegion(argsArray, 0, (jsize)args.size(), argsArrayNative);
        
        //Attach the tread and make the call
        jlong ret = env->CallStaticLongMethod(madara_jni_class(), madara_jni_filter_callback(), java_object, argsArray, &vars);
        
        if (ret <= 0)
          return Knowledge_Record::Integer(0);
        
        bool do_delete = true;
        //We need to see if they returned an arg we sent them, or a new value
        for (unsigned int x = 0; x < args.size(); x++)
        {
          if (ret == (jlong)&(args[x]))
          {
            do_delete = false;
            break;
          }
        }
        
        //The returned value is a pointer to a knowledge record, so we must free it
        Knowledge_Record record(*(Knowledge_Record*)ret);
        if (do_delete)
          delete (Knowledge_Record*)ret;
        
        return record;
      }
#endif

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
      
      bool is_python_callable (void) const
      {
        return type == PYTHON_CALLABLE && !python_function.is_none ();
      }

      boost::python::object python_function;
#endif
      
      bool is_extern_unnamed (void) const
      {
        return type == EXTERN_UNNAMED && extern_unnamed;
      }
      
      bool is_extern_named (void) const
      {
        return type == EXTERN_NAMED && extern_named;
      }
  
      bool is_karl_expression (void) const
      {
        return type == KARL_EXPRESSION;
      }

      bool is_uninitialized (void) const
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