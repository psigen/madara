
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

int id = 0;
int left = 0;
int processes = 1;
int stop = 10;
long value = 0;
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

  char s_id [64];
  char s_left [64];
  char expression [128];
  char continue_condition [128];

  ACE_OS::sprintf (s_id, "S%d", id);
  ACE_OS::sprintf (s_left, "S%d", id ? id - 1 : processes - 1);
  ACE_OS::sprintf (continue_condition, "%s < 10", s_id);

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) (%d of %d) synchronizing to %d\n",
                        id, processes, stop));

  // set the state to 0
  knowledge.set (s_id, value);

  if (id == 0)
  {
    ACE_OS::sprintf (expression, "%s == %s && ++%s", 
           s_id, s_left, s_id);   
  }
  else
  {
    ACE_OS::sprintf (expression, "%s != %s && (%s = %s)", 
            s_id, s_left, s_id, s_left);   
  }
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) (%d of %d) expression: %s\n",
              id, processes, expression));


  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Starting Knowledge\n"));
  knowledge.print_knowledge ();

  while (knowledge.evaluate (continue_condition))
  {
    knowledge.wait (expression);
    knowledge.print_knowledge ();

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
