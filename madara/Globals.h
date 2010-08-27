

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
      // SET | BLOCKING = 3
      // UNSET | BLOCKING = 2
      // 
      UNSET = 0,
      SET = 1,
      BLOCKING = 2
    };

    /* default constructor */
    Knowledge_Record ()
      : status (UNSET), value (UNSET) {}

    int status;
    long value;
  };

  typedef ::std::map <::std::string, Knowledge_Record> Knowledge_Map;
  typedef ::std::vector <::std::string> Knowledge_Rules;

  typedef ::std::string     Knowledge_Key;
  typedef long              Knowledge_Value;
}

#endif