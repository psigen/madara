
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <assert.h>

#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"
#include "ace/Signal.h"

#include "madara/knowledge_engine/Knowledge_Base.h"
#include "madara/transport/Transport.h"
#include "madara/utility/Utility.h"

int left = 0;
int stop = 10;
long value = 0;
unsigned long iterations = 100000;

Madara::Transport::Settings settings;

std::string host = "localhost";

volatile bool terminated = false;

// command line arguments
int parse_args (int argc, ACE_TCHAR * argv[]);


/**
 * Converts an unsigned long long into a string
 * @param   source   parameter to convert into a string
 * @return  comma separated version of the source in a string
 */
std::string
ull_to_string (unsigned long long source)
{
  std::stringstream buffer;

  // add commas
  std::locale loc(""); 
  buffer.imbue (loc); 

  // stream the source into the buffer
  buffer << source;

  // return the string representation
  return buffer.str ().c_str ();
}

int ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{
  int retcode = parse_args (argc, argv);

  if (retcode < 0)
    return retcode;

  ACE_TRACE (ACE_TEXT ("main"));

  // use ACE real time scheduling class
  int prio  = ACE_Sched_Params::next_priority
    (ACE_SCHED_FIFO,
     ACE_Sched_Params::priority_max (ACE_SCHED_FIFO),
     ACE_SCOPE_THREAD);
  ACE_OS::thr_setprio (prio);

  settings.type = Madara::Transport::SPLICE;
  settings.domains = "KaRL_Latency_Rounds";
  settings.reliability = Madara::Transport::RELIABLE;
  settings.latency_enabled = true;

  Madara::Knowledge_Engine::Knowledge_Base knowledge (host, settings);

  std::string input;

  while (input != "q")
  {
    std::cout << "Please enter a command.\n\nb = broadcast latency.";
    std::cout << " p = print. q = quit.\n\n";
    std::getline (std::cin, input);
    
    if (input.size () > 0)
    {
      if (input[0] == 'b' || input[0] == 'B')
      {
        knowledge.start_latency ();
        ACE_OS::sleep (2);
        knowledge.print_all_latencies ();
      }
      else if (input[0] == 'p' || input[0] == 'P')
        knowledge.print_all_latencies ();
    }
  }

  knowledge.print_all_latencies ();

  return 0;
}



int parse_args (int argc, ACE_TCHAR * argv[])
{
  // options string which defines all short args
  ACE_TCHAR options [] = ACE_TEXT ("i:s:p:o:v:n:h");

  // create an instance of the command line args
  ACE_Get_Opt cmd_opts (argc, argv, options);

  // set up an alias for '-n' to be '--name'
  cmd_opts.long_option (ACE_TEXT ("id"), 'i', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("iterations"), 'n', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("stop"), 's', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("processes"), 'p', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("help"), 'h', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("host"), 'o', ACE_Get_Opt::ARG_REQUIRED);
 
  // temp for current switched option
  int option;

  // used in processing std::stringstream stuff
  std::string extra;
//  ACE_TCHAR * arg;

  // iterate through the options until done
  while ((option = cmd_opts ()) != EOF)
  {
    std::stringstream buffer;
    //arg = cmd_opts.opt_arg ();
    switch (option)
    {
    case 'i':
      // thread number
      buffer << cmd_opts.opt_arg ();
      buffer >> settings.id;
      break;
    case 'n':
      // thread number
      buffer << cmd_opts.opt_arg ();
      buffer >> iterations;
      break;
    case 's':
      // stop value
      buffer << cmd_opts.opt_arg ();
      buffer >> stop;
      break;
    case 'p':
      // changing the processes is not allowed
      buffer << cmd_opts.opt_arg ();
      buffer >> settings.processes;
      break;
    case 'o':
      host = cmd_opts.opt_arg ();
      ACE_DEBUG ((LM_INFO, "(%P|%t) host set to %s\n", host.c_str ()));
      break;
    case ':':
      ACE_ERROR_RETURN ((LM_ERROR, 
        ACE_TEXT ("ERROR: -%c requires an argument"), 
           cmd_opts.opt_opt ()), -2); 
    case 'h':
    default:
      ACE_DEBUG ((LM_INFO, "Program Options:      \n\
      -p (--processes) number of processes (disabled - always 2) \n\
      -i (--id)        set process id (0 default)  \n\
      -n (--iterations) set number of iterations (100,000 by default)  \n\
      -s (--stop)      stop condition (10 default) \n\
      -o (--host)      this host ip/name (localhost default) \n\
      -h (--help)      print this menu             \n"));
      ACE_ERROR_RETURN ((LM_ERROR, 
        ACE_TEXT ("Returning from Help Menu")), -1); 
      break;
    }
  }

  return 0;
}
