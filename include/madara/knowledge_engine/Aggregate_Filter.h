
#ifndef _MADARA_AGGREGATE_FILTERS_H_
#define _MADARA_AGGREGATE_FILTERS_H_

#include <list>
#include <string>
#include "madara/MADARA_export.h"
#include "madara/knowledge_engine/Knowledge_Record.h"
#include "madara/knowledge_engine/Extern_Function_Variables.h"
#include "madara/transport/Transport_Context.h"

/**
 * @file Aggregate_Filter.h
 * @author James Edmondson <jedmondson@gmail.com>
 *
 * This file contains declarations for declaring external functions that
 * are accessible by the transport layer to do aggregate update filtering
 **/

namespace Madara
{
  namespace Knowledge_Engine
  {
    /**
     * @class Aggregate_Filter
     * @brief This class stores a function definition
     */
    class MADARA_Export Aggregate_Filter
    {
    public:
      /**
       * Default constructor
       **/
      Aggregate_Filter ()
        : filter (0)
      {
      }

      /**
       * Constructor for function pointer
       **/
      Aggregate_Filter (Knowledge_Record (*extern_func) (
        Knowledge_Map &, const Transport::Transport_Context &,
        Variables &))
        : filter (extern_func)
      {
      }
      
      /// mapped function call for aggregate filter
      Knowledge_Record (*filter) (
        Knowledge_Map &, const Transport::Transport_Context &,
        Variables &);
    };

    /**
     * Typedef for a list of aggregate filters
     **/
    typedef   std::list <Aggregate_Filter>    Aggregate_Filters;
  }
}




#endif // _MADARA_AGGREGATE_FILTERS_H_