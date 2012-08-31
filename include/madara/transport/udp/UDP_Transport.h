#ifndef UDP_TRANSPORT_H
#define UDP_TRANSPORT_H

#include <string>

#include "madara/transport/udp/UDP_Transport_Read_Thread.h"
#include "madara/knowledge_engine/Thread_Safe_Context.h"
#include "madara/transport/Transport.h"

namespace Madara
{
  namespace Transport
  {
    /**
     * @class UDP_Transport
     * @brief TCP-based transport (skeleton code)
     **/
    class UDP_Transport : public Base
    {
    public:
  
      enum {
        RELIABLE = 0
      };

      enum {
        ERROR_TCP_NOT_STARTED = -1,
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
      ~UDP_Transport ();

      /**
       * Sends a single knowledge assignment
       * @param   key     knowledge location for global variable
       * @param   value   value of the knowledge location
       * @return  result of write operation or -1 if we are shutting down
       **/
      long send_data (const std::string & key, const long long & value);
	  
      /**
       * Sends a knowledge expression to all connected hosts over UDP
       * @param   expression     series of assignments
       * @param   quality        knowledge quality of all variables in expression
       * @return  result of write operation or -1 if we are shutting down
       **/
      long send_multiassignment (const std::string & expression,
		  unsigned long quality);

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
      const std::string                               id_;
      // context for knowledge base
      Madara::Knowledge_Engine::Thread_Safe_Context & context_;

      UDP_Transport_Read_Thread *               thread_;

      bool                                      valid_setup_;
    };
  }
}

#endif // UDP_TRANSPORT_H