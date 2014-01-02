
#ifndef _MADARA_VECTOR_N_H_
#define _MADARA_VECTOR_N_H_

#include <vector>
#include <string>
#include "madara/knowledge_engine/Knowledge_Base.h"
#include "madara/knowledge_engine/Eval_Settings.h"

/**
 * @file Vector_N.h
 * @author James Edmondson <jedmondson@gmail.com>
 *
 * This file contains a C++ object that manages interactions for a
 * multidimensional array of variables
 **/

namespace Madara
{
  namespace Knowledge_Engine
  {
    /**
     * @class Vector_N
     * @brief This class stores an n-dimensional KaRL record array. The
     *        array represents accessing elements in a manner similar to
     *        int my_array [0][1][2][...][N].
     */
    class MADARA_Export Vector_N
    {
    public:
      /// A vector of unsigned ints is the supported indexing method
      typedef  std::vector <unsigned int>   Index;

      /**
       * Default constructor
       * @param  name         name of the vector in the knowledge base
       * @param  knowledge    the knowledge base that will contain the vector
       * @param  settings     settings for evaluating the vector
       **/
      Vector_N (const std::string & name,
              Knowledge_Base & knowledge,
              const Eval_Settings & settings = Eval_Settings (true));
      
      /**
       * Copy constructor
       **/
      Vector_N (const Vector_N & rhs);

      /**
       * Destructor
       **/
      ~Vector_N ();

      /**
       * Returns the name of the vector
       * @return name of the vector
       **/
      std::string get_name (void);

      /**
       * Retrieves a copy of the record from the map.
       * @param  index  the index of the variable entry. Type Index is
       *                a typdef of a STL vector of unsigned ints, so
       *                fill this vector with location information. For
       *                instance, to access array[1][5][8], you could
       *                pass an index initialized in the following way:<br />
       *                Index index;<br />
       *                index.push_back (1);<br />
       *                index.push_back (5);<br />
       *                index.push_back (8);
       * @return the value of the entry. Modifications to this will
       *         not be reflected in the context. This is a local copy.
       **/
      Knowledge_Record operator[] (const Index & index);
      
      
      /**
       * Read a file into the index
       * @param filename           file to read
       * @param  index  the index of the variable entry. Type Index is
       *                a typdef of a STL vector of unsigned ints, so
       *                fill this vector with location information. For
       *                instance, to access array[1][5][8], you could
       *                pass an index initialized in the following way:<br />
       *                Index index;<br />
       *                index.push_back (1);<br />
       *                index.push_back (5);<br />
       *                index.push_back (8);
       * @param settings           settings to use when evaluating/updating
       */
      int read_file (const Index & index, 
                     const std::string & filename, 
        const Eval_Settings & settings =
          Eval_Settings (true));
      
      /**
       * Atomically sets the value of an index to an arbitrary string.
       * @param  index  the index of the variable entry. Type Index is
       *                a typdef of a STL vector of unsigned ints, so
       *                fill this vector with location information. For
       *                instance, to access array[1][5][8], you could
       *                pass an index initialized in the following way:<br />
       *                Index index;<br />
       *                index.push_back (1);<br />
       *                index.push_back (5);<br />
       *                index.push_back (8);
       * @param   value     new value of the variable
       * @param   size      indicates the size of the value buffer
       * @param   settings  settings for applying the update
       * @return   0 if the value was set. -1 if null key
       **/
      int set_file (const Index & index,
        const unsigned char * value, size_t size, 
        const Eval_Settings & settings =
          Eval_Settings (true));
      
      /**
       * Atomically sets the value of an index to a JPEG image
       * @param  index  the index of the variable entry. Type Index is
       *                a typdef of a STL vector of unsigned ints, so
       *                fill this vector with location information. For
       *                instance, to access array[1][5][8], you could
       *                pass an index initialized in the following way:<br />
       *                Index index;<br />
       *                index.push_back (1);<br />
       *                index.push_back (5);<br />
       *                index.push_back (8);
       * @param   value     new value of the variable
       * @param   size      indicates the size of the value buffer
       * @param   settings  settings for applying the update
       * @return   0 if the value was set. -1 if null key
       **/
      int set_jpeg (const Index & index,
        const unsigned char * value, size_t size, 
        const Eval_Settings & settings =
          Eval_Settings (true));
      
      
      /**
       * Sets a knowledge variable to a specified value
       *
       * @param  index  the index of the variable entry. Type Index is
       *                a typdef of a STL vector of unsigned ints, so
       *                fill this vector with location information. For
       *                instance, to access array[1][5][8], you could
       *                pass an index initialized in the following way:<br />
       *                Index index;<br />
       *                index.push_back (1);<br />
       *                index.push_back (5);<br />
       *                index.push_back (8);
       * @param value           value to set at location
       * @param settings        settings for applying the update
       * @return                0 if successful, -1 if key is null, and
       *                        -2 if quality isn't high enough
       **/
      int set (const Index & index,
        Madara::Knowledge_Record::Integer value = 
          Madara::Knowledge_Record::MODIFIED, 
        const Eval_Settings & settings =
          Eval_Settings (true));

      /**
       * Sets an index within an array to a specified value
       *
       * @param  index  the index of the variable entry. Type Index is
       *                a typdef of a STL vector of unsigned ints, so
       *                fill this vector with location information. For
       *                instance, to access array[1][5][8], you could
       *                pass an index initialized in the following way:<br />
       *                Index index;<br />
       *                index.push_back (1);<br />
       *                index.push_back (5);<br />
       *                index.push_back (8);
       * @param sub_index       index of the location in the array
       * @param value           value to set at location
       * @param settings        settings for applying the update
       * @return                0 if successful, -1 if key is null, and
       *                        -2 if quality isn't high enough
       **/
      int set_index (const Index & index,
        size_t sub_index,
        Madara::Knowledge_Record::Integer value,
        const Eval_Settings & settings =
          Eval_Settings (true));

      /**
       * Sets a knowledge variable to a specified value
       *
       * @param  index  the index of the variable entry. Type Index is
       *                a typdef of a STL vector of unsigned ints, so
       *                fill this vector with location information. For
       *                instance, to access array[1][5][8], you could
       *                pass an index initialized in the following way:<br />
       *                Index index;<br />
       *                index.push_back (1);<br />
       *                index.push_back (5);<br />
       *                index.push_back (8);
       * @param value           array of integers to set at the location
       * @param size            number of elements in the array
       * @param settings        settings for applying the update
       * @return                0 if successful, -1 if key is null, and
       *                        -2 if quality isn't high enough
       **/
      int set (const Index & index,
        const Madara::Knowledge_Record::Integer * value,
        uint32_t size,
        const Eval_Settings & settings =
          Eval_Settings (true));
       
      /**
       * Sets a knowledge variable to a specified value
       *
       * @param  index  the index of the variable entry. Type Index is
       *                a typdef of a STL vector of unsigned ints, so
       *                fill this vector with location information. For
       *                instance, to access array[1][5][8], you could
       *                pass an index initialized in the following way:<br />
       *                Index index;<br />
       *                index.push_back (1);<br />
       *                index.push_back (5);<br />
       *                index.push_back (8);
       * @param value           array of integers to set at the location
       * @param settings        settings for applying the update
       * @return                0 if successful, -1 if key is null, and
       *                        -2 if quality isn't high enough
       **/
      int set (const Index & index,
        const std::vector <Knowledge_Record::Integer> & value,
        const Eval_Settings & settings =
          Eval_Settings (true));
       
      /**
       * Sets a knowledge variable to a specified value
       *
       * @param  index  the index of the variable entry. Type Index is
       *                a typdef of a STL vector of unsigned ints, so
       *                fill this vector with location information. For
       *                instance, to access array[1][5][8], you could
       *                pass an index initialized in the following way:<br />
       *                Index index;<br />
       *                index.push_back (1);<br />
       *                index.push_back (5);<br />
       *                index.push_back (8);
       * @param value           value to set at location
       * @param settings        settings for applying the update
       * @return                0 if successful, -1 if key is null, and
       *                        -2 if quality isn't high enough
       **/
      int set (const Index & index, double value, 
        const Eval_Settings & settings =
          Eval_Settings (true));

      /**
       * Sets an index within an array to a specified value
       *
       * @param  index  the index of the variable entry. Type Index is
       *                a typdef of a STL vector of unsigned ints, so
       *                fill this vector with location information. For
       *                instance, to access array[1][5][8], you could
       *                pass an index initialized in the following way:<br />
       *                Index index;<br />
       *                index.push_back (1);<br />
       *                index.push_back (5);<br />
       *                index.push_back (8);
       * @param sub_index       index of the location in the array
       * @param value           value to set at location
       * @param settings        settings for applying the update
       * @return                0 if successful, -1 if key is null, and
       *                        -2 if quality isn't high enough
       **/
      int set_index (const Index & index,
        size_t sub_index,
        double value,
        const Eval_Settings & settings =
          Eval_Settings (true));

      /**
       * Sets a knowledge variable to a specified value
       *
       * @param  index  the index of the variable entry. Type Index is
       *                a typdef of a STL vector of unsigned ints, so
       *                fill this vector with location information. For
       *                instance, to access array[1][5][8], you could
       *                pass an index initialized in the following way:<br />
       *                Index index;<br />
       *                index.push_back (1);<br />
       *                index.push_back (5);<br />
       *                index.push_back (8);
       * @param value           array of doubles to set at the location
       * @param size            number of elements in the array
       * @param settings        settings for applying the update
       * @return                0 if successful, -1 if key is null, and
       *                        -2 if quality isn't high enough
       **/
      int set (const Index & index,
        const double * value,
        uint32_t size,
        const Eval_Settings & settings =
          Eval_Settings (true));
       
      /**
       * Sets a knowledge variable to a specified value
       *
       * @param  index  the index of the variable entry. Type Index is
       *                a typdef of a STL vector of unsigned ints, so
       *                fill this vector with location information. For
       *                instance, to access array[1][5][8], you could
       *                pass an index initialized in the following way:<br />
       *                Index index;<br />
       *                index.push_back (1);<br />
       *                index.push_back (5);<br />
       *                index.push_back (8);
       * @param value           array of doubles to set at the location
       * @param settings        settings for applying the update
       * @return                0 if successful, -1 if key is null, and
       *                        -2 if quality isn't high enough
       **/
      int set (const Index & index,
        const std::vector <double> & value,
        const Eval_Settings & settings =
          Eval_Settings (true));
        
      /**
       * Sets a knowledge variable to a specified value
       *
       * @param  index  the index of the variable entry. Type Index is
       *                a typdef of a STL vector of unsigned ints, so
       *                fill this vector with location information. For
       *                instance, to access array[1][5][8], you could
       *                pass an index initialized in the following way:<br />
       *                Index index;<br />
       *                index.push_back (1);<br />
       *                index.push_back (5);<br />
       *                index.push_back (8);
       * @param value           value to set at location
       * @param settings        settings for applying the update
       * @return                0 if successful, -1 if key is null, and
       *                        -2 if quality isn't high enough
       **/
      int set (const Index & index, const std::string & value, 
        const Eval_Settings & settings =
          Eval_Settings (true));

      /**
       * Sets the quality of writing to a certain variable from this entity
       *
       * @param  index  the index of the variable entry. Type Index is
       *                a typdef of a STL vector of unsigned ints, so
       *                fill this vector with location information. For
       *                instance, to access array[1][5][8], you could
       *                pass an index initialized in the following way:<br />
       *                Index index;<br />
       *                index.push_back (1);<br />
       *                index.push_back (5);<br />
       *                index.push_back (8);
       * @param quality         quality of writing to this location
       * @param settings        settings for referring to knowledge variables
       **/
      void set_quality (const Index & index, uint32_t quality,
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

    /// provide the Array_N alias for the Vector_N class
    typedef  Vector_N   Array_N;
  }
}




#endif // _MADARA_VECTOR_N_H_