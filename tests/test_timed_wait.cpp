//
//#define MADARA_NTRACE    0
//#define ACE_NTRACE    0
////#define ACE_NLOGGING  0
//#define ACE_NDEBUG    0

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "madara/knowledge_engine/Knowledge_Base.h"

int ACE_TMAIN (int argc, char * argv[])
{
  // debug level for logging
  MADARA_debug_level = 7;

  // don't use a transport for this test.
  Madara::Transport::Settings transport_settings;
  transport_settings.type = Madara::Transport::NO_TRANSPORT;

  Madara::Knowledge_Engine::Knowledge_Base knowledge ("nohost",
    transport_settings);

  Madara::Knowledge_Engine::Wait_Settings wait_settings;
  std::string expression = "++.count && 0";
  wait_settings.post_print_statement =
    "WAIT ENDED: Number of executed waits was {.count}.\n";

  // set poll frequency to once every millisecond and wait for 10 seconds
  wait_settings.poll_frequency = .001;
  wait_settings.max_wait_time = 10.0;

  // **********************************************
  // * Test 1: Waiting on expression that will never finish, but
  // *         since we are setting a 10 second limit, it will finish
  // **********************************************

  // start waiting
  knowledge.wait (expression, wait_settings);

  // **********************************************
  // * Test 2: Waiting and evaluating only once
  // **********************************************

  // clear any existing knowledge
  knowledge.clear ();

  expression = "++.count && 1";

  // start waiting
  knowledge.wait (expression, wait_settings);

  // **********************************************
  // * Test 3: Waiting and evaluating twice
  // **********************************************

  // clear any existing knowledge
  knowledge.clear ();

  expression = "++.count && .count == 2";

  // start waiting
  knowledge.wait (expression, wait_settings);

  // **********************************************
  // * Test 3: Waiting and evaluating ten times
  // **********************************************

  // clear any existing knowledge
  knowledge.clear ();

  expression = "++.count && .count == 10";

  // start waiting
  knowledge.wait (expression, wait_settings);

  return 0;
}
