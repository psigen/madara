

#ifndef _KNOWLEDGE_RECORD_FILTERS_H_
#define _KNOWLEDGE_RECORD_FILTERS_H_

/**
 * @file Knowledge_Record_Filters.h
 * @author James Edmondson <jedmondson@gmail.com>
 *
 * This file contains the Knowledge_Record_Filters class, helper typdefs and functions
 **/

#include <string>
#include <vector>
#include <map>
#include <list>
#include "madara/knowledge_engine/Knowledge_Record.h"
#include "madara/knowledge_engine/Functions.h"
#include "madara/knowledge_engine/Thread_Safe_Context.h"
#include "madara/knowledge_engine/Knowledge_Record_Filters.h"
#include "madara/transport/Transport_Context.h"
#include "madara/utility/stdint.h"
#include "madara/MADARA_export.h"

namespace Madara
{
  namespace Knowledge_Engine
  {
    /// a chain of filters
    typedef  std::list <Function>              Filter_Chain;

    /// a map of types to filter chain
    typedef  std::map <uint32_t, Filter_Chain>  Filter_Map;
    
    /**
     * @class Knowledge_Record_Filters
     * @brief Provides map of data types to a filter chain to apply to the data
     */
    class MADARA_Export Knowledge_Record_Filters
    {
    public:
      /**
       * Default constructor
       **/
      Knowledge_Record_Filters ();
      
      /**
       * Copy constructor
       **/
      Knowledge_Record_Filters (const Knowledge_Record_Filters & filters);

      /**
       * Destructor
       **/
      virtual ~Knowledge_Record_Filters ();

      /**
       * Assignment operator
       * @param  rhs   the value to be copied into this class
       **/
      void operator= (const Knowledge_Record_Filters & rhs);

      /**
       * Adds a filter to the list of types
       * @param   types      the types to add the filter to
       * @param   function   the function that will take the knowledge record
       *                     in Function_Arguments.
       **/
      void add (uint32_t types,
        Knowledge_Record (*function) (Function_Arguments &, Variables &));

      /**
       * Attaches a context. If the context ever goes out of scope,
       * a 0 should be passed into this function to reset the context.
       * @param   context     context to be used for Variable lookups
       **/
      void attach (Thread_Safe_Context * context);

      /**
       * Clears the list of filters for the specified types
       * @param   types   the types to clear the filters of
       **/
      void clear (uint32_t types);

      /**
       * Filters an input according to its filter chain.
       *
       * <p>The arguments passed to the filter are the following:</p>
       *
       * args[0] : The knowledge record that the filter is acting upon<br />
       * args[1] : The name of the knowledge record, if applicable ("" if unnamed)<br />
       * args[2] : The type of operation calling the filter (integer valued).<br />
       *         : IDLE_OPERATION (should never see)<br />
       *         : SENDING_OPERATION (transport is about to send the record)<br />
       *         : RECEIVING_OPERATION (transport has received the record and is
       *         :                      about to apply the update)<br />
       *         : REBROADCASTING_OPERATION (transport is about to rebroadcast the
       *                                     record -- only happens if rebroadcast
       *                                     is enabled in Transport Settings)<br />
       * args[3] : Bandwidth used while sending through this transport<br />
       * args[4] : Bandwidth used while receiving from this transport<br />
       * args[5] : Message timestamp (when the message was originally sent)<br />
       * args[6] : Current timestamp (the result of time (NULL))<br />
       * 
       * @param   input   the argument to the filter chain
       * @param   name    variable name ("" if unnamed)
       * @param   context the context of the transport
       * @return  the result of filtering the input
       **/
      Knowledge_Record filter (const Knowledge_Record & input,
        const std::string & name,
        Transport::Transport_Context & context) const;

      /**
       * Prints the number of filters chained for each type
       **/
      void print_num_filters (void) const;

      /**
       * Returns the number of types that have filters
       * @return  the number of types that have filters
       **/
      size_t get_number_of_filtered_types (void) const;

    protected:
      /**
       * Container for mapping types to filter chains
       **/
      Filter_Map  filters_;

      /**
       * Context used by this filter
       **/
      Thread_Safe_Context * context_;
    };
  }
}

#endif   // _KNOWLEDGE_RECORD_FILTERS_H_