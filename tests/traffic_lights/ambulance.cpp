
#define ACE_NTRACE    0
//#define ACE_NLOGGING  0
#define ACE_NDEBUG    0

#include <string>
#include <vector>
#include <iostream>
#include <assert.h>
#include <stdlib.h>

#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"
#include "ace/OS.h"

#include "madara/knowledge_engine/Knowledge_Base.h"

int id = 0;
int pos_x = 0;
int pos_y = 0;
int velocity = 1;
int emergency = 0;
int stop = 20;
std::string host = "localhost";

// command line arguments
int parse_args (int argc, ACE_TCHAR * argv[]);

int ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{
  int retcode = parse_args (argc, argv);

  if (retcode < 0)
    return retcode;

  ACE_LOG_MSG->priority_mask (LM_DEBUG | LM_NOTICE, ACE_Log_Msg::PROCESS);

  ACE_TRACE (ACE_TEXT ("main"));

  Madara::Knowledge_Engine::Knowledge_Base knowledge(host, Madara::Transport::SPLICE);

  char expression [1024];

  // ambulance will broadcast this information atomically
  const char * update_expr = "ambulance%d.x=%d; \
                              ambulance%d.y=%d; \
                              ambulance%d.emergency=%d; \
                              ambulance%d.velocity=%d";

  const char * start_key = "ambulance%d.started";
  const char * finish_key = "ambulance%d.finished";

  // apply start_key to expression with current values
  ACE_OS::sprintf (expression, start_key, id);

  knowledge.set (expression, 1);

  for (int i = 0; i < stop; ++i)
  {
    // apply update_expr to expression with current values
    ACE_OS::sprintf (expression, update_expr, 
      id, pos_x, 
      id, pos_y, 
      id, emergency, 
      id, velocity);

    // evaluate the expression
    knowledge.evaluate (expression);

    ACE_OS::sleep (1);

    pos_x += rand () % 3;
    pos_y += rand () % 3;
    velocity = rand () % 55;
    emergency = rand () % 2;
  }

  // apply start_key to expression with current values
  ACE_OS::sprintf (expression, finish_key, id);

  knowledge.set (expression, 1);

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Final Knowledge\n"));
  knowledge.print_knowledge ();

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Exiting\n"));
  return 0;
}



int parse_args (int argc, ACE_TCHAR * argv[])
{
  // options string which defines all short args
  ACE_TCHAR options [] = ACE_TEXT ("i:x:y:o:v:eh");

  // create an instance of the command line args
  ACE_Get_Opt cmd_opts (argc, argv, options);

  // set up an alias for '-n' to be '--name'
  cmd_opts.long_option (ACE_TEXT ("help"), 'h', ACE_Get_Opt::NO_ARG);
  cmd_opts.long_option (ACE_TEXT ("id"), 'i', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("start.x"), 'x', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("start.y"), 'y', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("emergency"), 'e', ACE_Get_Opt::NO_ARG);
  cmd_opts.long_option (ACE_TEXT ("velocity"), 'v', ACE_Get_Opt::ARG_REQUIRED);
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
    case 'x':
      // thread number
      pos_x = atoi (cmd_opts.opt_arg ());
      break;
    case 'y':
      // thread number
      pos_y = atoi (cmd_opts.opt_arg ());
      break;
    case 'v':
      // thread number
      velocity = atoi (cmd_opts.opt_arg ());
      break;
    case 'e':
      // thread number
      emergency = 1;
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
      ACE_DEBUG ((LM_DEBUG, "Program Options:           \n\
      -h (--help)      print this menu                  \n\
      -i (--id)        set process id (0 default)       \n\
      -x (--start.x)   ambulance start x (0 default)    \n\
      -y (--start.y)   ambulance start y (0 default)    \n\
      -e (--emergency) ambulance starts in emergency    \n\
      -v (--velocity)  ambulance velocity (pos/s)       \n\
"));
      ACE_ERROR_RETURN ((LM_ERROR, 
        ACE_TEXT ("Returning from Help Menu")), -1); 
      break;
    }
  }

  return 0;
}
