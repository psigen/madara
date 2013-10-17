#ifndef SPLICE_TRANSPORT_READ_THREAD_H
#define SPLICE_TRANSPORT_READ_THREAD_H

/**
 * @file Splice_Transport_Read_Thread.h
 * @author James Edmondson <james.r.edmondson@vanderbilt.edu>
 *
 * This file contains the implementation of the container for reading
 * DDS updates via Splice waitsets
 */

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
    /**
     * @class Splice_Read_Thread
     * @brief Thread for reading knowledge updates via waitsets
     **/
    class Splice_Read_Thread : public ACE_Task<ACE_MT_SYNCH>
    {
    public:
      /**
       * Constructor
       * @param id unique identifier of this participant (e.g., host:port)
       * @param context where to store and access knowledge
       * @param update_reader reader created in @see Splice_DDS_Transport
       * @param update_writer writer created in @see Splice_DDS_Transport
       * @param    settings   Transport settings
       **/
      Splice_Read_Thread (const std::string & id,
        const Settings & settings,
        Madara::Knowledge_Engine::Thread_Safe_Context & context, 
        Knowledge::UpdateDataReader_ptr & update_reader,
        Knowledge::UpdateDataWriter_ptr & update_writer);

      /**
       * Destructor
       **/
      ~Splice_Read_Thread ();

      /**
       * Enters a barrier with the calling thread and any spawned threads
       **/
      int enter_barrier (void);

      /**
       * Closes this thread
       **/
      int close (void);

      /**
       * Main loop of the thread
       **/
      int svc (void);

      /**
       * Waits for the transport to be validated
       **/
      void wait_for_ready (void);
    private:
      /**
       * Handles an assignment update
       * @param  data  the update that was made
       **/
      void handle_assignment (Knowledge::Update & data);
 
#ifdef _USE_CID_
      /**
       * Handles a latency operation
       * @param  data  the update that was made
       **/
      void handle_latency (Knowledge::Update & data);

      /**
       * Handles a latency aggregation
       * @param  data  the aggregation that was made
       **/
      void handle_latency_aggregation (Knowledge::Update & data);

      /**
       * Handles a latency summation
       * @param  data  the summation that was made
       **/
      void handle_latency_summation (Knowledge::Update & data);

      /**
       * Handles a vote operation
       * @param  data  the vote that was made
       **/
      void handle_vote (Knowledge::Update & data);
      
#endif // #ifdef _USE_CID_

      /**
       * Handles a multi-assignment update
       * @param  data  the update that was made
       **/
      void handle_multiassignment (Knowledge::Update & data);

      /**
       * We currently allow multiassignments completely through the key. This
       * is probably the least efficient way to handle this and should be
       * changed.
       **/
      ::std::vector< ::std::string>                        assignment_symbols_;

      /**
       * Unique identifier for this entity (e.g., host:port)
       **/
      const std::string                                    id_;
      
      /**
       * Transport settings
       **/
      const Settings                                       &     settings_;

      /**
       * The knowledge context that we will be updating
       **/
      ::Madara::Knowledge_Engine::Thread_Safe_Context &    context_;

      /**
       * The DDS data reader that we will take from
       **/
      ::Knowledge::UpdateDataReader_var                    update_reader_;

      /**
       * The DDS data writer that we can write to
       **/
      ::Knowledge::UpdateDataWriter_var                    update_writer_;

      /**
       * Barrier used for proper shutdown operations
       **/
      ACE_Barrier barrier_;

      /**
       * Can be used to signal termination
       **/
      ACE_Atomic_Op<ACE_Mutex,bool>                        terminated_;

      /**
       * Mutex in case of multiple read threads
       **/
      ACE_Thread_Mutex                   mutex_;

      /**
       * Condition for waiting on readiness
       **/
      Madara::Transport::Condition       is_not_ready_;

      /**
       * If true, the transport is ready
       **/
      bool                               is_ready_;

      /// data received rules, defined in Transport settings
      Madara::Expression_Tree::Expression_Tree  on_data_received_;
    };
  }
}

#endif
