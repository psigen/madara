
//#define MADARA_NTRACE    0
//#define ACE_NTRACE    0
////#define ACE_NLOGGING  0
//#define ACE_NDEBUG    0

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <assert.h>

#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"
#include "ace/Signal.h"

#include "madara/knowledge_engine/Knowledge_Base.h"

int id = 0;
int left = 0;
int processes = 1;
int stop = 10;
long value = 0;
std::string host = "localhost";

volatile bool terminated = 0;

// command line arguments
int parse_args (int argc, ACE_TCHAR * argv[]);

// signal handler for someone hitting control+c
extern "C" void terminate (int)
{
  terminated = true;
}

int ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{
  int retcode = parse_args (argc, argv);

  if (retcode < 0)
    return retcode;

  ACE_LOG_MSG->priority_mask (LM_DEBUG | LM_INFO, ACE_Log_Msg::PROCESS);

  ACE_TRACE (ACE_TEXT ("main"));

  // signal handler for clean exit
  ACE_Sig_Action sa ((ACE_SignalHandler) terminate, SIGINT);

  Madara::Knowledge_Engine::Knowledge_Base knowledge(host, Madara::Transport::SPLICE);

  ACE_DEBUG ((LM_INFO, "(%P|%t) (%d of %d) synchronizing to %d\n",
                        id, processes, stop));

  // set my id
  knowledge.set (".self", id);

  // The state of the process to my left dictates my next state
  // if I am the bottom process, I look at the last process
  knowledge.set (".left", id ? id - 1 : processes - 1);

  // keep track of the right since they require knowledge from us
  knowledge.set (".right", id == processes - 1 ? 0 : id + 1);

  // set my stop state
  knowledge.set (".stop", stop);
  // set my initial value
  knowledge.set (".init", value);

  ACE_DEBUG ((LM_INFO, 
    "(%P|%t) (%d of %d) Waiting on my left (S%d) and right (S%d)\n",
    id, processes, knowledge.get (".left"), knowledge.get (".right")));

  // wait for left and right processes to startup before
  // executing application logic
  knowledge.wait (
    "(S{.self}.started = 1) && S{.left}.started && S{.right}.started");

  // set my state to an initial value (see command line args to change)
  knowledge.evaluate ("S{.self}=.init");

  // by default, the expression to evaluate is for a non-bottom process
  // if my state does not equal the left state, change my state to left state
  std::string expression ("S{.self} != S{.left} => S{.self} = S{.left}");

  // if I am the bottom process, however, I do NOT want to be my left state
  // so if the top process becomes my state, I move on to my next state
  // this allows for a ring of legitimate states that progress towards an
  // end goal (in our case the .stop condition)
  if (id == 0)
  {
    expression = "S{.self} == S{.left} => S{.self} = (S{.self} + 1) % .stop";   
  }

  ACE_DEBUG ((LM_INFO, "(%P|%t) (%d of %d) expression: %s\n",
    id, processes, expression.c_str ()));


  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Starting Knowledge\n"));
  knowledge.print_knowledge ();

  // termination is done via signalling from the user (Control+C)
  while (!terminated)
  {
    knowledge.wait (expression);
    knowledge.print("  {S{.left}} {S{.self}} {S{.right}}\n");

    ACE_OS::sleep (1);
  }

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Final Knowledge\n"));
  knowledge.print_knowledge ();

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Exiting\n"));
  return 0;
}



int parse_args (int argc, ACE_TCHAR * argv[])
{
  // options string which defines all short args
  ACE_TCHAR options [] = ACE_TEXT ("i:s:p:o:v:h");

  // create an instance of the command line args
  ACE_Get_Opt cmd_opts (argc, argv, options);

  // set up an alias for '-n' to be '--name'
  cmd_opts.long_option (ACE_TEXT ("id"), 'i', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("stop"), 's', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("processes"), 'p', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("help"), 'h', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("host"), 'o', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("value"), 'v', ACE_Get_Opt::ARG_REQUIRED);
 
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
      // thread number
      id = atoi (cmd_opts.opt_arg ());
      break;
    case 's':
      // thread number
      stop = atoi (cmd_opts.opt_arg ());
      break;
    case 'p':
      // thread number
      processes = atoi (cmd_opts.opt_arg ());
      break;
    case 'v':
      // thread number
      value = atoi (cmd_opts.opt_arg ());
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
      -i (--id)        set process id (0 default)  \n\
      -s (--stop)      stop condition (10 default) \n\
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
