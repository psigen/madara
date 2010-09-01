#ifndef SPLICE_DDS_TRANSPORT_H
#define SPLICE_DDS_TRANSPORT_H

#include "madara/transport/Splice_Transport_Read_Thread.h"
#include "madara/knowledge_engine/Thread_Safe_Context.h"
#include "madara/transport/Transport.h"
#include "ccpp_dds_dcps.h"
#include "madara/ccpp_Splice_Knowledge_Update.h"

namespace Madara
{
  namespace Transport
  {
    class Splice_DDS_Transport : public Base
    {
    public:
  
      enum {
        BEST_EFFORT = 0,
        RELIABLE = 1
      };

      enum {
        ERROR_OSPL_NOT_STARTED = -1,
        ERROR_SUB_BAD_CREATE = -2,
        ERROR_PUB_BAD_CREATE = -3
      };

      static const int PROFILES = 2;

      Splice_DDS_Transport (Madara::Knowledge_Engine::Thread_Safe_Context & context, 
        const int & reliability = BEST_EFFORT);
      ~Splice_DDS_Transport ();
      virtual long send (const std::string & key, const long & value);
      int reliability (void) const;
      int reliability (const int & setting);
      long read (void);
      void close (void);
      int setup (void);
    protected:
    private:
      // context for knowledge base
      Madara::Knowledge_Engine::Thread_Safe_Context & context_;

      const static char * topic_names_[];
      const static char * partition_;

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
      Knowledge::UpdateSeq_var           update_data_list_;
      DDS::StatusCondition_ptr           update_status_condition_;
      DDS::Topic_ptr                     update_topic_;

      Splice_Read_Thread *               thread_;

      int reliability_;
      bool valid_setup_;

      /// Splice handle checker
      void check_handle (void * handle, char *info);

      /// Splice status checker
      void check_status (DDS::ReturnCode_t status, const char * info);
      
      /// Return error name of the specific status
      char * get_error_name (DDS::ReturnCode_t status);
    };
  }
}

#endif