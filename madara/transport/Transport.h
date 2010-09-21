#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <string>
#include <vector>
#include "ace/Thread_Mutex.h"
#include "ace/Synch_T.h"
#include "ace/Synch.h"

namespace Madara
{
  namespace Transport
  {
    enum {
      NONE = 0,
      SPLICE = 1
    };
    typedef    ACE_Condition <ACE_Thread_Mutex>    Condition;
    class Base
    {
    public:
      /// public typedef for the hosts vector
      typedef    std::vector <std::string>           Hosts_Vector;

      /// default constructor
      Base () : is_valid_ (false), valid_setup_ (mutex_) 
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
    };
  }
}

#endif