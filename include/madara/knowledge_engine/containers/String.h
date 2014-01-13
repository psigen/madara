
#ifndef _MADARA_STRING_H_
#define _MADARA_STRING_H_

#include <vector>
#include <string>
#include "madara/knowledge_engine/Knowledge_Base.h"
#include "madara/knowledge_engine/Eval_Settings.h"

/**
 * @file String.h
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
       * @class String
       * @brief This class stores a vector of KaRL variables
       */
      class MADARA_Export String
      {
      public:
        /// trait that describes the value type
        typedef std::string  type;
        
        /**
         * Default constructor
         **/
        String (const Eval_Settings & settings = Eval_Settings (true));
      
        /**
         * Default constructor
         * @param  name       name of the variable in the knowledge base
         * @param  knowledge  the knowledge base that will contain the vector
         * @param  settings   settings for evaluating the vector
         **/
        String (const std::string & name,
                Knowledge_Base & knowledge,
                const Eval_Settings & settings = Eval_Settings (true));
      
        /**
         * Default constructor
         * @param  name       name of the variable in the knowledge base
         * @param  knowledge  the knowledge base that will contain the vector
         * @param  value      new value of the variable
         * @param  settings   settings for evaluating the vector
         **/
        String (const std::string & name,
                Knowledge_Base & knowledge,
                const std::string & value,
                const Eval_Settings & settings = Eval_Settings (true));
      
        /**
         * Copy constructor
         **/
        String (const String & rhs);

        /**
         * Destructor
         **/
        ~String ();

        /**
         * Returns the name of the vector
         * @return name of the vector
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
         * Sets the value of the variable
         * @param  value  the new value of the variable
         * @return the updated value (should be same as value param)
         **/
        type operator= (const type & value);

        /**
         * Returns the value of the variable
         * @return the value of the variable
         **/
        type operator* (void);
      
        /**
         * Returns the value as an integer
         * @return the value as an integer
         **/
        Madara::Knowledge_Record::Integer to_integer (void);
        
        /**
         * Returns the value as a double
         * @return the value as a double
         **/
        double to_double (void);
        
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
        Knowledge_Base * knowledge_;

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




#endif // _MADARA_STRING_H_