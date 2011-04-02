#ifndef _TEST_FRAMEWORK_H_
#define _TEST_FRAMEWORK_H_

/**
 * @file Knowledge_Base.h
 * @author James Edmondson <james.r.edmondson@vanderbilt.edu>
 *
 * This file contains the Knowledge_Base class
 */

#include "madara/kats/KATS_export.h"
#include "madara/knowledge_engine/Knowledge_Base.h"

namespace Madara
{

  namespace KATS
  {
    /**
     * @class Test_Framework
     * @brief This class provides a distributed testing framework to users
     */
    class KATS_Export Test_Framework
    {
    public:
      /// Default constructor
      Test_Framework ();

      /// Constructor for transport
      Test_Framework (const std::string & host, int transport);

      /// Constructor for transport and knowledge realm
      Test_Framework (const std::string & host, int transport,
        const std::string & knowledge_domain);

      /// Constructor for transport and transport settings
      Test_Framework (const std::string & host, 
        const Madara::Transport::Settings & config);

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
