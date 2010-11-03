
#define ACE_NTRACE    0
//#define ACE_NLOGGING  0
#define ACE_NDEBUG    0

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <assert.h>
#include <stdlib.h>

#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"
#include "ace/Signal.h"

#include "madara/knowledge_engine/Knowledge_Base.h"
#include "madara/utility/Utility.h"

int id = 0;
int left = 0;
int cars = 1;
int stop = 10;
long value = 0;
std::string mapfile = "/configs/maps/7x7_1hospital.map";
std::string logicfile = "/configs/logics/traffic_simulator.klf";
std::string settingsfile = "/configs/settings/default_simulation.klf";
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

  ACE_LOG_MSG->priority_mask (LM_INFO, ACE_Log_Msg::PROCESS);

  ACE_TRACE (ACE_TEXT ("main"));

  // signal handler for clean exit
  ACE_Sig_Action sa ((ACE_SignalHandler) terminate, SIGINT);

  Madara::Knowledge_Engine::Knowledge_Base knowledge(
    host, Madara::Transport::SPLICE);

  ACE_DEBUG ((LM_INFO, "(%P|%t) Starting traffic simulation...\n" \
    "  logic: %s\n  map: %s\n  settings: %s\n",
    logicfile.c_str (), mapfile.c_str (), settingsfile.c_str ()));

  // set my id
  knowledge.set (".id", id);

  // time related knowledge values
  // simulation_time = current time step in the simulation

  knowledge.set ("simulation_time", 0);   // 3 minutes (180 seconds)

  std::stringstream main_logic;

  std::string filename = getenv ("MADARA_ROOT");
  filename += mapfile;

  std::string map = Madara::Utility::file_to_string (filename);

  filename = getenv ("MADARA_ROOT");
  filename += logicfile;

  std::string logic = Madara::Utility::file_to_string (filename);

  filename = getenv ("MADARA_ROOT");
  filename += settingsfile;

  std::string settings = Madara::Utility::file_to_string (filename);

  std::ofstream main_logic_file;

  main_logic_file.open ("ts_main_logic.klf");

  if (main_logic_file.is_open ())
  {
    main_logic_file << logic;
    main_logic_file.close ();
  }

  ACE_DEBUG ((LM_INFO, "(%P|%t) Main logic is the following:\n%s\n\n",
    logic.c_str ()));

  ACE_DEBUG ((LM_INFO, "(%P|%t) Map is the following:\n%s\n\n",
    map.c_str ()));

  ACE_DEBUG ((LM_INFO, "(%P|%t) Simulation settings are the following:\n%s\n\n",
    settings.c_str ()));

  knowledge.evaluate (settings);

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Starting Knowledge\n"));
  knowledge.print_knowledge ();

  // termination is done via signalling from the user (Control+C)
  while (!terminated)
  {
    knowledge.evaluate (logic);
    knowledge.print("time: {simulation_time} of {simulation_cutoff}\n");

    // cutoff the simulation if we have passed the cutoff
    if (knowledge.get ("simulation_time") >= knowledge.get ("simulation_cutoff"))
    {
      terminated = true;
      knowledge.set ("simulation_finished");
    }

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
  ACE_TCHAR options [] = ACE_TEXT ("i:m:s:l:o:h");

  // create an instance of the command line args
  ACE_Get_Opt cmd_opts (argc, argv, options);

  // set up an alias for '-n' to be '--name'
  cmd_opts.long_option (ACE_TEXT ("id"), 'i', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("map"), 'm', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("settings"), 's', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("logic"), 'l', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("help"), 'h', ACE_Get_Opt::NO_ARG);
  cmd_opts.long_option (ACE_TEXT ("host"), 'o', ACE_Get_Opt::ARG_REQUIRED);
 
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
    case 'm':
      // thread number
      mapfile = cmd_opts.opt_arg ();
      break;
    case 'l':
      // thread number
      logicfile = cmd_opts.opt_arg ();
      break;
    case 's':
      // thread number
      settingsfile = cmd_opts.opt_arg ();
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
      -i (--id)        set process id (0 default)  \n\
      -m (--map)       map file name for traffic lights/hospitals/streets \n\
                       note that this path will start from $MADARA_ROOT \n\
      -l (--logic)     logic file name from $MADARA_ROOT \n\
      -s (--settings)  configuration for simulation from $MADARA_ROOT\n\
      -o (--host)      this host ip/name (localhost default) \n\
      -h (--help)      print this menu             \n"));
      ACE_ERROR_RETURN ((LM_ERROR, 
        ACE_TEXT ("Returning from Help Menu")), -1); 
      break;
    }
  }

  return 0;
}
