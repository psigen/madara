
#define MADARA_NTRACE    0
#define ACE_NTRACE    0
//#define ACE_NLOGGING  0
#define ACE_NDEBUG    0

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <assert.h>
#include <iomanip>

#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/OS_NS_Thread.h"
#include "ace/Sched_Params.h"
#include "ace/Process.h"

#include "madara/utility/Log_Macros.h"
#include "madara/kats/Test_Framework.h"

Madara::KATS::Settings settings;
ACE_Process_Options process_options;
std::string test_name;
std::string process_name;
std::stringstream command_line;
bool realtime = false;
bool process_set = false;
bool test_set = false;
bool kill_time_set = false;
bool delay_time_set = false;
bool debug_printing = false;
int exit_code = 0;
ACE_Time_Value kill_time (0);
ACE_Time_Value delay_time (0);

// default is SIGTERM, though we use terminate
// unless a signal is set to be more portable
bool signal_set = false;
int kill_signal = 15;

std::string pre_condition;
std::string post_condition;

// command line arguments
int parse_args (int argc, ACE_TCHAR * argv[]);


int ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{
  // set the default working directory to the current directory
  process_options.working_directory (".");

  // by default, see LM_INFO and LM_ERROR logging messages
  // the user has to provide -g to see LM_DEBUG
  ACE_LOG_MSG->priority_mask (LM_INFO | LM_ERROR, ACE_Log_Msg::PROCESS);

  // parse any arguments
  int retcode = parse_args (argc, argv);

  if (retcode < 0)
    return retcode;

  if (!delay_time_set)
  {
    MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
      "\nKATS_SLEEP:"\
      " no sleep time was specified. Try -h or --help\n"));
    return -1;
  }

  // setup the process options
  process_options.command_line ("%s %s", 
    process_name.c_str (), command_line.str ().c_str ());

  if (realtime)
  {
    // use ACE real time scheduling class
    int prio  = ACE_Sched_Params::next_priority
      (ACE_SCHED_FIFO,
       ACE_Sched_Params::priority_max (ACE_SCHED_FIFO),
       ACE_SCOPE_THREAD);
    ACE_OS::thr_setprio (prio);
  }

  Madara::KATS::Test_Framework testing (settings);

  testing.event (
    "updatevars","",".kats.id=.madara.id;.kats.processes=.madara.processes");

  if(debug_printing)
    testing.dump ();

  if (test_name != "")
    testing.barrier (test_name);

  // Before we check for delay, we first check for a precondition

  if (pre_condition != "")
  {
    std::stringstream buffer;
    buffer << test_name;
    buffer << ".pre.";
    buffer << "{.madara.id}";

    testing.event (buffer.str (), pre_condition, "");
  }

  // sleep for a set amount of time after the barrier (if specified)
  if (delay_time_set)
  {
    ACE_OS::sleep (delay_time);
  }


  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
    DLINFO "KATS_SLEEP: %: seconds of sleep completed\n",
    delay_time.sec () ));

  testing.set (".kats.return", (long long) exit_code);

  if (post_condition != "")
  {
    std::stringstream buffer;
    buffer << test_name;
    buffer << ".post.";
    buffer << "{.madara.id}";

    testing.event (buffer.str (), "", post_condition, "");
  }

  if(debug_printing)
    testing.dump ();

  return exit_code;
}

int parse_args (int argc, ACE_TCHAR * argv[])
{
  // options string which defines all short args
  ACE_TCHAR options [] = ACE_TEXT ("n:i:o:e:w:l:t:x:a:c:d:b:s:v:k:grh");

  // create an instance of the command line args
  ACE_Get_Opt cmd_opts (argc, argv, options);

  // set up an alias for '-n' to be '--name'
  cmd_opts.long_option (ACE_TEXT ("testname"), 'a', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("precondition"), 'b',
      ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("domain"), 'd', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("debug"), 'g', ACE_Get_Opt::NO_ARG);
  cmd_opts.long_option (ACE_TEXT ("help"), 'h', ACE_Get_Opt::NO_ARG);
  cmd_opts.long_option (ACE_TEXT ("id"), 'i', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("sleep"), 'l', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("delay"), 'l', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("processes"), 'n', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("host"), 'o', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("exitcode"), 'x', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("exitvalue"), 'x', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("returnvalue"), 'x', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("realtime"), 'r', ACE_Get_Opt::NO_ARG);
  cmd_opts.long_option (ACE_TEXT ("postcondition"), 's',
      ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("loglevel"), 'v', ACE_Get_Opt::ARG_REQUIRED);
 
  // temp for current switched option
  int option;

  // iterate through the options until done
  while ((option = cmd_opts ()) != EOF)
  {
    //arg = cmd_opts.opt_arg ();
    switch (option)
    {
    case 'a':
      // test name
      test_name = cmd_opts.opt_arg ();
      test_set = true;

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_SLEEP: test name set to %s\n",
        test_name.c_str ()));

      break;
    case 'b':
      // a precondition

      pre_condition = cmd_opts.opt_arg ();

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_SLEEP: precondition set to %s\n",
        pre_condition.c_str ()));

      break;
    case 'd':
      // the knowledge domain

      settings.domains = cmd_opts.opt_arg ();

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_SLEEP: knowledge domain set to %s\n",
        settings.domains.c_str ()));

      break;
    case 'g':
      // thread number
      debug_printing = true;

      if(debug_printing)
        ACE_LOG_MSG->priority_mask (LM_DEBUG | LM_INFO | LM_ERROR, ACE_Log_Msg::PROCESS);

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_SLEEP: enabling debug printing\n"));

      break;
    case 'i':
      // id
      {
        std::stringstream buffer;
        buffer << cmd_opts.opt_arg ();
        buffer >> settings.id;
      }

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_SLEEP: id in test process ring set to %u\n",
        settings.id));

      break;
    case 'l':
      // time to sleep after barrier
      {
        time_t time_in_seconds;
        std::stringstream buffer;
        buffer << cmd_opts.opt_arg ();
        buffer >> time_in_seconds;

        delay_time.sec (time_in_seconds);
        delay_time_set = true;
      }

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_SLEEP: sleep time set to %: seconds\n",
        delay_time.sec ()));

      break;
    case 'n':
      // processes
      {
        std::stringstream buffer;
        buffer << cmd_opts.opt_arg ();
        buffer >> settings.processes;
      }

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_SLEEP: processes involved in test set to %u\n",
        settings.processes));


      break;
    case 'o':
      // host name
      settings.host = cmd_opts.opt_arg ();

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_SLEEP: host set to %s\n",
        settings.host.c_str ()));

      break;
    case 'r':
      // thread number
      realtime = true;

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_SLEEP: enabling realtime scheduling\n"));

      break;
    case 's':
      // a postcondition

      post_condition = cmd_opts.opt_arg ();

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_SLEEP: postcondition set to %s\n",
        post_condition.c_str ()));

      break;
    case 'v':
      // log level
      {
        std::stringstream buffer;
        buffer << cmd_opts.opt_arg ();
        buffer >> MADARA_debug_level;
      }

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_SLEEP: logging level set to %u\n",
        MADARA_debug_level));

      break;
    case 'x':
      // processes
      {
        std::stringstream buffer;
        buffer << cmd_opts.opt_arg ();
        buffer >> exit_code;
      }

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_SLEEP: exit code set to %d\n",
        exit_code));


      break;
    case ':':
      MADARA_ERROR_RETURN (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, 
        ACE_TEXT ("KATS_SLEEP: ERROR: -%c requires an argument"),
           cmd_opts.opt_opt ()), -2); 
    case 'h':
    default:
      MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_INFO, "Program Options:    \n\
      -a (--testname)    name of the test (for barriers) \n\
      -b (--precondition) precondition to wait for after barrier \n\
      -d (--domain)      knowledge domain to isolate knowledge into \n\
      -g (--debug)       prints KATS debug messages \n\
      -i (--id)          this process id        \n\
      -h (--help)        print this menu        \n\
      -l (--sleep)       sleep time (secs) after barrier\n\
         (--delay)       \n\
      -n (--processes)   number of testing processes \n\
      -o (--host)        host identifier        \n\
      -r (--realtime)    run the process with real time scheduling \n\
      -s (--postcondition) postcondition to set after process exits \n\
      -t (--timeout)     time in seconds to wait before killing \n\
         -k --signal     kill signal to send after timeout \n\
      -v (--loglevel)    maximum log level to print from MADARA messages\n\
      -x (--exit_code)   exit code to return \n\
          --exit_value \n\
          --return_value \n\
"
      ));
      MADARA_ERROR_RETURN (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, 
        ACE_TEXT ("Returning from Help Menu\n")), -1); 
      break;
    }
  }

  return 0;
}

