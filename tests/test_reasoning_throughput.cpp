
#define ACE_NTRACE    0
//#define ACE_NLOGGING  0
#define ACE_NDEBUG    0

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <assert.h>

#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/OS_NS_Thread.h"
#include "ace/Sched_Params.h"

#include "madara/knowledge_engine/Knowledge_Base.h"

// command line arguments
int parse_args (int argc, ACE_TCHAR * argv[]);

// test functions
unsigned long long test_optimal_reinforcement (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     unsigned int iterations);
unsigned long long test_optimal_inference (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     unsigned int iterations);
unsigned long long test_simple_reinforcement (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     unsigned int iterations);
unsigned long long test_simple_inference (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     unsigned int iterations);
unsigned long long test_large_reinforcement (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     unsigned int iterations);
unsigned long long test_large_inference (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     unsigned int iterations);


// default iterations
unsigned int num_iterations = 100000;
unsigned int num_runs = 10;


int ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{
  int retcode = parse_args (argc, argv);

  if (retcode < 0)
    return retcode;

  ACE_LOG_MSG->priority_mask (LM_INFO, ACE_Log_Msg::PROCESS);

  ACE_TRACE (ACE_TEXT ("main"));

  // use ACE real time scheduling class
  int prio  = ACE_Sched_Params::next_priority
    (ACE_SCHED_FIFO,
     ACE_Sched_Params::priority_max (ACE_SCHED_FIFO),
     ACE_SCOPE_THREAD);
  ACE_OS::thr_setprio (prio);


  Madara::Knowledge_Engine::Knowledge_Base knowledge;


  unsigned long long sr_result = 0;
  unsigned long long lr_result = 0;
  unsigned long long si_result = 0;
  unsigned long long li_result = 0;
  unsigned long long or_result = 0;
  unsigned long long oi_result = 0;

  if (num_runs == 0 || num_iterations == 0)
  {
    ACE_DEBUG ((LM_INFO, 
      "\nERROR: num_runs (%d) and num_iterations (%d) cannot be set to 0:\n", 
      num_runs, num_iterations));
    exit (-1);
  }

  for (unsigned int i = 0; i < num_runs; ++i)
  {
    // run tests
    sr_result += test_simple_reinforcement (
      knowledge, num_iterations);
    lr_result += test_large_reinforcement (
      knowledge, num_iterations);
    si_result += test_simple_inference (
      knowledge, num_iterations);
    li_result += test_large_inference (
      knowledge, num_iterations);
    or_result += test_optimal_reinforcement (
      knowledge, num_iterations);
    oi_result += test_optimal_inference (
      knowledge, num_iterations);
  }

  // to find out the number of iterations per second, we need to 
  // multiply the numerator by the factor of the result, which is
  // given in microseconds.
  double sr_avg = (double) 1000000 * num_iterations * num_runs / sr_result;
  double lr_avg = (double) 1000000 * num_iterations * num_runs / lr_result;
  double si_avg = (double) 1000000 * num_iterations * num_runs / si_result;
  double li_avg = (double) 1000000 * num_iterations * num_runs / li_result;
  double oi_avg = (double) 1000000 * num_iterations * num_runs / oi_result;
  double or_avg = (double) 1000000 * num_iterations * num_runs / or_result;

  ACE_DEBUG ((LM_INFO, 
    "\nResults for tests with %d iterations were:\n", num_iterations));

  ACE_DEBUG ((LM_INFO, 
    "  Optimal C++ reinforcement avg:           %.2f per second\n", or_avg));
  ACE_DEBUG ((LM_INFO, 
    "  Optimal C++ inf w/reinforcement avg:     %.2f per second\n\n", oi_avg));
  ACE_DEBUG ((LM_INFO, 
    "  Simple reinforcement avg:                %.2f per second\n", sr_avg));
  ACE_DEBUG ((LM_INFO, 
    "  Large reinforcement avg:                 %.2f per second\n\n", lr_avg));
  ACE_DEBUG ((LM_INFO, 
    "  Simple inference with reinforcement avg: %.2f per second\n", si_avg));
  ACE_DEBUG ((LM_INFO, 
    "  Large inference with reinforcement avg:  %.2f per second\n", li_avg));

  return 0;
}

/// Tests logicals operators (&&, ||)
unsigned long long test_simple_reinforcement (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     unsigned int iterations)
{
  ACE_TRACE (ACE_TEXT ("test_simple_reinforcement"));

  knowledge.clear ();

  // keep track of time
  ACE_hrtime_t start, stop;

  start = ACE_OS::gethrtime ();

  for (unsigned int i = 0; i < iterations; ++i)
  {
    // test literals in conditionals
    knowledge.evaluate ("++.var1");
  }

  stop = ACE_OS::gethrtime ();

  ACE_DEBUG ((LM_INFO, 
    "(%P|%t) Time for %d iter of eval(\"++.var1\") was %d\n", 
    iterations, stop - start));

  ACE_DEBUG ((LM_INFO, 
    "(%P|%t)   Final value of eval(\"++.var1\") was %d\n", 
    knowledge.get (".var1")));

  return stop - start;
}

/// Tests logicals operators (&&, ||)
unsigned long long test_large_reinforcement (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     unsigned int iterations)
{
  ACE_TRACE (ACE_TEXT ("test_large_reinforcement"));

  knowledge.clear ();

  // keep track of time
  ACE_hrtime_t start, stop;

  // build a large chain of simple reinforcements
   std::stringstream buffer;

  unsigned max_size = iterations > 10000 ? 10000 : iterations;
  unsigned actual_iterations = iterations > 10000 ? iterations / 10000 : 1;
  
  for (unsigned int i = 0; i < max_size; ++i)
  {
    buffer << "++.var1;";
  }

  start = ACE_OS::gethrtime ();

  // execute that chain of reinforcements
  for (unsigned int i = 0; i < actual_iterations; ++i)
    knowledge.evaluate (buffer.str ());

  stop = ACE_OS::gethrtime ();

  ACE_DEBUG ((LM_INFO, 
    "(%P|%t) Time for %d length reinforcement chain in eval was %d\n",
    iterations, stop - start));

  ACE_DEBUG ((LM_INFO, 
    "(%P|%t)   Final value of large reinforcement eval was %d\n",
    knowledge.get (".var1")));

  return stop - start;
}

/// Tests logicals operators (&&, ||)
unsigned long long test_simple_inference (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     unsigned int iterations)
{
  ACE_TRACE (ACE_TEXT ("test_simple_inference"));

  knowledge.clear ();

  // keep track of time
  ACE_hrtime_t start, stop;

  start = ACE_OS::gethrtime ();

  for (unsigned int i = 0; i < iterations; ++i)
  {
    // test literals in conditionals
    knowledge.evaluate ("1 => ++.var1");
  }

  stop = ACE_OS::gethrtime ();

  ACE_DEBUG ((LM_INFO, 
    "(%P|%t) Time for %d iter of eval(\"1 => ++.var1\") was %d\n",
    iterations, stop - start));

  ACE_DEBUG ((LM_INFO, 
    "(%P|%t)   Final value of eval(\"1 => ++.var1\") was %d\n",
    knowledge.get (".var1")));

  return stop - start;
}

/// Tests logicals operators (&&, ||)
unsigned long long test_large_inference (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     unsigned int iterations)
{
  ACE_TRACE (ACE_TEXT ("test_large_inference"));

  knowledge.clear ();

  // keep track of time
  ACE_hrtime_t start, stop;

  // build a large chain of simple reinforcements
   std::stringstream buffer;

  unsigned max_size = iterations > 10000 ? 10000 : iterations;
  unsigned actual_iterations = iterations > 10000 ? iterations / 10000 : 1;
  
  for (unsigned int i = 0; i < max_size; ++i)
  {
    buffer << "1 => ++.var1;";
  }

  start = ACE_OS::gethrtime ();

  // execute that chain of reinforcements
  for (unsigned int i = 0; i < actual_iterations; ++i)
    knowledge.evaluate (buffer.str ());

  stop = ACE_OS::gethrtime ();

  ACE_DEBUG ((LM_INFO, 
    "(%P|%t) Time for %d length inference chain in eval was %d\n",
    iterations, stop - start));

  ACE_DEBUG ((LM_INFO, 
    "(%P|%t)   Final value of large inference eval was %d\n",
    knowledge.get (".var1")));

  return stop - start;
}

/// Tests logicals operators (&&, ||)
unsigned long long test_optimal_reinforcement (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     unsigned int iterations)
{
  ACE_TRACE (ACE_TEXT ("test_optimal_reinforcement"));

  knowledge.clear ();

  // keep track of time
  ACE_hrtime_t start, stop;

  long var1 = 0;

  start = ACE_OS::gethrtime ();

  for (unsigned int i = 0; i < iterations; ++i)
  {
    ++var1;
  }

  stop = ACE_OS::gethrtime ();

  ACE_DEBUG ((LM_INFO, 
    "(%P|%t) Time for %d optimal C++ reinforcements was %d\n",
    iterations, stop - start));

  ACE_DEBUG ((LM_INFO, 
    "(%P|%t)   Final value of optimal C++ reinforcements was %d\n",
    var1));

  return stop - start;
}

/// Tests logicals operators (&&, ||)
unsigned long long test_optimal_inference (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     unsigned int iterations)
{
  ACE_TRACE (ACE_TEXT ("test_optimal_inference"));

  knowledge.clear ();

  // keep track of time
  ACE_hrtime_t start, stop;

  long var1 = 0;
  long conditional = 1;

  start = ACE_OS::gethrtime ();

  for (unsigned int i = 0; i < iterations; ++i)
  {
    if (conditional)
      ++var1;
  }

  stop = ACE_OS::gethrtime ();

  ACE_DEBUG ((LM_INFO, 
    "(%P|%t) Time for %d optimal C++ inferences was %d\n",
    iterations, stop - start));

  ACE_DEBUG ((LM_INFO, 
    "(%P|%t)   Final value of optimal C++ inferences was %d\n",
    var1));

  return stop - start;
}


int parse_args (int argc, ACE_TCHAR * argv[])
{
  // options string which defines all short args
  ACE_TCHAR options [] = ACE_TEXT ("n:r:h");

  // create an instance of the command line args
  ACE_Get_Opt cmd_opts (argc, argv, options);

  // set up an alias for '-n' to be '--name'
  cmd_opts.long_option (ACE_TEXT ("help"), 'h', ACE_Get_Opt::NO_ARG);
  cmd_opts.long_option (ACE_TEXT ("iterations"), 'i', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("runs"), 'r', ACE_Get_Opt::ARG_REQUIRED);
 
  std::stringstream buffer;

  // temp for current switched option
  int option;
//  ACE_TCHAR * arg;

  // iterate through the options until done
  while ((option = cmd_opts ()) != EOF)
  {
    //arg = cmd_opts.opt_arg ();
    switch (option)
    {
    case 'n':
      // thread number
      buffer.str ("");
      buffer << cmd_opts.opt_arg ();
      buffer >> num_iterations;
      break;
    case 'r':
      // thread number
      buffer.str ("");
      buffer << cmd_opts.opt_arg ();
      buffer >> num_runs;
      break;
    case ':':
      ACE_ERROR_RETURN ((LM_ERROR, 
        ACE_TEXT ("ERROR: -%c requires an argument"), 
           cmd_opts.opt_opt ()), -2); 
    case 'h':
    default:
      ACE_DEBUG ((LM_DEBUG, "Program Options:      \n\
      -n (--iterations)  number of iterations      \n\
      -r (--runs)        number of runs            \n\
      -h (--help)        print this menu           \n"
      ));
      ACE_ERROR_RETURN ((LM_ERROR, 
        ACE_TEXT ("Returning from Help Menu")), -1); 
      break;
    }
  }

  return 0;
}

