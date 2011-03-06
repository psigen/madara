
#define ACE_NTRACE    0
//#define ACE_NLOGGING  0
#define ACE_NDEBUG    0

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <assert.h>

#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/OS_NS_Thread.h"
#include "ace/Sched_Params.h"

#include "ace/Signal.h"

#include "madara/knowledge_engine/Knowledge_Base.h"
#include "madara/transport/Transport.h"

int id = 0;
int left = 0;
int processes = 1;
int stop = 10;
long value = 0;
unsigned long iterations = 100000;

// test is 3 minutes long
unsigned long long time_limit = 3 * 60 * (unsigned long long) 1000000000;

std::string host = "localhost";

volatile bool terminated = false;

// command line arguments
int parse_args (int argc, ACE_TCHAR * argv[]);

// signal handler for someone hitting control+c
extern "C" void terminate (int)
{
  terminated = true;
}

std::string 
to_legible_hertz (unsigned long long hertz)
{
  std::stringstream buffer;

  std::locale loc(""); 
  buffer.imbue (loc); 

  const int ghz_mark = 1000000000;
  const int mhz_mark = 1000000;
  const int khz_mark = 1000;

  double freq = (double) hertz / ghz_mark;

  if (freq >= 1)
  {
    buffer << std::setprecision (2) << std::fixed;
    buffer << freq;
    buffer << " ghz";
    return buffer.str ().c_str ();
  }

  freq = (double) hertz / mhz_mark;

  if (freq >= 1)
  {
    buffer << std::setprecision (2) << std::fixed;
    buffer << freq;
    buffer << " mhz";
    return buffer.str ().c_str ();
  }

  freq = (double) hertz / khz_mark;

  if (freq >= 1)
  {
    buffer << std::setprecision (2) << std::fixed;
    buffer << freq;
    buffer << " khz";
    return buffer.str ().c_str ();
  }

  freq = (double) hertz;

  buffer << std::setprecision (2) << std::fixed;
  buffer << freq;
  buffer << "  hz";
  return buffer.str ().c_str ();
  
}

/**
 * Build a wait string based on some attribute
 * @param    attribute     extension added to P0 on which to build checks for
 * @param    count         number of processes, can be a subset of group
 */
std::string
build_wait_string (int id, const std::string & attribute, int count)
{
  std::stringstream buffer;

  // set our attribute to 1
  buffer << "(P";
  buffer << id;
  buffer << ".";
  buffer << attribute;
  buffer << " = 1)";

  // add logic to wait for all processes in the ring to set their
  // attribute to not-zero
  for (int i = 0; i < count; ++i)
  {
    buffer << " && P";
    buffer << i;
    buffer << ".";
    buffer << attribute;
  }

  return buffer.str ();
}

void
broadcast (Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
           unsigned long iterations)
{
  for (unsigned long i = 1; i <= iterations && !terminated; ++i)
  {
    knowledge.set ("info", i);
  }

  if (terminated)
    knowledge.set ("terminated",1);
}

int ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{
  int retcode = parse_args (argc, argv);

  if (retcode < 0)
    return retcode;

  ACE_LOG_MSG->priority_mask (LM_DEBUG | LM_INFO, ACE_Log_Msg::PROCESS);
  //ACE_LOG_MSG->priority_mask (LM_INFO, ACE_Log_Msg::PROCESS);

  ACE_TRACE (ACE_TEXT ("main"));

  // signal handler for clean exit
  ACE_Sig_Action sa ((ACE_SignalHandler) terminate, SIGINT);

  Madara::Transport::Settings settings;
  settings.type = Madara::Transport::SPLICE;
  settings.domains = "KaRL_Dissemination";
  settings.queue_length = 12000000;
  settings.reliability = Madara::Transport::BEST_EFFORT;

  Madara::Knowledge_Engine::Knowledge_Base knowledge(host, settings);

  // set my id
  knowledge.set (".self", id);
  knowledge.set (".processes", processes);
  knowledge.set (".iterations", iterations);

  std::string type;

  ACE_DEBUG ((LM_INFO, "(%P|%t) (%d of %d) waiting for other processes to join\n",
                        id, processes, iterations));

  // build a logic based on the started attribute and then wait
  // for all processes to start
  knowledge.wait (build_wait_string (id, "started", processes));

  ACE_DEBUG ((LM_INFO, "(%P|%t) (%d of %d) starting dissemination of %d events\n",
                        id, processes, iterations));

  // at this point, all processes are accounted for. Let's begin our tests.

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

  // everyone starts their timer
  timer.start ();

  if (id == 0)
  {
    type = "Publisher";
    // root process is the broadcaster
    // Let's make 1,000,000 modifications and broadcast to everyone
    broadcast (knowledge, iterations);
  }
  else
  {
    type = "Subscriber";
    // everyone else is a receiver
    knowledge.wait ("info >= .iterations || terminated");
  }

  timer.stop ();
  timer.elapsed_time (measured);

  ACE_DEBUG ((LM_INFO, "(%P|%t) (%d of %d) finished dissemination of %d events\n",
                        id, processes, iterations));

  //ACE_DEBUG ((LM_INFO, "(%P|%t) (%d of %d) waiting for other processes to stop\n",
  //                      id, processes, iterations));

  // find the dissemination rate per process
  unsigned long long hertz = ((unsigned long long) 1000000000 * iterations)
                               / measured;

  {
    std::stringstream buffer;
    buffer << " ";
    buffer << type;
    buffer << "\t\t";
    buffer << std::setw (33);
    buffer << to_legible_hertz (hertz);
    buffer << "\n";

    ACE_DEBUG ((LM_INFO, 
      buffer.str ().c_str ()));
  }

  ACE_DEBUG ((LM_INFO, "(%P|%t) (%d of %d) waiting for other processes to stop\n",
                        id, processes, iterations));

  // wait for everyone to stop
  knowledge.wait (build_wait_string (id, "stopped", processes));

  ACE_DEBUG ((LM_INFO, "(%P|%t) Final Knowledge\n"));
  knowledge.print_knowledge ();

  ACE_DEBUG ((LM_INFO, "(%P|%t) Exiting\n"));
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
    case 'n':
      // thread number
      iterations = atoi (cmd_opts.opt_arg ());
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
      ACE_DEBUG ((LM_INFO, "(%P|%t) host set to %s\n", host.c_str ()));
      break;
    case ':':
      ACE_ERROR_RETURN ((LM_ERROR, 
        ACE_TEXT ("ERROR: -%c requires an argument"), 
           cmd_opts.opt_opt ()), -2); 
    case 'h':
    default:
      ACE_DEBUG ((LM_INFO, "Program Options:      \n\
      -p (--processes) number of processes that will be running\n\
      -i (--id)        set process id (0 default)  \n\
      -n (--iterations) set number of iterations (100,000 by default)  \n\
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
