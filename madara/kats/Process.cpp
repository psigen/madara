
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

  // parse any arguments
  int retcode = parse_args (argc, argv);

  // for returning back to the user after process spawn
  int return_value = 0;

  if (retcode < 0)
    return retcode;

  if (!process_set)
  {
    MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
      "\nKATS_PROCESS:"\
      " no process name was specified. Try -h or --help\n"));
    return -1;
  }

  if (!test_set)
  {
    MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
      "\nKATS_PROCESS:"\
      " no test name was specified. Try -h or --help\n"));
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

  if(debug_printing)
    testing.dump ();

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

  ACE_Process process;
  ACE_exitcode status;
  process.spawn (process_options);

  // if a kill time is set, then we use a different type of wait
  if (kill_time_set)
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "KATS_PROCESS: User specified kill time of %: s\n",
        kill_time.sec () ));

    pid_t ret = process.wait (kill_time, &status);

    // if return value is zero, then we have a timeout.
    if (ret == 0)
    {
      // allow users to specify a kill signal
      if (signal_set)
      {
        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "KATS_PROCESS: Process timed out. Sending %s a %d signal\n",
          process_options.process_name (), kill_signal ));

        process.kill (kill_signal);
        process.wait ();
      }
      else
      {
        MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
          DLINFO "KATS_PROCESS: Process timed out. Terminating %s\n",
          process_options.process_name () ));

        // otherwise sigterm
        process.terminate ();
      }
    }
  }

  else
  {
    process.wait ();
  }

  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
    DLINFO "KATS_PROCESS: Process returned %u\n",
    process.exit_code () ));

  return_value = process.exit_code ();

  if (post_condition != "")
  {
    std::stringstream buffer;
    buffer << test_name;
    buffer << ".post.";
    buffer << "{.madara.id}";

    testing.event (buffer.str (), post_condition, "");
  }

  if(debug_printing)
    testing.dump ();

  return return_value;
}

int parse_args (int argc, ACE_TCHAR * argv[])
{
  // options string which defines all short args
  ACE_TCHAR options [] = ACE_TEXT ("n:i:o:e:w:l:t:x:a:c:d:b:s:k:grh");

  // create an instance of the command line args
  ACE_Get_Opt cmd_opts (argc, argv, options);

  // set up an alias for '-n' to be '--name'
  cmd_opts.long_option (ACE_TEXT ("help"), 'h', ACE_Get_Opt::NO_ARG);
  cmd_opts.long_option (ACE_TEXT ("debug"), 'g', ACE_Get_Opt::NO_ARG);
  cmd_opts.long_option (ACE_TEXT ("realtime"), 'r', ACE_Get_Opt::NO_ARG);
  cmd_opts.long_option (ACE_TEXT ("environment"), 'e',
      ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("commandline"), 'c',
      ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("executable"), 'x',
      ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("precondition"), 'b',
      ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("postcondition"), 's',
      ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("domain"), 'd', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("testname"), 'a', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("signal"), 'k', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("killtime"), 't', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("delay"), 'l', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("working"), 'w', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("processes"), 'n', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("id"), 'i', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("host"), 'o', ACE_Get_Opt::ARG_REQUIRED);
 
  // temp for current switched option
  int option;

  // iterate through the options until done
  while ((option = cmd_opts ()) != EOF)
  {
    //arg = cmd_opts.opt_arg ();
    switch (option)
    {
    case 'i':
      // id
      {
        std::stringstream buffer;
        buffer << cmd_opts.opt_arg ();
        buffer >> settings.id;
      }

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_PROCESS: id in test process ring set to %u\n",
        settings.id));

      break;
    case 'k':
      // kill signal
      {
        std::stringstream buffer;
        buffer << cmd_opts.opt_arg ();
        buffer >> kill_signal;
        signal_set = true;
      }

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_PROCESS: signal to kill executable set to %d\n",
        kill_signal));

      break;
    case 'n':
      // processes
      {
        std::stringstream buffer;
        buffer << cmd_opts.opt_arg ();
        buffer >> settings.processes;
      }

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_PROCESS: processes involved in test set to %u\n",
        settings.processes));


      break;
    case 'l':
      // time to delay after barrier before starting the process
      {
        time_t time_in_seconds;
        std::stringstream buffer;
        buffer << cmd_opts.opt_arg ();
        buffer >> time_in_seconds;

        delay_time.sec (time_in_seconds);
        delay_time_set = true;
      }

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_PROCESS: delay executable by %: seconds\n",
        delay_time.sec ()));

      break;
    case 't':
      // time to kill the process
      {
        int time_in_seconds;
        std::stringstream buffer;
        buffer << cmd_opts.opt_arg ();
        buffer >> time_in_seconds;

        kill_time.sec (time_in_seconds);
        kill_time_set = true;
      }

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_PROCESS: kill executable after by %: seconds\n",
        kill_time.sec ()));

      break;
    case 'e':
      // environment for process

      process_options.setenv (cmd_opts.opt_arg (), 
        strlen (cmd_opts.opt_arg ()));

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_PROCESS: environment set to %s\n",
        cmd_opts.opt_arg ()));

      break;
    case 'b':
      // a precondition

      pre_condition = cmd_opts.opt_arg ();

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_PROCESS: precondition set to %s\n",
        pre_condition.c_str ()));

      break;
    case 's':
      // a postcondition

      post_condition = cmd_opts.opt_arg ();

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_PROCESS: postcondition set to %s\n",
        post_condition.c_str ()));

      break;
    case 'd':
      // the knowledge domain

      settings.domains = cmd_opts.opt_arg ();

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_PROCESS: knowledge domain set to %s\n",
        settings.domains.c_str ()));

      break;
    case 'x':
      // the executable

      process_name = cmd_opts.opt_arg ();

      process_options.process_name (cmd_opts.opt_arg ());

      process_set = true;

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_PROCESS: executable set to %s\n",
        cmd_opts.opt_arg ()));

      break;
    case 'c':
      // the command line arguments

      //process_options.command_line ("%s", cmd_opts.opt_arg ());

      if (cmd_opts.opt_arg ())
        command_line << cmd_opts.opt_arg ();

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_PROCESS: command line set to %s\n",
        command_line.str ().c_str ()));

      break;
    case 'w':
      // working directory for process

      process_options.working_directory (cmd_opts.opt_arg ());

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_PROCESS: working directory set to %s\n",
        cmd_opts.opt_arg ()));

      break;
    case 'o':
      // host name
      settings.host = cmd_opts.opt_arg ();

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_PROCESS: host set to %s\n",
        settings.host.c_str ()));

      break;
    case 'a':
      // thread number
      test_name = cmd_opts.opt_arg ();
      test_set = true;

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_PROCESS: test name set to %s\n",
        test_name.c_str ()));

      break;
    case 'r':
      // thread number
      realtime = true;

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_PROCESS: enabling realtime scheduling\n"));

      break;
    case 'g':
      // thread number
      debug_printing = true;

      if(debug_printing)
        ACE_LOG_MSG->priority_mask (LM_DEBUG | LM_INFO, ACE_Log_Msg::PROCESS);
      else
        ACE_LOG_MSG->priority_mask (LM_INFO, ACE_Log_Msg::PROCESS);

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_PROCESS: enabling debug printing\n"));

      break;
    case ':':
      MADARA_ERROR_RETURN (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, 
        ACE_TEXT ("KATS_PROCESS: ERROR: -%c requires an argument"),
           cmd_opts.opt_opt ()), -2); 
    case 'h':
    default:
      MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_INFO, "Program Options:    \n\
      -n (--processes)   number of testing processes \n\
      -i (--id)          this process id        \n\
      -g (--debug)       prints KATS debug messages \n\
      -o (--host)        host identifier        \n\
      -e (--environment) environment variables (\"key=value\") \n\
      -l (--delay)       time delay (secs) after barrier to wait before spawn\n\
      -d (--domain)      knowledge domain to isolate knowledge into \n\
      -b (--precondition) precondition to wait for after barrier \n\
      -s (--postcondition) postcondition to set after process exits \n\
      -c (--commandline) command line arguments \n\
      -x (--executable)  executable \n\
      -w (--working)     working directory (default=.) \n\
      -a (--testname)    name of the test (for barriers) \n\
      -t (--timeout)     time in seconds to wait before killing \n\
         -k --signal     kill signal to send after timeout \n\
      -r (--realtime)    run the process with real time scheduling \n\
      -o (--host)        host identifier        \n\
      -h (--help)        print this menu        \n"
      ));
      MADARA_ERROR_RETURN (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, 
        ACE_TEXT ("Returning from Help Menu\n")), -1); 
      break;
    }
  }

  return 0;
}

