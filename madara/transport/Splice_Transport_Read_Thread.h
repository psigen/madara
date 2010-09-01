#ifndef SPLICE_TRANSPORT_READ_THREAD_H
#define SPLICE_TRANSPORT_READ_THREAD_H

#include "madara/knowledge_engine/Thread_Safe_Context.h"


#include "ccpp_dds_dcps.h"
#include "madara/ccpp_Splice_Knowledge_Update.h"

#include "ace/Task.h"
#include "ace/Mutex.h"
#include "ace/Barrier.h"
#include "ace/Atomic_Op_T.h"

namespace Madara
{
  namespace Transport
  {
    class Splice_Read_Thread : public ACE_Task<ACE_MT_SYNCH>
    {
    public:
      Splice_Read_Thread (
        Madara::Knowledge_Engine::Thread_Safe_Context & context, 
        Knowledge::UpdateDataReader_ptr & update_reader);
      ~Splice_Read_Thread ();

      /// service exit point for thread
      int enter_barrier (void);
      int close (void);
      int svc (void);
    private:
      Thread_Safe_Context &               context_;
      ::Knowledge::UpdateDataReader_ptr   update_reader_;
      /// typdef for a threadsafe counter
      ACE_Atomic_Op<ACE_Mutex,bool> terminated_;
      ACE_Barrier barrier_;

      //DDS::ConditionSeq_var              condition_list_;
      DDS::WaitSet                       waitset_;
      DDS::StatusCondition_ptr           condition_;
    };
  }
}

#endif