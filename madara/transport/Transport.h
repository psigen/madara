#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <string>
#include <vector>
#include "ace/Thread_Mutex.h"
#include "ace/Condition_T.h"
#include "madara/utility/Log_Macros.h"

namespace Madara
{
  namespace Transport
  {
    enum Types {
      NO_TRANSPORT = 0,
      SPLICE       = 1,
      NDDS         = 2,
      TCP          = 3
    } ;

    enum Reliabilities {
      BEST_EFFORT = 0,
      RELIABLE = 1
    } ;

    /**
     * Holds Transport Settings
     */
    class Settings
    {
    public:
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


      /// Constructor for this class
      Settings () : 
        domains (DEFAULT_DOMAIN), 
        queue_length (DEFAULT_QUEUE_LENGTH), 
        deadline (DEFAULT_DEADLINE), 
        type (DEFAULT_TRANSPORT),
        reliability (DEFAULT_RELIABILITY)
      {
      }

      /// Copy constructor
      Settings (const Settings & settings) : 
        domains (settings.domains), 
        queue_length (settings.queue_length), 
        deadline (settings.deadline), 
        type (settings.type),
        reliability (settings.reliability)
      {
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
       * Default constructor
       **/
      Base () : is_valid_ (false), shutting_down_ (false),
        valid_setup_ (mutex_), settings_ ()
      {
      }

      /**
       * Constructor for settings
       * @param   new_settings      settings to apply to the transport
       **/
      Base (const Settings & new_settings) 
        : is_valid_ (false), shutting_down_ (false),
        valid_setup_ (mutex_), settings_ (new_settings)
      {
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
       * Applies the configuration to this transport
       * @param   config     the settings to apply
       **/
      void settings (const Settings & config)
      {
        settings_ = config;
      }

      /**
       * Getter for the transport settings
       * @return   the current transport settings
       **/
      Settings settings (void)
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
      Settings settings_;
    };
  }
}

#endif
