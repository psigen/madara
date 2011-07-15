#include "madara/transport/splice/Splice_DDS_Transport.h"
#include "madara/utility/Log_Macros.h"
#include "madara/knowledge_engine/Update_Types.h"
#include "madara/utility/Utility.h"

#include <iostream>
#include <sstream>

const char * Madara::Transport::Splice_DDS_Transport::topic_names_[] = {
  "MADARA_KaRL_Data",
  "MADARA_KaRL_Control"
};

/* Array to hold the names for all ReturnCodes. */
const char * ret_code_names[13] = { 
    "DDS_RETCODE_OK",
    "DDS_RETCODE_ERROR",
    "DDS_RETCODE_UNSUPPORTED",
    "DDS_RETCODE_BAD_PARAMETER",
    "DDS_RETCODE_PRECONDITION_NOT_MET",
    "DDS_RETCODE_OUT_OF_RESOURCES",
    "DDS_RETCODE_NOT_ENABLED",
    "DDS_RETCODE_IMMUTABLE_POLICY",
    "DDS_RETCODE_INCONSISTENT_POLICY",
    "DDS_RETCODE_ALREADY_DELETED",
    "DDS_RETCODE_TIMEOUT",
    "DDS_RETCODE_NO_DATA",
    "DDS_RETCODE_ILLEGAL_OPERATION" };

const char * Madara::Transport::Splice_DDS_Transport::partition_ = "Madara_knowledge";

Madara::Transport::Splice_DDS_Transport::Splice_DDS_Transport (
  const std::string & id,
  Madara::Knowledge_Engine::Thread_Safe_Context & context, 
  const Settings & config, bool launch_transport)
  : Madara::Transport::Base (config), 
  id_ (id), context_ (context), domain_ (0), domain_factory_ (0), 
  domain_participant_ (0), publisher_ (0), subscriber_ (0), 
  datawriter_ (0), datareader_ (0), 
  update_writer_ (0), update_reader_ (0),
  update_topic_ (0), 
  dr_listener_ (id, context), sub_listener_ (id, context)
  //reliability_ (reliability), 
  //valid_setup_ (false),
  //data_topic_name_ (topic_names_[0]),
  //control_topic_name_ (topic_names_[1])
{
  if (launch_transport)
    setup ();
}
Madara::Transport::Splice_DDS_Transport::~Splice_DDS_Transport ()
{
  close ();
}

void
Madara::Transport::Splice_DDS_Transport::close (void)
{
  this->invalidate_transport ();

  if (subscriber_.in ())
  {
    subscriber_->delete_datareader (update_reader_);
  }

  if (publisher_.in ())
  {
    publisher_->delete_datawriter (update_writer_);
  }


  if (domain_participant_.in ())
  {
    domain_participant_->delete_subscriber (subscriber_);
    domain_participant_->delete_publisher (publisher_);
    domain_participant_->delete_topic (update_topic_);
  }

  if (domain_factory_.in ())
    domain_factory_->delete_participant (domain_participant_);

  update_reader_ = 0;
  update_writer_ = 0;
  update_topic_ = 0;
  subscriber_ = 0;
  publisher_ = 0;
  domain_participant_ = 0;
  domain_factory_ = 0;

  this->shutting_down_ = false;
}

int
Madara::Transport::Splice_DDS_Transport::reliability (void) const
{
  return this->settings_.reliability;
}

int
Madara::Transport::Splice_DDS_Transport::reliability (const int & setting)
{
  return this->settings_.reliability = setting;
}

int
Madara::Transport::Splice_DDS_Transport::setup (void)
{
  DDS::ReturnCode_t                         status;

  this->is_valid_ = false;

  // reset the valid setup flag
  //valid_setup_ = false;

  // get the domain participant factory
  domain_factory_ = DDS::DomainParticipantFactory::get_instance ();
  domain_factory_->get_default_participant_qos (part_qos_);
	domain_participant_ = domain_factory_->create_participant (domain_, 
    part_qos_, NULL, DDS::STATUS_MASK_NONE);

  // if dp == NULL, we've got an error
  if (domain_participant_ == NULL)
  {
    MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
      "\nSplice_DDS_Transport::setup:" \
      " splice daemon not running. Try 'ospl start'...\n"));
    exit (-2);
  }

  domain_participant_->get_default_topic_qos(topic_qos_);

  if (Madara::Transport::RELIABLE == this->settings_.reliability)
  {
    topic_qos_.reliability.kind = DDS::RELIABLE_RELIABILITY_QOS;
    topic_qos_.history.depth = this->settings_.queue_length;
    topic_qos_.resource_limits.max_samples_per_instance = 
      this->settings_.queue_length;
    topic_qos_.resource_limits.max_samples = this->settings_.queue_length;
    topic_qos_.destination_order.kind = 
      DDS::BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS;
    //topic_qos_.
  }
  //topic_qos_.resource_limits.max_samples_per_instance= 10;
  domain_participant_->set_default_topic_qos(topic_qos_);

  //  Register Update type
  status = this->update_type_support_.register_type (
    domain_participant_, "Knowledge::Update");
  check_status(status, "Knowledge::UpdateTypeSupport::register_type");

  //  Register Mutex type
  //status = this->mutex_type_support_.register_type (
  //  domain_participant_, "Knowledge::Mutex");
  //check_status(status, "Knowledge::MutexTypeSupport::register_type");

  MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
      DLINFO "Splice_DDS_Transport::setup:" \
      " Setting up domain (%s)\n", 
      Madara::Utility::dds_topicify (settings_.domains).c_str ()));

  // Create Update topic
  update_topic_ = domain_participant_->create_topic (
    Madara::Utility::dds_topicify (settings_.domains).c_str (), 
    "Knowledge::Update", 
    topic_qos_, NULL, DDS::STATUS_MASK_NONE);
  check_handle(update_topic_, 
    "DDS::DomainParticipant::create_topic (Knowledge_Update)");

  // Get default qos for publisher
  status = domain_participant_->get_default_publisher_qos (pub_qos_);
  check_status(status, "DDS::DomainParticipant::get_default_publisher_qos");


  if (Madara::Transport::RELIABLE == this->settings_.reliability)
  {
    pub_qos_.presentation.access_scope = DDS::TOPIC_PRESENTATION_QOS;
    pub_qos_.presentation.coherent_access = true;
    pub_qos_.presentation.ordered_access = false;
    //topic_qos_.
  }

  // Create publisher
  pub_qos_.partition.name.length (1);
  pub_qos_.partition.name[0] = DDS::string_dup (partition_);
  publisher_ = domain_participant_->create_publisher (
    pub_qos_, NULL, DDS::STATUS_MASK_NONE);
  check_handle(publisher_, "DDS::DomainParticipant::create_publisher");

  // Create subscriber
  status = domain_participant_->get_default_subscriber_qos (sub_qos_);
  check_status(status, "DDS::DomainParticipant::get_default_subscriber_qos");


  if (Madara::Transport::RELIABLE == this->settings_.reliability)
  {
    sub_qos_.presentation.access_scope = DDS::TOPIC_PRESENTATION_QOS;
    sub_qos_.presentation.coherent_access = true;
    sub_qos_.presentation.ordered_access = false;
  }

  sub_qos_.partition.name.length (1);
  sub_qos_.partition.name[0] = DDS::string_dup (partition_);
  subscriber_ = domain_participant_->create_subscriber (
    sub_qos_, &sub_listener_, DDS::DATA_AVAILABLE_STATUS | DDS::DATA_ON_READERS_STATUS);
  check_handle(subscriber_, "DDS::DomainParticipant::create_subscriber");

  if (!subscriber_ || !publisher_)
  {
    MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
      "\nSplice_DDS_Transport::setup:" \
      " pub or sub could not be created. Try 'ospl stop; ospl start'...\n"));
    exit (-2);
  }

  // Create datawriter
  publisher_->get_default_datawriter_qos (datawriter_qos_);
  publisher_->copy_from_topic_qos(datawriter_qos_, topic_qos_);

  if (Madara::Transport::RELIABLE == this->settings_.reliability)
  {
    datawriter_qos_.reliability.kind = DDS::RELIABLE_RELIABILITY_QOS;
    datawriter_qos_.history.depth = this->settings_.queue_length;
    datawriter_qos_.resource_limits.max_samples = this->settings_.queue_length;
    datawriter_qos_.resource_limits.max_samples_per_instance = 
      this->settings_.queue_length;
    datawriter_qos_.destination_order.kind = 
      DDS::BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS;
  }

  // Create Update writer
  datawriter_ = publisher_->create_datawriter (update_topic_, 
    datawriter_qos_, NULL, DDS::STATUS_MASK_NONE);
  check_handle(datawriter_, "DDS::Publisher::create_datawriter (Update)");
  update_writer_ = dynamic_cast<Knowledge::UpdateDataWriter_ptr> (datawriter_.in ());
  check_handle(update_writer_, "Knowledge::UpdateDataWriter_ptr::narrow");


  // Create datareader
  status = subscriber_->get_default_datareader_qos (datareader_qos_);
  subscriber_->copy_from_topic_qos (datareader_qos_, topic_qos_);
  //publisher_->copy_from_topic_qos(datawriter_qos_, topic_qos_);
  check_status(status, "DDS::Subscriber::get_default_datareader_qos");

  if (Madara::Transport::RELIABLE == this->settings_.reliability)
  {
    datareader_qos_.reliability.kind = DDS::RELIABLE_RELIABILITY_QOS;
    datareader_qos_.history.depth = this->settings_.queue_length;
    datareader_qos_.resource_limits.max_samples = this->settings_.queue_length;
    datareader_qos_.destination_order.kind = 
      DDS::BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS;

    // unlike the other qos, we do not set max_samples_per_instance here.
    // that shouldn't be as necessary, since we are using take on the reader
  }

  // Create Update datareader
  datareader_ = subscriber_->create_datareader (update_topic_, 
    datareader_qos_, &dr_listener_, DDS::STATUS_MASK_NONE);

  // notes: we set the mask to none because the listener will be called
  // by the subscriber listener with notify_datareaders. This is the Splice
  // way of doing this, since we require subscription information and they
  // have so far not implemented on_subscription_matched.

  check_handle(datareader_, "DDS::Subscriber::create_datareader (Update)");
  update_reader_ = dynamic_cast<Knowledge::UpdateDataReader_ptr>(datareader_.in ());
  check_handle(update_reader_, "Knowledge::UpdateDataReader_ptr::narrow");

  // Create Mutex datareader
  //datareader_ = subscriber_->create_datareader (mutex_topic_, 
  //  datareader_qos_, NULL, DDS::STATUS_MASK_NONE);
  //check_handle(datareader_, "DDS::Subscriber::create_datareader (Mutex)");
  //mutex_reader_ = dynamic_cast<Knowledge::MutexDataReader_ptr>(datareader_);
  //check_handle(mutex_reader_, "Knowledge::MutexDataReader_ptr::narrow");  

  //thread_ = new Madara::Transport::Splice_Read_Thread (id_, context_, 
  //  update_reader_);
  
  this->validate_transport ();

  return 0;
}

long
Madara::Transport::Splice_DDS_Transport::send_data (const std::string & key, 
                                               const long long & value)
{
  // check to see if we are shutting down
  long ret = this->check_transport ();
  if (-1 == ret)
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Splice_DDS_Transport::send_data: transport has been told to shutdown")); 
    return ret;
  }
  else if (-2 == ret)
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Splice_DDS_Transport::send_data: transport is not valid")); 
    return ret;
  }

  /// get current lamport clock. 
  unsigned long long cur_clock = context_.get_clock ();

  DDS::ReturnCode_t      dds_result;
  DDS::InstanceHandle_t  handle;

  Knowledge::Update data;
  data.key = key.c_str ();
  data.value = value;
  data.clock = cur_clock;
  data.quality = context_.get_write_quality (key);
  data.originator = id_.c_str ();
  data.type = Madara::Knowledge_Engine::ASSIGNMENT;

  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
    DLINFO "Splice_DDS_Transport::send:" \
    " sending data: %s=%q, time=%Q, quality=%u\n", 
    key.c_str (), data.value, cur_clock, data.quality));

  handle = update_writer_->register_instance (data);
  dds_result = update_writer_->write (data, handle); 
  //update_writer_->unregister_instance (data, handle);

  return dds_result;
}

long
Madara::Transport::Splice_DDS_Transport::send_multiassignment (
  const std::string & expression, unsigned long quality)
{
  // check to see if we are shutting down
  long ret = this->check_transport ();
  if (-1 == ret)
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Splice_DDS_Transport::send_multiassignment: transport is shutting down")); 
    return ret;
  }
  else if (-2 == ret)
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Splice_DDS_Transport::send_multiassignment: transport is not valid")); 
    return ret;
  }
  

  /// get current lamport clock. 
  unsigned long long cur_clock = context_.get_clock ();

  DDS::ReturnCode_t      dds_result;
  DDS::InstanceHandle_t  handle;

  Knowledge::Update data;
  data.key = expression.c_str ();
  data.value = 0;
  data.clock = cur_clock;
  data.quality = quality;
  data.originator = id_.c_str ();
  data.type = Madara::Knowledge_Engine::MULTIPLE_ASSIGNMENT;

  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
    DLINFO "Splice_DDS_Transport::send:" \
    " sending multiassignment: %s, time=%Q, quality=%u\n", 
    expression.c_str (), cur_clock, quality));

  handle = update_writer_->register_instance (data);
  dds_result = update_writer_->write (data, handle); 
  //update_writer_->unregister_instance (data, handle);

  return dds_result;
}

void
Madara::Transport::Splice_DDS_Transport::check_handle (void * handle, 
                                                      const char * info)
{
  if (!handle)
  {
    MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
      "\nSplice_DDS_Transport::check_handle:" \
      " error in %s: Creation failed: invalid handle\n", info));
    exit (-2);
  }
}

void
Madara::Transport::Splice_DDS_Transport::check_status (DDS::ReturnCode_t status,
                                                       const char * info)
{
  // if the status is okay, then return without issue
  if ((status == DDS::RETCODE_OK) || (status == DDS::RETCODE_NO_DATA)) 
    return;
  
  MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
      "\nSplice_DDS_Transport::check_status:" \
      " error in %s: Creation failed: %s\n", 
      info, get_error_name(status)));
  exit (-2);
}

const char *
Madara::Transport::Splice_DDS_Transport::get_error_name(DDS::ReturnCode_t status)
{
    return ret_code_names[status];
}
