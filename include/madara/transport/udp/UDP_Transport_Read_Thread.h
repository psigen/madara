#ifndef UDP_TRANSPORT_READ_THREAD_H
#define UDP_TRANSPORT_READ_THREAD_H

#include <string>

#include "madara/utility/Scoped_Array.h"
#include "madara/knowledge_engine/Thread_Safe_Context.h"
#include "madara/transport/QoS_Transport_Settings.h"
#include "madara/expression_tree/Expression_Tree.h"
#include "madara/transport/Transport.h"
#include "madara/transport/Message_Header.h"

#include "ace/Task.h"
#include "ace/Mutex.h"
#include "ace/Barrier.h"
#include "ace/Atomic_Op_T.h"
#include "ace/Thread_Mutex.h"

#include "ace/Synch.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram.h"

namespace Madara
{
  namespace Transport
  {
    /**
     * @class UDP_Transport_Read_Thread
     * @brief Thread for reading knowledge updates through a UDP socket
     **/
    class UDP_Transport_Read_Thread : public ACE_Task<ACE_MT_SYNCH>
    {
    public:
      /**
       * Constructor
       * @param    settings   Transport settings
       * @param    id      host:port identifier of this process, to allow for 
       *                   rejection of duplicates
       * @param    context    the knowledge variables to update
       * @param    addresses    the ACE socket addresses to communicate with 
       **/
      UDP_Transport_Read_Thread (
        const Settings & settings,
        const std::string & id,
        Madara::Knowledge_Engine::Thread_Safe_Context & context,
        std::map <std::string, ACE_INET_Addr> & addresses,
        ACE_SOCK_Dgram & socket);
      
      /**
      * Destructor
      **/
      ~UDP_Transport_Read_Thread ();

      /**
      * Signals the read thread to terminate
      **/
      int enter_barrier (void);
      
      /**
      * Closes the reading socket and clean up the thread
      **/
      int close (void);
      
      /**
      * Reads messages from a socket
      **/
      int svc (void);
      
      /**
       * Sends a rebroadcast packet to all peers.
       * @param   header   header for the rebroadcasted packet
       * @param   records  records to rebroadcast (already filtered for
       *                   rebroadcast)
       **/
      void rebroadcast (Message_Header * header,
        const Knowledge_Map & records);

      /**
      * Wait for the transport to be ready
      **/
      void wait_for_ready (void);
    private:
      /// Transport settings
      const Settings & settings_;

      /// host:port identifier of this process
      const std::string                                 id_;

      /// knowledge context
      ::Madara::Knowledge_Engine::Thread_Safe_Context & context_;
      
      /// barrier for closing and waiting on the read thread
      ACE_Barrier barrier_;

      /// atomic variable that signals termination
      ACE_Atomic_Op<ACE_Mutex, bool>     terminated_;
      
      /// Mutex for use with terminated_
      ACE_Thread_Mutex                   mutex_;
      
      /// Condition that is waited on my calling transport on startup
      Madara::Transport::Condition       is_not_ready_;

      /// Indicates whether the read thread is ready to accept messages
      bool                               is_ready_;
      
      /// internet addresses of our peers
      std::map <std::string, ACE_INET_Addr> & addresses_;
      
      /// The socket we are reading from
      ACE_SOCK_Dgram                     read_socket_;
      
      /// The socket we are writing to
      ACE_SOCK_Dgram       &             write_socket_;

      /// data received rules, defined in Transport settings
      Madara::Expression_Tree::Expression_Tree  on_data_received_;
      
      /// buffer for sending
      Madara::Utility::Scoped_Array <char>      buffer_;

      /// pointer to qos_settings (if applicable)
      const QoS_Transport_Settings *      qos_settings_;
    };
  }
}

#endif // UDP_TRANSPORT_READ_THREAD_H
