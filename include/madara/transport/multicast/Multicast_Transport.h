#ifndef _MULTICAST_TRANSPORT_H_
#define _MULTICAST_TRANSPORT_H_

/**
 * @file Multicast_Transport.h
 * @author James Edmondson <jedmondson@gmail.com>
 *
 * This file contains the Multicast_Transport class, which provides a
 * multicast transport for sending knowledge updates in KaRL
 **/

#include <string>

#include "madara/MADARA_export.h"
#include "madara/utility/Scoped_Array.h"
#include "madara/transport/multicast/Multicast_Transport_Read_Thread.h"
#include "madara/transport/QoS_Transport_Settings.h"
#include "madara/transport/Transport.h"
#include "madara/knowledge_engine/Thread_Safe_Context.h"
#include "ace/SOCK_Dgram.h"
#include "madara/knowledge_engine/Knowledge_Record.h"
#include "madara/expression_tree/Expression_Tree.h"
#include "madara/transport/Bandwidth_Monitor.h"

namespace Madara
{
  namespace Transport
  {
    /**
     * @class Multicast_Transport
     * @brief Multicast-based transport for knowledge. This transport currently
     * supports the following transport settings:<br />
     *        1) a single host:port pairing<br />
     *        2) the reduced message header<br />
     *        3) the normal message header<br />
     *        4) domain differentiation<br />
     *        5) on data received logic<br />
     *        6) multi-assignment of records<br />
     *        7) rebroadcasting<br />
     **/
    class MADARA_Export Multicast_Transport : public Base
    {
    public:
      /**
       * Constructor
       * @param   id   unique identifer - usually a combination of host:port
       * @param   context  knowledge context
       * @param   config   transport configuration settings
       * @param   launch_transport  whether or not to launch this transport
       **/
      Multicast_Transport (const std::string & id, 
        Madara::Knowledge_Engine::Thread_Safe_Context & context, 
        Settings & config, bool launch_transport);

      /**
       * Destructor
       **/
      virtual ~Multicast_Transport ();
      
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
      
      /// thread for reading knowledge updates
      Multicast_Transport_Read_Thread *         thread_;
      
      /// indicates whether the transport is correctly configured
      bool                                      valid_setup_;

      /// holds all multicast addresses we are sending to
      std::vector <ACE_INET_Addr>               addresses_;

      /// holds splitters for knowledge multiassignment expression for speed
      std::vector <std::string>                 splitters_;

      /// underlying socket for sending
      ACE_SOCK_Dgram                            socket_;
    };
  }
}

#endif // _MULTICAST_TRANSPORT_H_