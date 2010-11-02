
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

  ACE_LOG_MSG->priority_mask (LM_INFO, ACE_Log_Msg::PROCESS);

  ACE_TRACE (ACE_TEXT ("main"));

  // signal handler for clean exit
  ACE_Sig_Action sa ((ACE_SignalHandler) terminate, SIGINT);

  Madara::Knowledge_Engine::Knowledge_Base knowledge(host, Madara::Transport::SPLICE);

  ACE_DEBUG ((LM_INFO, "(%P|%t) (%d of %d) synchronizing to %d\n",
                        id, processes, stop));

  // set my id
  knowledge.set (".id", id);

  // time related knowledge values
  // .time_left_in_yellow              // time left in yellow light
  // .time_in_phase                    // time that has been spent in phased
  // .cut_off_time                     // cutoff time for being in phase  
  // .braking_time                     // time required to come to a stop
  // .time_min                         // minimum time in a phase

  knowledge.set (".cut_off_time", 20);   // 3 minutes (180 seconds)
  knowledge.set (".braking_time", 5);    // 5 seconds for breaking time
  knowledge.set (".time_min", 5);        // 1 minute (60 seconds) phase min
  knowledge.set (".time_left_in_yellow", 0);  // default for yellow
  knowledge.evaluate ("tl{.id}.phase = 1");    // start in north/south green

  std::stringstream main_logic;

  // increment the time we've spent in this phase at the top of the loop
  main_logic << " ++.time_in_phase;\n";

  // switching yellow is set if the phase is changing (we have yellow lights)
  main_logic << ".switching_yellow => --.time_left_in_yellow;\n";

  // state of the lights
  // tl{.id}.phase == 0                           
     // flashing yellow for highest priority lanes, red for intersecting lanes
  // tl{.id}.phase == 1 && !.time_left_in_yellow  
     // green for north/south, red for east/west. 
  // tl{.id}.phase == 1 && .time_left_in_yellow   
     // Yellow for north/south if .time_left_in_yellow > 0
  // tl{.id}.phase == 2 && !.time_left_in_yellow  
     // red for north/south, green for east/west.
  // tl{.id}.phase == 2 && .time_left_in_yellow   
      // Yellow for east/west if .time_left_in_yellow > 0

  // sensors or the hospital set these flags if an emergency vehicle
  // is inbound and within 6 seconds of a light
  //   tl{.id}.east.emergency
  //   tl{.id}.west.emergency
  //   tl{.id}.north.emergency
  //   tl{.id}.south.emergency

  // calculate reinforcement values for ease-of-use later in calculations 
  main_logic << "tl{.id}.south.reinforcement = " \
                "tl{.id}.south.inbound - tl{.id}.south.outbound;\n";
  main_logic << "tl{.id}.north.reinforcement = " \
                "tl{.id}.north.inbound - tl{.id}.north.outbound;\n";
  main_logic << "tl{.id}.east.reinforcement = " \
                "tl{.id}.east.inbound - tl{.id}.east.outbound;\n";
  main_logic << "tl{.id}.west.reinforcement = " \
                "tl{.id}.west.inbound - tl{.id}.west.outbound;\n";

  main_logic << "tl{.id}.eastwest.reinforcement = " \
                "tl{.id}.east.reinforcement + tl{.id}.west.reinforcement;\n";
  main_logic << "tl{.id}.northsouth.reinforcement = " \
                "tl{.id}.north.reinforcement + tl{.id}.south.reinforcement;\n";

  // simplify the rules that follow by setting a local variable to indicate 
  // eastwest or northsouth emergencies
  main_logic << ".northsouth.emergency = tl{.id}.north.emergency || tl{.id}.south.emergency;\n";
  main_logic << ".eastwest.emergency = tl{.id}.east.emergency || tl{.id}.west.emergency;\n";

  // if we are not in a yellow, let's see if we should be
  main_logic << "( !.switching_yellow && (.rule = 1)\n";
  main_logic << "  &&\n";

  // if we are in phase 1 and we don't have a northsouth emergency and we
  // are past our cutoff, then we should be going yellow to let the other 
  // side go
  main_logic << "  (\n";
  main_logic << "    ( tl{.id}.phase == 1 && (.rule = 2)\n";
  main_logic << "      &&\n"; 

  // if we don't have a northsouth emergency
  main_logic << "      ( !.northsouth.emergency && (.rule = 3)\n";
  main_logic << "        &&\n"; 

  // and we either have a eastwest emergency

  main_logic << "        ( .eastwest.emergency\n";
  main_logic << "          ||\n";
  // or we've either been in phase longer than the cutoff 
  // and the other lane has cars waiting to go
  main_logic << "          (\n";
  main_logic << "            ( .time_in_phase > .cut_off_time && (.rule = 4)\n";
  main_logic << "              &&\n";
  main_logic << "              ( tl{.id}.east.reinforcement > 0 || tl{.id}.west.reinforcement > 0 )\n";
  main_logic << "            )\n";
  main_logic << "            ||\n";
 // or we've been in phase for minumum and eastwest reinforcement is now greater than northsouth
  main_logic << "            ( .time_in_phase > .time_min && (.rule = 5)\n";
  main_logic << "              &&\n";
  main_logic << "              tl{.id}.eastwest.reinforcement > tl{.id}.northsouth.reinforcement\n";
  main_logic << "            )\n";
  main_logic << "          )\n";
  main_logic << "        )\n";
  main_logic << "      )\n"; // end !.northsouth.emergency
  main_logic << "    )\n"; // end if phase == 1 (northsouth green)

  // OR (only evaluate if one of the following ORs did not handle)
  main_logic << "    ||\n";

  // if we are in phase 1 and we don't have a northsouth emergency and we are past our cutoff,
  // then we should be going yellow to let the other side go
  main_logic << "    ( tl{.id}.phase == 2 && (.rule = 10)\n";
  main_logic << "      && \n";

  // if we don't have a eastwest emergency
  main_logic << "      ( !.eastwest.emergency && (.rule = 11)\n";
  main_logic << "        && \n";

  // and we either have a northsouth emergency
  main_logic << "        ( .northsouth.emergency\n";
  main_logic << "          ||\n";

  // and we've either been in phase longer than the cutoff
  main_logic << "          ( .time_in_phase > .cut_off_time && (.rule = 12)\n";
  main_logic << "            &&\n"; 
  main_logic << "            ( tl{.id}.north.reinforcement > 0 || tl{.id}.south.reinforcement > 0)\n";
  main_logic << "          )\n";
  main_logic << "          ||\n";
  // or we've been in phase for minumum and northwest reinforcement is now greater than eastwest
  main_logic << "          ( .time_in_phase > .time_min && (.rule = 13)\n";
  main_logic << "            && \n";
  main_logic << "            tl{.id}.northsouth.reinforcement > tl{.id}.eastwest.reinforcement\n";
  main_logic << "          )\n";
  main_logic << "        )\n"; // end || of northsouth.emergency with cutoff and min
  main_logic << "      )\n";  // end if !eastwest.emergency

  main_logic << "    )\n"; // end if phase == 2 (eastwest green)
  main_logic << "  )\n"; // end if phase == 1 || phase == 2
  // switch light to yellow and prep for changing phase
  main_logic << "  && (.switching_yellow = 1; .time_left_in_yellow = .braking_time; .rule = 14)\n";
  main_logic << ")\n"; // end if !.switching_yellow

  // OR (only evaluate if one of the following ORs did not handle)
  main_logic << "||\n";

  // only thing left is if we are in yellow but we 
      
  // we are currently in a yellow light
  main_logic << "( .switching_yellow && (.rule = 20)\n";
  main_logic << "  &&\n";

  // if the phase is 1 and time left in yellow is 0, then switch phase
  main_logic << "  (\n";
  main_logic << "    (\n";
  main_logic << "      tl{.id}.phase == 1 && .time_left_in_yellow == 0 && (.rule = 21) \n";
  main_logic << "        && \n";
  main_logic << "          (\n";
  main_logic << "            tl{.id}.phase = 2;\n";
  main_logic << "            .switching_yellow = 0;\n";
  main_logic << "            .time_in_phase = 0;\n";
  main_logic << "          )\n";
  main_logic << "    ) \n"; // end logic for phase == 1 and .time_left_in_yellow == 0

  // OR
  main_logic << "    ||\n";

  // if the phase is 2 and time left in yellow is 0, then switch phase
  main_logic << "    (\n";
  main_logic << "      tl{.id}.phase == 2 && .time_left_in_yellow == 0 && (.rule = 22)\n";
  main_logic << "        && \n";
  main_logic << "          (\n";
  main_logic << "            tl{.id}.phase = 1;\n";
  main_logic << "            .switching_yellow = 0\n";
  main_logic << "            .time_in_phase = 0;\n";
  main_logic << "          )\n";
  main_logic << "    ) \n"; // end logic for phase == 2 and .time_left_in_yellow == 0
  main_logic << "  )\n";
  main_logic << ")\n";

  std::string expression = main_logic.str ();

  std::ofstream main_logic_file;

  main_logic_file.open ("tf_main_logic.txt");

  if (main_logic_file.is_open ())
  {
    main_logic_file << expression;
    main_logic_file.close ();
  }

  ACE_DEBUG ((LM_INFO, "(%P|%t) Main logic is the following:\n%s\n\n",
    expression.c_str ()));

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Starting Knowledge\n"));
  knowledge.print_knowledge ();

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Press enter to continue: "));
  std::getline (std::cin, expression);

  expression = main_logic.str ();

  knowledge.print ("");

  // termination is done via signalling from the user (Control+C)
  while (!terminated)
  {
    knowledge.evaluate (expression);
    knowledge.print("RULE: {.rule}  P:{tl{.id}.phase}.{.time_in_phase}   Y:{.switching_yellow}.{.time_left_in_yellow} " \
      "NSR: {tl{.id}.northsouth.reinforcement}, EWR: {tl{.id}.eastwest.reinforcement}\n");

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
  cmd_opts.long_option (ACE_TEXT ("start"), 's', ACE_Get_Opt::ARG_REQUIRED);
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
      -s (--start)     start condition (10 default) \n\
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
