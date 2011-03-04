
#define ACE_NTRACE    0
//#define ACE_NLOGGING  0
#define ACE_NDEBUG    0

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <assert.h>
#include <iomanip>

#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/OS_NS_Thread.h"
#include "ace/Sched_Params.h"

#include "madara/knowledge_engine/Knowledge_Base.h"

// command line arguments
int parse_args (int argc, ACE_TCHAR * argv[]);

// test functions
unsigned long long test_volatile_reinforcement (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     unsigned long iterations);
unsigned long long test_volatile_inference (
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
unsigned long num_iterations = 100000;
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

std::string 
to_legible_hertz (unsigned long long hertz)
{
  std::stringstream buffer;

  const int ghz_mark = 1000000000;
  const int mhz_mark = 1000000;
  const int khz_mark = 1000;

  double freq = (double) hertz / ghz_mark;

  if (freq >= 1)
  {
    buffer << std::setprecision (2) << std::fixed;
    buffer << freq;
    buffer << " ghz";
    return buffer.str ().c_str ();
  }

  freq = (double) hertz / mhz_mark;

  if (freq >= 1)
  {
    buffer << std::setprecision (2) << std::fixed;
    buffer << freq;
    buffer << " mhz";
    return buffer.str ().c_str ();
  }

  freq = (double) hertz / khz_mark;

  if (freq >= 1)
  {
    buffer << std::setprecision (2) << std::fixed;
    buffer << freq;
    buffer << " khz";
    return buffer.str ().c_str ();
  }

  freq = (double) hertz;

  buffer << std::setprecision (2) << std::fixed;
  buffer << freq;
  buffer << "  hz";
  return buffer.str ().c_str ();
  
}

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

  if (num_runs == 0 || num_iterations == 0)
  {
    ACE_DEBUG ((LM_INFO, 
      "\nERROR: num_runs (%d) and num_iterations (%d) cannot be set to 0:\n", 
      num_runs, num_iterations));
    exit (-1);
  }

  const int num_test_types = 8;

  // make everything all pretty and for-loopy
  unsigned long long results[num_test_types];
  unsigned long long averages[num_test_types];
  unsigned long long (* test_functions [num_test_types]) (Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     unsigned long iterations);
  char * printouts [num_test_types] = {
    "KaRL: Simple Reinforcements   ",
    "KaRL: 10,000 Reinforcements   ",
    "KaRL: Simple Inference        ",
    "KaRL: Large Inference         ",
    "C++: Optimized Reinforcements ",
    "C++: Optimized Inferences     ",
    "C++: Volatile Reinforcements  ",
    "C++: Volatile Inferences      "
  };

  // enums for referencing 
  enum {
    SimpleReinforcement,
    LargeReinforcement,
    SimpleInference,
    LargeInference,
    OptimizedReinforcement,
    OptimizedInference,
    VolatileReinforcement,
    VolatileInference
  };

  // start from zero
  memset ((void *)results, 0, sizeof (unsigned long long) * 8);
  memset ((void *)averages, 0, sizeof (unsigned long long) * 8);

  test_functions[SimpleReinforcement] = test_simple_reinforcement;
  test_functions[LargeReinforcement] = test_large_reinforcement;
  test_functions[SimpleInference] = test_simple_inference;
  test_functions[LargeInference] = test_large_inference;

  test_functions[OptimizedReinforcement] = test_optimal_reinforcement;
  test_functions[OptimizedInference] = test_optimal_inference;
  test_functions[VolatileReinforcement] = test_volatile_reinforcement;
  test_functions[VolatileInference] = test_volatile_inference;

  for (unsigned long i = 0; i < num_runs; ++i)
  {
    // run tests
    for (int j = 0; j < num_test_types; ++j)
    {
      results[j] += test_functions[j] (knowledge, num_iterations);
    }
  }

  // to find out the number of iterations per second, we need to 
  // multiply the numerator by the factor of the result, which is
  // given in microseconds.

  unsigned long long evaluations = num_iterations * num_runs;

  for (int i = 0; i < num_test_types; ++i)
    averages[i] = (1000000000 * evaluations) / results[i];

  ACE_DEBUG ((LM_INFO, 
    "\nTotal time taken for each test with %d iterations * %d tests were:\n", 
        num_iterations, num_runs));

  for (int i = 0; i < num_test_types; ++i)
  {
    std::stringstream buffer;
    buffer << " ";
    buffer << printouts[i];
    buffer << "\t\t";
    buffer << results[i];
    buffer << " ns\n";

    ACE_DEBUG ((LM_INFO, 
      buffer.str ().c_str ()));
  }

  ACE_DEBUG ((LM_INFO, 
    "\nHertz for each test with %d iterations * %d tests were:\n", num_iterations, num_runs));

  for (int i = 0; i < num_test_types; ++i)
  {
    std::stringstream buffer;
    buffer << " ";
    buffer << printouts[i];
    buffer << "\t\t";
    buffer << std::setw (15);
    buffer << to_legible_hertz (averages[i]);
    buffer << "\n";

    ACE_DEBUG ((LM_INFO, 
      buffer.str ().c_str ()));
  }

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
unsigned long long test_volatile_inference (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     unsigned long iterations)
{
  ACE_TRACE (ACE_TEXT ("test_volatile_inference"));

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
unsigned long long test_volatile_reinforcement (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     unsigned long iterations)
{
  ACE_TRACE (ACE_TEXT ("test_volatile_reinforcement"));

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

