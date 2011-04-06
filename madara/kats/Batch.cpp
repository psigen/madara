
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

#include "madara/kats/Test_Framework.h"
#include "madara/kats/tinyxml.h"

class KATS_Process
{
public:
  KATS_Process ()
  {}

  KATS_Process (const KATS_Process & rhs)
    : process (), process_options (),
      executable (rhs.executable), command_line (rhs.command_line.str ())
  {
    // we're not allowed to copy an ACE_Process
  }
  
  ~KATS_Process ()
  {}

  KATS_Process & 
    operator= (const KATS_Process & rhs)
  {
    executable = rhs.executable;
    command_line.str (rhs.command_line.str ());
    //process_options = rhs.process_options;

    return *this;
  }

  void enable_realtime (void)
  {
    command_line << " -r";
  }

  void enable_debug (void)
  {
    command_line << " -g";
  }

  void set_precondition (const std::string & value)
  {
    command_line << " -b ";
    command_line << value;
  }

  void set_executable (const std::string & value)
  {
    executable = value;
  }

  void set_postcondition (const std::string & value)
  {
    command_line << " -s ";
    command_line << value;
  }

  void set_commandline (const std::string & value)
  {
    command_line << " -c \"";
    command_line << value;
    command_line << "\"";
  }

  void set_killtime (const std::string & value)
  {
    command_line << " -t ";
    command_line << value;
  }

  void set_killsignal (const std::string & value)
  {
    command_line << " -k ";
    command_line << value;
  }

  void set_delaytime (const std::string & value)
  {
    command_line << " -l ";
    command_line << value;
  }

  void set_id (const std::string & value)
  {
    command_line << " -i ";
    command_line << value;
  }

  void set_host (const std::string & value)
  {
    command_line << " -o ";
    command_line << value;
  }

  void set_processes (const std::string & value)
  {
    command_line << " -p ";
    command_line << value;
  }

  void set_workingdir (const std::string & value)
  {
    command_line << " -w ";
    command_line << value;
  }

  void set_environment (const std::string & value)
  {
    command_line << " -e ";
    command_line << value;
  }

  void set_domain (const std::string & value)
  {
    command_line << " -d ";
    command_line << value;
  }

  void set_testname (const std::string & value)
  {
    command_line << " -a ";
    command_line << value;
  }

  std::string
  query_setup (void)
  {
    std::stringstream buffer;
    buffer << executable;
    buffer << " ";
    buffer << command_line.str ();

    return buffer.str ();
  }

  void launch (void)
  {
    // setup the process options
    process_options.command_line ("%s", 
      query_setup ().c_str ());

    process.spawn (process_options);
  }

  int running (void)
  {
    return process.running ();
  }

  ACE_exitcode exit_code (void)
  {
    return process.exit_code ();
  }

  int terminate (void)
  {
    return process.terminate ();
  }

protected:

  ACE_Process process;
  ACE_Process_Options process_options;
  std::string executable;
  std::stringstream command_line;
};

Madara::KATS::Settings settings;

// settings that may be true or false
bool realtime = false;
bool debug_printing = false;
bool run_in_parallel = false;

// flags for whether or not command line arguments
// were supplied
bool process_set = false;
bool kill_time_set = false;
bool id_set = false;
bool processes_set = false;
bool host_set = false;
bool domain_set = false;
bool realtime_set = false;
bool debug_set = false;
bool pre_condition_set = false;
bool post_condition_set = false;
bool parallel_set = false;
bool delay_time_set = false;
bool test_name_set = false;

std::string test_name;
std::string pre_condition;
std::string post_condition;
std::string tests_file;
ACE_Time_Value kill_time (0);
ACE_Time_Value delay_time (0);
//std::string process_name;
//std::stringstream command_line;
//bool test_set = false;
//bool delay_time_set = false;

// default is SIGTERM, though we use terminate
// unless a signal is set to be more portable
//bool signal_set = false;
//int kill_signal = 15;
//

// command line arguments
int parse_args (int argc, ACE_TCHAR * argv[]);


int ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{
  ACE_TRACE (ACE_TEXT ("main"));

  std::vector <KATS_Process> processes;

  // parse any arguments
  int retcode = parse_args (argc, argv);

  // for returning back to the user after process spawn
  int return_value = 0;

  if (retcode < 0)
    return retcode;

  if(debug_printing)
    ACE_LOG_MSG->priority_mask (LM_DEBUG | LM_INFO, ACE_Log_Msg::PROCESS);
  else
    ACE_LOG_MSG->priority_mask (LM_INFO, ACE_Log_Msg::PROCESS);


  // read the file
  TiXmlDocument doc (tests_file);

  if (!doc.LoadFile ())
  {
    ACE_DEBUG ((LM_INFO, 
      "KATS_BATCH: Unable to open file %s\n", tests_file.c_str ()));
    return -2;
  }

  TiXmlNode * node = 0;
	TiXmlElement * el_tests = 0; 
	TiXmlElement * el_globals = 0; 
	TiXmlElement * el_domain = 0; 
  TiXmlElement * element = 0;
	TiXmlAttribute * attribute = 0; 

  el_tests  = doc.FirstChildElement ("tests");;

  if (!el_tests)
  {
    ACE_DEBUG ((LM_INFO, 
      "KATS_BATCH: Tests was not the root element\n"));
    return -2;
  }

  ACE_DEBUG ((LM_DEBUG, 
    "KATS_BATCH: Reading tests setup...\n"));

  el_globals = el_tests->FirstChildElement ("setup");

  // we have a globals element
  if (el_globals)
  {
    // only load globals from the file if they were not
    // supplied via command line (command line has highest priority)

    if (!id_set)
    {
      element = el_globals->FirstChildElement ("id");
      if (element)
      {
        if (element->GetText ())
        {
          std::stringstream buffer;
          buffer << element->GetText ();
          buffer >> settings.id;

          ACE_DEBUG ((LM_DEBUG, 
            "KATS_BATCH:   Read id = %d from tests file\n", settings.id));
        }
      } // if the id element existed
    } // if the user didn't specify id from the command line

    if (!domain_set)
    {
      element = el_globals->FirstChildElement ("domain");
      if (element)
      {
        if (element->GetText ())
        {
          settings.domains = element->GetText ();
          domain_set = true;

          ACE_DEBUG ((LM_DEBUG, 
            "KATS_BATCH:   Read domain = %s from tests file\n",
            settings.domains.c_str ()));
        }
      } // if the domain element existed
    } // if the user didn't specify domain from the command line

    if (!host_set)
    {
      element = el_globals->FirstChildElement ("host");
      if (element)
      {
        if (element->GetText ())
        {
          settings.host = element->GetText ();
          host_set = true;

          ACE_DEBUG ((LM_DEBUG, 
            "KATS_BATCH:   Read host = %s from tests file\n",
            settings.host.c_str ()));
        }
      } // if the host element existed
    } // if the user didn't specify host from the command line

    if (!processes_set)
    {
      element = el_globals->FirstChildElement ("processes");
      if (element)
      {
        if (element->GetText ())
        {
          std::stringstream buffer;
          buffer << element->GetText ();
          buffer >> settings.processes;

          ACE_DEBUG ((LM_DEBUG, 
            "KATS_BATCH:   Read processes = %d from tests file\n",
            settings.processes));
        }
      } // if the processes element existed
    } // if the user didn't specify processes from the command line

    if (!kill_time_set)
    {
      element = el_globals->FirstChildElement ("kill");
      if (element)
      {
        element = element->FirstChildElement ("time");
        if (element)
        {
          if (element->GetText ())
          {
            time_t time_in_seconds;
            std::stringstream buffer;
            buffer << element->GetText ();
            buffer >> time_in_seconds;

            kill_time.sec (time_in_seconds);
            kill_time_set = true;

            ACE_DEBUG ((LM_DEBUG,
              "KATS_BATCH:   Read kill time = %d s from tests file\n",
              time_in_seconds));
          }
        } // if element kill/time exists
      } // if element kill exists
    } // if the user didn't specify a kill time from the command line

    if (!pre_condition_set)
    {
      element = el_globals->FirstChildElement ("precondition");
      if (element)
      {
        if (element->GetText ())
        {
          pre_condition = element->GetText ();

          ACE_DEBUG ((LM_DEBUG, 
            "KATS_BATCH:   Read precondition = %s from tests file\n",
            pre_condition.c_str ()));
        }
      } // if precondition element existed
    } // if the user didn't specify a precondition from the command line

    if (!post_condition_set)
    {
      element = el_globals->FirstChildElement ("postcondition");
      if (element)
      {
        if (element->GetText ())
        {
          post_condition = element->GetText ();

          ACE_DEBUG ((LM_DEBUG, 
            "KATS_BATCH:   Read postcondition = %s from tests file\n",
            post_condition.c_str ()));
        }
      } // if postcondition element existed
    } // if the user didn't specify a postcondition from the command line

    if (!test_name_set)
    {
      element = el_globals->FirstChildElement ("name");
      if (element)
      {
        if (element->GetText ())
        {
          test_name = element->GetText ();

          ACE_DEBUG ((LM_DEBUG, 
            "KATS_BATCH:   Read test name = %s from tests file\n",
            test_name.c_str ()));
        }
      } // if name element existed
    } // if the user didn't specify a test name from the command line

    if (!realtime_set)
    {
      element = el_globals->FirstChildElement ("realtime");
      if (element)
      {
        realtime = true;
        realtime_set = true;
        ACE_DEBUG ((LM_DEBUG, 
          "KATS_BATCH:   Enabled realtime scheduling from tests file\n"));
      } // if realtime element existed
    } // if the user didn't specify real time scheduling from the command line

    if (!debug_set)
    {
      element = el_globals->FirstChildElement ("debug");
      if (element)
      {
        debug_printing = true;
        debug_set = true;
        ACE_DEBUG ((LM_DEBUG, 
          "KATS_BATCH:   Enabled debug mode printing from tests file\n"));
      } // if debug element existed
    } // if the user didn't specify debug mode from the command line
  }

  ACE_DEBUG ((LM_DEBUG, 
    "KATS_BATCH: Finished reading tests setup. Proceeding to tests...\n"));

  size_t cur = 0;

  for (cur = 0, element = el_tests->FirstChildElement ("test");
        element; element = element->NextSiblingElement ("test"), ++cur);

  ACE_DEBUG ((LM_DEBUG, 
    "KATS_BATCH: %d tests found in %s...\n", cur, tests_file.c_str ()));

  //processes.reserve (cur);
  processes.resize (cur);

  for (cur = 0, element = el_tests->FirstChildElement ("test");
        element; element = element->NextSiblingElement ("test"), ++cur)
  {
    ACE_DEBUG ((LM_DEBUG, 
        "KATS_BATCH:   Grabbing a test...\n"));
    KATS_Process process;

    TiXmlElement * el_temp1 = element->FirstChildElement ("name");
    TiXmlElement * el_temp2 = element->FirstChildElement ("executable");
    
    // if the file didn't have a name or executable for an individual test
    // then let them know about the problem
    if (!el_temp1 || !el_temp2 ||
        !el_temp1->GetText () || !el_temp2->GetText ())
    {
      ACE_DEBUG ((LM_INFO, 
        "KATS_BATCH:   Each test must have a <name> and <executable>\n"));
      return -3;
    }

    // go ahead and set the executable and test name

    processes[cur].set_testname (el_temp1->GetText ());
    processes[cur].set_executable (el_temp2->GetText ());

    // check the processes setting
    el_temp1 = element->FirstChildElement ("processes");
    if (el_temp1 && el_temp1->GetText ())
    {
      ACE_DEBUG ((LM_DEBUG, 
        "KATS_BATCH:   Read host = %s from tests file\n",
           el_temp1->GetText ()));
      processes[cur].set_processes (el_temp1->GetText ());
    }

    // check the id setting
    el_temp1 = element->FirstChildElement ("id");
    if (el_temp1 && el_temp1->GetText ())
    {
      ACE_DEBUG ((LM_DEBUG, 
        "KATS_BATCH:   Read id = %s from tests file\n",
           el_temp1->GetText ()));
      processes[cur].set_id (el_temp1->GetText ());
    }

    // check the executable environment
    el_temp1 = element->FirstChildElement ("environment");
    if (el_temp1 && el_temp1->GetText ())
    {
      ACE_DEBUG ((LM_DEBUG, 
        "KATS_BATCH:   Read env = %s from tests file\n",
           el_temp1->GetText ()));
      processes[cur].set_environment (el_temp1->GetText ());
    }

    // check the delay
    el_temp1 = element->FirstChildElement ("delay");
    if (el_temp1 && el_temp1->GetText ())
    {
      ACE_DEBUG ((LM_DEBUG, 
        "KATS_BATCH:   Read delay = %s from tests file\n",
           el_temp1->GetText ()));
      processes[cur].set_delaytime (el_temp1->GetText ());
    }

    // check the precondition
    el_temp1 = element->FirstChildElement ("precondition");
    if (el_temp1 && el_temp1->GetText ())
    {
      ACE_DEBUG ((LM_DEBUG, 
        "KATS_BATCH:   Read precondition = %s from tests file\n",
           el_temp1->GetText ()));
      processes[cur].set_precondition (el_temp1->GetText ());
    }

    // check the postcondition
    el_temp1 = element->FirstChildElement ("postcondition");
    if (el_temp1 && el_temp1->GetText ())
    {
      ACE_DEBUG ((LM_DEBUG, 
        "KATS_BATCH:   Read postcondition = %s from tests file\n",
           el_temp1->GetText ()));
      processes[cur].set_postcondition (el_temp1->GetText ());
    }

    // check the commandline
    el_temp1 = element->FirstChildElement ("commandline");
    if (el_temp1 && el_temp1->GetText ())
    {
      ACE_DEBUG ((LM_DEBUG, 
        "KATS_BATCH:   Read commandline = %s from tests file\n",
           el_temp1->GetText ()));
      processes[cur].set_commandline (el_temp1->GetText ());
    }

    // kill may be a nested element, e.g. the following:
    // <kill>
    //   <time>5</time>
    //   <signal>15</time>
    // </kill>
    el_temp1 = element->FirstChildElement ("kill");
    if (el_temp1)
    {
      // we have a kill element

      // check for a timeout
      el_temp2 = el_temp1->FirstChildElement ("time");
      if (el_temp2)
      {
        // we have a kill/time
        if (el_temp2->GetText ())
        {
          processes[cur].set_killtime (el_temp2->GetText ());
        }
      }

      // check for a signal
      el_temp2 = el_temp1->FirstChildElement ("signal");
      if (el_temp2)
      {
        // we have a kill/signal
        if (el_temp2->GetText ())
        {
          processes[cur].set_killsignal (el_temp2->GetText ());
        }
      }
    }

    // check the workingdir
    el_temp1 = element->FirstChildElement ("workingdir");
    if (el_temp1 && el_temp1->GetText ())
    {
      ACE_DEBUG ((LM_DEBUG, 
        "KATS_BATCH:   Read workingdir = %s from tests file\n",
           el_temp1->GetText ()));
      processes[cur].set_workingdir (el_temp1->GetText ());
    }

    // check the settings that are overridable from command line

    if (realtime || element->FirstChildElement ("realtime"))
    {
      ACE_DEBUG ((LM_DEBUG, 
        "KATS_BATCH:   Enabling real time scheduling\n"));
      processes[cur].enable_realtime (); 
    }

    if (debug_printing ||  element->FirstChildElement ("debug"))
    {
      ACE_DEBUG ((LM_DEBUG, 
        "KATS_BATCH:   Enabling real time scheduling\n"));
      processes[cur].enable_debug ();
    }

    // check the host setting
    if (!host_set)
    {
      el_temp1 = element->FirstChildElement ("host");
      if (el_temp1 && el_temp1->GetText ())
      {
        ACE_DEBUG ((LM_DEBUG, 
          "KATS_BATCH:   Read host = %s from test entry\n",
          el_temp1->GetText ()));
        processes[cur].set_id (el_temp1->GetText ());
      }
    }
    // user can override host on the command line
    else
    {
      ACE_DEBUG ((LM_DEBUG, 
        "KATS_BATCH:   Using host = %s from globals\n",
        settings.host.c_str ()));
      processes[cur].set_host (settings.host);
    }

    // check the domain setting
    if (!domain_set)
    {
      el_temp1 = element->FirstChildElement ("domain");
      if (el_temp1 && el_temp1->GetText ())
      {
        ACE_DEBUG ((LM_DEBUG, 
          "KATS_BATCH:   Read domain = %s from test entry\n",
          el_temp1->GetText ()));
        processes[cur].set_id (el_temp1->GetText ());
      }
    }
    // user can override domain on the command line
    else
    {
      ACE_DEBUG ((LM_DEBUG, 
        "KATS_BATCH:   Using domain = %s from globals\n",
        settings.domains.c_str ()));
      processes[cur].set_domain (settings.domains);
    }

    
  }

  return 0;


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

  return 0;
}

int parse_args (int argc, ACE_TCHAR * argv[])
{
  // options string which defines all short args
  ACE_TCHAR options [] = ACE_TEXT ("f:n:i:l:o:d:a:t:grh");

  // create an instance of the command line args
  ACE_Get_Opt cmd_opts (argc, argv, options);

  // set up an alias for '-n' to be '--name'
  cmd_opts.long_option (ACE_TEXT ("testname"), 'a', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("domain"), 'd', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("file"), 'f', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("debug"), 'g', ACE_Get_Opt::NO_ARG);
  cmd_opts.long_option (ACE_TEXT ("help"), 'h', ACE_Get_Opt::NO_ARG);
  cmd_opts.long_option (ACE_TEXT ("id"), 'i', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("processes"), 'n', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("host"), 'o', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("parallel"), 'p', ACE_Get_Opt::NO_ARG);
  cmd_opts.long_option (ACE_TEXT ("realtime"), 'r', ACE_Get_Opt::NO_ARG);
  cmd_opts.long_option (ACE_TEXT ("delay"), 'l', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("killtime"), 't', ACE_Get_Opt::ARG_REQUIRED);
 
  // temp for current switched option
  int option;

  // iterate through the options until done
  while ((option = cmd_opts ()) != EOF)
  {
    //arg = cmd_opts.opt_arg ();
    switch (option)
    {
    case 'a':
      // thread number
      test_name = cmd_opts.opt_arg ();
      test_name_set = true;

      ACE_DEBUG ((LM_DEBUG, "KATS_BATCH: test name set to %s\n",
        cmd_opts.opt_arg (), test_name.c_str () ));

      break;
    case 'b':
      // a precondition

      pre_condition = cmd_opts.opt_arg ();
      pre_condition_set = true;

      ACE_DEBUG ((LM_DEBUG, "KATS_BATCH: precondition set to %s.\n", 
        cmd_opts.opt_arg (), pre_condition.c_str () ));

      break;
    case 'd':
      // the knowledge domain

      settings.domains = cmd_opts.opt_arg ();
      domain_set = false;

      ACE_DEBUG ((LM_DEBUG, "KATS_BATCH: knowledge domain set to %s.\n", 
        cmd_opts.opt_arg (), settings.domains.c_str () ));

      break;
    case 'f':
      // the knowledge domain

      tests_file = cmd_opts.opt_arg ();

      ACE_DEBUG ((LM_DEBUG, "KATS_BATCH: reading tests from %s.\n", 
        cmd_opts.opt_arg (), tests_file.c_str () ));

      break;
    case 'g':
      // thread number
      debug_printing = true;
      debug_set = true;

      ACE_DEBUG ((LM_DEBUG, "KATS_BATCH: enabling debug printing\n"));

      break;
    case 'i':
      // id
      {
        std::stringstream buffer;
        buffer << cmd_opts.opt_arg ();
        buffer >> settings.id;
      }
      id_set = true;

      ACE_DEBUG ((LM_DEBUG, "KATS_BATCH: id set to %d\n", 
      cmd_opts.opt_arg (), settings.id ));

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

      ACE_DEBUG ((LM_DEBUG, "KATS_BATCH: delay time set to %d seconds\n", 
        cmd_opts.opt_arg (), delay_time.sec () ));

      break;
    case 'n':
      // processes
      {
        std::stringstream buffer;
        buffer << cmd_opts.opt_arg ();
        buffer >> settings.processes;
      }
      processes_set = true;

      ACE_DEBUG ((LM_DEBUG, "KATS_BATCH: processes set to %d\n", 
        cmd_opts.opt_arg (), settings.processes ));

      break;
    case 'o':
      // host name
      settings.host = cmd_opts.opt_arg ();
      host_set = true;

      ACE_DEBUG ((LM_DEBUG, "KATS_BATCH: host set to %s\n",
        cmd_opts.opt_arg (), settings.host.c_str () ));

      break;
    case 'p':
      // thread number
      run_in_parallel = true;
      parallel_set = true;

      ACE_DEBUG ((LM_DEBUG,
        "KATS_BATCH: running all batch tests in parallel\n"));

      break;
    case 'r':
      // thread number
      realtime = true;
      realtime_set = true;

      ACE_DEBUG ((LM_DEBUG, "KATS_BATCH: enabling realtime scheduling\n"));

      break;
    case 's':
      // a postcondition

      post_condition = cmd_opts.opt_arg ();
      post_condition_set = true;

      ACE_DEBUG ((LM_DEBUG, "KATS_BATCH: postcondition set to %s.\n",
        cmd_opts.opt_arg (), post_condition.c_str () ));

      break;
    case 't':
      // time to kill the process
      {
        time_t time_in_seconds;
        std::stringstream buffer;
        buffer << cmd_opts.opt_arg ();
        buffer >> time_in_seconds;

        kill_time.sec (time_in_seconds);
        kill_time_set = true;
      }

      ACE_DEBUG ((LM_DEBUG,
        "KATS_BATCH: -t = %s; terminate all test processes after %d seconds\n",
        cmd_opts.opt_arg (), kill_time.sec () ));

      break;
    case ':':
      ACE_ERROR_RETURN ((LM_ERROR, 
        ACE_TEXT ("KATS_BATCH: ERROR: -%c requires an argument"),
           cmd_opts.opt_opt ()), -2); 
    case 'h':
    default:
      ACE_DEBUG ((LM_INFO, "Program Options:      \n\
      -f (--file)        xml file containing testing information \n\
      -n (--processes)   number of testing processes \n\
      -i (--id)          this process id        \n\
      -g (--debug)       prints KATS debug messages \n\
      -o (--host)        host identifier        \n\
      -l (--delay)       time delay (secs) after barrier to wait before spawn\n\
      -d (--domain)      knowledge domain to isolate knowledge into \n\
      -b (--precondition) precondition to wait for after barrier \n\
      -s (--postcondition) postcondition to set after process exits \n\
      -a (--testname)    name of the test (for barriers) \n\
      -t (--timeout)     time in seconds to wait before killing \n\
      -r (--realtime)    run the process with real time scheduling \n\
      -o (--host)        host identifier        \n\
      -h (--help)        print this menu        \n"
      ));
      ACE_ERROR_RETURN ((LM_ERROR, 
        ACE_TEXT ("Returning from Help Menu\n")), -1); 
      break;
    }
  }

  return 0;
}

