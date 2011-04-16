#ifndef SPLICE_DDS_TRANSPORT_H
#define SPLICE_DDS_TRANSPORT_H

#include <string>

#include "madara/transport/Splice_Transport_Read_Thread.h"
#include "madara/knowledge_engine/Thread_Safe_Context.h"
#include "madara/transport/Transport.h"
#include "ccpp_dds_dcps.h"
#include "madara/ccpp_Splice_Knowledge_Update.h"
#include "madara/ccpp_Splice_Mutex_Message.h"

namespace Madara
{
  namespace Transport
  {
    /**
     * @class Splice_DDS_Transport
     * @brief This class provides an interface into the Open Splice
     *        dissemination transport
     */
    class Splice_DDS_Transport : public Base
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
      Splice_DDS_Transport (const std::string & id, 
        Madara::Knowledge_Engine::Thread_Safe_Context & context, 
        const Settings & config, bool launch_transport);

      /**
       * Destructor
       **/
      ~Splice_DDS_Transport ();

      /**
       * Sends a single knowledge assignment
       * @param   key     knowledge location for global variable
       * @param   value   value of the knowledge location
       * @return  result of dds write operation or -1 if we are shutting down
       **/
      virtual long send_data (const std::string & key, const long long & value);

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

      //DDS::pong_handler                        *active_handler;

      DDS::DomainParticipantQos          part_qos_;
      DDS::TopicQos                      topic_qos_;
      DDS::PublisherQos                  pub_qos_;
      DDS::DataWriterQos                 datawriter_qos_;
      DDS::SubscriberQos                 sub_qos_;
      DDS::DataReaderQos                 datareader_qos_;

      DDS::DomainId_t                    domain_;
      DDS::DomainParticipantFactory_ptr  domain_factory_;
      DDS::DomainParticipant_ptr         domain_participant_;
      DDS::Publisher_ptr                 publisher_;
      DDS::Subscriber_ptr                subscriber_;
      DDS::DataWriter_ptr                datawriter_;
      DDS::DataReader_ptr                datareader_;

      Knowledge::UpdateDataWriter_ptr    update_writer_;
      Knowledge::UpdateDataReader_ptr    update_reader_;
      Knowledge::UpdateTypeSupport       update_type_support_;
//      Knowledge::UpdateSeq_var           update_data_list_;

      Knowledge::MutexDataWriter_ptr     mutex_writer_;
      Knowledge::MutexDataReader_ptr     mutex_reader_;
      Knowledge::MutexTypeSupport        mutex_type_support_;
//      Knowledge::MutexSeq_var            mutex_data_list_;

      //DDS::StatusCondition_ptr           update_status_condition_;
      DDS::Topic_ptr                     update_topic_;
      DDS::Topic_ptr                     mutex_topic_;

      Splice_Read_Thread *               thread_;

      //std::string                        data_topic_name_;
      //std::string                        control_topic_name_;


      //int reliability_;
     // bool valid_setup_;
      bool enable_mutexing_;

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

#endif
