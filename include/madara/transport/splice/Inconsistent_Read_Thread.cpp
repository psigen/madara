#include "madara/transport/splice/Inconsistent_Read_Thread.h"
#include "madara/utility/Log_Macros.h"
#include "madara/knowledge_engine/Update_Types.h"
#include "madara/utility/Utility.h"

#include <iostream>
#include <sstream>

Madara::Transport::Inconsistent_Read_Thread::Inconsistent_Read_Thread (
  const std::string & id,
  Madara::Knowledge_Engine::Thread_Safe_Context & context, 
  Knowledge::UpdateDataReader_ptr & update_reader,
  Knowledge::UpdateDataWriter_ptr & update_writer,
  Madara::Transport::Settings & settings)
  : id_ (id), context_ (context), update_reader_ (update_reader),
    update_writer_ (update_writer),
    barrier_ (2), terminated_ (false), 
    mutex_ (), is_not_ready_ (mutex_), settings_ (settings), is_ready_ (false)
{
  assignment_symbols_.push_back ("=");
  assignment_symbols_.push_back (";");

  this->activate (THR_NEW_LWP | THR_DETACHED, 1);
  
  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
    DLINFO "Inconsistent_Read_Thread::Inconsistent_Read_Thread:" \
    " read thread started\n"));
}

Madara::Transport::Inconsistent_Read_Thread::~Inconsistent_Read_Thread ()
{
  close ();
}

int
Madara::Transport::Inconsistent_Read_Thread::close (void)
{
  terminated_ = true;
  enter_barrier ();

  return 0;
}

void
Madara::Transport::Inconsistent_Read_Thread::wait_for_ready (void)
{
  if (!is_ready_)
    is_not_ready_.wait ();
}

int Madara::Transport::Inconsistent_Read_Thread::enter_barrier (void)
{ 
  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
    DLINFO "Inconsistent_Read_Thread::enter_barrier:" \
    " beginning thread barrier for shut down of read thread\n"));

  barrier_.wait ();
  return 0;
}

#ifdef _USE_CID_

/**
 * originator == person who started the latency rounds
 * key == the person replying to the round
 * clock == the time the round was started
 * value == the step in the round (0 = initial, 1 = reply, 2 = reply to reply)
 **/

void Madara::Transport::Inconsistent_Read_Thread::handle_latency (
  Knowledge::Update & data)
{
  Madara::Transport::Settings::Context_Guard guard (settings_.mutex);
  /**
   * During value == 0 (initial), the originator called start_latency.
   **/
  if (data.value == 0)
  {
    if (settings_.latencies.ids[data.quality] != data.originator.val ())
      settings_.latencies.ids[data.quality] = data.originator.val ();

    // originator doesn't respond to itself
    if (data.originator.val () == id_)
    {
      settings_.num_responses = 1;
      settings_.stop_timer (data.quality, false);
      return;
    }
    else
      settings_.num_responses = 0;

    // everyone else needs to respond though

    Knowledge::Update reply_data;
    reply_data.key = id_.c_str ();
    reply_data.value = 1;
    reply_data.clock = data.clock;
    reply_data.quality = this->settings_.id;
    reply_data.originator = data.originator.val ();
    reply_data.type = Madara::Transport::LATENCY;

    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Splice_DDS_Transport::handle_latency:" \
      " originator=%s, replier=%s, time=%Q, step=%q\n", 
      reply_data.originator.val (), id_.c_str (),
      reply_data.clock, reply_data.value));

    settings_.start_all_timers ();

    update_writer_->write (reply_data, 0); 
  }

  /**
   * During value == 1 (reply), the other participants are replying to the orig
   **/
  else if (data.value == 1)
  {
    if (settings_.latencies.ids[data.quality] != data.key.val ())
      settings_.latencies.ids[data.quality] = data.key.val ();

    // key doesn't respond to itself
    if (data.key.val () == id_)
    {
      settings_.stop_timer (data.quality, false);
      return;
    }

    // if we're the originator, send a reply then record our latency
    if (data.originator.val () == id_)
    {
      // stop stored timer
      settings_.stop_timer (data.quality);
      ++settings_.num_responses;
    }

    // everyone sends a response to the key as the orig
    Knowledge::Update reply_data;
    reply_data.key = id_.c_str ();
    reply_data.value = 2;
    reply_data.clock = data.clock;
    reply_data.quality = this->settings_.id;
    reply_data.originator = data.key.val ();
    reply_data.type = Madara::Transport::LATENCY;

    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Splice_DDS_Transport::handle_latency:" \
      " originator=%s, replier=%s, time=%Q, step=%q\n", 
      reply_data.originator.val (), id_.c_str (),
      reply_data.clock, reply_data.value));

    update_writer_->write (reply_data, 0); 
  }

  /**
   * During value == 2 (reply-reply), people are replying to a reply
   * The only person who cares about this is the originator in the data
   **/
  else if (data.value == 2 && data.originator.val () == id_)
  {
    // stop the timer and store the latency
    settings_.stop_timer (data.quality);
    ++settings_.num_responses;

    if (settings_.latencies.ids[data.quality] != data.key.val ())
      settings_.latencies.ids[data.quality] = data.key.val ();
  }
}

#endif // #ifdef _USE_CID_

void Madara::Transport::Inconsistent_Read_Thread::handle_assignment (
  Knowledge::Update & data)
{
  if (data.key.val ())
  {
    // if we aren't evaluating a message from ourselves, process it
    std::string key = data.key.val ();
    long long value = data.value;
    int result = 0;

    MADARA_DEBUG (MADARA_LOG_MAJOR_DEBUG_INFO, (LM_DEBUG, 
        DLINFO "Inconsistent_Read_Thread::handle_assignment:" \
        " waiting to process assignment\n"));

    context_.lock ();

    unsigned long long clock = context_.inc_clock ();

    // if the data we are updating had a lower clock value or less quality
    // then that means this update is the latest value. Among
    // other things, this means our solution will work even
    // without FIFO channel transports

    // if the data we are updating had a lower clock value
    // then that means this update is the latest value. Among
    // other things, this means our solution will work even
    // without FIFO channel transports
    result = context_.set_if_unequal (key, value, 
                                      0, clock, false);

    context_.unlock ();
    context_.set_changed ();
    
    // if we actually updated the value
    if (result == 1)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Inconsistent_Read_Thread::handle_assignment:" \
        " received data[%s]=%q from %s.\n", 
        key.c_str (), value, data.originator.val ()));
    }
    // if the data was already current
    else if (result == 0)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "Inconsistent_Read_Thread::handle_assignment:" \
          " discarded data[%s]=%q from %s as the value was already set.\n",
          key.c_str (), value, data.originator.val ()));
    }
    else if (result == -1)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Inconsistent_Read_Thread::handle_assignment:" \
        " discarded data due to null key.\n"));
    }
    else if (result == -2)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Inconsistent_Read_Thread::handle_assignment:" \
        " discarded data[%s]=%q due to lower quality.\n",
        key.c_str (), value));
    }
    else if (result == -3)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Inconsistent_Read_Thread::handle_assignment:" \
        " discarded data[%s]=%q due to older timestamp.\n",
        key.c_str (), value));
    }
  }
}

void Madara::Transport::Inconsistent_Read_Thread::handle_multiassignment (
  Knowledge::Update & data)
{
  if (data.key.val ())
  {
    std::string key;
    char symbol;
    long long value;
    std::stringstream stream (data.key.val ());

    MADARA_DEBUG (MADARA_LOG_MAJOR_DEBUG_INFO, (LM_DEBUG, 
        DLINFO "Inconsistent_Read_Thread::multiassignment:" \
        " waiting to process multiassignment\n"));

    context_.lock ();
    
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Inconsistent_Read_Thread::multiassignment:" \
        " processing multiassignment (%s).\n",
          data.key.val ()));

    unsigned long long cur_clock = context_.inc_clock ();
    unsigned long cur_quality = 0;


    while (!stream.eof ())
    {
      stream >> key >> symbol >> value >> symbol;

      int result = 0;

      // if the data we are updating had a lower clock value
      // then that means this update is the latest value. Among
      // other things, this means our solution will work even
      // without FIFO channel transports
      result = context_.set_if_unequal (key, value, 
                                        0, cur_clock, false);

      // if we actually updated the value
      if (result == 1)
      {
        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "Inconsistent_Read_Thread::handle_multiassignment:" \
          " received data[%s]=%q from %s.\n",
          key.c_str (), value, data.originator.val ()));
      }
      // if the data was already current
      else if (result == 0)
      {
        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "Inconsistent_Read_Thread::handle_multiassignment:" \
          " discarded data[%s]=%q from %s as the value was already set.\n",
          key.c_str (), value, data.originator.val ()));
      }
      else if (result == -1)
      {
        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "Inconsistent_Read_Thread::handle_multiassignment:" \
          " discarded data due to null key.\n"));
      }
      else if (result == -2)
      {
        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "Inconsistent_Read_Thread::handle_multiassignment:" \
          " discarded data[%s]=%q due to lower quality.\n",
          key.c_str (), value));
      }
      else if (result == -3)
      {
        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "Inconsistent_Read_Thread::handle_multiassignment:" \
          " discarded data[%s]=%q due to older timestamp.\n",
          key.c_str (), value));
      }
    }
    
    context_.unlock ();
    context_.set_changed ();
  }
}

int
Madara::Transport::Inconsistent_Read_Thread::svc (void)
{
  DDS::SampleInfoSeq_var infoList = new DDS::SampleInfoSeq;
  DDS::ReturnCode_t      dds_result;
  int                    amount;
  DDS::Boolean           result = false;
  Knowledge::UpdateSeq_var update_data_list_ = new Knowledge::UpdateSeq;

  DDS::WaitSet                   waitset_;
  DDS::StatusCondition_ptr           condition_;
  // Add update datareader statuscondition to waitset
  condition_ = update_reader_->get_statuscondition ();
  condition_->set_enabled_statuses (DDS::DATA_AVAILABLE_STATUS);
  waitset_.attach_condition (condition_);

  ::DDS::Duration_t wait_time;
  wait_time.sec = 3;
  wait_time.nanosec = 0;

  // if we don't check originator for null, we get phantom sends
  // when the program exits.
  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
    DLINFO "Inconsistent_Read_Thread::svc:" \
    " entering processing loop.\n"));

  while (false == terminated_.value ())
  {
    // by using conditionals, we can wait for a message for a specific time limit
    // the conditionList would tell us which particular conditions were met, but
    // since we've only set up the wait
    DDS::ConditionSeq_var conditionList = new DDS::ConditionSeq();
    result = waitset_.wait (conditionList.inout (), wait_time);

    if (!is_ready_)
    {
      MADARA_DEBUG (MADARA_LOG_TRACE, (LM_DEBUG, 
        DLINFO "Inconsistent_Read_Thread::svc:" \
        " waking up anyone waiting for read thread.\n"));

      is_ready_ = true;
      is_not_ready_.broadcast ();
    }
    //ACE_DEBUG ((LM_DEBUG, "(%P|%t) Read thread take.\n"));

    MADARA_DEBUG (MADARA_LOG_TRACE, (LM_DEBUG, 
      DLINFO "Inconsistent_Read_Thread::svc:" \
      " entering a take on the DDS reader.\n"));

    dds_result = update_reader_->take (update_data_list_, infoList, 20, 
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
        if (!update_data_list_[i].originator.val ())
        {
          // if we don't check originator for null, we get phantom sends
          // when the program exits.
          MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
            DLINFO "Inconsistent_Read_Thread::svc:" \
            " discarding null originator event.\n"));

          continue;
        }

#ifdef _USE_CID_
        // a latency packet can have the same originator val
        if (Madara::Transport::LATENCY == update_data_list_[i].type)
        {
          MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
            DLINFO "Inconsistent_Read_Thread::svc:" \
            " processing latency from %s with time %Q and quality %u.\n",
            update_data_list_[i].originator.val (),
            update_data_list_[i].clock, update_data_list_[i].quality));

          handle_latency (update_data_list_[i]);
        }
        // everything else cannot
        else
          
#endif // #ifdef _USE_CID_

        if (id_ != update_data_list_[i].originator.val ())
        {
          if (Madara::Transport::ASSIGN == update_data_list_[i].type)
          {
            MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
              DLINFO "Inconsistent_Read_Thread::svc:" \
              " processing %s=%q from %s with time %Q and quality %u.\n", 
              update_data_list_[i].key.val (), update_data_list_[i].value, 
              update_data_list_[i].originator.val (),
              update_data_list_[i].clock, update_data_list_[i].quality));

            handle_assignment (update_data_list_[i]);
          }
          else if (Madara::Transport::MULTIASSIGN == update_data_list_[i].type)
          {
            MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
              DLINFO "Inconsistent_Read_Thread::svc:" \
              " processing multassignment from %s with time"
              " %Q and quality %u.\n",
              update_data_list_[i].originator.val (),
              update_data_list_[i].clock, update_data_list_[i].quality));

            handle_multiassignment (update_data_list_[i]);
          }
        }

        // otherwise the key was null, which is unusable
      }
    }
    dds_result = update_reader_->return_loan (update_data_list_, infoList);
  }

  enter_barrier ();
  return 0;
}
