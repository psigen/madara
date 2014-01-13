
#ifndef _MADARA_MAP_H_
#define _MADARA_MAP_H_

#include <vector>
#include <map>
#include <string>
#include "madara/knowledge_engine/Knowledge_Base.h"
#include "madara/knowledge_engine/Eval_Settings.h"

/**
 * @file Map.h
 * @author James Edmondson <jedmondson@gmail.com>
 *
 * This file contains a C++ object that manages interactions for a
 * map of variables
 **/

namespace Madara
{
  namespace Knowledge_Engine
  {
    namespace Containers
    {
      /**
       * @class Map
       * @brief This class stores a map of strings to KaRL variables
       */
      class MADARA_Export Map
      {
      public:
        /**
         * Default constructor
         **/
        Map (const Eval_Settings & settings = Eval_Settings (true));
      
        /**
         * Constructor
         **/
        Map (const std::string & name,
                Knowledge_Base & knowledge,
                const Eval_Settings & settings = Eval_Settings (true));
      
        /**
         * Copy constructor
         **/
        Map (const Map & rhs);

        /**
         * Destructor
         **/
        ~Map ();

        /**
         * Retrieves a copy of the record from the map.
         * @param  key  the name of the variable entry
         * @return the value of the entry. Modifications to this will
         *         not be reflected in the context. This is a local copy.
         **/
        Knowledge_Record operator[] (const std::string & key);
      
        /**
         * Returns the size of the map
         * @return size of the map
         **/
        size_t size (void);
      
        /**
         * Syncs the keys from the knowledge base. This can be useful
         * if you expect other knowledge bases to add variables to the map.
         * @return a vector of the keys that were added during the sync
         **/
        std::vector <std::string> sync_keys (void);

        /**
         * Returns the keys within the map
         * @return keys used in the map
         **/
        void keys (std::vector <std::string> & curkeys);

        /**
         * Checks for the existence of a key
         * @param key          map key
         * @return true if key exists in map. False otherwise.
         **/
        bool exists (const std::string & key);

        /**
         * Returns the name of the map
         * @return name of the map
         **/
        std::string get_name (void);
        
        /**
         * Sets the variable name that this refers to
         * @param varn_name  the name of the variable in the knowledge base
         * @param knowledge  the knowledge base the variable is housed in
         * @param sync_keys  sync the keys to existing vars in the new name
         **/
        void set_name (const std::string & var_name,
          Knowledge_Base & knowledge, bool sync_keys = true);

        /**
         * Clears the map. This does not change anything within the knowledge
         * base.
         **/
        void clear (void);

        /**
         * Read a file into a location in the map
         * @param filename     file to read
         * @param key          key to store the file into
         * @param settings     settings to use when evaluating/updating
         */
        int read_file (const std::string & key, 
                       const std::string & filename, 
          const Eval_Settings & settings =
            Eval_Settings (true));
      
        /**
         * Sets a location within the map to the specified value
         *
         * @param key             location within the map
         * @param value           value to set at location
         * @param settings        settings for applying the update
         * @return                0 if successful, -1 if key is null, and
         *                        -2 if quality isn't high enough
         **/
        int set (const std::string & key,
          Madara::Knowledge_Record::Integer value = 
            Madara::Knowledge_Record::MODIFIED, 
          const Eval_Settings & settings =
            Eval_Settings (true));

        /**
         * Sets an index within an array to a specified value
         *
         * @param key             location within the map
         * @param index           index of the location in the array
         * @param value           value to set at location
         * @param settings        settings for applying the update
         * @return                0 if successful, -1 if key is null, and
         *                        -2 if quality isn't high enough
         **/
        int set_index (const std::string & key,
          size_t index,
          Madara::Knowledge_Record::Integer value,
          const Eval_Settings & settings =
            Eval_Settings (true));

        /**
         * Sets a location within the map to the specified value
         *
         * @param key             location within the map
         * @param value           array of integers to set at the location
         * @param size            number of elements in the array
         * @param settings        settings for applying the update
         * @return                0 if successful, -1 if key is null, and
         *                        -2 if quality isn't high enough
         **/
        int set (const std::string & key,
          const Madara::Knowledge_Record::Integer * value,
          uint32_t size,
          const Eval_Settings & settings =
            Eval_Settings (true));
       
        /**
         * Sets a location within the map to the specified value
         *
         * @param key             location within the map
         * @param value           array of integers to set at the location
         * @param settings        settings for applying the update
         * @return                0 if successful, -1 if key is null, and
         *                        -2 if quality isn't high enough
         **/
        int set (const std::string & key,
          const std::vector <Knowledge_Record::Integer> & value,
          const Eval_Settings & settings =
            Eval_Settings (true));
       
        /**
         * Sets a location within the map to the specified value
         *
         * @param key             location within the map
         * @param value           value to set at location
         * @param settings        settings for applying the update
         * @return                0 if successful, -1 if key is null, and
         *                        -2 if quality isn't high enough
         **/
        int set (const std::string & key, double value, 
          const Eval_Settings & settings =
            Eval_Settings (true));

        /**
         * Sets an index within a map to a specified value
         *
         * @param key             location within the map
         * @param index           index of the location in the array
         * @param value           value to set at location
         * @param settings        settings for applying the update
         * @return                0 if successful, -1 if key is null, and
         *                        -2 if quality isn't high enough
         **/
        int set_index (const std::string & key,
          size_t index,
          double value,
          const Eval_Settings & settings =
            Eval_Settings (true));

        /**
         * Sets a location within the map to the specified value
         *
         * @param key             location within the map
         * @param value           array of doubles to set at the location
         * @param size            number of elements in the array
         * @param settings        settings for applying the update
         * @return                0 if successful, -1 if key is null, and
         *                        -2 if quality isn't high enough
         **/
        int set (const std::string & key,
          const double * value,
          uint32_t size,
          const Eval_Settings & settings =
            Eval_Settings (true));
       
        /**
         * Sets a location within the map to the specified value
         *
         * @param key             location within the map
         * @param value           array of doubles to set at the location
         * @param settings        settings for applying the update
         * @return                0 if successful, -1 if key is null, and
         *                        -2 if quality isn't high enough
         **/
        int set (const std::string & key,
          const std::vector <double> & value,
          const Eval_Settings & settings =
            Eval_Settings (true));
        
        /**
         * Sets a location within the map to the specified value
         *
         * @param key             location within the map
         * @param value           value to set at location
         * @param settings        settings for applying the update
         * @return                0 if successful, -1 if key is null, and
         *                        -2 if quality isn't high enough
         **/
        int set (const std::string & key, const std::string & value, 
          const Eval_Settings & settings =
            Eval_Settings (true));

        /**
         * Sets the quality of writing to a certain variable from this entity
         *
         * @param key             location within the map
         * @param quality         quality of writing to this location
         * @param settings        settings for referring to knowledge variables
         **/
        void set_quality (const std::string & key, uint32_t quality,
               const Knowledge_Reference_Settings & settings =
                       Knowledge_Reference_Settings (false));
      
        /**
         * Atomically sets the value of an index to an arbitrary string.
         * @param   index     index within vector
         * @param   value     new value of the variable
         * @param   size      indicates the size of the value buffer
         * @param   settings  settings for applying the update
         * @return   0 if the value was set. -1 if null key
         **/
        int set_file (const std::string & key,
          const unsigned char * value, size_t size, 
          const Eval_Settings & settings =
            Eval_Settings (true));
      
        /**
         * Atomically sets the value of an index to a JPEG image
         * @param   key       name of a variable
         * @param   value     new value of the variable
         * @param   size      indicates the size of the value buffer
         * @param   settings  settings for applying the update
         * @return   0 if the value was set. -1 if null key
         **/
        int set_jpeg (const std::string & key,
          const unsigned char * value, size_t size, 
          const Eval_Settings & settings =
            Eval_Settings (true));
      
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
         * Map of variables to values
         **/
        std::map <std::string, Variable_Reference> map_;

        /**
         * Settings for modifications
         **/
        Eval_Settings settings_;
      };
    }
  }
}




#endif // _MADARA_MAP_H_