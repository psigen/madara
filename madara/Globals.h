

#ifndef MADARA_GLOBALS_H
#define MADARA_GLOBALS_H

/**
 * @file Globals.h
 * @author James Edmondson <james.r.edmondson@vanderbilt.edu>
 *
 * This file contains classes and variables global to Madara services
 **/

#include <string>
#include <vector>
#include <map>

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

    /**
     * status of the knowledge record
     **/
    int status;

    /**
     * value of the knowledge record
     **/
    long long value;

    /**
     * last modification time
     **/
    unsigned long long clock;

    /**
     * scope (global or local)
     **/
    int scope;

    /**
     * priority of the update
     **/
    unsigned long quality;

    /**
     * write priority for any local updates
     **/
    unsigned long write_quality;
  };

  typedef ::std::map < ::std::string, Knowledge_Record> Knowledge_Map;
  typedef ::std::vector < ::std::string> Knowledge_Rules;
  typedef ::std::vector < ::std::string> String_Vector;

  typedef ::std::string     Knowledge_Key;
  typedef long              Knowledge_Value;
}

#endif
