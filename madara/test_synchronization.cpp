
#define ACE_NTRACE    0
//#define ACE_NLOGGING  0
#define ACE_NDEBUG    0

#include <string>
#include <vector>
#include <iostream>
#include <assert.h>

#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"
#include "ace/OS.h"

#include "madara/knowledge_engine/Knowledge_Base.h"

int id = 1;
int left = 0;
int processes = 1;
int stop = 10;

// command line arguments
int parse_args (int argc, ACE_TCHAR * argv[]);

int ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{
  parse_args (argc, argv);
  ACE_LOG_MSG->priority_mask (LM_DEBUG | LM_NOTICE, ACE_Log_Msg::PROCESS);

  ACE_TRACE (ACE_TEXT ("main"));

  Madara::Knowledge_Engine::Knowledge_Base knowledge(Madara::Transport::SPLICE);

  char s_id [64];
  char s_left [64];
  char expression [128];
  char continue_condition [128];

  ACE_OS::sprintf (s_id, "%d", id);
  ACE_OS::sprintf (s_left, "%d", id+1);
  ACE_OS::sprintf (continue_condition, "S%s <= 10", s_id);

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) (%d of %d) synchronizing to %d\n",
                        id, processes, stop));

  if (id == 0)
  {
    ACE_OS::sprintf (expression, "S%s == S%s && ++S%s", 
           s_id, s_left, s_id);   
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) (%d of %d) expression: %s\n",
                        id, processes, expression));
  }
  else
  {
    ACE_OS::sprintf (expression, "S%s != S%s && S%s = S%s", 
            s_id, s_left, s_id, s_left);   
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) (%d of %d) expression: %s\n",
                        id, processes, expression));
  }

  while (knowledge.evaluate (continue_condition)
  {
    knowledge.wait (expression);
    knowledge.print_knowledge ();
  }

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Final Knowledge\n"));
  knowledge.print_knowledge ();

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Exiting\n"));
  return 0;
}



int parse_args (int argc, ACE_TCHAR * argv[])
{
  // options string which defines all short args
  ACE_TCHAR options [] = ACE_TEXT ("i:s:p:h");

  // create an instance of the command line args
  ACE_Get_Opt cmd_opts (argc, argv, options);

  // set up an alias for '-n' to be '--name'
  cmd_opts.long_option (ACE_TEXT ("id"), 'i', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("stop"), 's', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("processes"), 'p', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("help"), 'h', ACE_Get_Opt::ARG_REQUIRED);
 
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
    case ':':
      ACE_ERROR_RETURN ((LM_ERROR, 
        ACE_TEXT ("ERROR: -%c requires an argument"), 
           cmd_opts.opt_opt ()), -2); 
    case 'h':
    default:
      ACE_DEBUG ((LM_DEBUG, "
Program Options:
      -h (--help)      print this menu
      -i (--id)        set process id (0 default)
      -s (--stop)      stop condition (10 default)
      -p (--processes) number of processes that will be running
"));
      break;
    }
  }

  return 0;
}
