
#include "madara/knowledge_engine/Knowledge_Base.h"
#include "madara/knowledge_engine/Timed_Executor.h"
#include "madara/utility/Utility.h"
#include <stdio.h>
#include <iostream>
#include <string>

namespace engine = Madara::Knowledge_Engine;
namespace utility = Madara::Utility;

void handle_arguments (int argc, char ** argv)
{
  for (int i = 1; i < argc; ++i)
  {
    std::string arg1 (argv[i]);

    if (arg1 == "-l" || arg1 == "--level")
    {
      if (i + 1 < argc)
      {
        std::stringstream buffer (argv[i + 1]);
        buffer >> MADARA_debug_level;
      }

      ++i;
    }
    else if (arg1 == "-f" || arg1 == "--logfile")
    {
      if (i + 1 < argc)
      {
        Madara::Knowledge_Engine::Knowledge_Base::log_to_file (argv[i + 1]);
      }

      ++i;
    }
    else
    {
      MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_DEBUG, 
        "\nProgram summary for %s:\n\n" \
        "  Test the timed executor, which executes periodic tasks.\n\n" \
        " [-l|--level level]       the logger level (0+, higher is higher detail)\n" \
        " [-f|--logfile file]      log to a file\n" \
        "\n",
        argv[0]));
      exit (0);
    }
  }
}

void test_timed_executor (void)
{
  engine::Knowledge_Base knowledge;
  engine::Timed_Executor executor;

  knowledge.print ("Launching 4 threads\n");
  executor.launch_threads (4);
  
  knowledge.print ("Shutting down threads\n");
  executor.shutdown ();
  
  knowledge.print ("Launching 2 threads\n");
  executor.launch_threads (2);
  
  knowledge.print (
    "Adding a periodic print task with 5 executions, once every 1s\n");
  executor.add (
    engine::fill_event (knowledge, "++executions; #print ('  Executed event.\n')", 1.0, 5));

  utility::sleep (10);
  knowledge.print ();
  knowledge.clear ();
  
  knowledge.print (
    "Adding an indefinite periodic print task, executing once every 1s\n");
  executor.add (
    engine::fill_event (knowledge, "++executions; #print ('  Executed event.\n')", 1.0));
  
  knowledge.print (
    "Should print 10-11 times.\n");
  utility::sleep (11);
  
  executor.shutdown ();

  knowledge.print ();
}

int main (int argc, char ** argv)
{
  handle_arguments (argc, argv);

  test_timed_executor ();

  return 0;
}
