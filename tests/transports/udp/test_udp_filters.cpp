
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <assert.h>

#include "madara/knowledge_engine/Knowledge_Base.h"
#include "madara/filters/Generic_Filters.h"
#include "madara/utility/Log_Macros.h"

std::string host ("");
const std::string default_host1 ("127.0.0.1:43110");
const std::string default_host2 ("127.0.0.1:43111");
Madara::Transport::QoS_Transport_Settings settings;

void handle_arguments (int argc, char ** argv)
{
  for (int i = 1; i < argc; ++i)
  {
    std::string arg1 (argv[i]);

    if (arg1 == "-a" || arg1 == "--address")
    {
      if (i + 1 < argc)
        settings.hosts_.push_back (argv[i + 1]);

      ++i;
    }
    else if (arg1 == "-o" || arg1 == "--host")
    {
      if (i + 1 < argc)
        host = argv[i + 1];

      ++i;
    }
    else if (arg1 == "-d" || arg1 == "--domain")
    {
      if (i + 1 < argc)
        settings.domains = argv[i + 1];

      ++i;
    }
    else if (arg1 == "-i" || arg1 == "--id")
    {
      if (i + 1 < argc)
      {
        std::stringstream buffer (argv[i + 1]);
        buffer >> settings.id;
      }

      ++i;
    }
    else if (arg1 == "-l" || arg1 == "--level")
    {
      if (i + 1 < argc)
      {
        std::stringstream buffer (argv[i + 1]);
        int debug_level;
        buffer >> debug_level;
        Madara::Knowledge_Engine::Knowledge_Base::log_level (debug_level);
      }

      ++i;
    }
    else if (arg1 == "-r" || arg1 == "--reduced")
    {
      settings.send_reduced_message_header = true;
    }
    else
    {
      MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_DEBUG, 
        "\nProgram summary for %s:\n\n" \
        "  Test the UDP transport. Requires 2+ processes. The result of\n" \
        "  running these processes should be that each process reports\n" \
        "  var2 and var3 being set to 1.\n\n" \
        " [-o|--host hostname]     the hostname of this process (def:localhost)\n" \
        " [-a|--address ip:port]   add a udp ip:port to send to (first add is self ip:port)\n" \
        " [-d|--domain domain]     the knowledge domain to send and listen to\n" \
        " [-i|--id id]             the id of this agent (should be non-negative)\n" \
        " [-l|--level level]       the logger level (0+, higher is higher detail)\n" \
        " [-r|--reduced]           use the reduced message header\n" \
        "\n",
        argv[0]));
      exit (0);
    }
  }
}



int main (int argc, char ** argv)
{
  handle_arguments (argc, argv);

  // if the user wants us to do defaults for either host1 or 2
  if (settings.hosts_.size () < 2)
  {
    size_t cur_size = settings.hosts_.size ();

    settings.hosts_.resize (2);
    
    // if we are id 0, use host1 as our ip
    if (settings.id == 0)
    {
      if (cur_size == 0)
      {
        settings.hosts_[0] = default_host1;
      }

      // and host2 as the other ip
      settings.hosts_[1] = default_host2;
    }
    else
    {
      // if we are id 1, use host2 as our ip
      if (cur_size == 0)
      {
        settings.hosts_[0] = default_host2;
      }

      // and host1 as the other ip
      settings.hosts_[1] = default_host1;
    }
  }

  settings.type = Madara::Transport::UDP;
  settings.add_send_filter (Madara::Knowledge_Record::DOUBLE,
                            Madara::Filters::discard);

  Madara::Knowledge_Engine::Wait_Settings wait_settings;
  wait_settings.max_wait_time = 5;

  Madara::Knowledge_Engine::Knowledge_Base knowledge (host, settings);

  knowledge.set (".id", (Madara::Knowledge_Record::Integer) settings.id);

  if (settings.id == 0)
  {
    Madara::Knowledge_Engine::Compiled_Expression compiled = 
      knowledge.compile (
        "(var2 = 1) ;> (var1 = 0) ;> (var4 = -2.0/3) ;> var3"
      );

    knowledge.wait (compiled, wait_settings);
  }
  else
  {
    Madara::Knowledge_Engine::Compiled_Expression compiled = 
      knowledge.compile ("!var1 && var2 => var3 = 1");

    knowledge.wait (compiled, wait_settings);

    if (knowledge.get ("var2").to_integer () == 1 &&
      knowledge.get ("var4").status () == Madara::Knowledge_Record::UNCREATED)
    {
      knowledge.print ("Double value was not received. Sent filter SUCCESS.\n");
    }
    else
    {
      knowledge.print ("Double value was received. Sent filter FAIL.\n");
    }
  }

  knowledge.print_knowledge ();

  return 0;
}