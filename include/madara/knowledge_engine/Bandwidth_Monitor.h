

#ifndef _MADARA_BANDWIDTH_MONITOR_H
#define _MADARA_BANDWIDTH_MONITOR_H

/**
 * @file Bandwidth_Monitor.h
 * @author James Edmondson <jedmondson@gmail.com>
 *
 * This file contains the Bandwidth_Monitor class, which is intended
 * to monitor bandwidth utilization of a transport
 **/

#include <deque>
#include <time.h>

#include "madara/utility/stdint.h"
#include "ace/Guard_T.h"
#include "ace/Recursive_Thread_Mutex.h"
#include "madara/MADARA_export.h"

namespace Madara
{
  namespace Knowledge_Engine
  {
    typedef  std::pair <time_t, uint32_t>   Bandwidth_Record;
    typedef  std::deque <Bandwidth_Record>        Bandwidth_Messages;

    /**
     * @class Bandwidth_Monitor
     * @brief Provides monitoring capability of a transport's bandwidth
     **/

    class MADARA_Export Bandwidth_Monitor
    {
    public:
      /**
       * Default constructor
       * @param   window_in_secs   Time window to measure bandwidth usage
       **/
      Bandwidth_Monitor (time_t window_in_secs = 10);
      
      /**
       * Copy constructor
       * @param  rhs   the value to be copied into this class
       **/
      Bandwidth_Monitor (const Bandwidth_Monitor & rhs);

      /**
       * Destructor
       **/
      virtual ~Bandwidth_Monitor ();

      /**
       * Assignment operator
       * @param  rhs   the value to be copied into this class
       **/
      void operator= (const Bandwidth_Monitor & rhs);
      
      /**
       * Adds a message to the monitor
       * @param   size       the size of the message
       **/
      void add (uint32_t size);

      /**
       * Adds a message to the monitor
       * @param   timestamp  the time the message occured
       * @param   size       the size of the message
       **/
      void add (time_t timestamp, uint32_t size);

      /**
       * Sets the window in seconds to measure bandwidth
       * @param   window_in_secs   Time window to measure bandwidth usage
       **/
      void set_window (time_t window_in_secs);

      /**
       * Queries the monitor for the current bandwidth utilization
       * @return   current bandwidth utilization in bytes
       *           over the entire window (this is not bytes/s)
       **/
      uint32_t get_utilization (void);
      
      /**
       * Queries the monitor for the current bandwidth utilization
       * per second over the past window
       * @return   current bandwidth utilization in bytes/s
       **/
      uint32_t get_bytes_per_second (void);

      /**
       * Clears the bandwidth monitor
       * @param   types   the types to clear the filters of
       **/
      void clear (void);

      /**
       * Prints the number of messages and utilization within the past window
       **/
      void print_utilization (void);

      /**
       * Returns the number of messages in the past window
       * @return  the number of types that have filters
       **/
      size_t get_number_of_messages (void) const;

    protected:
      /**
       * Updates utilization for most public functions
       * @return   current time
       **/
      time_t update_utilization (void);

      /**
       * Convenience typedef for the ACE_Guard
       **/
      typedef ACE_Guard<ACE_Recursive_Thread_Mutex> Bandwidth_Guard;

      /**
       * Mutex for supporting multithreaded monitor calls
       **/
      mutable ACE_Recursive_Thread_Mutex mutex_;

      /**
       * Map of timestamps to messages
       **/
      Bandwidth_Messages  messages_;

      /**
       * Utilization
       **/
      uint32_t utilization_;

      /**
       * Time window for useful messages to bandwidth calculations
       **/
      time_t window_;
    };
  }
}

#include "Bandwidth_Monitor.inl"

#endif   // _MADARA_BANDWIDTH_MONITOR_H
