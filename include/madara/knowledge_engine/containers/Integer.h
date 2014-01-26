
#ifndef _MADARA_INTEGER_H_
#define _MADARA_INTEGER_H_

#include <vector>
#include <string>
#include "madara/knowledge_engine/Knowledge_Base.h"
#include "madara/knowledge_engine/Thread_Safe_Context.h"
#include "madara/knowledge_engine/Eval_Settings.h"

/**
 * @file Integer.h
 * @author James Edmondson <jedmondson@gmail.com>
 *
 * This file contains a C++ object that manages interactions for an
 * array of variables
 **/

namespace Madara
{
  namespace Knowledge_Engine
  {
    namespace Containers
    {
      /**
       * @class Integer
       * @brief This class stores an integer within a variable context
       */
      class MADARA_Export Integer
      {
      public:
        /// trait that describes the value type
        typedef Knowledge_Record::Integer  type;
        
        /**
         * Default constructor
         **/
        Integer (const Eval_Settings & settings = Eval_Settings (true));
      
        /**
         * Constructor
         * @param  name       name of the integer in the knowledge base
         * @param  knowledge  the knowledge base that will contain the vector
         * @param  settings   settings for evaluating the vector
         **/
        Integer (const std::string & name,
                Knowledge_Base & knowledge,
                const Eval_Settings & settings = Eval_Settings (true));
      
        /**
         * Constructor
         * @param  name      the name of the map within the variable context
         * @param  knowledge the variable context
         * @param  settings  settings to apply by default
         **/
        Integer (const std::string & name,
                Variables & knowledge,
                const Eval_Settings & settings = Eval_Settings (true));
      
        /**
         * Default constructor
         * @param  name       name of the integer in the knowledge base
         * @param  knowledge  the knowledge base that will contain the vector
         * @param  value      new value of the variable in the knowledge base
         * @param  settings   settings for evaluating the vector
         **/
        Integer (const std::string & name,
                Knowledge_Base & knowledge,
                type value, 
                const Eval_Settings & settings = Eval_Settings (true));
      
        /**
         * Default constructor
         * @param  name       name of the integer in the knowledge base
         * @param  knowledge  the knowledge base that will contain the vector
         * @param  value      new value of the variable in the knowledge base
         * @param  settings   settings for evaluating the vector
         **/
        Integer (const std::string & name,
                Variables & knowledge,
                type value, 
                const Eval_Settings & settings = Eval_Settings (true));
      
        /**
         * Copy constructor
         **/
        Integer (const Integer & rhs);

        /**
         * Destructor
         **/
        ~Integer ();

        /**
         * Returns the name of the variable
         * @return name of the variable
         **/
        std::string get_name (void);
        
        /**
         * Sets the variable name that this refers to
         * @param varn_name  the name of the variable in the knowledge base
         * @param knowledge  the knowledge base the variable is housed in
         **/
        void set_name (const std::string & var_name,
          Knowledge_Base & knowledge);
        
        /**
         * Sets the variable name that this refers to
         * @param varn_name  the name of the variable in the knowledge base
         * @param knowledge  the knowledge base the variable is housed in
         **/
        void set_name (const std::string & var_name,
          Variables & knowledge);

        /**
         * Exchanges the integer at this location with the integer at another
         * location.
         * @param  other   the other integer to exchange with
         **/
        void exchange (Containers::Integer & other);

        /**
         * Sets the value of the variable
         * @param  value  the new value of the variable
         * @return the updated value (should be same as value param)
         **/
        type operator= (type value);

        /**
         * Returns the value of the variable
         * @return the value of the variable
         **/
        type operator* (void);
      
        /**
         * Returns the value as a double
         * @return the value as a double
         **/
        double to_double (void);
        
        /**
         * Returns the value as a string
         * @return the value as a string
         **/
        std::string to_string (void);

        /**
         * Sets the quality of writing to the variable
         *
         * @param quality         quality of writing to this location
         * @param settings        settings for referring to knowledge variables
         **/
        void set_quality (uint32_t quality,
               const Knowledge_Reference_Settings & settings =
                       Knowledge_Reference_Settings (false));
      

      private:
        /// guard for access and changes
        typedef ACE_Guard<ACE_Recursive_Thread_Mutex> Guard;
      
        /**
         * Mutex for local changes
         **/
        mutable ACE_Recursive_Thread_Mutex mutex_;

        /**
         * Variable context that we are modifying
         **/
        Thread_Safe_Context * context_;

        /**
         * Prefix of variable
         **/
        std::string name_;

        /**
         * Variable reference
         **/
        Variable_Reference variable_;

        /**
         * Settings for modifications
         **/
        Eval_Settings settings_;
      };
    }
  }
}




#endif // _MADARA_INTEGER_H_