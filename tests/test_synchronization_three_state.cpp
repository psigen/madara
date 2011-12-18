
//#define MADARA_NTRACE    0
//#define ACE_NTRACE    0
////#define ACE_NLOGGING  0
//#define ACE_NDEBUG    0

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <assert.h>

#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"
#include "ace/Signal.h"


#include "madara/knowledge_engine/Compiled_Expression.h"
#include "madara/knowledge_engine/Knowledge_Base.h"

bool logical_print = false;
int id = 2;
int left = 0;
int processes = 3;
int stop = 3;
long value = 0;
volatile bool terminated = 0;
std::string host = "localhost";
std::string domain = "three_state";

// signal handler for someone hitting control+c
extern "C" void terminate (int)
{
  terminated = true;
}

// command line arguments
int parse_args (int argc, ACE_TCHAR * argv[]);

int ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{
  int retcode = parse_args (argc, argv);

  if (retcode < 0)
    return retcode;

  // signal handler for clean exit
  ACE_Sig_Action sa ((ACE_SignalHandler) terminate, SIGINT);

  // transport settings
  Madara::Transport::Settings ts;
  ts.domains = domain;
  ts.type = Madara::Transport::SPLICE;

  // start the knowledge engine
  Madara::Knowledge_Engine::Knowledge_Base knowledge (
    host, ts);

  // variables for compiled expressions and wait settings
  Madara::Knowledge_Engine::Compiled_Expression compiled;
  Madara::Knowledge_Engine::Wait_Settings wait_settings;

  // set my id
  knowledge.set (".self", id);

  // The state of the process to my left dictates my next state
  // if I am the bottom process, I look at the last process
  knowledge.set (".left", id ? id - 1 : processes - 1);

  // The state of the process to my right can also dictate my state
  // if I am the top process (i.e. id == processes -1), then my
  // right process is the bottom process (i.e. process with id == 0)
  // we're basically wrapping around at both ends. See Dijkstra's third
  // synchronization algorithm defined in Self-stabilizing Systems  in 
  // Spite of Distributed Control
  knowledge.set (".right", id == processes - 1 ? 0 : id + 1);

  // set my stop state
  knowledge.set (".stop", stop);
  // set my initial value
  knowledge.set (".init", value);

  // set my state to an initial value (see command line args to change)
  knowledge.evaluate ("S{.self}=.init");

  // by default, the expression to evaluate is for a non-bottom process
  // if my state does not equal the left state, change my state to left state
  std::string expression = "(S{.self}.started = 1) && S{.left}.started && S{.right}.started";
  wait_settings.pre_print_statement = " Waiting on S{.left}.started and S{.right}.started\n";
  compiled = knowledge.compile (expression);

  // wait for left and right processes to startup before executing application logic
  knowledge.wait (compiled, wait_settings);

  if (id == 0)
  {
    expression = 
    // if we are the bottom process, (id == 0), then logic is
    // if (S+1)     % 3 == R       then S        = (S-1)          % 3
      "(S{.self}+1) % 3 == S{.right} => S{.self} = (S{.self}+3-1) % 3";

    //expression = s0_logic;
  }
  else if (id == processes - 1)
  {
    // top process logic
    expression = 
    // if  L   == R        && (L       +l)%3 != S       then S       = (L        + 1)%3
      "S{.left}==S{.right} && (S{.left}+1)%3 != S{.self} => S{.self} = (S{.left} + 1)%3";

    //expression = s2_logic;
  }
  else
  {
    // every other process is looking at both its left and right
    // to try to synchronize with two sources. Here is the logic:
    expression = 
    // if( S     + 1)%3 == L       then S        = L
      "(S{.self} + 1)%3 == S{.left} => S{.self} = S{.left};" \
    // if( S      + 1) % 3 ==    R    then   S     =    R
      "(S{.self} + 1)%3 == S{.right} => S{.self} = S{.right}";
    
    //expression = s1_logic;
  }

  // setup the main loop
  compiled = knowledge.compile (expression);
  wait_settings.pre_print_statement = "";
  wait_settings.post_print_statement = "  {S{.left}} {S{.self}} {S{.right}}\n";

  // termination is done via signalling from the user (Control+C)
  while (!terminated)
  {
    knowledge.wait (compiled, wait_settings);
    ACE_OS::sleep (1);
  }

  knowledge.print_knowledge ();

  return 0;
}



int parse_args (int argc, ACE_TCHAR * argv[])
{
  // options string which defines all short args
  ACE_TCHAR options [] = ACE_TEXT ("i:s:p:o:v:lh");

  // create an instance of the command line args
  ACE_Get_Opt cmd_opts (argc, argv, options);

  // set up an alias for '-n' to be '--name'
  cmd_opts.long_option (ACE_TEXT ("id"), 'i', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("stop"), 's', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("processes"), 'p', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("help"), 'h', ACE_Get_Opt::NO_ARG);
  cmd_opts.long_option (ACE_TEXT ("host"), 'o', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("value"), 'v', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("logical"), 'l', ACE_Get_Opt::NO_ARG);
 
  // temp for current switched option
  int option;
//  ACE_TCHAR * arg;

  // iterate through the options until done
  while ((option = cmd_opts ()) != EOF)
  {
    //arg = cmd_opts.opt_arg ();
    switch (option)
    {
    case 'i':
      {
        std::stringstream buffer;
        buffer << cmd_opts.opt_arg ();
        buffer >> id;
      }
      break;
    case 'd':
      domain = cmd_opts.opt_arg ();
      break;
    case 's':
      {
        std::stringstream buffer;
        buffer << cmd_opts.opt_arg ();
        buffer >> stop;
      }
      break;
    case 'p':
      {
        std::stringstream buffer;
        buffer << cmd_opts.opt_arg ();
        buffer >> processes;
      }
      break;
    case 'v':
      {
        std::stringstream buffer;
        buffer << cmd_opts.opt_arg ();
        buffer >> value;
      }

      if (value > 2 || value < 0)
      ACE_ERROR_RETURN ((LM_ERROR, 
        ACE_TEXT ("ERROR: -%c requires an argument that is >= 0 and <= 2"), 
           cmd_opts.opt_opt ()), -3); 

      break;
    case 'o':
      host = cmd_opts.opt_arg ();
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) host set to %s\n", host.c_str ()));
      break;
    case ':':
      ACE_ERROR_RETURN ((LM_ERROR, 
        ACE_TEXT ("ERROR: -%c requires an argument"), 
           cmd_opts.opt_opt ()), -2); 
    case 'h':
    default:
      ACE_DEBUG ((LM_DEBUG, "Program Options:      \n\
      -p (--processes) number of processes that will be running\n\
      -d (--domain)    domain to separate all traffic into\n\
      -i (--id)        set process id (0 default)  \n\
      -s (--stop)      stop condition (3 default) \n\
      -o (--host)      this host ip/name (localhost default) \n\
      -v (--value)     start process with a certain value (0 default) \n\
      -h (--help)      print this menu             \n"));
      ACE_ERROR_RETURN ((LM_ERROR, 
        ACE_TEXT ("Returning from Help Menu")), -1); 
      break;
    }
  }

  return 0;
}
