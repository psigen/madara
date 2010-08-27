#include "ace/Event_Handler.h"
#include "ace/OS.h"
#include "ace/Log_Msg.h"
#include "ace/Barrier.h"

#include "madara/Thread_Pool.h"

#include <iostream>

/// Constructor
Madara::Thread_Pool::Thread_Pool ()
: threads_ (0), barrier_ (0)
{
}


Madara::Thread_Pool::~Thread_Pool ()
{
  delete barrier_;
}

/// Open the thread pool with the indicated number of threads
int Madara::Thread_Pool::open (int pool_size)
{
  barrier_ = new ACE_Barrier (pool_size + 1);
  return this->activate (THR_NEW_LWP | THR_DETACHED, pool_size);
}

/// Close the thread pool
int Madara::Thread_Pool::close(void)
{ 
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) closing threads.\n"));
 
  // enqueue hangup messages
  for (int i = 0; i < threads_.value (); ++i)
    {
      ACE_Message_Block * hangup = new ACE_Message_Block (0, 
        ACE_Message_Block::MB_HANGUP);
      this->enqueue(hangup);
    }

  enter_barrier ();

  return 0;
}

int Madara::Thread_Pool::enter_barrier ()
{ 
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) entering barrier.\n"));
 
  barrier_->wait ();

  return 0;
}

/// Enqueue a message block for the svc function of threads in the pool
int Madara::Thread_Pool::enqueue (ACE_Message_Block * message)
{
  if( this->putq(message) == -1 )
    {
      message->release ();
      return -1;
    }

  return 0;
}

/// main function of each thread
int Madara::Thread_Pool::svc (void)
{
  // update the active thread count and decrement if the svc is exited
  // with a thread exit
  Counter_Guard thread_guard (threads_, *barrier_);
  ACE_Message_Block * message;

  /* Process the queue */
  while (this->getq (message) != -1)
    {
      // this will automatically release the message block
      // if we encounter a thread exit
      Message_Block_Guard message_block_guard (message);

      // process a thread shutdown order
      if (ACE_Message_Block::MB_HANGUP == message->msg_type ())
      {
        break;
      }

      // here's where we should be evaluating the types and processing data
      char * data = message->base ();
  
      ::std::cerr << "Data is " << data << ::std::endl;
    }

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Exiting.\n"));
 
  return 0;
} 