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
#include "madara/utility/stdint.h"
#include "madara/MADARA_export.h"

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
    };
  } // end Transport namespace
} // end Madara namespace

#endif