
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

bool finisher_mode = false;
std::string host = "localhost";

// command line arguments
int parse_args (int argc, ACE_TCHAR * argv[]);

int ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{
  parse_args (argc, argv);
  ACE_LOG_MSG->priority_mask (LM_DEBUG | LM_NOTICE, ACE_Log_Msg::PROCESS);

  ACE_TRACE (ACE_TEXT ("main"));

  Madara::Knowledge_Engine::Knowledge_Base knowledge (host, 
    Madara::Transport::SPLICE);

  if (finisher_mode)
  {
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Setting finished\n"));
    knowledge.set ("finished");
  }
  else
  {
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Waiting for finished\n"));
    knowledge.wait ("finished");
  }

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Knowledge\n"));
  knowledge.print_knowledge ();

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Exiting\n"));
  return 0;
}

int parse_args (int argc, ACE_TCHAR * argv[])
{
  // options string which defines all short args
  ACE_TCHAR options [] = ACE_TEXT ("o:fh");

  // create an instance of the command line args
  ACE_Get_Opt cmd_opts (argc, argv, options);

  // set up an alias for '-n' to be '--name'
  cmd_opts.long_option (ACE_TEXT ("finisher"), 'f', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("host"), 'o', ACE_Get_Opt::ARG_REQUIRED);
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
    case 'f':
      // thread number
      finisher_mode = true;
      break;
    case 'o':
      host = cmd_opts.opt_arg ();
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) host set to %s\n", host.c_str ()));
      break;
    case ':':
      ACE_ERROR_RETURN ((LM_ERROR, 
        ACE_TEXT ("ERROR: -%c requires an argument"), cmd_opts.opt_opt ()), -2); 
    case 'h':
    default:
      ACE_DEBUG ((LM_DEBUG, "Program Options:      \n\
      -h (--help)      print this menu             \n\
      -f (--finisher)  signal finish               \n\
      -o (--host)      this host ip/name (localhost default) \n"));
      ACE_ERROR_RETURN ((LM_ERROR, 
        ACE_TEXT ("Returning from Help Menu")), -1); 
      break;
    }
  }

  return 0;
}
