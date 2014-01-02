
#ifndef _MADARA_VECTOR_H_
#define _MADARA_VECTOR_H_

#include <vector>
#include <string>
#include "madara/knowledge_engine/Knowledge_Base.h"
#include "madara/knowledge_engine/Eval_Settings.h"

/**
 * @file Vector.h
 * @author James Edmondson <jedmondson@gmail.com>
 *
 * This file contains a C++ object that manages interactions for an
 * array of variables
 **/

namespace Madara
{
  namespace Knowledge_Engine
  {
    /**
     * @class Vector
     * @brief This class stores a vector of KaRL variables
     */
    class MADARA_Export Vector
    {
    public:
      /**
       * Default constructor
       * @param  name       name of the vector in the knowledge base
       * @param  size       size of the vector
       * @param  knowledge  the knowledge base that will contain the vector
       * @param  settings   settings for evaluating the vector
       **/
      Vector (const std::string & name, unsigned int size,
              Knowledge_Base & knowledge,
              const Eval_Settings & settings = Eval_Settings (true));
      
      /**
       * Copy constructor
       **/
      Vector (const Vector & rhs);

      /**
       * Destructor
       **/
      ~Vector ();

      /**
       * Resizes the vector
       * @param   size   maximum size of the vector
       **/
      void resize (unsigned int size);
      
      /**
       * Returns the size of the vector
       * @return size of the vector
       **/
      unsigned int size (void);
      
      /**
       * Returns the name of the vector
       * @return name of the vector
       **/
      std::string get_name (void);

      /**
       * Retrieves a copy of the record from the map.
       * @param  index  the index of the variable entry
       * @return the value of the entry. Modifications to this will
       *         not be reflected in the context. This is a local copy.
       **/
      Knowledge_Record operator[] (unsigned int index);
      
      
      /**
       * Read a file into the index
       * @param filename           file to read
       * @param index              index within vector
       * @param settings           settings to use when evaluating/updating
       */
      int read_file (unsigned int index, 
                     const std::string & filename, 
        const Eval_Settings & settings =
          Eval_Settings (true));
      
      /**
       * Atomically sets the value of an index to an arbitrary string.
       * @param   index     index within vector
       * @param   value     new value of the variable
       * @param   size      indicates the size of the value buffer
       * @param   settings  settings for applying the update
       * @return   0 if the value was set. -1 if null key
       **/
      int set_file (unsigned int index,
        const unsigned char * value, size_t size, 
        const Eval_Settings & settings =
          Eval_Settings (true));
      
      /**
       * Atomically sets the value of an index to a JPEG image
       * @param   index     index of the variable to set
       * @param   value     new value of the variable
       * @param   size      indicates the size of the value buffer
       * @param   settings  settings for applying the update
       * @return   0 if the value was set. -1 if null key
       **/
      int set_jpeg (unsigned int index,
        const unsigned char * value, size_t size, 
        const Eval_Settings & settings =
          Eval_Settings (true));
      
      
      /**
       * Sets a knowledge variable to a specified value
       *
       * @param index           index to set
       * @param value           value to set at location
       * @param settings        settings for applying the update
       * @return                0 if successful, -1 if key is null, and
       *                        -2 if quality isn't high enough
       **/
      int set (unsigned int index,
        Madara::Knowledge_Record::Integer value = 
          Madara::Knowledge_Record::MODIFIED, 
        const Eval_Settings & settings =
          Eval_Settings (true));

      /**
       * Sets an index within an array to a specified value
       *
       * @param index           index to set in the variable
       * @param sub_index       index of the location in the array
       * @param value           value to set at location
       * @param settings        settings for applying the update
       * @return                0 if successful, -1 if key is null, and
       *                        -2 if quality isn't high enough
       **/
      int set_index (unsigned int index,
        size_t sub_index,
        Madara::Knowledge_Record::Integer value,
        const Eval_Settings & settings =
          Eval_Settings (true));

      /**
       * Sets a knowledge variable to a specified value
       *
       * @param index           index to set
       * @param value           array of integers to set at the location
       * @param size            number of elements in the array
       * @param settings        settings for applying the update
       * @return                0 if successful, -1 if key is null, and
       *                        -2 if quality isn't high enough
       **/
      int set (unsigned int index,
        const Madara::Knowledge_Record::Integer * value,
        uint32_t size,
        const Eval_Settings & settings =
          Eval_Settings (true));
       
      /**
       * Sets a knowledge variable to a specified value
       *
       * @param index           index to set
       * @param value           array of integers to set at the location
       * @param settings        settings for applying the update
       * @return                0 if successful, -1 if key is null, and
       *                        -2 if quality isn't high enough
       **/
      int set (unsigned int index,
        const std::vector <Knowledge_Record::Integer> & value,
        const Eval_Settings & settings =
          Eval_Settings (true));
       
      /**
       * Sets a knowledge variable to a specified value
       *
       * @param index           index to set
       * @param value           value to set at location
       * @param settings        settings for applying the update
       * @return                0 if successful, -1 if key is null, and
       *                        -2 if quality isn't high enough
       **/
      int set (unsigned int index, double value, 
        const Eval_Settings & settings =
          Eval_Settings (true));

      /**
       * Sets an index within an array to a specified value
       *
       * @param index           index to set within the variable
       * @param sub_index       index of the location in the array
       * @param value           value to set at location
       * @param settings        settings for applying the update
       * @return                0 if successful, -1 if key is null, and
       *                        -2 if quality isn't high enough
       **/
      int set_index (unsigned int index,
        size_t sub_index,
        double value,
        const Eval_Settings & settings =
          Eval_Settings (true));

      /**
       * Sets a knowledge variable to a specified value
       *
       * @param index           index to set
       * @param value           array of doubles to set at the location
       * @param size            number of elements in the array
       * @param settings        settings for applying the update
       * @return                0 if successful, -1 if key is null, and
       *                        -2 if quality isn't high enough
       **/
      int set (unsigned int index,
        const double * value,
        uint32_t size,
        const Eval_Settings & settings =
          Eval_Settings (true));
       
      /**
       * Sets a knowledge variable to a specified value
       *
       * @param index           index to set
       * @param value           array of doubles to set at the location
       * @param settings        settings for applying the update
       * @return                0 if successful, -1 if key is null, and
       *                        -2 if quality isn't high enough
       **/
      int set (unsigned int index,
        const std::vector <double> & value,
        const Eval_Settings & settings =
          Eval_Settings (true));
        
      /**
       * Sets a knowledge variable to a specified value
       *
       * @param index           index to set
       * @param value           value to set at location
       * @param settings        settings for applying the update
       * @return                0 if successful, -1 if key is null, and
       *                        -2 if quality isn't high enough
       **/
      int set (unsigned int index, const std::string & value, 
        const Eval_Settings & settings =
          Eval_Settings (true));

      /**
       * Sets the quality of writing to a certain variable from this entity
       *
       * @param index           index to set
       * @param quality         quality of writing to this location
       * @param settings        settings for referring to knowledge variables
       **/
      void set_quality (unsigned int index, uint32_t quality,
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
      Knowledge_Base & knowledge_;

      /**
       * Prefix of variable
       **/
      const std::string name_;

      /**
       * Values of the array
       **/
      std::vector <Variable_Reference> vector_;

      /**
       * Settings for modifications
       **/
      Eval_Settings settings_;
    };

    /// provide the Array alias for the Vector class
    typedef  Vector   Array;
  }
}




#endif // _MADARA_VECTOR_H_