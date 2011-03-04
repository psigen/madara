
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
unsigned long long test_function_reinforcement (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     unsigned long iterations);
unsigned long long test_function_inference (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     unsigned long iterations);
unsigned long long test_optimal_reinforcement (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     unsigned long iterations);
unsigned long long test_optimal_inference (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     unsigned long iterations);
unsigned long long test_simple_reinforcement (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     unsigned long iterations);
unsigned long long test_simple_inference (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     unsigned long iterations);
unsigned long long test_large_reinforcement (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     unsigned long iterations);
unsigned long long test_large_inference (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     unsigned long iterations);

// C++ function for increment with boolean check, rather than allowing C++ 
// to optimize by putting things in registers
long increment (bool check, long value);

// C++ function for increment, rather than allowing C++ to optimize by putting
// things in registers. I would make this inline, but I have a feeling that
// the compiler would further optimize the increment into a register after
// inlining, which is precisely what I am trying to avoid.
long increment (long value);


// default iterations
unsigned long num_iterations = 10000000;
unsigned long num_runs = 10;

// still trying to stop this darn thing from optimizing the increments
class Incrementer
{
public:

  Incrementer () : value_ (0) {}

  long increment ()
  {
    return ++value_;
  }

  long increment (bool check)
  {
    return check ? ++value_ : value_;
  }

  long value ()
  {
    return value_;
  }

private:
  volatile long value_;
};


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
  unsigned long long fr_result = 0;
  unsigned long long fi_result = 0;
  unsigned long long full_li_result = 0;
  unsigned long long full_lr_result = 0;
  unsigned long long full_si_result = 0;
  unsigned long long full_sr_result = 0;

  if (num_runs == 0 || num_iterations == 0)
  {
    ACE_DEBUG ((LM_INFO, 
      "\nERROR: num_runs (%d) and num_iterations (%d) cannot be set to 0:\n", 
      num_runs, num_iterations));
    exit (-1);
  }

  for (unsigned long i = 0; i < num_runs; ++i)
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

    // instead of running smaller iterations per run. Let's see how well
    // ACE high res timers handle a 1,000,000 batch
    full_sr_result += test_simple_reinforcement (
      knowledge, 1000000);
    full_lr_result += test_large_reinforcement (
      knowledge, 1000000);
    full_si_result += test_simple_inference (
      knowledge, 1000000);
    full_li_result += test_large_inference (
      knowledge, 1000000);

    or_result += test_optimal_reinforcement (
      knowledge, num_iterations);
    oi_result += test_optimal_inference (
      knowledge, num_iterations);
    fr_result += test_function_reinforcement (
      knowledge, num_iterations);
    fi_result += test_function_inference (
      knowledge, num_iterations);
  }

  // to find out the number of iterations per second, we need to 
  // multiply the numerator by the factor of the result, which is
  // given in microseconds.

  unsigned long evaluations = num_iterations * num_runs;
  unsigned long full_evals = 1000000 * num_runs;

  // evaluations / sr_result == operations / ns
  // evaluations / second == operations / ns * 1,000,000,000 ns / s

  double sr_avg = (double) 1000000000 * evaluations / sr_result;
  double lr_avg = (double) 1000000000 * evaluations / lr_result;
  double si_avg = (double) 1000000000 * evaluations / si_result;
  double li_avg = (double) 1000000000 * evaluations / li_result;
  double oi_avg = (double) 1000000000 * evaluations / oi_result;
  double or_avg = (double) 1000000000 * evaluations / or_result;
  double fi_avg = (double) 1000000000 * evaluations / fi_result;
  double fr_avg = (double) 1000000000 * evaluations / fr_result;

  double full_sr_avg = (double) 1000000000 * full_evals / full_sr_result;
  double full_si_avg = (double) 1000000000 * full_evals / full_si_result;
  double full_lr_avg = (double) 1000000000 * full_evals / full_lr_result;
  double full_li_avg = (double) 1000000000 * full_evals / full_li_result;

  ACE_DEBUG ((LM_INFO, 
    "\nResults for tests with %d iterations were:\n", num_iterations));

  ACE_DEBUG ((LM_INFO, 
    "  Optimal C++ reinforcement avg:           %.2f per second\n", or_avg));
  ACE_DEBUG ((LM_INFO, 
    "  Optimal C++ inf w/reinforcement avg:     %.2f per second\n\n", oi_avg));

  ACE_DEBUG ((LM_INFO, 
    "  Function C++ reinforcement avg:           %.2f per second\n", fr_avg));
  ACE_DEBUG ((LM_INFO, 
    "  Function C++ inf w/reinforcement avg:     %.2f per second\n\n", fi_avg));

  ACE_DEBUG ((LM_INFO, 
    "  Simple reinforcement avg:\n" \
    "     %.2f/s\t\t %.2f/s\n", sr_avg, full_sr_avg));
  ACE_DEBUG ((LM_INFO, 
    "  Large reinforcement avg:\n" \
    "    %.2f/s\t\t%.2f/s\n", lr_avg, full_lr_avg));
  ACE_DEBUG ((LM_INFO, 
    "  Simple inference with reinforcement avg:\n" \
    "     %.2f/s\t\t %.2f/s\n", si_avg, full_si_avg));
  ACE_DEBUG ((LM_INFO, 
    "  Large inference with reinforcement avg:\n" \
    "    %.2f/s\t\t%.2f/s\n\n", li_avg, full_li_avg));

  return 0;
}

long increment (bool check, long value)
{
  return check ? ++value : value;
}

long increment (long value)
{
  return ++value;
}

/// Tests logicals operators (&&, ||)
unsigned long long test_simple_reinforcement (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     unsigned long iterations)
{
  ACE_TRACE (ACE_TEXT ("test_simple_reinforcement"));

  knowledge.clear ();

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

  timer.start ();

  for (unsigned long i = 0; i < iterations; ++i)
  {
    // test literals in conditionals
    knowledge.evaluate ("++.var1");
  }

  timer.stop ();
  timer.elapsed_time (measured);

  ACE_DEBUG ((LM_INFO, 
    "(%P|%t) Time for %d iter of eval(\"++.var1\") was %d\n", 
    iterations, measured));

  ACE_DEBUG ((LM_INFO, 
    "(%P|%t)   Final value of eval(\"++.var1\") was %d\n", 
    knowledge.get (".var1")));

  return measured;
}

/// Tests logicals operators (&&, ||)
unsigned long long test_large_reinforcement (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     unsigned long iterations)
{
  ACE_TRACE (ACE_TEXT ("test_large_reinforcement"));

  knowledge.clear ();

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

  // build a large chain of simple reinforcements
   std::stringstream buffer;

  unsigned max_size = iterations > 10000 ? 10000 : iterations;
  unsigned actual_iterations = iterations > 10000 ? iterations / 10000 : 1;
  
  for (unsigned long i = 0; i < max_size; ++i)
  {
    buffer << "++.var1;";
  }

  timer.start ();

  // execute that chain of reinforcements
  for (unsigned long i = 0; i < actual_iterations; ++i)
    knowledge.evaluate (buffer.str ());

  timer.stop ();
  timer.elapsed_time (measured);

  ACE_DEBUG ((LM_INFO, 
    "(%P|%t) Time for %d length reinforcement chain in eval was %d\n",
    iterations, measured));

  ACE_DEBUG ((LM_INFO, 
    "(%P|%t)   Final value of large reinforcement eval was %d\n",
    knowledge.get (".var1")));

  return measured;
}

/// Tests logicals operators (&&, ||)
unsigned long long test_simple_inference (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     unsigned long iterations)
{
  ACE_TRACE (ACE_TEXT ("test_simple_inference"));

  knowledge.clear ();

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

  timer.start ();

  for (unsigned long i = 0; i < iterations; ++i)
  {
    // test literals in conditionals
    knowledge.evaluate ("1 => ++.var1");
  }

  timer.stop ();
  timer.elapsed_time (measured);

  ACE_DEBUG ((LM_INFO, 
    "(%P|%t) Time for %d iter of eval(\"1 => ++.var1\") was %d\n",
    iterations, measured));

  ACE_DEBUG ((LM_INFO, 
    "(%P|%t)   Final value of eval(\"1 => ++.var1\") was %d\n",
    knowledge.get (".var1")));

  return measured;
}

/// Tests logicals operators (&&, ||)
unsigned long long test_large_inference (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     unsigned long iterations)
{
  ACE_TRACE (ACE_TEXT ("test_large_inference"));

  knowledge.clear ();

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

  // build a large chain of simple reinforcements
   std::stringstream buffer;

  unsigned max_size = iterations > 10000 ? 10000 : iterations;
  unsigned actual_iterations = iterations > 10000 ? iterations / 10000 : 1;
  
  for (unsigned long i = 0; i < max_size; ++i)
  {
    buffer << "1 => ++.var1;";
  }

  timer.start ();

  // execute that chain of reinforcements
  for (unsigned long i = 0; i < actual_iterations; ++i)
    knowledge.evaluate (buffer.str ());

  timer.stop ();
  timer.elapsed_time (measured);

  ACE_DEBUG ((LM_INFO, 
    "(%P|%t) Time for %d length inference chain in eval was %d\n",
    iterations, measured));

  ACE_DEBUG ((LM_INFO, 
    "(%P|%t)   Final value of large inference eval was %d\n",
    knowledge.get (".var1")));

  return measured;
}

/// Tests logicals operators (&&, ||)
unsigned long long test_optimal_inference (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     unsigned long iterations)
{
  ACE_TRACE (ACE_TEXT ("test_optimal_inference"));

  knowledge.clear ();

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

  long var1 = 0;
  bool conditional = true;

  timer.start ();

  for (unsigned long i = 0; i < iterations; ++i)
  {
    // don't use the var1 by reference because the compiler appears
    // to optimize it to a register still
    var1 = increment (conditional, var1);
  }

  timer.stop ();
  timer.elapsed_time (measured);

  ACE_DEBUG ((LM_INFO, 
    "(%P|%t) Time for %d optimal C++ inferences was %d\n",
    iterations, measured));

  ACE_DEBUG ((LM_INFO, 
    "(%P|%t)   Final value of optimal C++ inferences was %d\n",
    var1));

  return measured;
}

/// Tests logicals operators (&&, ||)
unsigned long long test_optimal_reinforcement (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     unsigned long iterations)
{
  ACE_TRACE (ACE_TEXT ("test_optimal_reinforcement"));

  knowledge.clear ();

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

  long var1 = 0;

  timer.start ();

  for (unsigned long i = 0; i < iterations; ++i)
  {
    // use a function so var1 isn't put into a processor register
    // and super-optimized
    var1 = increment(var1);
  }

  timer.stop ();
  timer.elapsed_time (measured);

  ACE_DEBUG ((LM_INFO, 
    "(%P|%t) Time for %d optimal C++ reinforcements was %d\n",
    iterations, measured));

  ACE_DEBUG ((LM_INFO, 
    "(%P|%t)   Final value of optimal C++ reinforcements was %d\n",
    var1));

  return measured;
}

/// Tests C++ function inference
unsigned long long test_function_inference (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     unsigned long iterations)
{
  ACE_TRACE (ACE_TEXT ("test_function_inference"));

  knowledge.clear ();
  Incrementer accumulator;

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

  long var1 = 0;
  bool conditional = true;

  timer.start ();

  for (unsigned long i = 0; i < iterations; ++i)
  {
    // don't use the var1 by reference because the compiler appears
    // to optimize it to a register still
    accumulator.increment (conditional);
  }

  timer.stop ();
  timer.elapsed_time (measured);

  var1 = accumulator.value ();

  ACE_DEBUG ((LM_INFO, 
    "(%P|%t) Time for %d functional C++ inferences was %d\n",
    iterations, measured));

  ACE_DEBUG ((LM_INFO, 
    "(%P|%t)   Final value of functional C++ inferences was %d\n",
    var1));

  return measured;
}

/// Tests C++ function inference
unsigned long long test_function_reinforcement (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     unsigned long iterations)
{
  ACE_TRACE (ACE_TEXT ("test_function_reinforcement"));

  knowledge.clear ();
  Incrementer accumulator;

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

  long var1 = 0;

  timer.start ();

  for (unsigned long i = 0; i < iterations; ++i)
  {
    // don't use the var1 by reference because the compiler appears
    // to optimize it to a register still
    accumulator.increment ();
  }

  timer.stop ();
  timer.elapsed_time (measured);


  var1 = accumulator.value ();

  ACE_DEBUG ((LM_INFO, 
    "(%P|%t) Time for %d functional C++ reinforcements was %d\n",
    iterations, measured));

  ACE_DEBUG ((LM_INFO, 
    "(%P|%t)   Final value of functional C++ reinforcements was %d\n",
    var1));

  return measured;
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

