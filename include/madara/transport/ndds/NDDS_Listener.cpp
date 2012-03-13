#include "madara/transport/ndds/NDDS_Listener.h"
#include "madara/utility/Log_Macros.h"
#include "madara/knowledge_engine/Update_Types.h"
#include "madara/utility/Utility.h"

#include <sstream>

Madara::Transport::NDDS_Listener::NDDS_Listener(
  const std::string & id,
  Madara::Knowledge_Engine::Thread_Safe_Context & context)
: id_ (id), context_ (context)
{
}

Madara::Transport::NDDS_Listener::NDDS_Listener(const NDDS_Listener &ref)
: id_ (ref.id_), context_ (ref.context_)
{
}

Madara::Transport::NDDS_Listener::~NDDS_Listener()
{}

void Madara::Transport::NDDS_Listener::handle_assignment (
  NDDS_Knowledge_Update & data)
{
  if (data.key)
  {
    // if we aren't evaluating a message from ourselves, process it
    std::string key = data.key;
    long long value = data.value;
    int result = 0;

    MADARA_DEBUG (MADARA_LOG_MAJOR_DEBUG_INFO, (LM_DEBUG, 
        DLINFO "NDDS_Read_Thread::handle_assignment:" \
        " waiting to process assignment\n"));

    context_.lock ();

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
    context_.set_changed ();
    
    // if we actually updated the value
    if (result == 1)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "NDDS_Read_Thread::handle_assignment:" \
        " received data[%s]=%q from %s.\n", 
        key.c_str (), value, data.originator));
    }
    // if the data was already current
    else if (result == 0)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "NDDS_Read_Thread::handle_assignment:" \
          " discarded data[%s]=%q from %s as the value was already set.\n",
          key.c_str (), value, data.originator));
    }
    else if (result == -1)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "NDDS_Read_Thread::handle_assignment:" \
        " discarded data due to null key.\n"));
    }
    else if (result == -2)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "NDDS_Read_Thread::handle_assignment:" \
        " discarded data[%s]=%q due to lower quality.\n",
        key.c_str (), value));
    }
    else if (result == -3)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "NDDS_Read_Thread::handle_assignment:" \
        " discarded data[%s]=%q due to older timestamp.\n",
        key.c_str (), value));
    }
  }
}

void Madara::Transport::NDDS_Listener::handle_multiassignment (
  NDDS_Knowledge_Update & data)
{
  if (data.key)
  {
    long long value;

    MADARA_DEBUG (MADARA_LOG_MAJOR_DEBUG_INFO, (LM_DEBUG, 
        DLINFO "NDDS_Read_Thread::multiassignment:" \
        " waiting to process multiassignment\n"));

    std::vector <std::string> tokens, pivots, splitters;

    splitters.resize (2);
    splitters[0] = "=";
    splitters[1] = ";";

    context_.lock ();
    
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "NDDS_Read_Thread::multiassignment:" \
        " processing multiassignment (%s).\n",
          data.key));

    Madara::Utility::tokenizer (data.key, splitters, tokens, pivots);

    for (unsigned int i = 0; i + 1 < tokens.size (); i+=2)
    {
      std::string key (tokens[i]);
      std::stringstream buffer (tokens[i + 1]);
      buffer >> value;

      int result = 0;
      //unsigned long long cur_clock = context_.get_clock (key);
      //unsigned long cur_quality = context_.get_quality (key);

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
          DLINFO "NDDS_Read_Thread::handle_multiassignment:" \
          " received data[%s]=%q from %s.\n",
          key.c_str (), value, data.originator));
      }
      // if the data was already current
      else if (result == 0)
      {
        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "NDDS_Read_Thread::handle_multiassignment:" \
          " discarded data[%s]=%q from %s as the value was already set.\n",
          key.c_str (), value, data.originator));
      }
      else if (result == -1)
      {
        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "NDDS_Read_Thread::handle_multiassignment:" \
          " discarded data due to null key.\n"));
      }
      else if (result == -2)
      {
        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "NDDS_Read_Thread::handle_multiassignment:" \
          " discarded data[%s]=%q due to lower quality.\n",
          key.c_str (), value));
      }
      else if (result == -3)
      {
        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "NDDS_Read_Thread::handle_multiassignment:" \
          " discarded data[%s]=%q due to older timestamp.\n",
          key.c_str (), value));
      }
    }
    
    context_.unlock ();
    context_.set_changed ();
  }
}

void
Madara::Transport::NDDS_Listener::on_subscription_matched (
  DDSDataReader* reader, const DDS_SubscriptionMatchedStatus& status)
{
  context_.set_changed ();
}

void 
Madara::Transport::NDDS_Listener::on_data_available(DDSDataReader * reader)
{
  NDDS_Knowledge_UpdateSeq update_data_list;
  DDS_SampleInfoSeq info_seq;
  DDS_ReturnCode_t rc;

  NDDS_Knowledge_UpdateDataReader * update_reader = 
    NDDS_Knowledge_UpdateDataReader::narrow(reader);
  if (update_reader == NULL)
  {
    MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
      "\nNDDS_Listener::svc:" \
      " Unable to create specialized reader. Leaving callback...\n"));
    return;
  }

  rc = update_reader->take(
    update_data_list, 
    info_seq, 
    DDS_LENGTH_UNLIMITED,
    DDS_ANY_SAMPLE_STATE, 
    DDS_ANY_VIEW_STATE, 
    DDS_ANY_INSTANCE_STATE);

  if (rc == DDS_RETCODE_NO_DATA)
  {
    return;
  }
  else if (rc != DDS_RETCODE_OK)
  {
    MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
      DLINFO "NDDS_Listener::svc:" \
      " could not take current sample.\n"));
    return;
  }

  for (int i = 0; i < update_data_list.length(); ++i) {
    if (info_seq[i].valid_data) {
      if (Madara::Knowledge_Engine::ASSIGNMENT == update_data_list[i].type)
      {
        MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
          DLINFO "NDDS_Listener::svc:" \
          " processing %s=%q from %s with time %Q and quality %u.\n", 
          update_data_list[i].key, update_data_list[i].value, 
          update_data_list[i].originator,
          update_data_list[i].clock, update_data_list[i].quality));

        handle_assignment (update_data_list[i]);
      }
      else if (Madara::Knowledge_Engine::MULTIPLE_ASSIGNMENT == update_data_list[i].type)
      {
        MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
          DLINFO "NDDS_Listener::svc:" \
          " processing multassignment from %s with time %Q and quality %u.\n",
          update_data_list[i].originator,
          update_data_list[i].clock, update_data_list[i].quality));

        handle_multiassignment (update_data_list[i]);
      }        
    }
  }

  rc = update_reader->return_loan(update_data_list, info_seq);
  if (rc != DDS_RETCODE_OK)
  {
    MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
      DLINFO "NDDS_Listener::svc:" \
      " could return DDS sample instance loan.\n"));
  }
}