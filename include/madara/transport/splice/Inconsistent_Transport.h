#ifndef INCONSISTENT_DDS_TRANSPORT_H
#define INCONSISTENT_DDS_TRANSPORT_H

#include <string>

//#include "madara/transport/splice/Splice_DataReader_Listener.h"
//#include "madara/transport/splice/Splice_Subscriber_Listener.h"
#include "madara/transport/splice/Inconsistent_Read_Thread.h"
#include "madara/knowledge_engine/Thread_Safe_Context.h"
#include "madara/transport/Transport.h"
#include "ccpp_dds_dcps.h"
#include "madara/transport/splice/ccpp_Splice_Knowledge_Update.h"

namespace Madara
{
  namespace Transport
  {
    /**
     * @class Inconsistent_Transport
     * @brief This class provides an interface into the Open Splice
     *        dissemination transport
     */
    class Inconsistent_Transport : public Base
    {
    public:

      enum {
        ERROR_OSPL_NOT_STARTED = -1,
        ERROR_SUB_BAD_CREATE = -2,
        ERROR_PUB_BAD_CREATE = -3
      };

      static const int PROFILES = 2;

      /**
       * Constructor
       * @param   id   unique identifer - usually a combination of host:port
       * @param   context  knowledge context
       * @param   config   transport configuration settings
       * @param   launch_transport  whether or not to launch this transport
       **/
      Inconsistent_Transport (const std::string & id, 
        Madara::Knowledge_Engine::Thread_Safe_Context & context, 
        Settings & config, bool launch_transport);

      /**
       * Destructor
       **/
      ~Inconsistent_Transport ();

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
       * Start a three phase latency round with all other participants
       * @return  result of dds write operation or -1 if we are shutting down
       **/
      long start_latency (void);

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
      const static char *                             ret_code_names[];
      //DDS::pong_handler                        *active_handler;

      DDS::DomainParticipantQos          part_qos_;
      DDS::TopicQos                      topic_qos_;
      DDS::PublisherQos                  pub_qos_;
      DDS::DataWriterQos                 datawriter_qos_;
      DDS::SubscriberQos                 sub_qos_;
      DDS::DataReaderQos                 datareader_qos_;

      DDS::DomainId_t                    domain_;
      DDS::DomainParticipantFactory_var  domain_factory_;
      DDS::DomainParticipant_var         domain_participant_;
      DDS::Publisher_var                 publisher_;
      DDS::Subscriber_var                subscriber_;
      DDS::DataWriter_var                datawriter_;
      DDS::DataReader_var                datareader_;
      DDS::DataWriter_var                latencywriter_;

      Knowledge::UpdateDataWriter_var    update_writer_;
      Knowledge::UpdateDataWriter_var    latency_update_writer_;
      Knowledge::UpdateDataReader_var    update_reader_;
      Knowledge::UpdateTypeSupport       update_type_support_;

      DDS::Topic_var                     update_topic_;

      Inconsistent_Read_Thread *               thread_;

      /**
       * Splice handle checker
       **/
      void check_handle (void * handle, const char *info);

      /**
       * Splice status checker
       **/
      void check_status (DDS::ReturnCode_t status, const char * info);
      
      /**
       * Returns error name of the specific status
       **/
      const char * get_error_name (DDS::ReturnCode_t status);
    };
  }
}

#endif // INCONSISTENT_DDS_TRANSPORT_H
