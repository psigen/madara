#ifndef _QOS_TRANSPORT_SETTINGS_H_
#define _QOS_TRANSPORT_SETTINGS_H_

/**
 * @file QoS_Transport_Settings.h
 * @author James Edmondson <jedmondson@gmail.com>
 *
 * This file contains the QoS_Transport_Settings class, which provides a
 * settings container for quality of service parameters for transport classes
 **/

#include <string>

#include "madara/transport/Transport_Settings.h"
#include "madara/knowledge_engine/Thread_Safe_Context.h"
#include "madara/utility/stdint.h"
#include "madara/MADARA_export.h"
#include "madara/knowledge_engine/Knowledge_Record_Filters.h"

namespace Madara
{
  namespace Transport
  {
    /**
     * @class QoS_Transport_Settings
     * @brief Container for quality-of-service settings
     **/
    class MADARA_Export QoS_Transport_Settings : public Settings
    {
    public:
      /**
       * Default constructor
       **/
      QoS_Transport_Settings ();

      /**
       * Copy constructor
       **/
      QoS_Transport_Settings (const QoS_Transport_Settings & settings);
      
      /**
       * Copy constructor
       **/
      QoS_Transport_Settings (const Settings & settings);

      /**
       * Destructor
       **/
      virtual ~QoS_Transport_Settings ();
       
      /**
       * Assignment operator
       **/
      void operator= (const QoS_Transport_Settings & settings);
      
      /**
       * Assignment operator
       **/
      void operator= (const Settings & settings);

      /**
       * Adds a type to the acceptable rebroadcast types. @see
       * Madara::Knowledge_Record for list of types.
       * @param   type    type to add to the mask
       * @return          resulting mask
       **/
      uint32_t add_rebroadcastable_type (uint32_t type);
      
      /**
       * Adds a filter that will be applied to certain types before sending
       * @param   types      the types to add the filter to
       * @param   function   the function that will take the knowledge record
       *                     in Function_Arguments.
       **/
      void add_send_filter (uint32_t types,
        Knowledge_Record (*function) (Knowledge_Engine::Function_Arguments &,
                                      Knowledge_Engine::Variables &));
       
      /**
       * Adds a filter that will be applied to certain types after receiving
       * and before applying to the local knowledge base
       * @param   types      the types to add the filter to
       * @param   function   the function that will take the knowledge record
       *                     in Function_Arguments.
       **/
      void add_receive_filter (uint32_t types,
        Knowledge_Record (*function) (Knowledge_Engine::Function_Arguments &,
                                      Knowledge_Engine::Variables &));
       
      /**
       * Adds a filter that will be applied to certain types after receiving
       * and before rebroadcasting (if TTL > 0)
       * @param   types      the types to add the filter to
       * @param   function   the function that will take the knowledge record
       *                     in Function_Arguments.
       **/
      void add_rebroadcast_filter (uint32_t types,
        Knowledge_Record (*function) (Knowledge_Engine::Function_Arguments &,
                                      Knowledge_Engine::Variables &));
      
      /**
       * Attaches a context to the various filtering systems. If the context
       * ever goes out of scope, a 0 should be passed into this function to
       * the context.
       * @param   context     context to be used for Variable lookups
       **/
      void attach (Knowledge_Engine::Thread_Safe_Context * context);
      
      /**
       * Clears the list of filters for the specified types
       * @param   types   the types to clear the filters of
       **/
      void clear_send_filters (uint32_t types);

      /**
       * Clears the list of filters for the specified types
       * @param   types   the types to clear the filters of
       **/
      void clear_receive_filters (uint32_t types);

      /**
       * Clears the list of filters for the specified types
       * @param   types   the types to clear the filters of
       **/
      void clear_rebroadcast_filters (uint32_t types);

      
      /**
       * Filters an input according to send's filter chain
       * @param   input   the argument to the filter chain
       * @return  the result of filtering the input
       **/
      Knowledge_Record filter_send (const Knowledge_Record & input);
         
      /**
       * Filters an input according to receive's filter chain
       * @param   input   the argument to the filter chain
       * @return  the result of filtering the input
       **/
      Knowledge_Record filter_receive (const Knowledge_Record & input);
      
      /**
       * Filters an input according to rebroadcast's filter chain
       * @param   input   the argument to the filter chain
       * @return  the result of filtering the input
       **/
      Knowledge_Record filter_rebroadcast (const Knowledge_Record & input);

       
      /**
       * Prints the number of filters chained for each type to the
       * send filter
       **/
      void print_num_filters_send (void);
       
      /**
       * Prints the number of filters chained for each type to the
       * receive filter
       **/
      void print_num_filters_receive (void);
       
      /**
       * Prints the number of filters chained for each type to the
       * rebroadcast filter
       **/
      void print_num_filters_rebroadcast (void);


      /**
       * Removes a type from the acceptable rebroadcast types. @see
       * Madara::Knowledge_Record for list of types.
       * @param   type    type to remove from the mask
       * @return          resulting mask
       **/
      uint32_t remove_rebroadcastable_type (uint32_t type);

      /**
       * Checks if type is included in rebroadcastable types.
       **/
      bool is_rebroadcastable_type (uint32_t type);

      /**
       * Sets the time to live for rebroadcasting. This number is decremented
       * by one by each receiver. If time to live is greater than 0, the
       * message will be rebroadcasted. Default is 0.
       **/
      uint32_t set_rebroadcast_ttl (uint32_t ttl);

      /**
       * Gets the time to live for rebroadcasting (number of rebroadcasts
       * per message).
       **/
      uint32_t get_rebroadcast_ttl (void);
      
      /**
       * Returns the number of types that are filtered before send
       * @return  the number of types that have filters
       **/
      size_t get_number_of_send_filtered_types (void);
      
      /**
       * Returns the number of types that are filtered before rebroadcast
       * @return  the number of types that have filters
       **/
      size_t get_number_of_rebroadcast_filtered_types (void);

      /**
       * Returns the number of types that are filtered after received
       * @return  the number of types that have filters
       **/
      size_t get_number_of_received_filtered_types (void);

      /**
       * Adds a trusted peer. By default, all peers are trusted unless
       * a trusted peer or banned peer is added to the settings.
       * @param   peer    peer to add (generally ip:port)
       **/
      void add_trusted_peer (const std::string & peer);

      /**
       * Adds a banned peer. By default, all peers are trusted unless
       * a trusted peer or banned peer is added to the settings.
       * @param   peer    peer to add (generally ip:port)
       **/
      void add_banned_peer (const std::string & peer);
      
      /**
       * Removes a trusted peer, if it exists in the list.
       * @param   peer    peer to remove (generally ip:port)
       * @return  true if peer existed, false if peer was not in list
       **/
      bool remove_trusted_peer (const std::string & peer);

      /**
       * Removes a trusted peer, if it exists in the list.
       * @param   peer    peer to remove (generally ip:port)
       * @return  true if peer existed, false if peer was not in list
       **/
      bool remove_banned_peer (const std::string & peer);

      /**
       * Checks if a peer is trusted.
       * @param   peer    peer to check (generally ip:port)
       * @return true if peer is trusted, false otherwise
       **/
      bool is_trusted (const std::string & peer) const;
    private:
       
      /**
       * number of rebroadcasts for receivers to ultimately do. This field
       * is experimental and is meant to allow for rebroadcasting messages
       * across a number of loosely connected agents or amongst a routing
       * network. For instance, if A is connected to B and B is connected
       * to C, and A would like to have its update known to C, then a
       * rebroadcast_ttl of 1 would instruct B to rebroadcast updates
       * with a ttl of (rebroadcast_ttl - 1), resulting in 0 ttl when
       * received by C (it will not rebroadcast).
       **/
      uint32_t rebroadcast_ttl_;

      /**
       * A mask for the acceptable types for a rebroadcast. The types are
       * now all powers of 2 (@see Knowledge_Record), so we can logically
       * or types together to get an acceptable mask.
       **/
      uint32_t rebroadcast_types_;

      /**
       * A container of all trusted peers
       **/
      std::map <std::string, int> trusted_peers_;
      
      /**
       * A container of all banned peers
       **/
      std::map <std::string, int> banned_peers_;

      /**
       * A container for rebroadcast filters
       **/
      Knowledge_Engine::Knowledge_Record_Filters  rebroadcast_filters_;
         
      /**
       * A container for receive filters
       **/
      Knowledge_Engine::Knowledge_Record_Filters  receive_filters_;

      /**
       * A container for filters applied before sending from this host
       **/
      Knowledge_Engine::Knowledge_Record_Filters  send_filters_;

    };
  } // end Transport namespace
} // end Madara namespace

#endif