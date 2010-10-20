

#ifndef MADARA_GLOBALS_H
#define MADARA_GLOBALS_H

/**
 * @file Globals.h
 * @author James Edmondson <james.r.edmondson@vanderbilt.edu>
 *
 * This file contains classes and variables global to Madara services
 */

#include <string>
#include <vector>
#include <map>

namespace Madara
{

  /**
   * @class Knowledge_Record
   * @brief This class encapsulates an entry in a Knowledge_Base
   */

  class Knowledge_Record
  {
  public:
    enum
    {
      // note that these should be logically or'd together
      // SET | MODIFIED = 3
      // UNSET | MODIFIED = 2
      // 
      UNMODIFIED = 0,
      MODIFIED = 1
    };

    enum
    {
      GLOBAL_SCOPE = 0,
      LOCAL_SCOPE = 1
    };

    /* default constructor */
    Knowledge_Record ()
      : status (UNMODIFIED), value (UNMODIFIED), scope (LOCAL_SCOPE) {}

    int status;
    long value;
    long clock;
    int scope;
  };

  typedef ::std::map < ::std::string, Knowledge_Record> Knowledge_Map;
  typedef ::std::vector < ::std::string> Knowledge_Rules;
  typedef ::std::vector < ::std::string> String_Vector;

  typedef ::std::string     Knowledge_Key;
  typedef long              Knowledge_Value;
}

#endif
