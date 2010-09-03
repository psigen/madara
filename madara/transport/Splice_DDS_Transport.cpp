#include "madara/transport/Splice_DDS_Transport.h"
#include "madara/transport/Splice_Transport_Read_Thread.h"
#include "ace/Log_Msg.h"

#include <iostream>

const char * Madara::Transport::Splice_DDS_Transport::topic_names_[] = {
  "Madara_knowledge_best_effort",
  "Madara_knowledge_reliable"
};

/* Array to hold the names for all ReturnCodes. */
char * ret_code_names[13] = { 
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
  Madara::Knowledge_Engine::Thread_Safe_Context & context, const int & reliability)
: context_ (context), domain_ (NULL), domain_factory_ (NULL), 
  domain_participant_ (NULL), publisher_ (NULL), subscriber_ (NULL), datawriter_ (NULL), 
  datareader_ (NULL), update_writer_ (NULL), update_reader_ (NULL),
  update_data_list_ (new Knowledge::UpdateSeq), 
  update_status_condition_ (NULL), update_topic_ (NULL), thread_ (0),
  reliability_ (reliability), valid_setup_ (false)
{
  setup ();
}

Madara::Transport::Splice_DDS_Transport::~Splice_DDS_Transport ()
{
  close ();
}

void
Madara::Transport::Splice_DDS_Transport::close (void)
{
  if (subscriber_)
    subscriber_->delete_datareader (update_reader_);

  if (publisher_)
    publisher_->delete_datawriter (update_writer_);

  if (domain_participant_)
  {
    domain_participant_->delete_subscriber (subscriber_);
    domain_participant_->delete_publisher (publisher_);
    domain_participant_->delete_topic (update_topic_);
    domain_factory_->delete_participant (domain_participant_);
  }

  if (thread_)
  {
    thread_->close ();
    delete thread_;
  }

  thread_ = 0;
  update_reader_ = 0;
  update_writer_ = 0;
  subscriber_ = 0;
  publisher_ = 0;
  update_topic_ = 0;
  domain_participant_ = 0;
}

int
Madara::Transport::Splice_DDS_Transport::reliability (void) const
{
  return reliability_;
}

int
Madara::Transport::Splice_DDS_Transport::reliability (const int & setting)
{
  return reliability_ = setting;
}

int
Madara::Transport::Splice_DDS_Transport::setup (void)
{
  DDS::ReturnCode_t                         status;

  // reset the valid setup flag
  valid_setup_ = false;

  // get the domain participant factory
  domain_factory_ = DDS::DomainParticipantFactory::get_instance ();
  domain_factory_->get_default_participant_qos (part_qos_);
	domain_participant_ = domain_factory_->create_participant (domain_, 
    part_qos_, NULL, DDS::STATUS_MASK_NONE);

  // if dp == NULL, we've got an error
  if (domain_participant_ == NULL)
  {
    ACE_DEBUG ((LM_DEBUG, "ERROR - Splice Daemon not running\n"));
    return -1;
  }

  domain_participant_->get_default_topic_qos(topic_qos_);

  if (Madara::Transport::Splice_DDS_Transport::RELIABLE == reliability_)
  {
    topic_qos_.reliability.kind = DDS::RELIABLE_RELIABILITY_QOS;
    topic_qos_.history.depth = 100000;
    topic_qos_.resource_limits.max_samples = 100000;
    topic_qos_.destination_order.kind = DDS::BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS;
    //topic_qos_.
  }
  //topic_qos_.resource_limits.max_samples_per_instance= 10;
  domain_participant_->set_default_topic_qos(topic_qos_);

  //  Register type
  status = this->update_type_support_.register_type (
    domain_participant_, "Knowledge::Update");
  check_status(status, "Knowledge::UpdateTypeSupport::register_type");

  // Create topic
  update_topic_ = domain_participant_->create_topic (
    topic_names_ [reliability_], "Knowledge::Update", topic_qos_, NULL, DDS::STATUS_MASK_NONE);
  check_handle(update_topic_, "DDS::DomainParticipant::create_topic (Knowledge_Update)");


  // Get default qos for publisher
  status = domain_participant_->get_default_publisher_qos (pub_qos_);
  check_status(status, "DDS::DomainParticipant::get_default_publisher_qos");


  if (Madara::Transport::Splice_DDS_Transport::RELIABLE == reliability_)
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


  if (Madara::Transport::Splice_DDS_Transport::RELIABLE == reliability_)
  {
    sub_qos_.presentation.access_scope = DDS::TOPIC_PRESENTATION_QOS;
    sub_qos_.presentation.coherent_access = true;
    sub_qos_.presentation.ordered_access = false;
  }

  sub_qos_.partition.name.length (1);
  sub_qos_.partition.name[0] = DDS::string_dup (partition_);
  subscriber_ = domain_participant_->create_subscriber (
    sub_qos_, NULL, DDS::STATUS_MASK_NONE);
  check_handle(subscriber_, "DDS::DomainParticipant::create_subscriber");

  if (!subscriber_ || !publisher_)
  {
    ACE_DEBUG ((LM_DEBUG, 
       "ERROR - The publisher or subscriber could not be created.\n"));
    if (!subscriber_)
      return Madara::Transport::Splice_DDS_Transport::ERROR_SUB_BAD_CREATE;

    if (!publisher_)
      return Madara::Transport::Splice_DDS_Transport::ERROR_PUB_BAD_CREATE;
  }

  // Create datawriter
  publisher_->get_default_datawriter_qos (datawriter_qos_);
  publisher_->copy_from_topic_qos(datawriter_qos_, topic_qos_);

  if (Madara::Transport::Splice_DDS_Transport::RELIABLE == reliability_)
  {
    datawriter_qos_.reliability.kind = DDS::RELIABLE_RELIABILITY_QOS;
    datawriter_qos_.history.depth = 100000;
    datawriter_qos_.resource_limits.max_samples = 100000;
    datawriter_qos_.destination_order.kind = DDS::BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS;
  }

  datawriter_ = publisher_->create_datawriter (update_topic_, 
    datawriter_qos_, NULL, DDS::STATUS_MASK_NONE);
  check_handle(datawriter_, "DDS::Publisher::create_datawriter (Update)");
  update_writer_ = dynamic_cast<Knowledge::UpdateDataWriter_ptr> (datawriter_);
  check_handle(update_writer_, "Knowledge::UpdateDataWriter_ptr::narrow");

  // Create datareader
  status = subscriber_->get_default_datareader_qos (datareader_qos_);
  subscriber_->copy_from_topic_qos (datareader_qos_, topic_qos_);
  //publisher_->copy_from_topic_qos(datawriter_qos_, topic_qos_);
  check_status(status, "DDS::Subscriber::get_default_datareader_qos");

  if (Madara::Transport::Splice_DDS_Transport::RELIABLE == reliability_)
  {
    datareader_qos_.reliability.kind = DDS::RELIABLE_RELIABILITY_QOS;
    datareader_qos_.history.depth = 100000;
    datareader_qos_.resource_limits.max_samples = 100000;
    datareader_qos_.destination_order.kind = DDS::BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS;
  }
  datareader_ = subscriber_->create_datareader (update_topic_, 
    datareader_qos_, NULL, DDS::STATUS_MASK_NONE);
  check_handle(datareader_, "DDS::Subscriber::create_datareader (Update)");

  update_reader_ = dynamic_cast<Knowledge::UpdateDataReader_ptr>(datareader_);
  check_handle(update_reader_, "Knowledge::UpdateDataReader_ptr::narrow");

  thread_ = new Madara::Transport::Splice_Read_Thread (context_, update_reader_);

  valid_setup_ = true;

  return 0;
}

long
Madara::Transport::Splice_DDS_Transport::send (const std::string & key, 
                                               const long & value)
{
  if (!valid_setup_)
    return -1;

  DDS::ReturnCode_t      dds_result;
  DDS::InstanceHandle_t  handle;

  Knowledge::Update data;
  data.key = key.c_str ();
  data.value = value;

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) SENDING data: %s=%d\n", key.c_str (), data.value));

  //std::cout << "Sending data: " << key << "=" << value << std::endl;

  handle = update_writer_->register_instance (data);
  dds_result = update_writer_->write (data, handle); 

  

  return dds_result;
}

long
Madara::Transport::Splice_DDS_Transport::read (void)
{
  DDS::SampleInfoSeq_var infoList = new DDS::SampleInfoSeq;
  DDS::ReturnCode_t      dds_result;
  int                    amount;
  DDS::Boolean           result = false;

  dds_result = update_reader_->take (update_data_list_, infoList, 1, 
    DDS::ANY_SAMPLE_STATE, DDS::ANY_VIEW_STATE, DDS::ANY_INSTANCE_STATE);

  amount = update_data_list_->length ();
  if (amount != 0)
  {

  }

  return 0;
}

void
Madara::Transport::Splice_DDS_Transport::check_handle (void * handle, 
                                                      char * info)
{
  if (!handle)
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Error in %s: Creation failed: invalid handle\n", info));
}

void
Madara::Transport::Splice_DDS_Transport::check_status (DDS::ReturnCode_t status,
                                                       const char * info)
{
  // if the status is okay, then return without issue
  if ((status == DDS::RETCODE_OK) || (status == DDS::RETCODE_NO_DATA)) 
    return;
  
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Error in %s: Creation failed: %s\n", 
      info, get_error_name(status)));
}

char *
Madara::Transport::Splice_DDS_Transport::get_error_name(DDS::ReturnCode_t status)
{
    return ret_code_names[status];
}
