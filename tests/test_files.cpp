
#define ACE_NTRACE    0
//#define ACE_NLOGGING  0
#define ACE_NDEBUG    0

#include <string>
#include <vector>
#include <iostream>
#include <assert.h>

#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"

#include "madara/knowledge_engine/Knowledge_Base.h"

// command line arguments
int parse_args (int argc, ACE_TCHAR * argv[]);

// test functions
void test_read_file (Madara::Knowledge_Engine::Knowledge_Base & knowledge);

int ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{
  int retcode = parse_args (argc, argv);

  if (retcode < 0)
    return retcode;

  ACE_LOG_MSG->priority_mask (LM_INFO | LM_DEBUG, ACE_Log_Msg::PROCESS);

  ACE_TRACE (ACE_TEXT ("main"));

  Madara::Knowledge_Engine::Knowledge_Base knowledge;


  // run tests
//  test_tree_compilation (knowledge);
  test_read_file (knowledge);

  knowledge.print_knowledge ();

  return 0;
}

/// Tests logicals operators (&&, ||)
void test_read_file (Madara::Knowledge_Engine::Knowledge_Base & knowledge)
{
  ACE_TRACE (ACE_TEXT ("test_assignments"));

  knowledge.clear ();

  knowledge.read_file ("question.jpg", "/files/question.jpg");
  knowledge.write_file ("question.jpg", "/files/question_copy.jpg");
  knowledge.read_policy ("shape_images.small","/files/shaping.policy");

}

int parse_args (int argc, ACE_TCHAR * argv[])
{
  // options string which defines all short args
  ACE_TCHAR options [] = ACE_TEXT ("h");

  // create an instance of the command line args
  ACE_Get_Opt cmd_opts (argc, argv, options);

  // set up an alias for '-n' to be '--name'
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
    case ':':
      ACE_ERROR_RETURN ((LM_ERROR, 
        ACE_TEXT ("ERROR: -%c requires an argument"), 
           cmd_opts.opt_opt ()), -2); 
    case 'h':
    default:
      ACE_DEBUG ((LM_DEBUG, "Program Options:      \n\
      -h (--help)      print this menu             \n"));
      ACE_ERROR_RETURN ((LM_ERROR, 
        ACE_TEXT ("Returning from Help Menu")), -1); 
      break;
    }
  }

  return 0;
}

