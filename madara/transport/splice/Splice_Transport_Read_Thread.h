#ifndef SPLICE_TRANSPORT_READ_THREAD_H
#define SPLICE_TRANSPORT_READ_THREAD_H

#include <string>

#include "madara/knowledge_engine/Thread_Safe_Context.h"
#include "madara/transport/Transport.h"

#include "ccpp_dds_dcps.h"
#include "madara/transport/splice/ccpp_Splice_Knowledge_Update.h"

#include "ace/Task.h"
#include "ace/Mutex.h"
#include "ace/Barrier.h"
#include "ace/Atomic_Op_T.h"
#include "ace/Thread_Mutex.h"

#include "ace/Synch.h"

namespace Madara
{
  namespace Transport
  {
    class Splice_Read_Thread : public ACE_Task<ACE_MT_SYNCH>
    {
    public:
      Splice_Read_Thread (
        const std::string & id,
        Madara::Knowledge_Engine::Thread_Safe_Context & context, 
        Knowledge::UpdateDataReader_ptr & update_reader);
      ~Splice_Read_Thread ();

      /// service exit point for thread
      int enter_barrier (void);
      int close (void);
      int svc (void);
      void wait_for_ready (void);
    private:
      void handle_assignment (Knowledge::Update & data);
      void handle_multiassignment (Knowledge::Update & data);

      ::std::vector< ::std::string>                        assignment_symbols_;

      const std::string                                    id_;
      ::Madara::Knowledge_Engine::Thread_Safe_Context &    context_;
      ::Knowledge::UpdateDataReader_ptr                    update_reader_;
      /// typdef for a threadsafe counter
      ACE_Barrier barrier_;
      ACE_Atomic_Op<ACE_Mutex,bool>                        terminated_;

      //DDS::ConditionSeq_var              condition_list_;
      DDS::WaitSet                       waitset_;
      DDS::StatusCondition_ptr           condition_;
      ACE_Thread_Mutex                   mutex_;
      Madara::Transport::Condition       is_not_ready_;
      bool                               is_ready_;
    };
  }
}

#endif
