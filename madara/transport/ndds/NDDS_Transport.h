#ifndef _NDDS_TRANSPORT_H_
#define _NDDS_TRANSPORT_H_

#include <string>

#include "madara/knowledge_engine/Thread_Safe_Context.h"
#include "madara/transport/Transport.h"
#include <ndds/ndds_cpp.h>
#include "madara/transport/ndds/Ndds_Knowledge_Update.h"
#include "madara/transport/ndds/Ndds_Knowledge_UpdateSupport.h"
#include "madara/transport/ndds/NDDS_Listener.h"

namespace Madara
{
  namespace Transport
  {
    /**
     * @class NDDS_Transport
     * @brief This class provides an interface into the NDDS
     *        dissemination transport
     */
    class NDDS_Transport : public Base
    {
    public:

      /**
       * Constructor
       * @param   id   unique identifer - usually a combination of host:port
       * @param   context  knowledge context
       * @param   config   transport configuration settings
       * @param   launch_transport  whether or not to launch this transport
       **/
      NDDS_Transport (const std::string & id, 
        Madara::Knowledge_Engine::Thread_Safe_Context & context, 
        const Settings & config, bool launch_transport);

      /**
       * Destructor
       **/
      ~NDDS_Transport ();

      /**
       * Sends a single knowledge assignment
       * @param   key     knowledge location for global variable
       * @param   value   value of the knowledge location
       * @return  result of dds write operation or -1 if we are shutting down
       **/
      long send_data (const std::string & key, const long long & value);

      /**
       * Sends a multiple assignment of knowledge variables
       * @param   expression  key=value pairings separated by commas
       * @param   quality     maximum quality of knowledge writings
       * @return  result of dds write operation or -1 if we are shutting down
       **/
      long send_multiassignment (const std::string & expression, 
        unsigned long quality);

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
       * Closes this transport
       **/
      void close (void);

      /**
       * Activates this transport
       * @return  0 for success. This function may exit the process
       *          if the OSPL daemon hasn't been started.
       **/
      int setup (void);
    protected:
    private:
      // identifier of this knowledge base
      const std::string                               id_;

      // context for knowledge base
      Madara::Knowledge_Engine::Thread_Safe_Context & context_;

      const static char *                             topic_names_[];
      const static char *                             partition_;

      // NDDS variables
      DDSDomainParticipant *   domain_participant_;
      DDSPublisher *   publisher_;
      DDSSubscriber *   subscriber_;
      DDSTopic *        update_topic_;
      DDSDataReader *   data_reader_;
      DDSDataWriter *   data_writer_;
      NDDS_Knowledge_UpdateDataWriter * update_writer_;
      NDDS_Listener listener_;
    };
  }
}

#endif
