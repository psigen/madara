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
      #define DEFAULT_KATS_DOMAIN      "KATS"
      #define DEFAULT_ID          0
      #define DEFAULT_PROCESSES   1
      #define DEFAULT_HOST        "localhost"

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
      /// Constructor for transport and transport settings
      Test_Framework (const Settings & config);

      /// Copy constructor
      Test_Framework (const Test_Framework & original);

      /// Destructor
      ~Test_Framework ();

    private:
      Madara::Knowledge_Engine::Knowledge_Base knowledge_;
    };

  }
}

// include the inline functions
#include "Test_Framework.inl"

#endif   // _TEST_FRAMEWORK_H_
