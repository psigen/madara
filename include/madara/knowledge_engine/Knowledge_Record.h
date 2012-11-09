

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
#include <stdint.h>

namespace Madara
{

  /**
   * @class Knowledge_Record
   * @brief This class encapsulates an entry in a Knowledge_Base
   **/

  class Knowledge_Record
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

    /* default constructor */
    Knowledge_Record ()
      : status (UNCREATED), value (0), clock (0), scope (LOCAL_SCOPE),
        quality (0), write_quality (0) 
    {}

    typedef  int64_t      VALUE_TYPE;
    
    static bool lt (VALUE_TYPE lhs, VALUE_TYPE rhs);

    static bool lte (VALUE_TYPE lhs, VALUE_TYPE rhs);

    static bool eq (VALUE_TYPE lhs, VALUE_TYPE rhs);

    static bool gt (VALUE_TYPE lhs, VALUE_TYPE rhs);

    static bool gte (VALUE_TYPE lhs, VALUE_TYPE rhs);

    /**
     * status of the knowledge record
     **/
    int status;

    /**
     * value of the knowledge record
     **/
    VALUE_TYPE value;

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
  };

  typedef ::std::map < ::std::string, Knowledge_Record> Knowledge_Map;
  typedef ::std::vector < ::std::string> Knowledge_Rules;
  typedef ::std::vector < ::std::string> String_Vector;
  typedef ::std::map < ::std::string, Knowledge_Record *> Knowledge_Records;

  typedef ::std::string     Knowledge_Key;
  typedef long              Knowledge_Value;
  
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




#endif  // _KNOWLEDGE_RECORD_H_
