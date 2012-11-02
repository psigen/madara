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

#include "madara/transport/broadcast/Broadcast_Transport_Read_Thread.h"
#include "madara/knowledge_engine/Thread_Safe_Context.h"
#include "ace/SOCK_Dgram_Bcast.h"
#include <stdint.h>

namespace Madara
{
  namespace Transport
  {
    /**
     * @class Broadcast_Transport
     * @brief Multicast-based transport for knowledge
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
      ~Broadcast_Transport ();

      /**
       * Sends a single knowledge assignment
       * @param   key     knowledge location for global variable
       * @param   value   value of the knowledge location
       * @return  result of write operation or -1 if we are shutting down
       **/
      long send_data (const std::string & key,
        const Madara::Knowledge_Record::VALUE_TYPE & value);
	  
      /**
       * Sends a knowledge expression to all connected hosts over UDP
       * @param   expression     series of assignments
       * @param   quality        knowledge quality of all variables in expression
       * @return  result of write operation or -1 if we are shutting down
       **/
      long send_multiassignment (const std::string & expression,
		  uint32_t quality);
      
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

      /// knowledge context
      Madara::Knowledge_Engine::Thread_Safe_Context & context_;

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

    };
  }
}

#endif // _BROADCAST_TRANSPORT_H_