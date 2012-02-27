#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <string>
#include <vector>
#include <iostream>
#include "ace/Thread_Mutex.h"
#include "ace/Recursive_Thread_Mutex.h"
#include "ace/Condition_T.h"
#include "ace/Guard_T.h"
#include "madara/utility/Log_Macros.h"
#include "ace/High_Res_Timer.h"
#include "madara/cid/Settings.h"
#include "madara/cid/Convenience.h"

namespace Madara
{
  namespace Transport
  {
    typedef   ACE_High_Res_Timer                   Timer;
    typedef   std::vector<Timer>                   Timers;
    typedef   Madara::Cid::Settings                Latency_Settings;

    enum Types {
      NO_TRANSPORT = 0,
      SPLICE       = 1,
      NDDS         = 2,
      TCP          = 3,
      INCONSISTENT = 100
    };

    enum Reliabilities {
      BEST_EFFORT = 0,
      RELIABLE = 1
    };

    enum Messages {
      ASSIGN = 0,
      OPERATION = 1,
      MULTIASSIGN = 2,
      LATENCY = 10
    };

    /**
     * Holds Transport Settings
     */
    class Settings
    {
    public:
      // for ease-of-use, typedef the templated guard
      typedef ACE_Guard <ACE_Recursive_Thread_Mutex> Context_Guard;


      /// Default knowledge domain
      #define DEFAULT_DOMAIN      "KaRL"

      /// Default queue length for event history (must be high for
      /// reliable transport
      static const unsigned long DEFAULT_QUEUE_LENGTH = 100000;

      /// Default deadline
      static const unsigned long DEFAULT_DEADLINE = 0;

      /// Default transport
      static const unsigned long DEFAULT_TRANSPORT = NO_TRANSPORT;

      /// Default reliability
      static const unsigned long DEFAULT_RELIABILITY = RELIABLE;

      /**
       * Default id in group
       **/
      #define DEFAULT_ID                0

      /**
       * Default number of processes in group
       **/
      #define DEFAULT_PROCESSES         1

      /**
       * Latency collection is disabled by default
       **/
      #define DEFAULT_LATENCY_ENABLED   false

      /**
       * Set default latency timeout to 10 seconds
       **/
      #define DEFAULT_LATENCY_TIMEOUT   10.0

      /**
       * Set default latency to 200 seconds
       **/
      #define DEFAULT_LATENCY          200000000000

      /// Constructor for this class
      Settings () : 
        domains (DEFAULT_DOMAIN), 
        queue_length (DEFAULT_QUEUE_LENGTH), 
        deadline (DEFAULT_DEADLINE), 
        type (DEFAULT_TRANSPORT),
        reliability (DEFAULT_RELIABILITY),
        id (DEFAULT_ID),
        processes (DEFAULT_PROCESSES),
        latency_enabled (DEFAULT_LATENCY_ENABLED),
        latency_timeout (DEFAULT_LATENCY_TIMEOUT),
        latency_default (DEFAULT_LATENCY)
      {
      }

      /// Copy constructor
      Settings (const Settings & settings) : 
        domains (settings.domains), 
        queue_length (settings.queue_length), 
        deadline (settings.deadline), 
        type (settings.type),
        reliability (settings.reliability),
        id (settings.id),
        processes (settings.processes),
        latency_enabled (settings.latency_enabled),
        latency_timeout (settings.latency_timeout),
        latency_default (settings.latency_default)
      {
      }

      void operator= (const Settings & settings)
      {
        domains = settings.domains;
        queue_length = settings.queue_length;
        deadline = settings.deadline;
        type = settings.type;
        reliability = settings.reliability;
        id = settings.id;
        processes = settings.processes;
        latency_enabled = settings.latency_enabled;
        latency_timeout = settings.latency_timeout;
        latency_default = settings.latency_default;
      }

      /**
       * Resets timers and latencies
       **/
      inline void reset_timers (void)
      {
        Context_Guard guard (mutex);

        for (unsigned int i = 0; i < processes; ++i)
          timers[i].reset ();

        Madara::Cid::reset_latencies (latencies, latency_default);
      }

      /**
       * Starts all latency timers and resets timers
       **/
      inline void start_all_timers (void)
      {
        Context_Guard guard (mutex);

        reset_timers ();
        
        for (unsigned int i = 0; i < processes; ++i)
          timers[i].start ();
      }

      /**
       * Stops a timer and records latency
       **/
      inline void stop_timer (int index, bool roundtrip = true)
      {
        ACE_hrtime_t measured;
        Context_Guard guard (mutex);

        timers[index].stop ();
        timers[index].elapsed_time (measured);

        latencies.network_latencies[id][index].second = roundtrip ?
          (unsigned long long) measured / 2 : (unsigned long long) measured;
      }

      /**
       * Allocates latency gathering data structures if necesssary. Called
       * by transports. Users shouldn't need to call this.
       **/
      inline void setup (void)
      {
        Context_Guard guard (mutex);

        if (latency_enabled)
        {
          Madara::Cid::init (processes, latencies);
          timers.resize (processes);

          Madara::Cid::reset_latencies (latencies, latency_default);
        }
      }

      /**
       * Prints all latencies from this id in the format id -> latency
       **/
      void print_my_latencies (void)
      {
        Context_Guard guard (mutex);
        Madara::Cid::Identifiers & ids = latencies.ids;
        Madara::Cid::Latency_Vector & current = latencies.network_latencies[id];

        std::cout << "Latencies for id = " << id << std::endl;

        // print each id -> latency
        for (unsigned int i = 0; i < processes; ++i)
        {
          std::cout << ids[i] << " = " << current[i].second << std::endl;
        }
      }

      /// All class members are accessible to users for easy setup

      /// Domains should be separated by commas
      std::string domains;

      /// Length of the buffer used to store history of events
      unsigned long queue_length;

      /// Deadline for sessions with any other reasoning entity
      unsigned long deadline;

      /// Type of transport. See Madara::Transport::Types for options
      unsigned long type;

      /// Reliability required of the transport. 
      /// See Madara::Transport::Reliabilities for options
      unsigned long reliability;

      /// the id of this process. May be useful for latency gathering
      /// or testing purposes
      unsigned long id;

      /// number of processes expected in the network (best to overestimate
      /// if building latency tables
      unsigned long processes;

      /// should we try to gather latencies?
      bool latency_enabled;

      /// maximum time allowed before starting another latency round
      double latency_timeout;

      /// default value for latency times in nanoseconds. This is the value
      /// recorded if a process is unreachable.
      unsigned long long latency_default;

      /// mutex used for mutating latencies or timers
      ACE_Recursive_Thread_Mutex mutex;

      /// latency information
      Latency_Settings latencies;

      /// timers used to establish latency
      Timers timers;

      /// number of responses received so far
      unsigned long num_responses;
    };

    typedef    ACE_Condition <ACE_Thread_Mutex>    Condition;

    /**
     * Base class from which all transports must be derived
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
       **/
      Base (Settings & new_settings) 
        : is_valid_ (false), shutting_down_ (false),
        valid_setup_ (mutex_), settings_ (new_settings)
      {
        settings_.setup ();
      }

      /**
       * Add a vector of hosts to the current host list
       * @param   host        a vector of hosts to add to the list
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
          DLINFO "Transport::validate_transport: transport is ready"));

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
          DLINFO "Transport::check_transport: checking for valid transport"));

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

      /**
       * Send a single assignment
       * @return  result of send operation or -1 if we are shutting down
       **/
      virtual long send_data (const std::string &, const long long &)
      {
        return check_transport ();
      }

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
       * Sends a multiple assignment of knowledge variables
       * @return  result of operation or -1 if we are shutting down
       **/
      virtual long send_multiassignment (const std::string &, unsigned long)
      {
        return check_transport ();
      }

      /**
       * Invalidates a transport to indicate it is shutting down
       **/
      inline void invalidate_transport (void)
      {
        is_valid_ = false;
        shutting_down_ = true;
        valid_setup_.broadcast ();

        MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_DEBUG, 
          DLINFO "Transport::invalidate_transport: invalidating transport"));
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
      Settings & settings_;
    };
  }
}

#endif
