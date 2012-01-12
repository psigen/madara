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
    /**
     * @class NDDS_Listener
     * @brief Container for NDDS callbacks
     **/
    class NDDS_Listener: public DDSDataReaderListener
    {
    public:
      /**
       * Constructor
       * @param id  unique identifier of this entity (e.g., host:port)
       * @param context the knowledge context to access and mutate
       **/
      NDDS_Listener(const std::string & id,
        Madara::Knowledge_Engine::Thread_Safe_Context & context);

      /**
       * Copy constructor
       **/
      NDDS_Listener(const NDDS_Listener &ref);

      /**
       * Destructor
       **/
      ~NDDS_Listener();

      /**
       * Handles an assignment update
       * @param  data  the update that was made
       **/
      void handle_assignment (NDDS_Knowledge_Update & data);

      /**
       * Handles a multi-assignment update
       * @param  data  the update that was made
       **/
      void handle_multiassignment (NDDS_Knowledge_Update & data);

      /**
       * Handles the case where a subscription has been matched
       * @param reader the DDS data reader to read from
       * @status the status of the matched subscription
       **/
      void on_subscription_matched (DDSDataReader* reader,
                        const DDS_SubscriptionMatchedStatus& status);

      /**
       * Handles the case that data has become available
       * @param reader the DDS data reader to read from
       **/
      void on_data_available(DDSDataReader* reader);
    private:

      // NDDS variables
      const std::string id_;
      ::Madara::Knowledge_Engine::Thread_Safe_Context & context_;

    };  // End of class NDDS_Listener
  }
}

#endif // 