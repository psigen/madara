
//#define MADARA_NTRACE    0
//#define ACE_NTRACE    0
////#define ACE_NLOGGING  0
//#define ACE_NDEBUG    0

#include <string>
#include <vector>
#include <iostream>
#include <assert.h>

#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"

#include "madara/knowledge_engine/Knowledge_Base.h"

std::string host ("");
const std::string default_multicast ("239.255.0.1:4150");
Madara::Transport::Settings settings;

// command line arguments
int parse_args (int argc, ACE_TCHAR * argv[]);

// test functions
void read_and_create_files (Madara::Knowledge_Engine::Knowledge_Base & knowledge);
void write_transported_files (Madara::Knowledge_Engine::Knowledge_Base & knowledge);

int ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{
  settings.hosts_.resize (1);
  settings.hosts_[0] = default_multicast;
  int retcode = parse_args (argc, argv);

  settings.type = Madara::Transport::MULTICAST;
  Madara::Knowledge_Engine::Wait_Settings wait_settings;
  wait_settings.max_wait_time = 10;
  wait_settings.poll_frequency = 1.0;

  if (settings.id == 1)
  {
    wait_settings.pre_print_statement = 
      "Waiting on an_xml to change from process.id==1...\n";
  }

  if (retcode < 0)
    return retcode;

  ACE_LOG_MSG->priority_mask (LM_INFO | LM_DEBUG, ACE_Log_Msg::PROCESS);

  ACE_TRACE (ACE_TEXT ("main"));

  Madara::Knowledge_Engine::Knowledge_Base knowledge ("", settings);
  
  knowledge.set (".id", (Madara::Knowledge_Record::Integer) settings.id);
  
  // run tests
//  test_tree_compilation (knowledge);
  if (settings.id == 0)
    read_and_create_files (knowledge);
  else
  {
    knowledge.wait ("finished_transmitting", wait_settings);
    write_transported_files (knowledge);
  }

  //knowledge.print_knowledge ();

  return 0;
}

/// Tests logicals operators (&&, ||)
void read_and_create_files (Madara::Knowledge_Engine::Knowledge_Base & knowledge)
{
  ACE_TRACE (ACE_TEXT ("test_assignments"));

  knowledge.clear ();

  knowledge.read_file ("sample", "/files/sample.jpg");
  knowledge.write_file ("sample", "/files/sample_copy.jpg");

  knowledge.set (".a_string", "Hello world!");
  knowledge.write_file (".a_string", "/files/hello_world.txt");
  knowledge.read_file ("hello_world", "/files/hello_world.txt");
  knowledge.write_file ("hello_world", "/files/hello_world_copy.txt");
  
  knowledge.set (".an_integer", Madara::Knowledge_Record::Integer (10));
  knowledge.write_file (".an_integer", "/files/ten.txt");
  knowledge.read_file ("ten", "/files/ten.txt");
  knowledge.write_file ("ten", "/files/ten_copy.txt");
  
  knowledge.set (".a_double", 12.5);
  knowledge.write_file (".a_double", "/files/twelve.txt");
  knowledge.read_file ("double", "/files/twelve.txt");
  knowledge.write_file ("double", "/files/twelve_copy.txt");

  knowledge.set (".an_xml",
    "<tree><leaf>15</leaf><leaf>This leaf is empty.</leaf></tree>");
  knowledge.write_file (".an_xml", "/files/an_xml_file.xml");
  knowledge.read_file ("a_tree", "/files/an_xml_file.xml");
  knowledge.write_file ("a_tree", "/files/an_xml_file_copy.xml");

  
  knowledge.set ("finished_transmitting");
  
}


/// Tests logicals operators (&&, ||)
void write_transported_files (
  Madara::Knowledge_Engine::Knowledge_Base & knowledge)
{
  knowledge.write_file ("a_tree", "/files/an_xml_file_transported.xml");
  knowledge.write_file ("double", "/files/twelve_transported.txt");
  knowledge.write_file ("ten", "/files/ten_transported.txt");
  knowledge.write_file ("hello_world", "/files/hello_world_transported.txt");
  knowledge.write_file ("sample", "/files/sample_transported.jpg");
}

int parse_args (int argc, ACE_TCHAR * argv[])
{
  for (int i = 1; i < argc; ++i)
  {
    std::string arg1 (argv[i]);

    if (arg1 == "-m" || arg1 == "--multicast")
    {
      if (i + 1 < argc)
        settings.hosts_[0] = argv[i + 1];

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
        buffer >> MADARA_debug_level;
      }

      ++i;
    }
    else
    {
      MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_DEBUG, 
        "\nProgram summary for %s:\n\n" \
"This test checks the functionality of file reading/writing. To properly\n"
"test file functionality, please create a file at /files/sample.jpg,\n"
"preferably with a real image. Check the sample_copy.jpg for correctness\n\n",
        " [-o|--host hostname]     the hostname of this process (def:localhost)\n" \
        " [-m|--multicast ip:port] the multicast ip to send and listen to\n" \
        " [-d|--domain domain]     the knowledge domain to send and listen to\n" \
        " [-i|--id id]             the id of this agent (should be non-negative)\n" \
        " [-l|--level level]       the logger level (0+, higher is higher detail)\n" \
        "\n",
        argv[0]));
      exit (0);
    }
  }

  return 0;
}

