#ifndef _TEST_FRAMEWORK_H_
#define _TEST_FRAMEWORK_H_

/**
 * @file Knowledge_Base.h
 * @author James Edmondson <james.r.edmondson@vanderbilt.edu>
 *
 * This file contains the Knowledge_Base class
 */

#include "madara/kats/KATS_export.h"
#include "madara/transport/Transport.h"
#include "madara/knowledge_engine/Knowledge_Base.h"

namespace Madara
{

  namespace KATS
  {
    class KATS_Export Settings : public Madara::Transport::Settings
    {
    public:
      /// Default knowledge domain
      #define DEFAULT_KATS_DOMAIN "KATS"
      #define DEFAULT_ID          0
      #define DEFAULT_PROCESSES   1
      #define DEFAULT_HOST        "localhost"

    /**
     * @class Settings
     * @brief Provides a testing configuration to a KATS Test Framework
     */
      Settings ()
        : Madara::Transport::Settings ()
      {
        // set the underlying members
        this->domains = DEFAULT_KATS_DOMAIN;

        // set our local members
        id = DEFAULT_ID;
        processes = DEFAULT_PROCESSES;
        host = DEFAULT_HOST;
      }

      /// our test framework id (0 ... processes-1 are valid ids)
      unsigned long id;

      /// number of testing processes in the network
      unsigned long processes;

      /// our host name/ip (localhost by default, if multiple hosts
      /// please use actual ips or valid hosts). This is not used for
      /// creating communication channels but is used to identify the
      /// source of data in KaRL
      std::string host;
    };

    /**
     * @class Test_Framework
     * @brief This class provides a distributed testing framework to users
     */
    class KATS_Export Test_Framework
    {
    public:
      /**
       * Constructor
       * @param   config   framework configuration
       **/
      Test_Framework (const Settings & config);

      /**
       * Copy constructor
       **/
      Test_Framework (const Test_Framework & original);

      /**
       * Destructor
       **/
      ~Test_Framework ();

      /**
       * Barriers on all processes until everyone is at the event
       * @param    event_name    name of the event to barrier on
       */
      long long barrier (const std::string & event_name);

      /**
       * Creates a testing event
       * @param name               the name of this event
       * @param pre_condition      condition that needs to be true 
       *                           for this event to be true
       * @param post_condition     variables to set after the pre_condition
       *                           is satisfied
       * @param fail_condition     condition that would result in
       *                           aborting the test
       * @param continue_condition condition to skip the post_condition and
       *                           event barrier
       * @param barrier_this_event barrier across all processes as part
       *                           of the pre_condition
       * @returns          0 if all conditions passed, -1 if fail_condition
       *                   was met
       **/
      int event (const std::string & name, 
        const std::string & pre_condition,
        const std::string & post_condition,
        const std::string & fail_condition = "",
        const std::string & continue_condition = "",
        bool barrier_this_event = false,
        bool close_transport = false
      );

      /**
       * Logs according to a severity level
       * @param level      level of severity
       * @param statement  statement to print (can contain variable expansion
       *                   from the knowledge base)
       **/
      void log (int level, const std::string & statement);

      /**
       * Dumps all knowledge to std::err
       * @param level      level of severity
       **/
      void dump (int level);

    private:
      Madara::Knowledge_Engine::Knowledge_Base knowledge_;
    };

  }
}

// include the inline functions
#include "Test_Framework.inl"

#endif   // _TEST_FRAMEWORK_H_
