#ifndef UDP_TRANSPORT_H
#define UDP_TRANSPORT_H

#include <string>

#include "madara/transport/udp/UDP_Transport_Read_Thread.h"
#include "madara/transport/Transport.h"

#include <string>
#include <map>

#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram.h"


namespace Madara
{
  namespace Transport
  {
    /**
     * @class UDP_Transport
     * @brief UDP-based transport for knowledge records
     **/
    class UDP_Transport : public Base
    {
    public:
  
      enum {
        RELIABLE = 0
      };

      enum {
        ERROR_UDP_NOT_STARTED = -1,
      };

      static const int PROFILES = 1;

      /**
       * Constructor
       * @param   id   unique identifer - usually a combination of host:port
       * @param   context  knowledge context
       * @param   config   transport configuration settings
       * @param   launch_transport  whether or not to launch this transport
       **/
      UDP_Transport (const std::string & id, 
        Madara::Knowledge_Engine::Thread_Safe_Context & context, 
        Settings & config, bool launch_transport);

      /**
       * Destructor
       **/
      virtual ~UDP_Transport ();

      /**
       * Sends a list of knowledge updates to listeners
       * @param   updates listing of all updates that must be sent
       * @return  result of write operation or -1 if we are shutting down
       **/
      long send_data (const Madara::Knowledge_Records & updates);
	  
      /**
       * Accesses reliability setting. If this returns zero, it doesn't
       * make much sense.
       * @return  whether we are using reliable dissemination or not
       **/
      int reliability (void) const;

      /**
       * Accesses reliability setting. If this returns zero, it doesn't
       * make much sense.
       * @return  whether we are using reliable dissemination or not
       **/
      int reliability (const int & setting);
      long read (void);
      void close (void);
      int setup (void);
    protected:
    private:
      UDP_Transport_Read_Thread *               thread_;

      bool                                      valid_setup_;
      
      std::map <std::string, ACE_INET_Addr>     addresses_;
      
      /// underlying socket for sending
      ACE_SOCK_Dgram                            socket_;
    };
  }
}

#endif // UDP_TRANSPORT_H