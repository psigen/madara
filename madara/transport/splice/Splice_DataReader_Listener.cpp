#include "madara/transport/splice/Splice_DataReader_Listener.h"
#include "madara/utility/Log_Macros.h"
#include "madara/knowledge_engine/Update_Types.h"
#include "madara/utility/Utility.h"

#include <iostream>
#include <sstream>

Madara::Transport::Splice_DataReader_Listener::Splice_DataReader_Listener(
  const std::string & id,
  Madara::Knowledge_Engine::Thread_Safe_Context & context)
: id_ (id), context_ (context)
{
}

Madara::Transport::Splice_DataReader_Listener::Splice_DataReader_Listener(
  const Splice_DataReader_Listener &ref)
: id_ (ref.id_), context_ (ref.context_)
{
}

Madara::Transport::Splice_DataReader_Listener::~Splice_DataReader_Listener()
{}

void Madara::Transport::Splice_DataReader_Listener::handle_assignment (
  Knowledge::Update & data)
{
  if (data.key.val ())
  {
    // if we aren't evaluating a message from ourselves, process it
    std::string key = data.key.val ();
    long long value = data.value;
    int result = 0;

    MADARA_DEBUG (MADARA_LOG_MAJOR_DEBUG_INFO, (LM_DEBUG, 
        DLINFO "Splice_DataReader_Listener::handle_assignment:" \
        " waiting to process assignment\n"));

    context_.lock ();
    unsigned long long cur_clock = context_.get_clock (key);
    unsigned long cur_quality = context_.get_quality (key);

    // if the data we are updating had a lower clock value or less quality
    // then that means this update is the latest value. Among
    // other things, this means our solution will work even
    // without FIFO channel transports

    // if the data we are updating had a lower clock value
    // then that means this update is the latest value. Among
    // other things, this means our solution will work even
    // without FIFO channel transports
    result = context_.set_if_unequal (key, value, 
                                      data.quality, data.clock, false);

    context_.unlock ();
    
    // if we actually updated the value
    if (result == 1)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Splice_DataReader_Listener::handle_assignment:" \
        " received data[%s]=%q from %s.\n", 
        key.c_str (), value, data.originator.val ()));
    }
    // if the data was already current
    else if (result == 0)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "Splice_DataReader_Listener::handle_assignment:" \
          " discarded data[%s]=%q from %s as the value was already set.\n",
          key.c_str (), value, data.originator.val ()));
    }
    else if (result == -1)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Splice_DataReader_Listener::handle_assignment:" \
        " discarded data due to null key.\n"));
    }
    else if (result == -2)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Splice_DataReader_Listener::handle_assignment:" \
        " discarded data[%s]=%q due to lower quality (%u vs %u).\n",
        key.c_str (), value, cur_quality, data.quality));
    }
    else if (result == -3)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Splice_DataReader_Listener::handle_assignment:" \
        " discarded data[%s]=%q due to older timestamp (%Q vs %Q).\n",
        key.c_str (), value, cur_clock, data.clock));
    }
  }
}

void Madara::Transport::Splice_DataReader_Listener::handle_multiassignment (
  Knowledge::Update & data)
{
  if (data.key.val ())
  {
    std::string key;
    char symbol;
    long long value;
    std::stringstream stream (data.key.val ());

    MADARA_DEBUG (MADARA_LOG_MAJOR_DEBUG_INFO, (LM_DEBUG, 
        DLINFO "Splice_DataReader_Listener::multiassignment:" \
        " waiting to process multiassignment\n"));

    context_.lock ();
    
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Splice_DataReader_Listener::multiassignment:" \
        " processing multiassignment (%s).\n",
          data.key.val ()));

    while (!stream.eof ())
    {
      stream >> key >> symbol >> value >> symbol;

      int result = 0;
      unsigned long long cur_clock = context_.get_clock (key);
      unsigned long cur_quality = context_.get_quality (key);

      // if the data we are updating had a lower clock value
      // then that means this update is the latest value. Among
      // other things, this means our solution will work even
      // without FIFO channel transports
      result = context_.set_if_unequal (key, value, 
                                        data.quality, data.clock, false);

      // if we actually updated the value
      if (result == 1)
      {
        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "Splice_DataReader_Listener::handle_multiassignment:" \
          " received data[%s]=%q from %s.\n",
          key.c_str (), value, data.originator.val ()));
      }
      // if the data was already current
      else if (result == 0)
      {
        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "Splice_DataReader_Listener::handle_multiassignment:" \
          " discarded data[%s]=%q from %s as the value was already set.\n",
          key.c_str (), value, data.originator.val ()));
      }
      else if (result == -1)
      {
        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "Splice_DataReader_Listener::handle_multiassignment:" \
          " discarded data due to null key.\n"));
      }
      else if (result == -2)
      {
        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "Splice_DataReader_Listener::handle_multiassignment:" \
          " discarded data[%s]=%q due to lower quality (%u vs %u).\n",
          key.c_str (), value, cur_quality, data.quality));
      }
      else if (result == -3)
      {
        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "Splice_DataReader_Listener::handle_multiassignment:" \
          " discarded data[%s]=%q due to older timestamp (%Q vs %Q).\n",
          key.c_str (), value, cur_clock, data.clock));
      }
    }
    
    context_.unlock ();
  }
}

void
Madara::Transport::Splice_DataReader_Listener::on_sample_lost (
  DDS::DataReader_ptr, const DDS::SampleLostStatus &status)
{
}

void
Madara::Transport::Splice_DataReader_Listener::on_sample_rejected (
  DDS::DataReader_ptr, const DDS::SampleRejectedStatus &status)
{
}

void
Madara::Transport::Splice_DataReader_Listener::on_requested_incompatible_qos (
  DDS::DataReader_ptr, const DDS::RequestedIncompatibleQosStatus &status)
{
}

void
Madara::Transport::Splice_DataReader_Listener::on_requested_deadline_missed (
  DDS::DataReader_ptr reader, const DDS::RequestedDeadlineMissedStatus & status)
{
}

void
Madara::Transport::Splice_DataReader_Listener::on_liveliness_changed (
  DDS::DataReader_ptr reader, const DDS::LivelinessChangedStatus & status)
{
}

void
Madara::Transport::Splice_DataReader_Listener::on_subscription_matched (
  DDS::DataReader_ptr reader, const DDS::SubscriptionMatchedStatus & status)
{
  context_.set_changed ();
}

void 
Madara::Transport::Splice_DataReader_Listener::on_data_available(
  DDS::DataReader_ptr reader)
{
  DDS::SampleInfoSeq_var infoList = new DDS::SampleInfoSeq;
  DDS::ReturnCode_t      dds_result;
  int                    amount;
  DDS::Boolean           result = false;
  Knowledge::UpdateSeq_var update_data_list_ = new Knowledge::UpdateSeq;

  Knowledge::UpdateDataReader_ptr update_reader = 
    dynamic_cast<Knowledge::UpdateDataReader_ptr> (reader);

  if (update_reader == 0)
  {
    MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
      "\nSplice_DataReader_Listener::on_data_available:" \
      " Unable to create specialized reader. Leaving callback...\n"));
    return;
  }

  dds_result = update_reader->take (update_data_list_, infoList, 1, 
    DDS::ANY_SAMPLE_STATE, DDS::ANY_VIEW_STATE, DDS::ANY_INSTANCE_STATE);

  amount = update_data_list_->length ();

  //ACE_DEBUG ((LM_DEBUG, "(%P|%t) Returning from take with %d items.\n",  
  //        amount));

  if (amount != 0)
  {
    for (int i = 0; i < amount; ++i)
    {
      // if we are evaluating a message from ourselves, just continue
      // to the next one. It's also possible to receive null originators
      // from what I can only guess is the ospl daemon messing up
      if (!update_data_list_[i].originator.val () || 
        id_ == update_data_list_[i].originator.val ())
      {
        // if we don't check originator for null, we get phantom sends
        // when the program exits.
        MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
          DLINFO "\nSplice_DataReader_Listener::on_data_available:" \
          " discarding null originator event.\n"));

        continue;
      }

      if (Madara::Knowledge_Engine::ASSIGNMENT == update_data_list_[i].type)
      {
        MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
          DLINFO "\nSplice_DataReader_Listener::on_data_available:" \
          " processing %s=%q from %s with time %Q and quality %u.\n", 
          update_data_list_[i].key.val (), update_data_list_[i].value, 
          update_data_list_[i].originator.val (),
          update_data_list_[i].clock, update_data_list_[i].quality));

        handle_assignment (update_data_list_[i]);
      }
      else if (Madara::Knowledge_Engine::MULTIPLE_ASSIGNMENT == update_data_list_[i].type)
      {
        MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
          DLINFO "\nSplice_DataReader_Listener::on_data_available:" \
          " processing multassignment from %s with time %Q and quality %u.\n",
          update_data_list_[i].originator.val (),
          update_data_list_[i].clock, update_data_list_[i].quality));

        handle_multiassignment (update_data_list_[i]);
      }

      // otherwise the key was null, which is unusable
    }
  }
  dds_result = update_reader->return_loan (update_data_list_, infoList);

}