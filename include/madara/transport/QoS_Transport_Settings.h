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
#include "madara/filters/Aggregate_Filter.h"
#include "madara/filters/Record_Filter.h"
#include "madara/knowledge_engine/Knowledge_Record_Filters.h"

#ifdef _MADARA_JAVA_
  #include <jni.h>
#endif

#ifdef _MADARA_PYTHON_CALLBACKS_
  #include <boost/python.hpp>
#endif


namespace Madara
{
  namespace Transport
  {
    /**
     * Enumeration for packet drop policy types
     **/
    enum PacketDropType {
      PACKET_DROP_DETERMINISTIC,
      PACKET_DROP_PROBABLISTIC
    };

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
       * Adds a filter that will be applied to certain types before sending
       * @param   types      the types to add the filter to
       * @param   filter     an instance of an individual record filter that
       *                     will be managed by the underlying infrastructure
       **/
      void add_send_filter (uint32_t types,
        Filters::Record_Filter * filter);
      
      /**
       * Adds an aggregate update filter that will be applied before sending,
       * after individual record filters.
       * @param   function  an aggregate update filter
       **/
      void add_send_filter (Knowledge_Record (*function) (
        Knowledge_Map &, const Transport::Transport_Context &,
        Knowledge_Engine::Variables &));
      
      /**
       * Adds an aggregate update filter that will be applied before sending,
       * after individual record filters.
       * @param   filter     an instance of an aggregate record filter that
       *                     will be managed by the underlying infrastructure
       **/
      void add_send_filter (Filters::Aggregate_Filter * filter);
      
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
       * @param   types      the types to add the filter to
       * @param   filter     an instance of an individual record filter that
       *                     will be managed by the underlying infrastructure
       **/
      void add_receive_filter (uint32_t types,
        Filters::Record_Filter * filter);
      
      /**
       * Adds an aggregate update filter that will be applied after receiving,
       * after individual record filters.
       * @param   function  an aggregate update filter
       **/
      void add_receive_filter (Knowledge_Record (*function) (
        Knowledge_Map &, const Transport::Transport_Context &,
        Knowledge_Engine::Variables &));
 
      /**
       * Adds an aggregate update filter that will be applied after receiving,
       * after individual record filters.
       * @param   filter     an instance of an aggregate record filter that
       *                     will be managed by the underlying infrastructure
       **/
      void add_receive_filter (Filters::Aggregate_Filter * filter);
      
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
       * Adds a filter that will be applied to certain types after receiving
       * and before rebroadcasting (if TTL > 0)
       * @param   types      the types to add the filter to
       * @param   filter     an instance of an individual record filter that
       *                     will be managed by the underlying infrastructure
       **/
      void add_rebroadcast_filter (uint32_t types,
        Filters::Record_Filter * filter);
      
      /**
       * Adds an aggregate update filter that will be applied before
       * rebroadcasting, after individual record filters.
       * @param   function  an aggregate update filter
       **/
      void add_rebroadcast_filter (Knowledge_Record (*function) (
        Knowledge_Map &, const Transport::Transport_Context &,
        Knowledge_Engine::Variables &));
 
      /**
       * Adds an aggregate update filter that will be applied before
       * rebroadcasting, after individual record filters.
       * @param   filter     an instance of an aggregate record filter that
       *                     will be managed by the underlying infrastructure
       **/
      void add_rebroadcast_filter (Filters::Aggregate_Filter * filter);
      
#ifdef _MADARA_JAVA_
      
      /**
       * Adds a filter that will be applied to certain types after receiving
       * and before applying updates to the Knowledge_Base
       * @param   types      the types to add the filter to
       * @param   object     a java callback that takes in a list of args
       *                     and the Variables interface.
       **/
      void add_receive_filter (
                               uint32_t types, jobject & object);
      
      /**
       * Adds a filter that will be applied to certain types before sending
       * @param   types      the types to add the filter to
       * @param   object     a java callback that takes in a list of args
       *                     and the Variables interface.
       **/
      void add_send_filter (
                            uint32_t types, jobject & object);
      
      /**
       * Adds a filter that will be applied to certain types after receiving
       * and before rebroadcasting (if TTL > 0)
       * @param   types      the types to add the filter to
       * @param   object     a java callback that takes in a list of args
       *                     and the Variables interface.
       **/
      void add_rebroadcast_filter (
                                   uint32_t types, jobject & object);
      
      /**
       * Adds an aggregate filter for a map of variables to values
       * before applying updates to the Knowledge_Base
       * @param   object     a java callback that takes in a list of args
       *                     and the Variables interface.
       **/
      void add_receive_filter (jobject & object);
      
      /**
       * Adds an aggregate filter for a map of variables to values
       * before sending
       * @param   object     a java callback that takes in a list of args
       *                     and the Variables interface.
       **/
      void add_send_filter (jobject & object);
      
      /**
       * Adds an aggregate filter for a map of variables to values
       * after receiving and before rebroadcasting (if TTL > 0)
       * @param   object     a java callback that takes in a list of args
       *                     and the Variables interface.
       **/
      void add_rebroadcast_filter (jobject & object);
      
#endif
      
#ifdef _MADARA_PYTHON_CALLBACKS_
      
      /**
       * Adds a filter that will be applied to certain types after receiving
       * and before applying updates to the Knowledge_Base
       * @param   types      the types to add the filter to
       * @param   object     a python callback that takes in a list of args
       *                     and the Variables interface.
       **/
      void add_receive_filter (
        uint32_t types, boost::python::object & object);
      
      /**
       * Adds a filter that will be applied to certain types before sending
       * @param   types      the types to add the filter to
       * @param   object     a python callback that takes in a list of args
       *                     and the Variables interface.
       **/
      void add_send_filter (
        uint32_t types, boost::python::object & object);
      
      /**
       * Adds a filter that will be applied to certain types after receiving
       * and before rebroadcasting (if TTL > 0)
       * @param   types      the types to add the filter to
       * @param   object     a python callback that takes in a list of args
       *                     and the Variables interface.
       **/
      void add_rebroadcast_filter (
        uint32_t types, boost::python::object & object);
      
      /**
       * Adds an aggregate filter for a map of variables to values
       * before applying updates to the Knowledge_Base
       * @param   object     a python callback that takes in a list of args
       *                     and the Variables interface.
       **/
      void add_receive_filter (boost::python::object & object);
      
      /**
       * Adds an aggregate filter for a map of variables to values
       * before sending
       * @param   object     a python callback that takes in a list of args
       *                     and the Variables interface.
       **/
      void add_send_filter (boost::python::object & object);
      
      /**
       * Adds an aggregate filter for a map of variables to values 
       * after receiving and before rebroadcasting (if TTL > 0)
       * @param   object     a python callback that takes in a list of args
       *                     and the Variables interface.
       **/
      void add_rebroadcast_filter (boost::python::object & object);
      
#endif

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
       * Clears the list of send time aggregate filters
       **/
      void clear_send_aggregate_filters (void);

      /**
       * Clears the list of filters for the specified types
       * @param   types   the types to clear the filters of
       **/
      void clear_receive_filters (uint32_t types);
      
      /**
       * Clears the list of receive time aggregate filters
       **/
      void clear_receive_aggregate_filters (void);

      /**
       * Clears the list of filters for the specified types
       * @param   types   the types to clear the filters of
       **/
      void clear_rebroadcast_filters (uint32_t types);
      
      /**
       * Clears the list of rebroadcast time aggregate filters
       **/
      void clear_rebroadcast_aggregate_filters (void);
      
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
       * Filters aggregate records according to the send filter chain
       * @param  records             the aggregate record map
       * @param  transport_context   the context of the transport
       * @return  the result of filtering the input
       **/
      Knowledge_Record filter_send (Knowledge_Map & records,
        const Transport::Transport_Context & transport_context) const;
          
      /**
       * Filters an input according to the receive filter chain
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
       * Filters aggregate records according to the receive filter chain
       * @param  records             the aggregate record map
       * @param  transport_context   the context of the transport
       * @return  the result of filtering the input
       **/
      Knowledge_Record filter_receive (Knowledge_Map & records,
        const Transport::Transport_Context & transport_context) const;
          
      /**
       * Filters an input according to the rebroadcast filter chain
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
       * Filters aggregate records according to the rebroadcast filter chain
       * @param  records             the aggregate record map
       * @param  transport_context   the context of the transport
       * @return  the result of filtering the input
       **/
      Knowledge_Record filter_rebroadcast (Knowledge_Map & records,
        const Transport::Transport_Context & transport_context) const;
          
       
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
       * Returns the number of aggregate filters applied before sending
       * @ return the number of aggregate filters
       **/
      size_t get_number_of_send_aggregate_filters (void) const;
       
      /**
       * Returns the number of aggregate filters applied before
       * rebroadcasting
       * @ return the number of aggregate filters
       **/
      size_t get_number_of_rebroadcast_aggregate_filters (void) const;

      /**
       * Returns the number of types that are filtered before rebroadcast
       * @return  the number of types that have filters
       **/
      size_t get_number_of_rebroadcast_filtered_types (void) const;
       
      /**
       * Returns the number of aggregate filters applied after receiving
       * @ return the number of aggregate filters
       **/
      size_t get_number_of_receive_aggregate_filters (void) const;

      /**
       * Returns the number of types that are filtered after received
       * @return  the number of types that have filters
       **/
      size_t get_number_of_receive_filtered_types (void) const;

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

      /**
       * Updates a packet drop rate, type, and burst
       * @param   drop_rate    percent drop rate for sending packets
       * @param   drop_type    type of drop rate policy to use
       *                       @see PacketDropTypes
       * @param   drop_burst   number of packets to drop consecutively
       **/
      void update_drop_rate (double drop_rate,
        int drop_type = PACKET_DROP_DETERMINISTIC,
        uint64_t drop_burst = 1);

      /**
       * Returns the percentage of dropped packets to enforce on sends
       * @return the percentage of dropped packets to enforce
       **/
      double get_drop_rate (void) const;
      
      /**
       * Returns the policy type for packet drops
       * @return  the policy type for packet drops
       **/
      int get_drop_type (void) const;

      /**
       * Returns the bursts of packet drops
       * @return  the consecutive bursts of dropped packets
       **/
      uint64_t get_drop_burst (void) const;

      /**
       * Sets a bandwidth limit for sending on this transport in bytes per sec
       * @param   bandwidth  send bandwidth in bytes per second
       **/
      void set_send_bandwidth_limit (int64_t bandwidth);
      
      /**
       * Returns the limit for sending on this transport in bytes per second
       * @return the send bandwidth limiting in bytes per second
       **/
      int64_t  get_send_bandwidth_limit (void) const;
       
      /**
       * Sets a bandwidth limit for receiving and sending over the transport.
       * -1 means no limit.
       * @param   bandwidth   total bandwidth limit in bytes per second
       **/
      void set_total_bandwidth_limit (int64_t bandwidth);
       
      /**
       * Returns the total limit for this transport in bytes per second.
       * -1 means no limit.
       * @return the total bandwidth limit in bytes per second
       **/
      int64_t get_total_bandwidth_limit (void) const;

      /**
       * Sets the packet deadline in seconds. Note that most transports only
       * enforce deadline in seconds. However, future transports may allow
       * for microsecond or millisecond deadlines, so we use a double here
       * for expansion purposes. -1 means no deadline.
       * @param   deadline   deadline in seconds
       **/
       void set_deadline (double deadline);

      /**
       * Returns the latency deadline in seconds. -1 means no deadline.
       * @return  deadline in seconds
       **/
       double get_deadline (void) const;

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

      /**
       * Rate for dropping packets
       **/
      double packet_drop_rate_;

      /**
       * Drop rate type
       **/
      int packet_drop_type_;

      /**
       * Burst of packet drops
       **/
      uint64_t packet_drop_burst_;

      /**
       * Maximum send bandwidth usage per second before packets drop
       **/
      int64_t max_send_bandwidth_;

      /**
       * Maximum bandwidth usage for the transport (receive/send) before drop
       **/
      int64_t max_total_bandwidth_;

      /**
       * Deadline for packets at which packets drop
       **/
      double latency_deadline_;
    };
  } // end Transport namespace
} // end Madara namespace

#endif