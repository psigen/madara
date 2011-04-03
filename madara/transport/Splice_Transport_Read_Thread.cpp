#include "madara/transport/Splice_Transport_Read_Thread.h"
#include "ace/Log_Msg.h"
#include "madara/knowledge_engine/Update_Types.h"
#include "madara/utility/Utility.h"

#include <iostream>
#include <sstream>

Madara::Transport::Splice_Read_Thread::Splice_Read_Thread (
  const std::string & id,
  Madara::Knowledge_Engine::Thread_Safe_Context & context, 
  Knowledge::UpdateDataReader_ptr & update_reader, 
  Knowledge::MutexDataReader_ptr & mutex_reader, bool enable_mutexing)
  : id_ (id), context_ (context), update_reader_ (update_reader), 
    mutex_reader_ (mutex_reader), 
    barrier_ (2), terminated_ (false), 
    mutex_ (), is_not_ready_ (mutex_), is_ready_ (false),
    enable_mutexing_ (enable_mutexing)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Splice_Read_Thread started\n"));
  
  assignment_symbols_.push_back ("=");
  assignment_symbols_.push_back (";");

  // Add update datareader statuscondition to waitset
  condition_ = update_reader_->get_statuscondition ();
  condition_->set_enabled_statuses (DDS::DATA_AVAILABLE_STATUS);
  waitset_.attach_condition (condition_);

  if (enable_mutexing)
  {
    // Add mutex datareader statuscondition to waitset
    condition_ = mutex_reader_->get_statuscondition ();
    condition_->set_enabled_statuses (DDS::DATA_AVAILABLE_STATUS);
    waitset_.attach_condition (condition_);
  }
  this->activate (THR_NEW_LWP | THR_DETACHED, 1);
}

Madara::Transport::Splice_Read_Thread::~Splice_Read_Thread ()
{
}

int
Madara::Transport::Splice_Read_Thread::close (void)
{
  terminated_ = true;
  enter_barrier ();

  return 0;
}

void
Madara::Transport::Splice_Read_Thread::wait_for_ready (void)
{
  if (!is_ready_)
    is_not_ready_.wait ();
}

int Madara::Transport::Splice_Read_Thread::enter_barrier (void)
{ 
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) entering barrier.\n"));
 
  barrier_.wait ();

  return 0;
}

void Madara::Transport::Splice_Read_Thread::handle_assignment (
  Knowledge::Update & data)
{
  if (data.key.val ())
  {
    // if we aren't evaluating a message from ourselves, process it
    std::string key = data.key.val ();
    long long value = data.value;
    int result = 0;

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
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) RECEIVED data[%s]=%d from %s.\n", 
        key.c_str (), value, data.originator.val ()));
    }
    // if the data was already current
    else if (result == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) DISCARDED data[%s] was already %d.\n", 
        key.c_str (), value));
    }
    else if (result == -1)
    {
      ACE_DEBUG ((LM_DEBUG, 
        "(%P|%t) DISCARDED data had invalid key (null variable name).\n", 
        key.c_str (), cur_quality, data.quality));
    }
    else if (result == -2)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) DISCARDED data[%s] was low quality (%d vs %d).\n", 
        key.c_str (), cur_quality, data.quality));
    }
    else if (result == -3)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) DISCARDED data[%s] was old (%d vs %d).\n", 
        key.c_str (), cur_clock, data.clock));
    }
  }
}

void Madara::Transport::Splice_Read_Thread::handle_multiassignment (
  Knowledge::Update & data)
{
  if (data.key.val ())
  {
    std::string key;
    char symbol;
    long long value;
    std::stringstream stream (data.key.val ());

    context_.lock ();
    
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Processing multiassignment (%s).\n", 
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
        ACE_DEBUG ((LM_DEBUG, "(%P|%t) RECEIVED data[%s]=%d from %s.\n", 
          key.c_str (), value, data.originator.val ()));
      }
      // if the data was already current
      else if (result == 0)
      {
        ACE_DEBUG ((LM_DEBUG, "(%P|%t) DISCARDED data[%s] was already %d.\n", 
          key.c_str (), value));
      }
      else if (result == -1)
      {
        ACE_DEBUG ((LM_DEBUG, 
          "(%P|%t) DISCARDED data had invalid key (null variable name).\n", 
          key.c_str (), cur_quality, data.quality));
      }
      else if (result == -2)
      {
        ACE_DEBUG ((LM_DEBUG, "(%P|%t) DISCARDED data[%s] was low quality (%d vs %d).\n", 
          key.c_str (), cur_quality, data.quality));
      }
      else if (result == -3)
      {
        ACE_DEBUG ((LM_DEBUG, "(%P|%t) DISCARDED data[%s] was old (%d vs %d).\n", 
          key.c_str (), cur_clock, data.clock));
      }

    }
    
    context_.unlock ();
  }
}

int
Madara::Transport::Splice_Read_Thread::svc (void)
{
  DDS::SampleInfoSeq_var infoList = new DDS::SampleInfoSeq;
  DDS::ReturnCode_t      dds_result;
  int                    amount;
  DDS::Boolean           result = false;
  Knowledge::UpdateSeq_var update_data_list_ = new Knowledge::UpdateSeq;

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Entering Splice_Read_Thread::svc.\n"));

  ::DDS::Duration_t wait_time;
  wait_time.sec = 3;
  wait_time.nanosec = 0;

  while (false == terminated_.value ())
  {
    // by using conditionals, we can wait for a message for a specific time limit
    // the conditionList would tell us which particular conditions were met, but
    // since we've only set up the wait
    DDS::ConditionSeq_var conditionList = new DDS::ConditionSeq();
    result = waitset_.wait (conditionList.inout (), wait_time);

    if (!is_ready_)
    {
      is_ready_ = true;
      is_not_ready_.broadcast ();
    }
    //ACE_DEBUG ((LM_DEBUG, "(%P|%t) Read thread take.\n"));

    dds_result = update_reader_->take (update_data_list_, infoList, 1, 
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
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) Discarding data from %s.\n", 
            update_data_list_[i].originator.val ()));
          continue;
        }

        if (Madara::Knowledge_Engine::ASSIGNMENT == update_data_list_[i].type)
        {
          ACE_DEBUG ((LM_DEBUG, 
            "(%P|%t) Processing %s=%d from %s with time %d and quality %d.\n", 
            update_data_list_[i].key.val (), update_data_list_[i].value, 
            update_data_list_[i].originator.val (),
            update_data_list_[i].clock, update_data_list_[i].quality));
          handle_assignment (update_data_list_[i]);
        }
        else if (Madara::Knowledge_Engine::MULTIPLE_ASSIGNMENT == update_data_list_[i].type)
        {
          ACE_DEBUG ((LM_DEBUG, 
            "(%P|%t) Processing multiassignment from %s with time %d and quality %d.\n", 
            update_data_list_[i].originator.val (),
            update_data_list_[i].clock, update_data_list_[i].quality));
          handle_multiassignment (update_data_list_[i]);
        }

        // otherwise the key was null, which is unusable
      }
    }
    dds_result = update_reader_->return_loan (update_data_list_, infoList);
  }

  enter_barrier ();
  return 0;
}