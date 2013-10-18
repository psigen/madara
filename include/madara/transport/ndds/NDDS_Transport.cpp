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
  Settings & config, bool launch_transport)
  : Madara::Transport::Base (id, config, context),
  domain_participant_ (0), update_topic_ (0), update_writer_ (0),
  listener_ (0)
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

  if (subscriber_)
  {
    subscriber_->delete_datareader (data_reader_);
  }

  if (publisher_)
  {
    publisher_->delete_datawriter (data_writer_);
  }

  if (domain_participant_)
  {
    domain_participant_->delete_subscriber (subscriber_);
    domain_participant_->delete_publisher (publisher_);
    domain_participant_->delete_topic (update_topic_);
  }

  if (domain_participant_)
  {
      /* Perform a clean shutdown of the participant and all the contained
       * entities
       */
      rc = domain_participant_->delete_contained_entities();
      if (rc != DDS_RETCODE_OK)
      {
        MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG,
          DLINFO
          "NDDS_Transport::close: unable to delete participant entities\n"));
      }

      rc = DDSDomainParticipantFactory::get_instance()->delete_participant(
                      domain_participant_);
      if (rc != DDS_RETCODE_OK)
      {
        MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG,
          DLINFO "NDDS_Transport::close: unable to delete participant\n"));
      }
  }

  domain_participant_ = 0;
  subscriber_ = 0;
  publisher_ = 0;
  update_writer_ = 0;
  update_topic_ = 0;

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
  Base::setup ();

  DDS_ReturnCode_t rc;
  DDS_DomainId_t domain = 0;
  this->is_valid_ = false;

  std::stringstream domainreader;
  domainreader << this->settings_.domains;
  domainreader >> domain;

  // create the domain participant
  domain_participant_ = DDSDomainParticipantFactory::get_instance()->
                        create_participant(
                        domain,
                        DDS_PARTICIPANT_QOS_DEFAULT,
                        NULL,   /* Listener */
                        DDS_STATUS_MASK_NONE);
  if (domain_participant_ == NULL)
  {
    MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
      "\nNDDS_Transport::setup:" \
      " Unable to start the NDDS transport. Exiting...\n"));
    exit (-2);
  }

  DDS_TopicQos topic_qos;
  domain_participant_->get_default_topic_qos(topic_qos);

  if (Madara::Transport::RELIABLE == this->settings_.reliability)
  {
    topic_qos.reliability.kind = DDS_RELIABLE_RELIABILITY_QOS;
    topic_qos.history.depth = this->settings_.queue_length;
    topic_qos.resource_limits.max_samples_per_instance = 
      this->settings_.queue_length;
    topic_qos.resource_limits.max_samples = this->settings_.queue_length;
    topic_qos.destination_order.kind = 
      DDS_BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS;
  }
  //topic_qos_.resource_limits.max_samples_per_instance= 10;
  domain_participant_->set_default_topic_qos(topic_qos);

  // register the Knowledge Update Type
  rc = NDDS_Knowledge_UpdateTypeSupport::register_type (
           domain_participant_,
           NDDS_Knowledge_UpdateTypeSupport::get_type_name());

  if (rc != DDS_RETCODE_OK)
  {
    MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
      "\nNDDS_Transport::setup:" \
      " Unable to register the knowledge update data type. Exiting...\n"));
    exit (-2);
  }

  // create the knowledge topic
  update_topic_ = domain_participant_->create_topic (
                        topic_names_[0],
                        NDDS_Knowledge_UpdateTypeSupport::get_type_name(),
                        topic_qos,
                        NULL,           /* listener */
                        DDS_STATUS_MASK_NONE);
  if (update_topic_ == 0)
  {
    MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
      "\nNDDS_Transport::setup:" \
      " Unable to create topic. Exiting...\n"));
    exit (-2);
  }

  DDS_PublisherQos pub_qos;

  domain_participant_->get_default_publisher_qos (pub_qos);

  if (Madara::Transport::RELIABLE == this->settings_.reliability)
  {
    pub_qos.presentation.access_scope = DDS_TOPIC_PRESENTATION_QOS;
    pub_qos.presentation.coherent_access = true;
    pub_qos.presentation.ordered_access = false;
  }

  // create the publisher_
  publisher_ = domain_participant_->create_publisher (
                      pub_qos,
                      NULL,           /* listener */
                      DDS_STATUS_MASK_NONE);
  if (publisher_ == 0)
  {
    MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
      "\nNDDS_Transport::setup:" \
      " Unable to create publisher_. Exiting...\n"));
    exit (-2);
  }

  DDS_DataWriterQos datawriter_qos;
  publisher_->get_default_datawriter_qos (datawriter_qos);
  publisher_->copy_from_topic_qos (datawriter_qos, topic_qos);

  // create a topic data writer
  data_writer_ = publisher_->create_datawriter (
                      update_topic_,
                      datawriter_qos,
                      NULL,           /* listener */
                      DDS_STATUS_MASK_NONE);
  if (data_writer_ == 0)
  {
    MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
      "\nNDDS_Transport::setup:" \
      " Unable to create topic data writer. Exiting...\n"));
    exit (-2);
  }

  // create the specialized data writer for our data type
  update_writer_ = NDDS_Knowledge_UpdateDataWriter::narrow(data_writer_);
  if (update_writer_ == 0)
  {
    MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
      "\nNDDS_Transport::setup:" \
      " Unable to create narrowed data writer. Exiting...\n"));
    exit (-2);
  }

  DDS_SubscriberQos sub_qos;

  if (Madara::Transport::RELIABLE == this->settings_.reliability)
  {
    sub_qos.presentation.access_scope = DDS_TOPIC_PRESENTATION_QOS;
    sub_qos.presentation.coherent_access = true;
    sub_qos.presentation.ordered_access = false;
  }

  // create a subscriber_
  subscriber_ = domain_participant_->create_subscriber (
                      sub_qos,
                      NULL,           /* listener */
                      DDS_STATUS_MASK_NONE);
  if (subscriber_ == 0)
  {
    MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
      "\nNDDS_Read_Thread::svc:" \
      " Unable to create subscriber_. Exiting...\n"));
    exit (-2);
  }

  DDS_DataReaderQos datareader_qos;
  subscriber_->get_default_datareader_qos (datareader_qos);
  subscriber_->copy_from_topic_qos (datareader_qos, topic_qos);

  if (Madara::Transport::RELIABLE == this->settings_.reliability)
  {
    datareader_qos.reliability.kind = DDS_RELIABLE_RELIABILITY_QOS;
    datareader_qos.history.depth = this->settings_.queue_length;
    datareader_qos.resource_limits.max_samples = this->settings_.queue_length;
    datareader_qos.destination_order.kind = 
      DDS_BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS;
  }

  listener_ = new NDDS_Listener (settings_, id_, context_,
    send_monitor_, receive_monitor_, packet_scheduler_);

  // create a reader for the topic
  data_reader_ = subscriber_->create_datareader (
                      update_topic_,
                      datareader_qos,
                      listener_,
                      DDS_STATUS_MASK_ALL);
  if (data_reader_ == 0) {
    MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
      "\nNDDS_Read_Thread::svc:" \
      " Unable to create reader. Leaving thread...\n"));
    return -2;
  }

  this->validate_transport ();

  return 0;
}

long
Madara::Transport::NDDS_Transport::send_data (
  const Madara::Knowledge_Records & updates)
{
  long result =
    prep_send (updates, "Splice_DDS_Transport::send_data:");
  
  // get the maximum quality from the updates
  uint32_t quality = Madara::max_quality (updates);

  /// get current lamport clock. 
  unsigned long long cur_clock = context_.get_clock ();

  DDS_ReturnCode_t rc;
  
  std::stringstream buffer;
  NDDS_Knowledge_Update data;

  NDDS_Knowledge_Update_initialize (&data);
  
  data.clock = cur_clock;
  data.quality = quality;

  data.buffer.ensure_length (result, result);
  data.buffer.from_array ((DDS_Octet *)buffer_.get_ptr (), result);
  data.clock = cur_clock;
  data.quality = quality;
  data.updates = DDS_UnsignedLong (updates.size ());
  data.originator = new char [id_.size () + 1];
  strncpy (data.originator, id_.c_str (), id_.size () + 1);
  data.type = Madara::Transport::MULTIASSIGN;
  data.ttl = settings_.get_rebroadcast_ttl ();
  data.timestamp = time (NULL);
  data.madara_id = new char [strlen (MADARA_IDENTIFIER) + 1];
  strncpy (data.madara_id, MADARA_IDENTIFIER, strlen (MADARA_IDENTIFIER) + 1);
  
  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
    DLINFO "NDDS_Transport::send:" \
    " sending multiassignment: %d updates, time=%Q, quality=%u\n", 
    data.updates, cur_clock, quality));

  DDS_InstanceHandle_t handle = update_writer_->register_instance (data);
  rc = update_writer_->write (data, handle); 

  NDDS_Knowledge_Update_finalize (&data);

  return rc;
}
