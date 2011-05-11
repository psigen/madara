
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

#include "madara/kats/Test_Framework.h"
#include "madara/kats/tinyxml.h"
#include "madara/utility/Utility.h"

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
bool loglevel_set = false;

std::string test_name;
std::string pre_condition;
std::string post_condition;
std::string tests_file;
ACE_Time_Value kill_time (0);
ACE_Time_Value delay_time (0);

std::string path;


class KATS_Process
{
public:
  KATS_Process ()
  {
    std::stringstream buffer;
    buffer << path;
    buffer << "kats_process";
    executable = buffer.str ();
  }

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
    command_line << " -b \"";
    command_line << value;
    command_line << "\"";
  }

  void set_executable (const std::string & value)
  {
    command_line << " -x ";
    command_line << value;
  }

  void set_postcondition (const std::string & value)
  {
    command_line << " -s \"";
    command_line << value;
    command_line << "\"";
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

  void set_stdin (const std::string & value)
  {
    command_line << " -0 ";
    command_line << value;
  }

  void set_stdout (const std::string & value)
  {
    command_line << " -1 ";
    command_line << value;
  }

  void set_stderr (const std::string & value)
  {
    command_line << " -2 ";
    command_line << value;
  }

  void set_loglevel (const std::string & value)
  {
    command_line << " -v ";
    command_line << value;
  }

  void set_processes (const std::string & value)
  {
    command_line << " -n ";
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

  pid_t wait (void)
  {
    return process.wait ();
  }

  pid_t wait (const ACE_Time_Value & value)
  {
    return process.wait (value);
  }

protected:

  ACE_Process process;
  ACE_Process_Options process_options;
  std::string executable;
  std::stringstream command_line;
};

std::string
extract_path (const std::string & name)
{
  std::string::size_type start = 0;
  for (std::string::size_type i = 0; i < name.size (); ++i)
  {
    // check for directory delimiters and update start
    if (name[i] == '/' || name[i] == '\\')
    {
      // if they have supplied a directory with an
      // ending slash, then use the previous start
      if (i != name.size () - 1)
        start = i + 1;
    }
  }

  // return the substring from 0 with start number of elements
  return name.substr (0, start);
}


// command line arguments
int parse_args (int argc, ACE_TCHAR * argv[]);


int ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{
  path = extract_path (argv[0]);

  // by default, see LM_INFO and LM_ERROR logging messages
  // the user has to provide -g to see LM_DEBUG
  ACE_LOG_MSG->priority_mask (LM_INFO | LM_ERROR, ACE_Log_Msg::PROCESS);

  std::vector <KATS_Process> processes;

  // parse any arguments
  int retcode = parse_args (argc, argv);

  // for returning back to the user after process spawn

  if (retcode < 0)
    return retcode;

  if (tests_file == "")
  {
    MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
      "\nKATS_BATCH:"\
      " no process group file was specified. Try -h or --help\n"));
    return -1;

  }

  // read the file
  TiXmlDocument doc (tests_file);

  if (!doc.LoadFile ())
  {
    ACE_DEBUG ((LM_INFO, 
      "KATS_BATCH:  Unable to open file %s\n", tests_file.c_str ()));
    return -2;
  }

  TiXmlElement * el_tests = 0; 
  TiXmlElement * el_globals = 0; 
  TiXmlElement * element = 0;
  TiXmlAttribute * attribute = 0;

  el_tests  = doc.FirstChildElement ("group");;

  if (!el_tests)
  {
    ACE_DEBUG ((LM_INFO, 
      "KATS_BATCH:  group was not the root element\n"));
    return -2;
  }

  ACE_DEBUG ((LM_DEBUG, 
    "KATS_BATCH:  Reading process group setup...\n"));

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
            "KATS_BATCH:    Read id = %d from process group file\n", settings.id));
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
          settings.domains = Madara::Utility::expand_envs (
                              element->GetText ());
          domain_set = true;

          ACE_DEBUG ((LM_DEBUG, 
            "KATS_BATCH:    Read domain = %s from process group file\n",
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
          settings.host = Madara::Utility::expand_envs (element->GetText ());
          host_set = true;

          ACE_DEBUG ((LM_DEBUG, 
            "KATS_BATCH:    Read host = %s from group file\n",
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
            "KATS_BATCH:    Read processes = %d from group file\n",
            settings.processes));
        }
      } // if the processes element existed
    } // if the user didn't specify processes from the command line

    if (!loglevel_set)
    {
      element = el_globals->FirstChildElement ("loglevel");
      if (element)
      {
        if (element->GetText ())
        {
          std::stringstream buffer;
          buffer << element->GetText ();
          buffer >> MADARA_debug_level;

          loglevel_set = true;

          ACE_DEBUG ((LM_DEBUG, 
            "KATS_BATCH:    Read loglevel = %u from process group\n",
            MADARA_debug_level));
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
              "KATS_BATCH:    Read kill time = %d s from process group\n",
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
          pre_condition = Madara::Utility::expand_envs (element->GetText ());

          ACE_DEBUG ((LM_DEBUG, 
            "KATS_BATCH:    Read precondition = %s from process group\n",
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
          post_condition = Madara::Utility::expand_envs (element->GetText ());

          ACE_DEBUG ((LM_DEBUG, 
            "KATS_BATCH:    Read postcondition = %s from process group\n",
            post_condition.c_str ()));
        }
      } // if postcondition element existed
    } // if the user didn't specify a postcondition from the command line

    if (!test_name_set)
    {
      element = el_globals->FirstChildElement ("barrier");
      if (element)
      {
        if (element->Attribute ("name"))
          test_name = Madara::Utility::expand_envs (element->Attribute ("name"));
        else if (element->GetText ())
          test_name = Madara::Utility::expand_envs (element->GetText ());

        ACE_DEBUG ((LM_DEBUG, 
          "KATS_BATCH:    Read barrier name = %s from process group file\n",
          test_name.c_str ()));
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
          "KATS_BATCH:    Enabled realtime scheduling from process group file\n"));
      } // if realtime element existed
    } // if the user didn't specify real time scheduling from the command line

    if (!parallel_set)
    {
      element = el_globals->FirstChildElement ("parallel");
      if (element)
      {
        run_in_parallel = true;
        parallel_set = true;
        ACE_DEBUG ((LM_DEBUG, 
          "KATS_BATCH:    Enabled parallel execution from process group file\n"));
      } // if debug element existed
    } // if the user didn't specify debug mode from the command line

    if (!debug_set)
    {
      element = el_globals->FirstChildElement ("debug");
      if (element)
      {
        debug_printing = true;
        debug_set = true;
        ACE_DEBUG ((LM_DEBUG, 
          "KATS_BATCH:    Enabled debug mode printing from process group file\n"));
      } // if debug element existed
    } // if the user didn't specify debug mode from the command line
  }

  ACE_DEBUG ((LM_DEBUG, 
    "KATS_BATCH:  Finished reading tests setup. Proceeding to KATS setup...\n"));

  // *************** BEGIN KATS PRELIMINARY WORK ******************
  if (realtime)
  {
    ACE_DEBUG ((LM_DEBUG, 
      "KATS_BATCH:    Setting scheduling priority to max...\n"));

    // use ACE real time scheduling class
    int prio  = ACE_Sched_Params::next_priority
      (ACE_SCHED_FIFO,
       ACE_Sched_Params::priority_max (ACE_SCHED_FIFO),
       ACE_SCOPE_THREAD);
    ACE_OS::thr_setprio (prio);
  }

  ACE_DEBUG ((LM_DEBUG, 
    "KATS_BATCH:    Starting framework (if hangs, check transport)...\n"));

  Madara::KATS::Test_Framework testing (settings);

  if(debug_printing)
    testing.dump ();

  ACE_DEBUG ((LM_DEBUG, 
    "KATS_BATCH:    Starting barrier (if necessary)...\n"));

  testing.barrier (test_name);

  // Before we check for delay, we first check for a precondition

  if (pre_condition != "")
  {
    ACE_DEBUG ((LM_DEBUG, 
      "KATS_BATCH:    Checking precondition...\n"));

    std::stringstream buffer;
    buffer << test_name;
    buffer << ".pre.";
    buffer << "{.madara.id}";

    testing.event (buffer.str (), pre_condition, "");
  }

  // sleep for a set amount of time after the barrier (if specified)
  if (delay_time_set)
  {
    ACE_DEBUG ((LM_DEBUG, 
      "KATS_BATCH:    Delaying %d s according to user preferences...\n",
      delay_time.sec ()));

    ACE_OS::sleep (delay_time);
  }

  ACE_DEBUG ((LM_DEBUG, 
    "KATS_BATCH:  Finished KATS setup. Proceeding to processes...\n"));

  // *************** END KATS PRELIMINARY WORK ******************


  size_t cur = 0;

  for (cur = 0, element = el_tests->FirstChildElement ("process");
        element; element = element->NextSiblingElement ("process"), ++cur);

  ACE_DEBUG ((LM_DEBUG, 
    "KATS_BATCH:  %d processes found in %s...\n", cur, tests_file.c_str ()));

  //processes.reserve (cur);
  processes.resize (cur);

  ACE_High_Res_Timer timer;

  timer.start ();

  for (cur = 0, element = el_tests->FirstChildElement ("process");
        element; element = element->NextSiblingElement ("process"), ++cur)
  {
    ACE_DEBUG ((LM_DEBUG, 
        "KATS_BATCH:    Grabbing a process...\n"));
    KATS_Process process;

    TiXmlElement * el_temp2 = element->FirstChildElement ("executable");
    
    // if the file didn't have an executable for an individual test
    // then let them know about the problem
    if (!el_temp2 || !el_temp2->GetText ())
    {
      ACE_DEBUG ((LM_INFO, 
        "KATS_BATCH:    Each test must have an <executable>\n"));
      return -3;
    }

    // set the executable
    processes[cur].set_executable (Madara::Utility::expand_envs (
      el_temp2->GetText ()));

    // do we have a barrier name?
    TiXmlElement * el_temp1 = element->FirstChildElement ("barrier");
    if (el_temp1)
    {
      if (el_temp1->Attribute ("name"))
      {
        processes[cur].set_testname (Madara::Utility::expand_envs (
          el_temp1->Attribute ("name")));
        ACE_DEBUG ((LM_DEBUG, 
          "KATS_BATCH:    Read barrier name = %s from process group file\n",
          el_temp1->Attribute ("name")));
      }
      else if (el_temp1->GetText ())
      {
        processes[cur].set_testname (Madara::Utility::expand_envs (
          el_temp1->GetText ()));
        ACE_DEBUG ((LM_DEBUG, 
          "KATS_BATCH:    Read barrier name = %s from process group file\n",
          el_temp1->GetText ()));
      }
    }

    // check the loglevel setting
    if (!loglevel_set)
    {
      el_temp1 = element->FirstChildElement ("loglevel");
      if (el_temp1 && el_temp1->GetText ())
      {
        ACE_DEBUG ((LM_DEBUG, 
          "KATS_BATCH:    Read loglevel = %s from process group file\n",
             el_temp1->GetText ()));
        processes[cur].set_loglevel (Madara::Utility::expand_envs (
           el_temp1->GetText ()));
      }
    }
    else
    {
      std::stringstream buffer;
      buffer << MADARA_debug_level;  
      processes[cur].set_loglevel (buffer.str ());
    }

    // check the processes setting
    el_temp1 = element->FirstChildElement ("processes");
    if (el_temp1 && el_temp1->GetText ())
    {
      ACE_DEBUG ((LM_DEBUG, 
        "KATS_BATCH:    Read host = %s from process group file\n",
           el_temp1->GetText ()));
      processes[cur].set_processes (Madara::Utility::expand_envs (
         el_temp1->GetText ()));
    }

    // check the id setting
    el_temp1 = element->FirstChildElement ("id");
    if (el_temp1 && el_temp1->GetText ())
    {
      ACE_DEBUG ((LM_DEBUG, 
        "KATS_BATCH:    Read id = %s from process group file\n",
           el_temp1->GetText ()));
      processes[cur].set_id (Madara::Utility::expand_envs (
        el_temp1->GetText ()));
    }

    // check the executable environment
    el_temp1 = element->FirstChildElement ("environment");
    if (el_temp1 && el_temp1->GetText ())
    {
      ACE_DEBUG ((LM_DEBUG, 
        "KATS_BATCH:    Read env = %s from process group file\n",
           el_temp1->GetText ()));
      processes[cur].set_environment (Madara::Utility::expand_envs (
        el_temp1->GetText ()));
    }

    // check the delay
    el_temp1 = element->FirstChildElement ("delay");
    if (el_temp1 && el_temp1->GetText ())
    {
      ACE_DEBUG ((LM_DEBUG, 
        "KATS_BATCH:    Read delay = %s from process group file\n",
           el_temp1->GetText ()));
      processes[cur].set_delaytime (Madara::Utility::expand_envs (
        el_temp1->GetText ()));
    }

    // check for stdout redirect
    el_temp1 = element->FirstChildElement ("stdout");
    if (el_temp1 && el_temp1->GetText ())
    {
      ACE_DEBUG ((LM_DEBUG, 
        "KATS_BATCH:    Read stdout redirect = %s from process group file\n",
           el_temp1->GetText ()));
      processes[cur].set_stdout (Madara::Utility::expand_envs (
        el_temp1->GetText ()));
    }

    // check for stderr redirect
    el_temp1 = element->FirstChildElement ("stderr");
    if (el_temp1 && el_temp1->GetText ())
    {
      ACE_DEBUG ((LM_DEBUG, 
        "KATS_BATCH:    Read stderr redirect = %s from process group file\n",
           el_temp1->GetText ()));
      processes[cur].set_stderr (Madara::Utility::expand_envs (
        el_temp1->GetText ()));
    }

    // check for stderr redirect
    el_temp1 = element->FirstChildElement ("stdin");
    if (el_temp1 && el_temp1->GetText ())
    {
      ACE_DEBUG ((LM_DEBUG, 
        "KATS_BATCH:    Read stdin redirect = %s from process group file\n",
           el_temp1->GetText ()));
      processes[cur].set_stdin (Madara::Utility::expand_envs (
        el_temp1->GetText ()));
    }

    // check the precondition
    el_temp1 = element->FirstChildElement ("precondition");
    if (el_temp1 && el_temp1->GetText ())
    {
      ACE_DEBUG ((LM_DEBUG, 
        "KATS_BATCH:    Read precondition = %s from process group file\n",
           el_temp1->GetText ()));
      processes[cur].set_precondition (Madara::Utility::expand_envs (
        el_temp1->GetText ()));
    }

    // check the postcondition
    el_temp1 = element->FirstChildElement ("postcondition");
    if (el_temp1 && el_temp1->GetText ())
    {
      ACE_DEBUG ((LM_DEBUG, 
        "KATS_BATCH:    Read postcondition = %s from process group file\n",
           el_temp1->GetText ()));
      processes[cur].set_postcondition (Madara::Utility::expand_envs (
        el_temp1->GetText ()));
    }

    // check the commandline
    el_temp1 = element->FirstChildElement ("commandline");
    if (el_temp1 && el_temp1->GetText ())
    {
      ACE_DEBUG ((LM_DEBUG, 
        "KATS_BATCH:    Read commandline = %s from process group file\n",
           el_temp1->GetText ()));
      processes[cur].set_commandline (Madara::Utility::expand_envs (
        el_temp1->GetText ()));
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
          ACE_DEBUG ((LM_DEBUG, 
            "KATS_BATCH:    Read kill time = %s from process group file\n",
            el_temp2->GetText ()));
          processes[cur].set_killtime (Madara::Utility::expand_envs (
            el_temp2->GetText ()));
        }
      }

      // check for a signal
      el_temp2 = el_temp1->FirstChildElement ("signal");
      if (el_temp2)
      {
        // we have a kill/signal
        if (el_temp2->GetText ())
        {
          ACE_DEBUG ((LM_DEBUG, 
            "KATS_BATCH:    Read kill signal = %s from process group file\n",
            el_temp2->GetText ()));
          processes[cur].set_killsignal (Madara::Utility::expand_envs (
            el_temp2->GetText ()));
        }
      }
    }

    // check the workingdir
    el_temp1 = element->FirstChildElement ("workingdir");
    if (el_temp1 && el_temp1->GetText ())
    {
      ACE_DEBUG ((LM_DEBUG, 
        "KATS_BATCH:    Read workingdir = %s from process group file\n",
           el_temp1->GetText ()));
      processes[cur].set_workingdir (Madara::Utility::expand_envs (
        el_temp1->GetText ()));
    }

    // check the settings that are overridable from command line

    if (realtime || element->FirstChildElement ("realtime"))
    {
      ACE_DEBUG ((LM_DEBUG, 
        "KATS_BATCH:    Enabling real time scheduling\n"));
      processes[cur].enable_realtime (); 
    }

    if (debug_printing ||  element->FirstChildElement ("debug"))
    {
      ACE_DEBUG ((LM_DEBUG, 
        "KATS_BATCH:    Enabling real time scheduling\n"));
      processes[cur].enable_debug ();
    }

    // check the host setting
    if (!host_set)
    {
      el_temp1 = element->FirstChildElement ("host");
      if (el_temp1 && el_temp1->GetText ())
      {
        ACE_DEBUG ((LM_DEBUG, 
          "KATS_BATCH:    Read host = %s from test entry\n",
          el_temp1->GetText ()));
        processes[cur].set_id (Madara::Utility::expand_envs (
          el_temp1->GetText ()));
      }
    }
    // user can override host on the command line
    else
    {
      ACE_DEBUG ((LM_DEBUG, 
        "KATS_BATCH:    Using host = %s from globals\n",
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
          "KATS_BATCH:    Read domain = %s from test entry\n",
          el_temp1->GetText ()));
        processes[cur].set_domain (Madara::Utility::expand_envs (
          el_temp1->GetText ()));
      }
    }
    // user can override domain on the command line
    else
    {
      ACE_DEBUG ((LM_DEBUG, 
        "KATS_BATCH:    Using domain = %s from globals\n",
        settings.domains.c_str ()));
      processes[cur].set_domain (settings.domains);
    }

    // print the command line
    ACE_DEBUG ((LM_INFO, 
      "KATS_BATCH:  Launching %s\n",
      processes[cur].query_setup ().c_str ()));    

    processes[cur].launch ();

    // this isn't going to cut it. We need to start a timer before
    // the loop and check against it.
    if (!run_in_parallel && !kill_time_set)
    {
      ACE_DEBUG ((LM_DEBUG, 
        "KATS_BATCH:  Waiting on process to finish.\n"));
      processes[cur].wait ();
    }
    else if (!run_in_parallel)
    {
      timer.stop ();
      // check the elapsed time
      ACE_Time_Value elapsed;
      timer.elapsed_time (elapsed);

      // if we haven't already gone past our time limit,
      // then wait for the process with the remaining time
      if (elapsed.sec () <= kill_time.sec ())
      {
        ACE_Time_Value new_wait_time;
        new_wait_time.sec (kill_time.sec () - elapsed.sec ());

        ACE_DEBUG ((LM_DEBUG, 
          "KATS_BATCH:  Waiting for %d s or process to finish.",
          new_wait_time.sec ()));
        processes[cur].wait (new_wait_time);
      }

      timer.stop ();
      timer.elapsed_time (elapsed);
      
      // if we haven't already gone past our time limit,
      // then wait for the process with the remaining time
      if (elapsed.sec () > kill_time.sec ())
      {
        processes[cur].terminate ();
      }
    }
  }

  if (run_in_parallel)
  {
    ACE_DEBUG ((LM_DEBUG, 
      "KATS_BATCH:  Waiting for %d s before terminating all processes.",
      kill_time.sec ()));

    ACE_OS::sleep (kill_time);

    for (size_t i = 0; i < cur; ++i)
    {
      if (kill_time_set)
      {
        processes[i].terminate ();
      }
      else
        processes[i].wait ();
    }
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
  ACE_TCHAR options [] = ACE_TEXT ("0:1:2:f:n:i:l:o:d:a:t:v:grh");

  // create an instance of the command line args
  ACE_Get_Opt cmd_opts (argc, argv, options);

  // set up an alias for '-n' to be '--name'
  cmd_opts.long_option (ACE_TEXT ("stdin"), '0', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("stdout"), '1', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("stderr"), '2', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("testname"), 'a', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("barriername"), 'a', ACE_Get_Opt::ARG_REQUIRED);
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
  cmd_opts.long_option (ACE_TEXT ("loglevel"), 'v', ACE_Get_Opt::ARG_REQUIRED); 

  // temp for current switched option
  int option;

  // iterate through the options until done
  while ((option = cmd_opts ()) != EOF)
  {
    //arg = cmd_opts.opt_arg ();
    switch (option)
    {
    case '0':
      // redirecting stdout

      freopen (cmd_opts.opt_arg (), "r", stdin);

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG,
        DLINFO "KATS_PROCESS: stdin redirected from %s\n",
        cmd_opts.opt_arg ()));

      break;
    case '1':
      // redirecting stdout

      freopen (cmd_opts.opt_arg (), "w", stdout);

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG,
        DLINFO "KATS_PROCESS: stdout redirected to %s\n",
        cmd_opts.opt_arg ()));

      break;
    case '2':
      // redirecting stderr
      freopen (cmd_opts.opt_arg (), "w", stderr);

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG,
        DLINFO "KATS_PROCESS: stderr redirected to %s\n",
        cmd_opts.opt_arg ()));

      break;
    case 'a':
      // thread number
      test_name = cmd_opts.opt_arg ();
      test_name_set = true;

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_BATCH: test name set to %s\n",
        test_name.c_str ()));

      break;
    case 'b':
      // a precondition

      pre_condition = cmd_opts.opt_arg ();
      pre_condition_set = true;

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_BATCH: precondition set to %s\n",
        pre_condition.c_str ()));

      break;
    case 'd':
      // the knowledge domain

      settings.domains = cmd_opts.opt_arg ();
      domain_set = false;

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_BATCH: knowledge domain set to %s\n",
        settings.domains.c_str ()));

      break;
    case 'f':
      // the knowledge domain

      tests_file = cmd_opts.opt_arg ();

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG,
        "KATS_BATCH:  reading tests from %s.\n",
        tests_file.c_str () ));

      break;
    case 'g':
      // thread number
      debug_printing = true;
      debug_set = true;

      if(debug_printing)
        ACE_LOG_MSG->priority_mask (LM_DEBUG | LM_INFO | LM_ERROR, ACE_Log_Msg::PROCESS);

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_BATCH: enabling debug printing\n"));

      break;
    case 'i':
      // id
      {
        std::stringstream buffer;
        buffer << cmd_opts.opt_arg ();
        buffer >> settings.id;
      }
      id_set = true;

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_BATCH: id in test process ring set to %u\n",
        settings.id));

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
        DLINFO "KATS_BATCH: delay processes launch by %: seconds\n",
        delay_time.sec ()));

      break;
    case 'n':
      // processes
      {
        std::stringstream buffer;
        buffer << cmd_opts.opt_arg ();
        buffer >> settings.processes;
      }
      processes_set = true;

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_PROCESS: processes set to %u\n",
        settings.processes));

      break;
    case 'o':
      // host name
      settings.host = cmd_opts.opt_arg ();
      host_set = true;

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_BATCH: host set to %s\n",
        settings.host.c_str ()));

      break;
    case 'p':
      // thread number
      run_in_parallel = true;
      parallel_set = true;

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG,
        "KATS_BATCH:  running all batch tests in parallel\n"));

      break;
    case 'r':
      // thread number
      realtime = true;
      realtime_set = true;

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_BATCH: enabling realtime scheduling\n"));

      break;
    case 's':
      // a postcondition

      post_condition = cmd_opts.opt_arg ();
      post_condition_set = true;

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_BATCH: postcondition set to %s\n",
        post_condition.c_str ()));

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

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
        DLINFO "KATS_BATCH:  terminate all test processes after %: seconds\n",
        kill_time.sec ()));

      break;
    case 'v':
      // log level
      {
        std::stringstream buffer;
        buffer << cmd_opts.opt_arg ();
        buffer >> MADARA_debug_level;
      }

      loglevel_set = true;

      MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG,
        DLINFO "KATS_BATCH: logging level set to %u\n",
        MADARA_debug_level));

      break;
    case ':':
      MADARA_ERROR_RETURN (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, 
        ACE_TEXT ("KATS_BATCH:  ERROR: -%c requires an argument"),
           cmd_opts.opt_opt ()), -2); 
    default:
      {
        char c_option = (char) option;
        MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_DEBUG,
          DLINFO "KATS_PROCESS: unrecognized argument %c\n",
          c_option));

        MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_DEBUG,
          DLINFO "KATS_PROCESS: printing help...\n",
          c_option));
      }

    case 'h':
      MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_INFO,
"kats_batch options:      \n\
      -0 (--stdin)       redirect stdin from a file \n\
      -1 (--stdout)      redirect stdout to a file \n\
      -2 (--stderr)      redirect stderr to a file \n\
      -a (--testname)    name of the test (for barriers) \n\
         (--barriername) \n\
      -b (--precondition) precondition to wait for after barrier \n\
      -d (--domain)      knowledge domain to isolate knowledge into \n\
      -f (--file)        xml file containing testing information \n\
      -g (--debug)       prints KATS debug messages \n\
      -h (--help)        print this menu        \n\
      -i (--id)          this process id        \n\
      -l (--delay)       time delay (secs) after barrier to wait before spawn\n\
      -n (--processes)   number of testing processes \n\
      -o (--host)        host identifier        \n\
      -r (--realtime)    run the process with real time scheduling \n\
      -s (--postcondition) postcondition to set after process exits \n\
      -t (--timeout)     time in seconds to wait before killing \n\
      -v (--loglevel)    maximum log level to print from MADARA messages\n\
"
      ));
      MADARA_ERROR_RETURN (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, 
        ACE_TEXT ("Returning from Help Menu\n")), -1); 
      break;
    }
  }

  return 0;
}

