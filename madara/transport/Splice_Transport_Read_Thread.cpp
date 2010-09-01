#include "madara/transport/Splice_Transport_Read_Thread.h"
#include "ace/Log_Msg.h"

#include <iostream>

Madara::Transport::Splice_Read_Thread::Splice_Read_Thread (
  Madara::Knowledge_Engine::Thread_Safe_Context & context, Knowledge::UpdateDataReader_ptr & update_reader)
  : context_ (context), update_reader_ (update_reader), barrier_ (2), terminated_ (false)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Splice_Read_Thread started\n"));
  
  // Add datareader statuscondition to waitset
  condition_ = update_reader_->get_statuscondition ();
  condition_->set_enabled_statuses (DDS::DATA_AVAILABLE_STATUS);
  waitset_.attach_condition (condition_);

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


int Madara::Transport::Splice_Read_Thread::enter_barrier ()
{ 
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) entering barrier.\n"));
 
  barrier_.wait ();

  return 0;
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

    dds_result = update_reader_->take (update_data_list_, infoList, 1, 
      DDS::ANY_SAMPLE_STATE, DDS::ANY_VIEW_STATE, DDS::ANY_INSTANCE_STATE);

    amount = update_data_list_->length ();
    if (amount != 0)
    {
      for (int i = 0; i < amount; ++i)
      {
        std::string key = update_data_list_[i].key.val ();
        long value = update_data_list_[i].value;

        int result = context_.set_if_unequal (key, value);

        // if we actually updated the value
        if (result == 1)
        {
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) RECEIVED data[%s]=%d.\n", 
            key.c_str (), value));
        }
        // if the data was already current
        else if (result == 0)
        {
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) DISCARDED data[%s] was already %d.\n", 
            key.c_str (), value));
        }
        // otherwise the key was null, which is unusable
      }
    }
    dds_result = update_reader_->return_loan (update_data_list_, infoList);
  }

  enter_barrier ();
  return 0;
}