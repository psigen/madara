#ifndef _MADARA_TRANSPORT_H_
#define _MADARA_TRANSPORT_H_

/**
 * @file Transport.h
 * @author James Edmondson <jedmondson@gmail.com>
 *
 * This file contains the Transport::Base class, which provides an
 * extensible transport layer for sending knowledge updates in KaRL.
 * To support knowledge updates, only the send_multiassignment method
 * is currently required to be extended as the set, evaluate, and wait
 * methods all call send_multiassignment. For example transport, @see
 * Madara::Transport::Multicast_Transport.
 **/

#include <string>
#include <sstream>
#include <vector>
#include <ostream>
#include "ace/Thread_Mutex.h"
#include "ace/Recursive_Thread_Mutex.h"
#include "ace/Condition_T.h"
#include "ace/Guard_T.h"
#include "madara/utility/Log_Macros.h"
#include "ace/High_Res_Timer.h"
#include "madara/transport/QoS_Transport_Settings.h"

#ifdef _USE_CID_

#include "madara/cid/CID_Settings.h"
#include "madara/cid/CID_Convenience.h"
#include "madara/cid/CID_Heuristic.h"
#include "madara/cid/CID_Genetic.h"
#include "madara/utility/Utility.h"

#endif // _USE_CID_

#include "madara/knowledge_engine/Knowledge_Record.h"
#include "madara/knowledge_engine/Thread_Safe_Context.h"
#include "madara/expression_tree/Expression_Tree.h"
#include "madara/expression_tree/Interpreter.h"

namespace Madara
{
  namespace Transport
  {
    typedef    ACE_Condition <ACE_Thread_Mutex>    Condition;

    /**
     * Base class from which all transports must be derived.
     * To support knowledge updates, only the send_multiassignment method
     * is currently required to be extended as the set, evaluate, and wait
     * methods all call send_multiassignment. For example transport, @see
     * Madara::Transport::Multicast_Transport.
     **/
    class Base
    {
    public:
      /**
       * Used to define a vector of hosts to contact
       **/
      typedef    std::vector <std::string>           Hosts_Vector;

      /**
       * Constructor for settings
       * @param   new_settings      settings to apply to the transport
       * @param   context           the knowledge record context
       **/
      Base (Settings & new_settings,
        Madara::Knowledge_Engine::Thread_Safe_Context & context) 
        : is_valid_ (false), shutting_down_ (false),
        valid_setup_ (mutex_), settings_ (new_settings), context_ (context)
      {

#ifdef _USE_CID_

        settings_.setup ();

#endif // _USE_CID_
        settings_.attach (&context_);
      }

      virtual ~Base ()
      {
      }

      /**
       * Add a vector of hosts to the current host list
       * @param   hosts        a vector of hosts to add to the list
       * @return  0 if successful
       **/
      virtual long setHosts (const Hosts_Vector & hosts) 
      { 
        hosts_ = hosts; 
        return 0;
      }

      /**
       * Add a host to the list of hosts (if the transport allows it)
       * @param   host        a host to add to the @see Hosts_Vector
       * @return  0 if successful
       **/
      virtual long addHost (const std::string & host) 
      { 
        hosts_.push_back (host); 
        return 0;
      }

      /**
       * Validates a transport to indicate it is not shutting down
       **/
      inline int validate_transport (void) 
      { 
        is_valid_ = true; 
        shutting_down_ = false;
        valid_setup_.broadcast ();

        MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_DEBUG, 
          DLINFO "Transport::validate_transport: transport is ready\n"));

        return 0;
      }

      /**
       * all subclasses should call this method at the end of its setup
       **/
      inline int setup (void) 
      {
        return validate_transport ();
      }

      /**
       * all subclasses should call this method at the beginning of send_data
       **/
      inline int check_transport (void) 
      {
        MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_DEBUG, 
          DLINFO "Transport::check_transport: checking for valid transport\n"));

        if (!is_valid_)
          return -2;

        if (shutting_down_)
          return -1;

        return 0;
      }

      /**
       * Getter for the transport settings
       * @return   the current transport settings
       **/
      Settings & settings (void)
      {
        return settings_;
      }

#ifdef _USE_CID_

      /**
       * Start latency
       * @return  result of operation or -1 if we are shutting down
       **/
      virtual long start_latency (void)
      {
        if (!settings_.latency_enabled)
        {
          MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_DEBUG, 
              DLINFO "Splice_DDS_Transport::start_latency:" \
              " Latency enabled is not set in your"
              " Madara::Transport::Settings instance. Update your"
              " program's code and set settings.latency_enabled = true.\n"));

          return -2;
        }
        return check_transport ();
      }

      /**
       * Vote for an algorithm result
       * @return  result of operation or -1 if we are shutting down
       **/
      virtual long vote (void)
      {
        if (!settings_.latency_enabled)
        {
          MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_DEBUG, 
              DLINFO "Splice_DDS_Transport::vote:" \
              " Latency enabled is not set in your"
              " Madara::Transport::Settings instance. Update your"
              " program's code and set settings.latency_enabled = true.\n"));

          return -2;
        }
        return check_transport ();
      }

#endif //_USE_CID_

      /**
       * Sends a list of updates to the domain. This function must be
       * implemented by your transport
       * @return  result of operation or -1 if we are shutting down
       **/
      virtual long send_data (const Madara::Knowledge_Records &) = 0;

      /**
       * Invalidates a transport to indicate it is shutting down
       **/
      inline void invalidate_transport (void)
      {
        is_valid_ = false;
        shutting_down_ = true;
        valid_setup_.broadcast ();

        MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_DEBUG, 
          DLINFO "Transport::invalidate_transport: invalidating transport\n"));
      }

      /**
       * Closes this transport
       **/
      virtual void close (void)
      {
        invalidate_transport ();
      }
      
    protected:
      volatile bool is_valid_;
      volatile bool shutting_down_;
      Hosts_Vector hosts_;
      ACE_Thread_Mutex mutex_;
      Condition valid_setup_;
      QoS_Transport_Settings settings_;
      // context for knowledge base
      Madara::Knowledge_Engine::Thread_Safe_Context & context_;
    };

    typedef   std::vector <Base *>    Transports;
  }
}

#endif  // _MADARA_TRANSPORT_H_
