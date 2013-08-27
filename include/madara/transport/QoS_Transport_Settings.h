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
#include "madara/transport/Transport_Context.h"
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
       * @param   name    variable name of input ("" for unnamed)
       * @param   input   the argument to the filter chain
       * @param   context the context of the transport
       * @return  the result of filtering the input
       **/
      Knowledge_Record filter_send (
        const Madara::Knowledge_Record & input,
        const std::string & name,
        Transport::Transport_Context & context) const;
         
      /**
       * Filters an input according to receive's filter chain
       * @param   name    variable name of input ("" for unnamed)
       * @param   input   the argument to the filter chain
       * @param   context the context of the transport
       * @return  the result of filtering the input
       **/
      Knowledge_Record filter_receive (
        const Madara::Knowledge_Record & input,
        const std::string & name,
        Transport::Transport_Context & context) const;
      
      /**
       * Filters an input according to rebroadcast's filter chain
       * @param   name    variable name of input ("" for unnamed)
       * @param   input   the argument to the filter chain
       * @param   context the context of the transport
       * @return  the result of filtering the input
       **/
      Knowledge_Record filter_rebroadcast (
        const Madara::Knowledge_Record & input,
        const std::string & name,
        Transport::Transport_Context & context) const;

       
      /**
       * Prints the number of filters chained for each type to the
       * send filter
       **/
      void print_num_filters_send (void) const;
       
      /**
       * Prints the number of filters chained for each type to the
       * receive filter
       **/
      void print_num_filters_receive (void) const;
       
      /**
       * Prints the number of filters chained for each type to the
       * rebroadcast filter
       **/
      void print_num_filters_rebroadcast (void) const;

      /**
       * Sets the time to live for our packets. This number is decremented
       * by one by each receiver. If time to live is greater than 0, the
       * message will be rebroadcasted. Default is 0.
       * @param  ttl   time-to-live for message sent over this transport
       **/
      void set_rebroadcast_ttl (unsigned char ttl);

      /**
       * Gets the time to live for rebroadcasting (number of rebroadcasts
       * per message).
       **/
      unsigned char get_rebroadcast_ttl (void) const;
      
      /**
       * Returns the number of types that are filtered before send
       * @return  the number of types that have filters
       **/
      size_t get_number_of_send_filtered_types (void) const;
      
      /**
       * Returns the number of types that are filtered before rebroadcast
       * @return  the number of types that have filters
       **/
      size_t get_number_of_rebroadcast_filtered_types (void) const;

      /**
       * Returns the number of types that are filtered after received
       * @return  the number of types that have filters
       **/
      size_t get_number_of_received_filtered_types (void) const;

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

      /**
       * Enables rebroadcast support up to a certain time to live for other
       * agent's messages. Default is 0 (no participation in rebroadcasts)
       * @param   maximum_ttl   Maximum time to live to support (0 for not
       *                        participating in rebroadcasts). A transport
       *                        cannot participate in more than 255 resends.
       **/
      void enable_participant_ttl (unsigned char maximum_ttl = 255);

      /**
       * Returns the maximum time to live participation of this transport in
       * rebroadcasting of other agent's messages.
       * @return                the maximum number of ttls this transport will
       *                        participate in (per message)
       **/
      unsigned char get_participant_ttl (void) const;
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
      unsigned char rebroadcast_ttl_;

      /**
       * This field is meant to limit the number of rebroadcasts that this
       * transport will participate in. If a ttl > 0 is received on a
       * message header, the new ttl for rebroadcasts will be set to the
       * minimum of this field and the decrement of the ttl of the message.
       * A zero here means the transport will not participate in rebroadcasts
       **/
      unsigned char participant_rebroadcast_ttl_;

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