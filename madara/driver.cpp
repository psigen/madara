
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
void test_logicals (Madara::Knowledge_Engine::Knowledge_Base & knowledge);
void test_implies (Madara::Knowledge_Engine::Knowledge_Base & knowledge);
void test_conditionals (Madara::Knowledge_Engine::Knowledge_Base & knowledge);
void test_assignments (Madara::Knowledge_Engine::Knowledge_Base & knowledge);
void test_unaries (Madara::Knowledge_Engine::Knowledge_Base & knowledge);
void test_mathops (Madara::Knowledge_Engine::Knowledge_Base & knowledge);
void test_tree_compilation (Madara::Knowledge_Engine::Knowledge_Base & knowledge);

int ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{
  parse_args (argc, argv);
  ACE_LOG_MSG->priority_mask (LM_DEBUG | LM_NOTICE, ACE_Log_Msg::PROCESS);

  ACE_TRACE (ACE_TEXT ("main"));

  Madara::Knowledge_Engine::Knowledge_Base knowledge;

  // run tests
  test_tree_compilation (knowledge);
  test_assignments (knowledge);
  test_unaries (knowledge);
  test_conditionals (knowledge);
  test_logicals (knowledge);
  test_implies (knowledge);

  //Madara::Thread_Pool pool;
  //pool.open (4);
  //
  //for (int i = 0; i < 20; ++i)
  //{
  //  char data [64];
  //  sprintf (data, "data %d", i);
  //  ACE_Message_Block * message;
  //  ACE_NEW_RETURN (message, ACE_Message_Block (data), -1);
  //  pool.enqueue (message);
  //}

  knowledge.clear ();

  knowledge.evaluate ("bob=0; bill=1; jack=2; thomas=3");

  knowledge.test (20);

  ACE_OS::sleep (8);

  knowledge.print_knowledge ();

  //pool.close ();

  return 0;
}

/// Tests logicals operators (&&, ||)
void test_logicals (Madara::Knowledge_Engine::Knowledge_Base & knowledge)
{
  ACE_TRACE (ACE_TEXT ("test_assignments"));

  knowledge.clear ();

  // test variables in conditionals
  knowledge.evaluate ("var1 = 1; var2 = 0; var3 = var1 && var2");
  assert (knowledge.get ("var3") == 0);

  knowledge.evaluate ("var1 = 1; var2 = 0; var3 = var1 || var2");
  assert (knowledge.get ("var3") == 1);


  // test literals in conditionals
  knowledge.evaluate ("var1 = 1; var2 = 0; var3 = 1 && 0");
  assert (knowledge.get ("var3") == 0);

  knowledge.evaluate ("var1 = 1; var2 = 0; var3 = 1 || 0");
  assert (knowledge.get ("var3") == 1);


  // test assignments in conditionals
  knowledge.evaluate ("var1 = 1; var2 = 0; var3 = (var1 = 1) && (var2 = 0)");
  assert (knowledge.get ("var3") == 0);

  knowledge.evaluate ("var1 = 1; var2 = 0; var3 = (var1 = 1) || (var2 = 0)");
  assert (knowledge.get ("var3") == 1);
  
  knowledge.evaluate ("var1 = 1; var2 = 0; var3 = (var1 = 1 && 0) || (var2 = 0)");
  assert (knowledge.get ("var3") == 0);
  
  knowledge.evaluate ("var1 = 1; var2 = 0; var3 = (var1 = 1 && 0) || (var2 = 1 || 0)");
  assert (knowledge.get ("var3") == 1);


  knowledge.evaluate ("var1 = 1; var2 = 0; var3 = (++var1) || (++var2)");
  assert (knowledge.get ("var3") == 1);

  knowledge.evaluate ("var1 = 1; var2 = 0; var3 = (++var1) && (++var2)");
  assert (knowledge.get ("var3") == 1);

  knowledge.evaluate ("var1 = 1; var2 = -1; var3 = (++var1) && (++var2)");
  assert (knowledge.get ("var3") == 0);

}

/// Tests assignment operator (=)
void test_assignments (Madara::Knowledge_Engine::Knowledge_Base & knowledge)
{
  ACE_TRACE (ACE_TEXT ("test_assignments"));

  knowledge.clear ();
  // test assignment
  knowledge.evaluate ("var1 = 1; var2 = 0");
  assert (knowledge.get ("var1") == 1 && knowledge.get ("var2") == 0);

  knowledge.evaluate ("var3 = var2 = var1 = 4");
  assert (knowledge.get ("var1") == 4 && knowledge.get ("var2") == 4 &&
          knowledge.get ("var3") == 4);

  knowledge.evaluate ("var2 = 8; var5 = var3 = var2 = var1 = -1; var2 = 0");
  assert (knowledge.get ("var1") == -1 && knowledge.get ("var2") == 0 &&
          knowledge.get ("var3") == -1 && knowledge.get ("var5") == -1);
}

/// Tests the unaries (++, --, -, !)
void test_unaries (Madara::Knowledge_Engine::Knowledge_Base & knowledge)
{
  ACE_TRACE (ACE_TEXT ("test_unaries"));

  knowledge.clear ();

  // test increments
  knowledge.evaluate ("++var1");
  assert (knowledge.get ("var1") == 1);

  knowledge.evaluate ("++var1");
  assert (knowledge.get ("var1") == 2);

  knowledge.evaluate ("++var1");
  assert (knowledge.get ("var1") == 3);

  knowledge.evaluate ("var2 = 1 + (++var1)");
  assert (knowledge.get ("var1") == 4);
  assert (knowledge.get ("var2") == 5);

  // the following is not allowed
  //knowledge.evaluate ("++++var1");
  //assert (knowledge.get ("var1") == 5);

  // test decrements
  knowledge.evaluate ("--var4");
  assert (knowledge.get ("var4") == -1);

  knowledge.evaluate ("var1 = 3; --var1");
  assert (knowledge.get ("var1") == 2);

  // the following is not allowed
  //knowledge.evaluate ("----var1");
  //assert (knowledge.get ("var1") == 0);

  // test logical not
  knowledge.evaluate ("var1 = !var3");
  assert (knowledge.get ("var1") == 1);

  knowledge.evaluate ("var2 = !var1");
  assert (knowledge.get ("var2") == 0);

  knowledge.evaluate ("var1 = 8; var2 = !var1");
  assert (knowledge.get ("var2") == 0);

  knowledge.evaluate ("var2 = !var2");
  assert (knowledge.get ("var2") == 1);

  // test negation
  knowledge.evaluate ("var1 = 1; var2 = -var1");
  assert (knowledge.get ("var2") == -1);

  knowledge.evaluate ("var1 = -var2");
  assert (knowledge.get ("var1") == 1);

}

/// Tests the conditionals (==, !=, <, <=, >, >=)
void test_conditionals (Madara::Knowledge_Engine::Knowledge_Base & knowledge)
{
  ACE_TRACE (ACE_TEXT ("test_conditionals"));

  knowledge.clear ();

  // test assignment
  knowledge.evaluate ("var1 = 1; var2 = 0");
  assert (knowledge.get ("var1") == 1 && knowledge.get ("var2") == 0);

  // test greater thans
  knowledge.evaluate ("var3 = var1 > var2");
  assert (knowledge.get ("var3") == 1);

  knowledge.evaluate ("var3 = var1 >= var2");
  assert (knowledge.get ("var3") == 1);

  knowledge.evaluate ("var3 = var2 > var1");
  assert (knowledge.get ("var3") == 0);

  knowledge.evaluate ("var3 = var2 >= var1");
  assert (knowledge.get ("var3") == 0);

  // test lesser thans

  knowledge.evaluate ("var3 = var1 < var2");
  assert (knowledge.get ("var3") == 0);

  knowledge.evaluate ("var3 = var1 <= var2");
  assert (knowledge.get ("var3") == 0);

  knowledge.evaluate ("var3 = var2 < var1");
  assert (knowledge.get ("var3") == 1);

  knowledge.evaluate ("var3 = var2 <= var1");
  assert (knowledge.get ("var3") == 1);

  // test equality and inequality

  knowledge.evaluate ("var3 = var1 == var2");
  assert (knowledge.get ("var3") == 0);

  knowledge.evaluate ("var3 = var1 != var2");
  assert (knowledge.get ("var3") == 1); 
}


/// Tests the implication ops (+, -, *, /)
void test_implies (Madara::Knowledge_Engine::Knowledge_Base & knowledge)
{
  knowledge.clear ();

  // test implication
  knowledge.evaluate ("var1 = 1; var2 = 0; var1 => var2 = 1");
  assert (knowledge.get ("var2") == 1);

  knowledge.evaluate ("var1 = 0; var2 = 0; var1 => var2 = 1");
  assert (knowledge.get ("var2") == 0);
}

/// Tests the math ops (+, -, *, /)
void test_mathops (Madara::Knowledge_Engine::Knowledge_Base & knowledge)
{
  knowledge.clear ();

  // 
  knowledge.evaluate ("var1 = 8; var2 = 3");
  
  knowledge.evaluate ("var3 = var1 + var2");
  assert (knowledge.get ("var3") == 11);

  knowledge.evaluate ("var3 = var1 - var2");
  assert (knowledge.get ("var3") == 5);

  knowledge.evaluate ("var3 = var1 -- var2");
  assert (knowledge.get ("var3") == 11);

  knowledge.evaluate ("var3 = var1 * var2");
  assert (knowledge.get ("var3") == 24);

  knowledge.evaluate ("var3 = var1 / var2");
  assert (knowledge.get ("var3") == 2);

  knowledge.evaluate ("var3 = 9 * var1 / var2");
  assert (knowledge.get ("var3") == 24);

  knowledge.evaluate ("var3 = var1 / var2 * 9");
  assert (knowledge.get ("var3") == 18);

  knowledge.evaluate ("var3 = var1 / -var2");
  assert (knowledge.get ("var3") == -2);

  knowledge.evaluate ("var3 = -var1 / -var2");
  assert (knowledge.get ("var3") == 2);

  knowledge.evaluate ("var3 = -var1 / var2");
  assert (knowledge.get ("var3") == -2);

  knowledge.evaluate ("var2 = 2; var1 = 8; var3 = var1 + ++var2");
  assert (knowledge.get ("var3") == 11);


}


/// Tests the conditionals (==, !=, <, <=, >, >=)
void test_tree_compilation (Madara::Knowledge_Engine::Knowledge_Base & knowledge)
{
  ACE_TRACE (ACE_TEXT ("test_tree_compilation"));
  knowledge.clear ();
  int result = 0;

  knowledge.set ("var1", 5);

  result = knowledge.evaluate ("1 * 1 + 2 * 2 + 3 * 3 + 4 * 4 + 5 * 5 - 18");
  assert (result == 37);

  // 15*200 = 3000 + 16 = 3016
  result = knowledge.evaluate ("15 * 200 - 18 + 33 + (5 == 5)");
  assert (result == 3016);

  // 8*5 = 40 * 5 = 200 * (7) = 1400 - 8 = 1392
  result = knowledge.evaluate ("(8 * 5) * 5 * (4 + 3) - 8");
  assert (result == 1392);

  result = knowledge.evaluate ("(8 * var1) * var1 * (4 + 3) - 8");
  assert (result == 1392);

}

int parse_args (int argc, ACE_TCHAR * argv[])
{
  // options string which defines all short args
  // here 'n' would be called -n and can be called with or without
  // a parameter. So, -n James and -n are both valid
  ACE_TCHAR options [] = ACE_TEXT ("n:j:");

  // create an instance of the command line args
  ACE_Get_Opt cmd_opts (argc, argv, options);

  // set up an alias for '-n' to be '--name'
  cmd_opts.long_option (ACE_TEXT ("name"), 'n', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("job"), 'j', ACE_Get_Opt::ARG_REQUIRED);
 
  // temp for current switched option
  int option;
  ACE_TCHAR * arg;

  // iterate through the options until done
  while ((option = cmd_opts ()) != EOF)
  {
    arg = cmd_opts.opt_arg ();
    switch (option)
    {
    case 'n':
      // boilerplate for later
      break;
    case 'j':
      // boilerplate for later
      break;
    case ':':
      ACE_ERROR_RETURN ((LM_ERROR, 
        ACE_TEXT ("ERROR: -%c requires an argument"), cmd_opts.opt_opt ()), -2); 
    default:
      ACE_ERROR_RETURN ((LM_ERROR, 
        ACE_TEXT ("ERROR: Bad argument. -%c is unknown"), cmd_opts.opt_opt ()), -1); 
      break;
    }
  }

  return 0;
}
