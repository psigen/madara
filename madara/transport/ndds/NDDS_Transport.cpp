#include "madara/transport/ndds/NDDS_Transport.h"
//#include "madara/transport/ndds/NDDS_Transport_Read_Thread.h"
#include "madara/utility/Log_Macros.h"
#include "madara/knowledge_engine/Update_Types.h"
#include "madara/utility/Utility.h"

#include <iostream>
#include <sstream>

const char * Madara::Transport::NDDS_Transport::topic_names_[] = {
  "MADARA_KaRL_Data",
  "MADARA_KaRL_Control"
};

const char * Madara::Transport::NDDS_Transport::partition_ = "Madara_knowledge";

Madara::Transport::NDDS_Transport::NDDS_Transport (
  const std::string & id,
  Madara::Knowledge_Engine::Thread_Safe_Context & context, 
  const Settings & config, bool launch_transport)
  : Madara::Transport::Base (config), 
  id_ (id), context_ (context),
  participant_ (0), topic_ (0), update_writer_ (0),
  listener_ (id, context)
{
  if (launch_transport)
    setup ();
}

Madara::Transport::NDDS_Transport::~NDDS_Transport ()
{
  close ();
}

void
Madara::Transport::NDDS_Transport::close (void)
{
  DDS_ReturnCode_t rc;
  this->invalidate_transport ();

  if (participant_)
  {
      /* Perform a clean shutdown of the participant and all the contained
       * entities
       */
      rc = participant_->delete_contained_entities();
      if (rc != DDS_RETCODE_OK)
      {
        MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG,
          DLINFO
          "NDDS_Transport::close: unable to delete participant entities"));
      }

      rc = DDSDomainParticipantFactory::get_instance()->delete_participant(
                      participant_);
      if (rc != DDS_RETCODE_OK)
      {
        MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG,
          DLINFO "NDDS_Transport::close: unable to delete participant"));
      }
  }

  participant_ = 0;
  topic_ = 0;
  update_writer_ = 0;

  this->shutting_down_ = false;
}

int
Madara::Transport::NDDS_Transport::reliability (void) const
{
  return this->settings_.reliability;
}

int
Madara::Transport::NDDS_Transport::reliability (const int & setting)
{
  return this->settings_.reliability = setting;
}

int
Madara::Transport::NDDS_Transport::setup (void)
{
  DDS_ReturnCode_t rc;
  this->is_valid_ = false;

  // create the domain participant
  participant_ = DDSDomainParticipantFactory::get_instance()->
                        create_participant(
                        0,
                        DDS_PARTICIPANT_QOS_DEFAULT,
                        NULL,   /* Listener */
                        DDS_STATUS_MASK_NONE);
  if (participant_ == NULL)
  {
    MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
      "\nNDDS_Transport::setup:" \
      " Unable to start the NDDS transport. Exiting...\n"));
    exit (-2);
  }

  // register the Knowledge Update Type
  rc = NDDS_Knowledge_UpdateTypeSupport::register_type(
           participant_,
           NDDS_Knowledge_UpdateTypeSupport::get_type_name());

  if (rc != DDS_RETCODE_OK)
  {
    MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
      "\nNDDS_Transport::setup:" \
      " Unable to register the knowledge update data type. Exiting...\n"));
    exit (-2);
  }

  // create the knowledge topic
  topic_ = participant_->create_topic(
                        topic_names_[0],
                        NDDS_Knowledge_UpdateTypeSupport::get_type_name(),
                        DDS_TOPIC_QOS_DEFAULT,
                        NULL,           /* listener */
                        DDS_STATUS_MASK_NONE);
  if (topic_ == 0)
  {
    MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
      "\nNDDS_Transport::setup:" \
      " Unable to create topic. Exiting...\n"));
    exit (-2);
  }

  DDSPublisher * publisher = 0;
  DDSDataWriter * data_writer = 0;

  // create the publisher
  publisher = participant_->create_publisher(
                      DDS_PUBLISHER_QOS_DEFAULT,
                      NULL,           /* listener */
                      DDS_STATUS_MASK_NONE);
  if (publisher == 0)
  {
    MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
      "\nNDDS_Transport::setup:" \
      " Unable to create publisher. Exiting...\n"));
    exit (-2);
  }

  // create a topic data writer
  data_writer = publisher->create_datawriter(
                      topic_,
                      DDS_DATAWRITER_QOS_DEFAULT,
                      NULL,           /* listener */
                      DDS_STATUS_MASK_NONE);
  if (data_writer == 0)
  {
    MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
      "\nNDDS_Transport::setup:" \
      " Unable to create topic data writer. Exiting...\n"));
    exit (-2);
  }

  // create the specialized data writer for our data type
  update_writer_ = NDDS_Knowledge_UpdateDataWriter::narrow(data_writer);
  if (update_writer_ == 0)
  {
    MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
      "\nNDDS_Transport::setup:" \
      " Unable to create narrowed data writer. Exiting...\n"));
    exit (-2);
  }

  DDSSubscriber * subscriber = NULL;
  DDSDataReader * reader = NULL;
  
  // create a subscriber
  subscriber = participant_->create_subscriber(
                      DDS_SUBSCRIBER_QOS_DEFAULT,
                      NULL,           /* listener */
                      DDS_STATUS_MASK_NONE);
  if (subscriber == 0)
  {
    MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
      "\nNDDS_Read_Thread::svc:" \
      " Unable to create subscriber. Exiting...\n"));
    exit (-2);
  }

  // create a reader for the topic
  reader = subscriber->create_datareader(
                      topic_,
                      DDS_DATAREADER_QOS_DEFAULT,
                      &listener_,
                      DDS_STATUS_MASK_ALL);
  if (reader == 0) {
    MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
      "\nNDDS_Read_Thread::svc:" \
      " Unable to create reader. Leaving thread...\n"));
    return -2;
  }

  this->validate_transport ();

  return 0;
}

long
Madara::Transport::NDDS_Transport::send_data (const std::string & key, 
                                               const long long & value)
{
  // check to see if we are shutting down
  long ret = this->check_transport ();
  if (-1 == ret)
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "NDDS_Transport::send_data: transport is shutting down")); 
    return ret;
  }
  else if (-2 == ret)
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "NDDS_Transport::send_data: transport is not valid")); 
    return ret;
  }
  
  /// get current lamport clock. 
  unsigned long long cur_clock = context_.get_clock ();

  DDS_ReturnCode_t rc;

  NDDS_Knowledge_Update data;

  NDDS_Knowledge_Update_initialize (&data);

  strcpy (data.key, key.c_str ());

  data.value = value;
  data.clock = cur_clock;
  data.quality = context_.get_write_quality (key);

  strcpy (data.originator, id_.c_str ());

  data.type = Madara::Knowledge_Engine::ASSIGNMENT;

  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
    DLINFO "Splice_DDS_Transport::send:" \
    " sending data: %s=%q, time=%Q, quality=%u\n", 
    key.c_str (), data.value, cur_clock, data.quality));

  DDS_InstanceHandle_t handle = update_writer_->register_instance (data);
  rc = update_writer_->write (data, handle); 

  NDDS_Knowledge_Update_finalize (&data);

  return rc;
}

long
Madara::Transport::NDDS_Transport::send_multiassignment (
  const std::string & expression, unsigned long quality)
{
  // check to see if we are shutting down
  long ret = this->check_transport ();
  if (-1 == ret)
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "NDDS_Transport::send_multiassignment: transport is shutting down")); 
    return ret;
  }
  else if (-2 == ret)
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "NDDS_Transport::send_multiassignment: transport is not valid")); 
    return ret;
  }
  

  /// get current lamport clock. 
  unsigned long long cur_clock = context_.get_clock ();

  DDS_ReturnCode_t rc;

  NDDS_Knowledge_Update data;

  NDDS_Knowledge_Update_initialize (&data);

  strcpy (data.key, expression.c_str ());
  
  data.value = 0;
  data.clock = cur_clock;
  data.quality = quality;

  strcpy (data.originator, id_.c_str ());

  data.type = Madara::Knowledge_Engine::MULTIPLE_ASSIGNMENT;

  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
    DLINFO "NDDS_Transport::send:" \
    " sending multiassignment: %s, time=%Q, quality=%u\n", 
    expression.c_str (), cur_clock, quality));

  DDS_InstanceHandle_t handle = update_writer_->register_instance (data);
  rc = update_writer_->write (data, handle); 

  NDDS_Knowledge_Update_finalize (&data);

  return rc;
}
