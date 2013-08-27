#ifndef _BROADCAST_TRANSPORT_H_
#define _BROADCAST_TRANSPORT_H_

/**
 * @file Broadcast_Transport.h
 * @author James Edmondson <jedmondson@gmail.com>
 *
 * This file contains the Broadcast_Transport class, which provides a
 * broadcast transport for sending knowledge updates in KaRL
 **/

#include <string>

#include "madara/utility/Scoped_Array.h"
#include "madara/transport/broadcast/Broadcast_Transport_Read_Thread.h"
#include "madara/knowledge_engine/Thread_Safe_Context.h"
#include "madara/knowledge_engine/Bandwidth_Monitor.h"
#include "ace/SOCK_Dgram_Bcast.h"
#include "madara/utility/stdint.h"
#include "madara/expression_tree/Expression_Tree.h"


namespace Madara
{
  namespace Transport
  {
    /**
     * @class Broadcast_Transport
     * @brief Multicast-based transport for knowledge. This transport currently
     *        the following transport settings:
     *        1) a single host:port pairing
     *        2) the reduced message header
     *        3) the normal message header
     *        4) domain differentiation
     *        5) on data received logic
     *        6) multi-assignment of records
     **/
    class Broadcast_Transport : public Base
    {
    public:
      /**
       * Constructor
       * @param   id   unique identifer - usually a combination of host:port
       * @param   context  knowledge context
       * @param   config   transport configuration settings
       * @param   launch_transport  whether or not to launch this transport
       **/
      Broadcast_Transport (const std::string & id, 
        Madara::Knowledge_Engine::Thread_Safe_Context & context, 
        Settings & config, bool launch_transport);

      /**
       * Destructor
       **/
      virtual ~Broadcast_Transport ();

      /**
       * Sends a list of knowledge updates to listeners
       * @param   updates listing of all updates that must be sent
       * @return  result of write operation or -1 if we are shutting down
       **/
      long send_data (const Madara::Knowledge_Records & updates);
	    
      /**
       * Closes the transport
       **/
      void close (void);
      
      /**
       * Accesses reliability setting
       * @return  whether we are using reliable dissemination or not
       **/
      int reliability (void) const;
      
      /**
       * Sets the reliability setting
       * @return  the changed setting
       **/
      int reliability (const int & setting);

      /**
       * Initializes the transport
       * @return  0 if success
       **/
      int setup (void);

    private:
      
      /// host:port identifier of this process
      const std::string                               id_;

      /// thread for reading knowledge updates
      Broadcast_Transport_Read_Thread *         thread_;

      /// indicates whether the transport is correctly configured
      bool                                      valid_setup_;
      
      /// holds all multicast addresses we are sending to
      std::vector <ACE_INET_Addr>               addresses_;

      /// holds splitters for knowledge multiassignment expression for speed
      std::vector <std::string>                 splitters_;

      /// underlying socket for sending
      ACE_SOCK_Dgram_Bcast                      socket_;

      /// data received rules, defined in Transport settings
      Madara::Expression_Tree::Expression_Tree  on_data_received_;

      /// buffer for sending
      Madara::Utility::Scoped_Array <char>      buffer_;
      
      /// monitor for sending bandwidth usage
      Knowledge_Engine::Bandwidth_Monitor       send_monitor_;
      
      /// monitor for receiving bandwidth usage
      Knowledge_Engine::Bandwidth_Monitor       receive_monitor_;
    };
  }
}

#endif // _BROADCAST_TRANSPORT_H_