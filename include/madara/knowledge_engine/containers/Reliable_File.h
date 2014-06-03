
#ifndef _MADARA_CONTAINERS_RELIABLE_FILE_H_
#define _MADARA_CONTAINERS_RELIABLE_FILE_H_

#include <vector>
#include <string>
#include "madara/knowledge_engine/containers/Integer_Vector.h"
#include "madara/knowledge_engine/containers/Buffer_Vector.h"
#include "madara/knowledge_engine/Knowledge_Base.h"
#include "madara/knowledge_engine/Thread_Safe_Context.h"
#include "madara/knowledge_engine/Knowledge_Update_Settings.h"

/**
 * @file Reliable_File.h
 * @author James Edmondson <jedmondson@gmail.com>
 *
 * This file contains a C++ object that manages interactions for a
 * string inside a variable context
 **/

namespace Madara
{
  namespace Knowledge_Engine
  {
    namespace Containers
    {
      /**
       * @class Reliable_File
       * @brief This class manages a file and acks to the file from a set
       *        of processes.
       */
      class MADARA_Export Reliable_File
      {
      public:
        /// trait that describes the value type
        typedef std::string  type;
        
        /**
         * Default constructor
         **/
        Reliable_File (const Knowledge_Update_Settings & settings =
          Knowledge_Update_Settings ());
      
        /**
         * Default constructor
         * @param  name       name of the variable in the knowledge base
         * @param  knowledge  the knowledge base that will contain the vector
         * @param  settings   settings for evaluating the vector
         **/
        Reliable_File (const std::string & name,
                Knowledge_Base & knowledge,
                const Knowledge_Update_Settings & settings =
                  Knowledge_Update_Settings ());
      
        /**
         * Constructor
         * @param  name       name of the variable in the knowledge base
         * @param  knowledge  the knowledge base that will contain the vector
         * @param  value      new value of the variable
         * @param  settings   settings for evaluating the vector
         **/
        Reliable_File (const std::string & name,
                Knowledge_Base & knowledge,
                const std::string & filename,
                const Knowledge_Update_Settings & settings =
                  Knowledge_Update_Settings ());
      
        /**
         * Copy constructor
         **/
        Reliable_File (const Reliable_File & rhs);

        /**
         * Destructor
         **/
        ~Reliable_File ();
        
        /**
         * Assignment operator
         * @param  rhs    value to copy
         **/
        void operator= (const Reliable_File & rhs);

        /**
         * Exchanges the string at this location with the string at another
         * location.
         * @param  other   the other string to exchange with
         **/
        void exchange (Reliable_File & other);

        /**
         * Sends file chunks that have not been fully acked
         * @return number of file chunks sent. If this number
         *         is zero, all file chunks have been acked
         **/
        int send (void);

        /**
         * Sets the id of this process, for the purpose of acks
         * @param   id          the id of this process
         **/
        void set_id (size_t id);

        /**
         * Sets the number of processes that should ack
         * @param   processes   the number of processes within the ack
         **/
        void set_processes (size_t processes);

        /**
         * Gets the id of this process, for the purposes of acks
         * @return the id of this process
         **/
        size_t get_id (void);
        
        /**
         * Gets the number of processes participating in the ack
         * @return the id of this process
         **/
        size_t get_processes (void);

        /**
         * Returns true if all acks have been received
         * @return true if all acks have been received
         **/
        bool is_finished (void) const;

        /**
         * Returns the name of the vector
         * @return name of the vector
         **/
        std::string get_name (void) const;
        
        /**
         * Sets the variable name that this refers to
         * @param var_name  the name of the variable in the knowledge base
         * @param knowledge  the knowledge base the variable is housed in
         **/
        void set_name (const std::string & var_name,
          Knowledge_Base & knowledge);
        
        /**
         * Reads from a file
         * @param filename           file to read
         */
        int read (const std::string & filename);
      
        /**
         * Reads from a variable
         * @param  name       variable name to read from
         **/
        int from_variable (const std::string & name);

        /**
         * Atomically sets the value of an index to an arbitrary string
         * @param   value     new value of the variable
         * @param   size      indicates the size of the value buffer
         * @return   0 if the value was set. -1 if null key
         **/
        int set_file (const unsigned char * value, size_t size);
      
        /**
         * Sets the value of the variable
         * @param  value  the new value of the variable
         * @return the updated value (should be same as value param)
         **/
        type operator= (const type & value);
        
        /**
         * Checks for equality
         * @param  value  the value to compare to
         * @return true if equal, false otherwise
         **/
        bool operator== (const Reliable_File & value) const;
        
        /**
         * Checks for inequality
         * @param  value  the value to compare to
         * @return true if inequal, false otherwise
         **/
        bool operator!= (const Reliable_File & value) const;
        
        /**
         * Sets the update settings for the variable
         * @param  settings  the new settings to use
         * @return the old update settings
         **/
        Knowledge_Update_Settings set_settings (
          const Knowledge_Update_Settings & settings);

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

        /**
         * Splits the buffer at file by the size into fragments
         **/
        void split (void);

        /**
         * Creates an ack vector of current processes
         **/
        void create_acks (void);

        /**
         * Clears fragments
         **/
        void clear_frags (void);

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
         * original file
         **/
        char * file_;

        /**
         * size of the file
         **/
        size_t size_;

        /**
         * maximum fragment size
         **/
        size_t max_frag_size_;
        
        /**
         * id of this process
         **/
        size_t id_;

        /**
         * number of processes responding with acks
         **/
        size_t processes_;

        /**
         * Variable reference
         **/
        Buffer_Array fragments_;

        /**
         * Variable reference
         **/
        std::vector <Integer_Array> acks_;

        /**
         * Settings for modifications
         **/
        Knowledge_Update_Settings settings_;
      };
    }
  }
}

#endif // _MADARA_CONTAINERS_RELIABLE_FILE_H_