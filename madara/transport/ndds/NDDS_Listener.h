#ifndef _NDDS_LISTENER_H_
#define _NDDS_LISTENER_H_


#include <string>

#include "madara/knowledge_engine/Thread_Safe_Context.h"
#include "madara/transport/Transport.h"

#include <ndds/ndds_cpp.h>

#include "madara/transport/ndds/Ndds_Knowledge_Update.h"
#include "madara/transport/ndds/Ndds_Knowledge_UpdateSupport.h"

namespace Madara
{
  namespace Transport
  {
    /* The class containing DDS event callbacks */
    class NDDS_Listener: public DDSDataReaderListener
    {
    public:
      NDDS_Listener(const std::string & id,
        Madara::Knowledge_Engine::Thread_Safe_Context & context);

      NDDS_Listener(const NDDS_Listener &ref);

      ~NDDS_Listener();

      void handle_assignment (NDDS_Knowledge_Update & data);
      void handle_multiassignment (NDDS_Knowledge_Update & data);

    public:
      void on_subscription_matched (DDSDataReader* reader,
                        const DDS_SubscriptionMatchedStatus& status);

      void on_data_available(DDSDataReader* reader);
    private:

      // NDDS variables
      const std::string id_;
      ::Madara::Knowledge_Engine::Thread_Safe_Context & context_;

    };  // End of class NDDS_Listener
  }
}

#endif // 