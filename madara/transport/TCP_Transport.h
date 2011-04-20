#ifndef TCP_TRANSPORT_H
#define TCP_TRANSPORT_H

#include <string>

#include "madara/transport/TCP_Transport_Read_Thread.h"
#include "madara/knowledge_engine/Thread_Safe_Context.h"
#include "madara/transport/Transport.h"

namespace Madara
{
  namespace Transport
  {
    class TCP_Transport : public Base
    {
    public:
  
      enum {
        RELIABLE = 0
      };

      enum {
        ERROR_TCP_NOT_STARTED = -1,
      };

      static const int PROFILES = 1;

      TCP_Transport (const std::string & id,
        Madara::Knowledge_Engine::Thread_Safe_Context & context, 
        const int & reliability);
      ~TCP_Transport ();
      long send_data (const std::string & key, const long long & value);
      int reliability (void) const;
      int reliability (const int & setting);
      long read (void);
      void close (void);
      int setup (void);
    protected:
    private:
      const std::string                               id_;
      // context for knowledge base
      Madara::Knowledge_Engine::Thread_Safe_Context & context_;

      TCP_Transport_Read_Thread *               thread_;

      bool                                      valid_setup_;
    };
  }
}

#endif // TCP_TRANSPORT_H