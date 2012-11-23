

#ifndef _KNOWLEDGE_RECORD_H_
#define _KNOWLEDGE_RECORD_H_

/**
 * @file Knowledge_Record.h
 * @author James Edmondson <jedmondson@gmail.com>
 *
 * This file contains the Knowledge_Record class, helper typdefs and functions
 **/

#include <string>
#include <vector>
#include <map>
#include "madara/MADARA_export.h"
#include "madara/utility/stdint.h"
#include "madara/utility/Scoped_Array.h"

namespace Madara
{

  namespace Knowledge_Engine
  {
    class Thread_Safe_Context;
  }

  /**
   * @class Knowledge_Record
   * @brief This class encapsulates an entry in a Knowledge_Base
   **/

  class MADARA_Export Knowledge_Record
  {
  public:
    enum
    {
      UNCREATED = 0,
      UNMODIFIED = 1,
      MODIFIED = 2
    };

    enum
    {
      GLOBAL_SCOPE = 0,
      LOCAL_SCOPE = 1
    };

    enum
    {
      INTEGER = 0,
      STRING = 1,
      DOUBLE = 2,
      FLOAT = 2,
      FILE = 3,
      IMAGE = 4,
      XML = 5
    };
    
    typedef  int64_t     Integer;

    /**
     * status of the knowledge record
     **/
    int status;
    
    /**
     * last modification time
     **/
    uint64_t clock;

    /**
     * scope (global or local)
     **/
    int32_t scope;

    /**
     * priority of the update
     **/
    uint32_t quality;

    /**
     * write priority for any local updates
     **/
    uint32_t write_quality;

  private:
   
    /**
     * size of the value
     **/
    uint32_t size_;
    
    /**
     * type of variable (INTEGER, DOUBLE, STRING, FILE, IMAGE)
     **/
    int32_t type_;
    
    /**
     * potential integer or double value of the knowledge record
     **/
    union {
      Integer int_value_;
      double double_value_;
    };

    /**
     * potential string value of the node (size int)
     **/
    Madara::Utility::Scoped_Array <char> str_value_;

    /**
     * potential file value of the node
     **/
    Madara::Utility::Scoped_Array <unsigned char> file_value_;

  public:

    /* default constructor */
    Knowledge_Record ();
    
    /* Integer constructor */
    Knowledge_Record (Integer value);
    
    /* Double constructor */
    Knowledge_Record (double value);
    
    /* String constructor */
    Knowledge_Record (const std::string & value);
    
    /* copy constructor */
    Knowledge_Record (const Knowledge_Record & rhs);
    
    /* destructor */
    ~Knowledge_Record ();
    
    /**
     * converts the value to a string
     **/
    std::string to_string (void) const;

    /**
     * converts the value to an integer
     **/
    Integer to_integer (void) const;

    /**
     * converts the value to a float/double
     **/
    double to_double (void) const;
    
    /**
     * sets the value to a double
     * @param    new_value   new value of the Knowledge Record
     **/
    void set_value (const Integer & new_value);

    /**
     * sets the value to a string
     * @param    new_value   new value of the Knowledge Record
     **/
    void set_value (const std::string & new_value);
    
    /**
     * sets the value to a double
     * @param    new_value   new value of the Knowledge Record
     **/
    void set_value (const double & new_value);

    /**
     * resets the variable to an integer
     **/
    void reset_value (void);

    /**
     * clears any dynamic values. This method does not attempt to set
     * the value of the Knowledge Record, as clear does, and is consequently
     * more efficient.
     **/
    void clear_value (void);
    
    /**
     * returns the size of the value
     **/
    inline uint32_t size (void) const
    {
      return size_;
    }
    
    /**
     * returns the size of the value
     **/
    inline int32_t type (void) const
    {
      return type_;
    }
    
    /**
     * Less than
     **/
    Knowledge_Record operator< (const Knowledge_Record & rhs) const;
    
    /**
     * Less than or equal to
     **/
    Knowledge_Record operator<= (const Knowledge_Record & rhs) const;
    
    /**
     * Equal to
     **/
    Knowledge_Record operator== (const Knowledge_Record & rhs) const;
    
    /**
     * Equal to
     **/
    bool operator== (Integer value) const;
    
    /**
     * Equal to
     **/
    bool operator== (double value) const;
    
    /**
     * Equal to
     **/
    bool operator== (const std::string & value) const;
    
    /**
     * Unequal to
     **/
    Knowledge_Record operator!= (const Knowledge_Record & rhs) const;
    
    /**
     * Greater than
     **/
    Knowledge_Record operator> (const Knowledge_Record & rhs) const;
    
    /**
     * Greater than or equal to
     **/
    Knowledge_Record operator>= (const Knowledge_Record & rhs) const;
    
    /**
     * Logical not.
     **/
    Knowledge_Record operator! (void) const;

    /**
     * Negate.
     **/
    Knowledge_Record operator- (void) const;

    /**
     * Assignment
     **/
    Knowledge_Record operator= (const Knowledge_Record & rhs);
    
    /**
     * Atomic addition operator
     **/
    Knowledge_Record operator+= (const Knowledge_Record & rhs);
    
    /**
     * Atomic subtraction operator
     **/
    Knowledge_Record operator-= (const Knowledge_Record & rhs);
    
    /**
     * Atomic multiplication operator
     **/
    Knowledge_Record operator*= (const Knowledge_Record & rhs);
    
    /**
     * Atomic division operator
     **/
    Knowledge_Record operator/= (const Knowledge_Record & rhs);
    
    /**
     * Atomic modulus operator
     **/
    Knowledge_Record operator%= (const Knowledge_Record & rhs);
    
    /**
     * Times operator
     **/
    Knowledge_Record operator* (const Knowledge_Record & rhs) const;
    
    /**
     * Divides operator
     **/
    Knowledge_Record operator/ (const Knowledge_Record & rhs) const;
    
    /**
     * Modulus operator
     **/
    Knowledge_Record operator% (const Knowledge_Record & rhs) const;
    
    /**
     * Plus operator
     **/
    Knowledge_Record operator+ (const Knowledge_Record & rhs) const;
    
    /**
     * Minus operator
     **/
    Knowledge_Record operator- (const Knowledge_Record & rhs) const;
    
    /**
     * And operator
     **/
    Knowledge_Record operator&& (const Knowledge_Record & rhs) const;
    
    /**
     * Or operator
     **/
    Knowledge_Record operator|| (const Knowledge_Record & rhs) const;
    
    /**
     * Preincrement operator
     **/
    Knowledge_Record operator++ (void);
    
    /**
     * Predecrement operator
     **/
    Knowledge_Record operator-- (void);

    /**
     * Reads a knowledge record from a buffer encoded via the write method
     * @param  buffer   buffer to read from
     **/
    unsigned int read (const char * buffer);

    /**
     * Reads a key and knowledge record from a buffer encoded via the write
     * method.
     * @param  buffer   buffer to read
     * @param  key      the key that needs to be mapped to the record
     **/
    unsigned int read (const char * buffer, std::string & key);

    /**
     * Writes type, size, and value to a character buffer in network byte order
     * @param  buffer   buffer to write to
     * @return  the number of bytes written
     **/
    unsigned int write (char * buffer);
    
    /**
     * Writes a key as well as type, size, and value to a character buffer 
     * in network byte order
     * @param  buffer   buffer to write to
     * @param  key      a user-provided key that maps to this record
     * @return  the number of bytes written
     **/
    unsigned int write (char * buffer, const std::string & key);

    /**
     * Apply the knowledge record to a context, given some quality and clock
     **/
    int apply (Madara::Knowledge_Engine::Thread_Safe_Context & context,
               const std::string & key, unsigned int quality, uint64_t clock,
               bool perform_lock);

    /**
     * Checks to see if the record is true. For integers and doubles, true
     * means the value is non-zero. For strings, it means the value is non "";
     * @return  true if the record is non-zero or not-"". False otherwise.
     **/
    bool is_true (void) const;

    /**
     * Checks to see if the record is false. For integers and boules, false
     * means the value is zero. For strings, it means the value is "".
     * @return  true if the record is zero or "". False otherwise.
     **/
    bool is_false (void) const;
  };
  
  typedef ::std::map < std::string, Knowledge_Record>   Knowledge_Map;
  typedef ::std::vector < std::string>                  Knowledge_Rules;
  typedef ::std::vector < std::string>                  String_Vector;
  typedef ::std::map < std::string, Knowledge_Record *> Knowledge_Records;

  typedef std::string     Knowledge_Key;
  typedef Knowledge_Record              Knowledge_Value;
  
  /**
   * Returns the maximum quality within the records
   * @param    records     the list of records to gauge quality of
   * @return   the maximum quality within the list of records
   **/
  inline uint32_t max_quality (const Knowledge_Records & records)
  {
    uint32_t max = 0;

    // iterate over the list and return the max
    for (Knowledge_Records::const_iterator i = records.begin ();
         i != records.end (); ++i)
    {
      max = std::max <uint32_t> (i->second->quality, max);
    }
    return max;
  }

}


/**
  * output stream buffering
  **/
MADARA_Export std::ostream & operator<< (std::ostream & stream,
  const Madara::Knowledge_Record & rhs);
  


#endif  // _KNOWLEDGE_RECORD_H_
