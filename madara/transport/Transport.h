#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <string>
#include <vector>
#include "ace/Thread_Mutex.h"
#include "ace/Condition_T.h"

namespace Madara
{
  namespace Transport
  {
    enum Types {
      NO_TRANSPORT = 0,
      SPLICE       = 1,
      TCP          = 2
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
    class Base
    {
    public:
      /// public typedef for the hosts vector
      typedef    std::vector <std::string>           Hosts_Vector;

      /// default constructor
      Base () : is_valid_ (false), valid_setup_ (mutex_), settings_ ()
      {
      }

      /// default constructor
      Base (const Settings & new_settings) 
        : is_valid_ (false), valid_setup_ (mutex_), settings_ (new_settings)
      {
      }

      virtual long setHosts (const Hosts_Vector & hosts) 
      { 
        hosts_ = hosts; 
        return 0;
      }

      virtual long addHost (const std::string & host) 
      { 
        hosts_.push_back (host); 
        return 0;
      }

      /// all subclasses should call this method at the end of its setup
      virtual int setup (void) 
      { 
        is_valid_ = true; 
        valid_setup_.broadcast ();

        return 0;
      }

      /// all subclasses should call this method at the beginning of send_data
      virtual long send_data (const std::string & key, const long & value) 
      {
        if (!is_valid_)
          valid_setup_.wait ();

        return 0;
      }

      /// all subclasses should call this method at the beginning of 
      /// a call to send_multiassignment of the derived class
      virtual long send_multiassignment (const std::string & expression,
        unsigned long quality = 0) 
      {
        if (!is_valid_)
          valid_setup_.wait ();

        return 0;
      }

      /// all subclasses should call this method at the beginning of send_mutex
      /// @param key        resource to lock (e.g. variable name)
      /// @param requester  requester id (e.g. host:port for unique id of requester)
      /// @param time       lamport clock value, preferably synchronized globally
      /// @param type       mutex message type identifier (
      virtual long send_mutex (const std::string & key, 
        const std::string & requester, const long & time, const long & type) 
      {
        if (!is_valid_)
          valid_setup_.wait ();

        return 0;
      }

      void settings (const Settings & config)
      {
        settings_ = config;
      }

      Settings settings (void)
      {
        return settings_;
      }

      /// all subclasses should call this method at the beginning of close
      virtual void close (void)
      {
        is_valid_ = false;
      }

    protected:
      bool is_valid_;
      Hosts_Vector hosts_;
      ACE_Thread_Mutex mutex_;
      Condition valid_setup_;
      Settings settings_;
    };
  }
}

#endif