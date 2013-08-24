
//#define MADARA_NTRACE    0
//#define ACE_NTRACE    0
////#define ACE_NLOGGING  0
//#define ACE_NDEBUG    0

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

#include "madara/knowledge_engine/Compiled_Expression.h"
#include "madara/knowledge_engine/Knowledge_Base.h"
#include "madara/knowledge_engine/Knowledge_Update_Settings.h"

// command line arguments
int parse_args (int argc, ACE_TCHAR * argv[]);

// test functions
uint64_t test_virtual_reinforcement (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     uint32_t iterations);
uint64_t test_volatile_reinforcement (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     uint32_t iterations);
uint64_t test_volatile_assignment (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     uint32_t iterations);
uint64_t test_volatile_inference (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     uint32_t iterations);
uint64_t test_optimal_assignment (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     uint32_t iterations);
uint64_t test_optimal_reinforcement (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     uint32_t iterations);
uint64_t test_optimal_inference (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     uint32_t iterations);
uint64_t test_simple_reinforcement (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     uint32_t iterations);
uint64_t test_simple_inference (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     uint32_t iterations);
uint64_t test_large_reinforcement (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     uint32_t iterations);
uint64_t test_large_inference (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     uint32_t iterations);

uint64_t test_compiled_sr (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     uint32_t iterations);
uint64_t test_compiled_si (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     uint32_t iterations);
uint64_t test_compiled_lr (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     uint32_t iterations);
uint64_t test_compiled_li (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     uint32_t iterations);


uint64_t test_compiled_sa (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     uint32_t iterations);
uint64_t test_compiled_la (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     uint32_t iterations);

uint64_t test_compiled_sfi (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     uint32_t iterations);
uint64_t test_compiled_lfi (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     uint32_t iterations);

uint64_t test_extern_call (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     uint32_t iterations);

uint64_t test_looped_sr (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     uint32_t iterations);
uint64_t test_looped_si (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     uint32_t iterations);
uint64_t test_optimal_loop (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     uint32_t iterations);
uint64_t test_looped_li (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     uint32_t iterations);
uint64_t test_normal_set (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     uint32_t iterations);
uint64_t test_var_ref_set (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     uint32_t iterations);
uint64_t test_get_ref (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     uint32_t iterations);
uint64_t test_get_expand_ref (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     uint32_t iterations);
uint64_t test_variables_inc_var_ref (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     uint32_t iterations);


// C++ function for increment with boolean check, rather than allowing C++ 
// to optimize by putting things in registers
long increment (bool check, long value);

// C++ function for increment, rather than allowing C++ to optimize by putting
// things in registers. I would make this inline, but I have a feeling that
// the compiler would further optimize the increment into a register after
// inlining, which is precisely what I am trying to avoid.
long increment (long value);


// default iterations
uint32_t num_iterations = 100000;
uint32_t num_runs = 10;
bool conditional = true;
uint32_t step = 1;

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

class Base_Operation
{
public:
  virtual Madara::Knowledge_Record::Integer evaluate (void)
  {
    return value_;
  }

  Madara::Knowledge_Record::Integer get_value (void)
  {
    return value_;
  }

protected:
  Madara::Knowledge_Record::Integer value_;
};

class Increment_Operation : public Base_Operation
{
  virtual Madara::Knowledge_Record::Integer evaluate (void)
  {
    return ++value_;
  }
};

void
  print (uint64_t time, Madara::Knowledge_Record value,
       uint32_t iterations, const std::string & type)
{
  std::stringstream buffer;

  std::locale loc ("C"); 
  buffer.imbue (loc); 

  buffer << type;
  buffer << "\n  ops=";
  buffer << iterations;
  buffer << ", time=";
  buffer << time;
  buffer << " ns, value=";
  buffer << value.to_integer ();
  buffer << "\n";

  ACE_DEBUG ((LM_INFO, 
    buffer.str ().c_str ()));
}

Madara::Knowledge_Engine::Compiled_Expression   increment_ce;
Madara::Knowledge_Engine::Variable_Reference    increment_var;

Madara::Knowledge_Record
  increment_var1 (Madara::Knowledge_Engine::Function_Arguments & args,
            Madara::Knowledge_Engine::Variables & variables)
{
  return variables.evaluate (increment_ce,
    Madara::Knowledge_Engine::Knowledge_Update_Settings (false, false));
}

Madara::Knowledge_Record
  increment_var1_through_variables (Madara::Knowledge_Engine::Function_Arguments & args,
            Madara::Knowledge_Engine::Variables & variables)
{
  return variables.inc (increment_var);
}

Madara::Knowledge_Record
  no_op (Madara::Knowledge_Engine::Function_Arguments & args,
            Madara::Knowledge_Engine::Variables & variables)
{
  return Madara::Knowledge_Record::Integer (0);
}

std::string 
to_legible_hertz (uint64_t hertz)
{
  std::stringstream buffer;

  std::locale loc ("C"); 
  buffer.imbue (loc); 

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

  increment_ce = knowledge.compile ("++.var1");
  increment_var = knowledge.get_ref (".var1");

  if (num_runs == 0 || num_iterations == 0)
  {
    ACE_DEBUG ((LM_INFO, 
      "\nERROR: num_runs (%d) and num_iterations (%d) cannot be set to 0:\n", 
      num_runs, num_iterations));
    exit (-1);
  }

  const int num_test_types = 29;

  // make everything all pretty and for-loopy
  uint64_t results[num_test_types];
  uint64_t averages[num_test_types];
  uint64_t (* test_functions [num_test_types]) (Madara::Knowledge_Engine::Knowledge_Base & knowledge,
     uint32_t iterations);
  const char * printouts [num_test_types] = {
    "KaRL: Simple Reinforcements   ",
    "KaRL: Large Reinforcements    ",
    "KaRL: Simple Inference        ",
    "KaRL: Large Inference         ",
    "KaRL: Compiled SR             ",
    "KaRL: Compiled LR             ",
    "KaRL: Compiled SI             ",
    "KaRL: Compiled LI             ",
    "KaRL: Compiled SA             ",
    "KaRL: Compiled LA             ",
    "KaRL: Extern Function Call    ",
    "KaRL: Compiled SFI            ",
    "KaRL: Compiled LFI            ",
    "KaRL: Looped SR               ",
    "KaRL: Optimal Loop            ",
    "KaRL: Looped SI               ",
    "KaRL: Looped LI               ",
    "KaRL: Get Variable Reference  ",
    "KaRL: Get Expanded Reference  ",
    "KaRL: Normal Set Operation    ",
    "KaRL: Variable Reference Set  ",
    "KaRL: Variables Inc Var Ref   ",
    "C++: Optimized Assignments    ",
    "C++: Optimized Reinforcements ",
    "C++: Optimized Inferences     ",
    "C++: Virtual Reinforcements   ",
    "C++: Volatile Assignments     ",
    "C++: Volatile Reinforcements  ",
    "C++: Volatile Inferences      "
  };

  // enums for referencing 
  enum {
    SimpleReinforcement,
    LargeReinforcement,
    SimpleInference,
    LargeInference,
    CompiledSR,
    CompiledLR,
    CompiledSI,
    CompiledLI,
    CompiledSA,
    CompiledLA,
    ExternCall,
    CompiledSFI,
    CompiledLFI,
    LoopedSR,
    OptimalLoop,
    LoopedSI,
    LoopedLI,
    GetVariableReference,
    GetExpandedReference,
    NormalSet,
    VariableReferenceSet,
    VariablesIncVarRef,
    OptimizedAssignment,
    OptimizedReinforcement,
    OptimizedInference,
    VirtualReinforcement,
    VolatileAssignment,
    VolatileReinforcement,
    VolatileInference
  };

  // start from zero
  memset ((void *)results, 0, sizeof (uint64_t) * num_test_types);
  memset ((void *)averages, 0, sizeof (uint64_t) * num_test_types);

  test_functions[SimpleReinforcement] = test_simple_reinforcement;
  test_functions[LargeReinforcement] = test_large_reinforcement;
  test_functions[SimpleInference] = test_simple_inference;
  test_functions[LargeInference] = test_large_inference;

  test_functions[CompiledSR] = test_compiled_sr;
  test_functions[CompiledLR] = test_compiled_lr;
  test_functions[CompiledSI] = test_compiled_si;
  test_functions[CompiledLI] = test_compiled_li;
  test_functions[CompiledSA] = test_compiled_sa;
  test_functions[CompiledLA] = test_compiled_la;

  test_functions[ExternCall] = test_extern_call;
  test_functions[CompiledSFI] = test_compiled_sfi;
  test_functions[CompiledLFI] = test_compiled_lfi;
  
  test_functions[LoopedSR] = test_looped_sr;
  test_functions[OptimalLoop] = test_optimal_loop;
  test_functions[LoopedSI] = test_looped_si;
  test_functions[LoopedLI] = test_looped_li;
  
  test_functions[GetExpandedReference] = test_get_expand_ref;
  test_functions[GetVariableReference] = test_get_ref;
  test_functions[NormalSet] = test_normal_set;
  test_functions[VariableReferenceSet] = test_var_ref_set;
  test_functions[VariablesIncVarRef] = test_variables_inc_var_ref;
  
  test_functions[OptimizedAssignment] = test_optimal_assignment;
  test_functions[OptimizedReinforcement] = test_optimal_reinforcement;
  test_functions[OptimizedInference] = test_optimal_inference;
  test_functions[VirtualReinforcement] = test_virtual_reinforcement;
  test_functions[VolatileAssignment] = test_volatile_assignment;
  test_functions[VolatileReinforcement] = test_volatile_reinforcement;
  test_functions[VolatileInference] = test_volatile_inference;


  knowledge.define_function ("inc", increment_var1);
  knowledge.define_function ("no_op", no_op);


  for (uint32_t i = 0; i < num_runs; ++i)
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

  uint64_t evaluations = num_iterations * num_runs;

  for (int i = 0; i < num_test_types; ++i)
  {
    // avoid blowing up with a division by zero on fast processors. Culprit here
    // is the C++ optimization of the for loop which simply sets a register to the
    // max increment, so time taken is often 0 ns.
    if (results[i] == 0)
      results[i] = 1;

    averages[i] = (1000000000 * evaluations) / results[i];
  }

  ACE_DEBUG ((LM_INFO, 
    "\n\nTotal time taken for each test with %d iterations * %d tests was:\n", 
        num_iterations, num_runs));

  ACE_DEBUG ((LM_INFO, 
    "=========================================================================\n"));

  for (int i = 0; i < num_test_types; ++i)
  {
    std::stringstream buffer;
    
    std::locale loc ("C"); 
    buffer.imbue (loc); 

    buffer << " ";
    buffer << printouts[i];
    buffer << "\t\t";
    buffer << std::setw (30);
    buffer << results[i];
    buffer << " ns\n";

    ACE_DEBUG ((LM_INFO, 
      buffer.str ().c_str ()));
  }

  ACE_DEBUG ((LM_INFO, 
    "=========================================================================\n\n"));

  ACE_DEBUG ((LM_INFO, 
    "\nAverage time taken per rule evaluation was:\n", 
        num_iterations, num_runs));

  ACE_DEBUG ((LM_INFO, 
    "=========================================================================\n"));

  for (int i = 0; i < num_test_types; ++i)
  {
    std::stringstream buffer;
    
    std::locale loc ("C"); 
    buffer.imbue (loc); 

    buffer << " ";
    buffer << printouts[i];
    buffer << "\t\t";
    buffer << std::setw (30);
    buffer << (results[i] / (num_iterations * num_runs));
    buffer << " ns\n";

    ACE_DEBUG ((LM_INFO, 
      buffer.str ().c_str ()));
  }

  ACE_DEBUG ((LM_INFO, 
    "=========================================================================\n\n"));

  ACE_DEBUG ((LM_INFO, 
    "\nHertz for each test with %d iterations * %d tests was:\n", num_iterations, num_runs));

  
  ACE_DEBUG ((LM_INFO, 
    "=========================================================================\n"));


  for (int i = 0; i < num_test_types; ++i)
  {
    std::stringstream buffer;
    buffer << " ";
    buffer << printouts[i];
    buffer << "\t\t";
    buffer << std::setw (33);
    buffer << to_legible_hertz (averages[i]);
    buffer << "\n";

    ACE_DEBUG ((LM_INFO, 
      buffer.str ().c_str ()));
  }

  ACE_DEBUG ((LM_INFO, 
    "=========================================================================\n\n"));


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

long assignment (long & var, long value)
{
  return var = value;
}

/// Tests logicals operators (&&, ||)
uint64_t test_simple_reinforcement (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     uint32_t iterations)
{
  ACE_TRACE (ACE_TEXT ("test_simple_reinforcement"));

  knowledge.clear ();

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

  timer.start ();

  for (uint32_t i = 0; i < iterations; ++i)
  {
    // test literals in conditionals
    knowledge.evaluate ("++.var1",
      Madara::Knowledge_Engine::Eval_Settings (false, false, false));
  }

  timer.stop ();
  timer.elapsed_time (measured);

  print (measured, knowledge.get (".var1"), iterations,
    "Simple Reinforcement: ");

  return measured;
}

uint64_t test_compiled_sr (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     uint32_t iterations)
{
  ACE_TRACE (ACE_TEXT ("test_compiled_sr"));

  knowledge.clear ();
  Madara::Knowledge_Engine::Compiled_Expression ce;

  ce = knowledge.compile ("++.var1");

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

  timer.start ();

  for (uint32_t i = 0; i < iterations; ++i)
  {
    // test literals in conditionals
    knowledge.evaluate (ce, 
      Madara::Knowledge_Engine::Eval_Settings (false, false, false));
  }

  timer.stop ();
  timer.elapsed_time (measured);

  print (measured, knowledge.get (".var1"), iterations,
    "Compiled SR: ");

  return measured;
}


uint64_t test_compiled_sa (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     uint32_t iterations)
{
  ACE_TRACE (ACE_TEXT ("test_compiled_sa"));

  knowledge.clear ();
  Madara::Knowledge_Engine::Compiled_Expression ce;

  ce = knowledge.compile (".var1=1");

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

  timer.start ();

  for (uint32_t i = 0; i < iterations; ++i)
  {
    // test literals in conditionals
    knowledge.evaluate (ce, 
      Madara::Knowledge_Engine::Eval_Settings (false, false, false));
  }

  timer.stop ();
  timer.elapsed_time (measured);

  print (measured, knowledge.get (".var1"), iterations,
    "Compiled SA: ");

  return measured;
}


uint64_t test_compiled_sfi (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     uint32_t iterations)
{
  ACE_TRACE (ACE_TEXT ("test_compiled_sfi"));

  knowledge.clear ();
  Madara::Knowledge_Engine::Compiled_Expression ce;

  ce = knowledge.compile ("inc ()");

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

  timer.start ();

  for (uint32_t i = 0; i < iterations; ++i)
  {
    // test literals in conditionals
    knowledge.evaluate (ce, 
      Madara::Knowledge_Engine::Eval_Settings (false, false, false));
  }

  timer.stop ();
  timer.elapsed_time (measured);

  print (measured, knowledge.get (".var1"), iterations,
    "Compiled SFI: ");

  return measured;
}

uint64_t test_extern_call (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     uint32_t iterations)
{
  ACE_TRACE (ACE_TEXT ("test_extern_call"));

  knowledge.clear ();
  Madara::Knowledge_Engine::Compiled_Expression ce;

  ce = knowledge.compile ("no_op ()");

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

  timer.start ();

  for (uint32_t i = 0; i < iterations; ++i)
  {
    // test literals in conditionals
    knowledge.evaluate (ce, 
      Madara::Knowledge_Engine::Eval_Settings (false, false, false));
  }

  timer.stop ();
  timer.elapsed_time (measured);

  print (measured, knowledge.get (".var1"), iterations,
    "Extern function call: ");

  return measured;
}


/// Tests looped long inferences (++.var1)
uint64_t test_looped_sr (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     uint32_t iterations)
{
  ACE_TRACE (ACE_TEXT ("test_looped_sr"));

  knowledge.clear ();

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

  // build a large chain of simple reinforcements
   std::string buffer;

  unsigned max_size = iterations > 10000 ? 10000 : iterations;
  unsigned actual_iterations = iterations > 10000 ? iterations / 10000 : 1;

  knowledge.set (".iterations", (Madara::Knowledge_Record::Integer) iterations);
  knowledge.set (".actual_iterations", (Madara::Knowledge_Record::Integer) actual_iterations);

  buffer = ".var2[0->.iterations) (++.var1)";

  Madara::Knowledge_Engine::Compiled_Expression ce;

  ce = knowledge.compile (buffer);

  timer.start ();

  // execute that chain of reinforcements
  knowledge.evaluate (ce, 
    Madara::Knowledge_Engine::Eval_Settings (false, false, false));

  timer.stop ();
  timer.elapsed_time (measured);

  print (measured, knowledge.get (".var1"), iterations,
    "Compiled Looped LR: ");

  return measured;
}

/// Tests logicals operators (&&, ||)
uint64_t test_large_reinforcement (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     uint32_t iterations)
{
  ACE_TRACE (ACE_TEXT ("test_large_reinforcement"));

  knowledge.clear ();

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

  // build a large chain of simple reinforcements
   std::stringstream buffer;

  unsigned max_size = iterations > 1000 ? 1000 : iterations;
  unsigned actual_iterations = iterations > 1000 ? iterations / 1000 : 1;
  
  for (uint32_t i = 0; i < max_size; ++i)
  {
    buffer << "++.var1;";
  }

  timer.start ();

  // execute that chain of reinforcements
  for (uint32_t i = 0; i < actual_iterations; ++i)
    knowledge.evaluate (buffer.str (), 
      Madara::Knowledge_Engine::Eval_Settings (false, false, false));
  
  timer.stop ();
  timer.elapsed_time (measured);

  print (measured, knowledge.get (".var1"), iterations,
    "Large Reinforcement: ");

  return measured;
}

/// Tests logicals operators (&&, ||)
uint64_t test_compiled_lr (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     uint32_t iterations)
{
  ACE_TRACE (ACE_TEXT ("test_compiled_lr"));

  knowledge.clear ();

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

  // build a large chain of simple reinforcements
   std::stringstream buffer;

  unsigned max_size = iterations > 1000 ? 1000 : iterations;
  unsigned actual_iterations = iterations > 1000 ? iterations / 1000 : 1;
  
  for (uint32_t i = 0; i < max_size; ++i)
  {
    buffer << "++.var1;";
  }

  Madara::Knowledge_Engine::Compiled_Expression ce;

  ce = knowledge.compile (buffer.str ());

  timer.start ();

  // execute that chain of reinforcements
  for (uint32_t i = 0; i < actual_iterations; ++i)
    knowledge.evaluate (ce, 
      Madara::Knowledge_Engine::Eval_Settings (false, false, false));

  timer.stop ();
  timer.elapsed_time (measured);

  print (measured, knowledge.get (".var1"), iterations,
    "Compiled LR: ");

  return measured;
}


/// Tests assignment speed
uint64_t test_compiled_la (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     uint32_t iterations)
{
  ACE_TRACE (ACE_TEXT ("test_compiled_la"));

  knowledge.clear ();

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

  // build a large chain of simple reinforcements
   std::stringstream buffer;

  unsigned max_size = iterations > 1000 ? 1000 : iterations;
  unsigned actual_iterations = iterations > 1000 ? iterations / 1000 : 1;
  
  for (uint32_t i = 0; i < max_size; ++i)
  {
    buffer << "var1=";
    buffer << i;
    buffer << ";";
  }

  Madara::Knowledge_Engine::Compiled_Expression ce;

  ce = knowledge.compile (buffer.str ());

  timer.start ();

  // execute that chain of reinforcements
  for (uint32_t i = 0; i < actual_iterations; ++i)
    knowledge.evaluate (ce, 
      Madara::Knowledge_Engine::Eval_Settings (false, false, false));

  timer.stop ();
  timer.elapsed_time (measured);

  print (measured, knowledge.get (".var1"), iterations,
    "Compiled LA: ");

  return measured;
}


/// Tests assignment speed
uint64_t test_compiled_lfi (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     uint32_t iterations)
{
  ACE_TRACE (ACE_TEXT ("test_compiled_lfi"));

  knowledge.clear ();

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

  // build a large chain of simple reinforcements
   std::stringstream buffer;

  unsigned max_size = iterations > 1000 ? 1000 : iterations;
  unsigned actual_iterations = iterations > 1000 ? iterations / 1000 : 1;
  
  for (uint32_t i = 0; i < max_size; ++i)
  {
    buffer << "inc ();";
  }

  Madara::Knowledge_Engine::Compiled_Expression ce;

  ce = knowledge.compile (buffer.str ());

  timer.start ();

  // execute that chain of reinforcements
  for (uint32_t i = 0; i < actual_iterations; ++i)
    knowledge.evaluate (ce, 
      Madara::Knowledge_Engine::Eval_Settings (false, false, false));

  timer.stop ();
  timer.elapsed_time (measured);

  print (measured, knowledge.get (".var1"), iterations,
    "Compiled LFI: ");

  return measured;
}


/// Tests looped long inferences (1 => ++.var1)
uint64_t test_optimal_loop (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     uint32_t iterations)
{
  ACE_TRACE (ACE_TEXT ("test_optimal_loop"));

  knowledge.clear ();

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

  // build a large chain of simple reinforcements
   std::string buffer;

  unsigned max_size = iterations > 10000 ? 10000 : iterations;
  unsigned actual_iterations = iterations > 10000 ? iterations / 10000 : 1;

  knowledge.set (".iterations", (Madara::Knowledge_Record::Integer) iterations);
  knowledge.set (".actual_iterations", (Madara::Knowledge_Record::Integer) actual_iterations);

  buffer = ".var2[.iterations)";

  Madara::Knowledge_Engine::Compiled_Expression ce;

  ce = knowledge.compile (buffer);

  timer.start ();

  // execute that chain of reinforcements
  knowledge.evaluate (ce, 
      Madara::Knowledge_Engine::Eval_Settings (false, false, false));

  timer.stop ();
  timer.elapsed_time (measured);

  print (measured, knowledge.get (".var1"), iterations,
    "Optimal Loop: ");

  return measured;
}

uint64_t test_simple_inference (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     uint32_t iterations)
{
  ACE_TRACE (ACE_TEXT ("test_simple_inference"));

  knowledge.clear ();

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

  timer.start ();

  for (uint32_t i = 0; i < iterations; ++i)
  {
    // test literals in conditionals
    knowledge.evaluate ("1 => ++.var1", 
      Madara::Knowledge_Engine::Eval_Settings (false, false, false));
  }

  timer.stop ();
  timer.elapsed_time (measured);

  print (measured, knowledge.get (".var1"), iterations,
    "Simple Inference: ");

  return measured;
}

uint64_t test_normal_set (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     uint32_t iterations)
{
  ACE_TRACE (ACE_TEXT ("test_compiled_si"));

  knowledge.clear ();

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

  Madara::Knowledge_Engine::Compiled_Expression ce;

  ce = knowledge.compile ("1 => ++.var1");

  timer.start ();

  for (uint32_t i = 0; i < iterations; ++i)
  {
    knowledge.set ("var1", Madara::Knowledge_Record::Integer (i));
  }

  timer.stop ();
  timer.elapsed_time (measured);

  print (measured, knowledge.get (".var1"), iterations,
    "Normal Set Operation: ");

  return measured;
}

uint64_t test_get_ref (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     uint32_t iterations)
{
  ACE_TRACE (ACE_TEXT ("test_var_ref_set"));

  knowledge.clear ();

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

  Madara::Knowledge_Engine::Compiled_Expression ce;

  ce = knowledge.compile ("1 => ++.var1");

  timer.start ();

  for (uint32_t i = 0; i < iterations; ++i)
  {
    Madara::Knowledge_Engine::Variable_Reference variable =
      knowledge.get_ref ("var1");
  }

  timer.stop ();
  timer.elapsed_time (measured);

  print (measured, knowledge.get (".var1"), iterations,
    "Get Reference Operation: ");

  return measured;
}

uint64_t test_get_expand_ref (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     uint32_t iterations)
{
  ACE_TRACE (ACE_TEXT ("test_get_expand_ref"));

  knowledge.clear ();

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

  Madara::Knowledge_Engine::Compiled_Expression ce;

  ce = knowledge.compile ("1 => ++.var1");

  timer.start ();

  for (uint32_t i = 0; i < iterations; ++i)
  {
    Madara::Knowledge_Engine::Variable_Reference variable =
      knowledge.get_ref ("var1",
      Madara::Knowledge_Engine::Knowledge_Reference_Settings (true));
  }

  timer.stop ();
  timer.elapsed_time (measured);

  print (measured, knowledge.get (".var1"), iterations,
    "Get Expanded Ref: ");

  return measured;
}

uint64_t test_var_ref_set (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     uint32_t iterations)
{
  ACE_TRACE (ACE_TEXT ("test_var_ref_set"));

  knowledge.clear ();

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

  Madara::Knowledge_Engine::Compiled_Expression ce;

  ce = knowledge.compile ("1 => ++.var1");

  timer.start ();

  Madara::Knowledge_Engine::Variable_Reference variable =
    knowledge.get_ref ("var1");

  for (uint32_t i = 0; i < iterations; ++i)
  {
    knowledge.set (variable, Madara::Knowledge_Record::Integer (i));
  }

  timer.stop ();
  timer.elapsed_time (measured);

  print (measured, knowledge.get (".var1"), iterations,
    "Variable Reference Set: ");

  return measured;
}

uint64_t test_variables_inc_var_ref (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     uint32_t iterations)
{
  ACE_TRACE (ACE_TEXT ("test_variables_inc_var_ref"));

  knowledge.clear ();

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

  Madara::Knowledge_Engine::Compiled_Expression ce;

  ce = knowledge.compile ("inc_var_ref ()");

  timer.start ();

  for (uint32_t i = 0; i < iterations; ++i)
  {
    knowledge.evaluate (ce);
  }

  timer.stop ();
  timer.elapsed_time (measured);

  print (measured, knowledge.get (".var1"), iterations,
    "Variables Inc Var Ref: ");

  return measured;
}

uint64_t test_compiled_si (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     uint32_t iterations)
{
  ACE_TRACE (ACE_TEXT ("test_compiled_si"));

  knowledge.clear ();

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

  Madara::Knowledge_Engine::Compiled_Expression ce;

  ce = knowledge.compile ("1 => ++.var1");

  timer.start ();

  for (uint32_t i = 0; i < iterations; ++i)
  {
    // test literals in conditionals
    knowledge.evaluate (ce, 
      Madara::Knowledge_Engine::Eval_Settings (false, false, false));
  }

  timer.stop ();
  timer.elapsed_time (measured);

  print (measured, knowledge.get (".var1"), iterations,
    "Compiled SI: ");

  return measured;
}

/// Tests looped simple inferences (1 => ++.var1)
uint64_t test_looped_si (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     uint32_t iterations)
{
  ACE_TRACE (ACE_TEXT ("test_looped_si"));

  knowledge.clear ();

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

  // build a large chain of simple reinforcements
   std::string buffer;

  unsigned max_size = iterations > 10000 ? 10000 : iterations;
  unsigned actual_iterations = iterations > 10000 ? iterations / 10000 : 1;

  knowledge.set (".iterations", (Madara::Knowledge_Record::Integer) iterations);
  
  buffer = ".var2[0->.iterations) (1 => ++.var1)";

  Madara::Knowledge_Engine::Compiled_Expression ce;

  ce = knowledge.compile (buffer);

  timer.start ();

  // execute that chain of reinforcements
  knowledge.evaluate (ce, 
      Madara::Knowledge_Engine::Eval_Settings (false, false, false));

  timer.stop ();
  timer.elapsed_time (measured);

  print (measured, knowledge.get (".var1"), iterations,
    "Compiled Looped LR: ");

  return measured;
}

uint64_t test_large_inference (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     uint32_t iterations)
{
  ACE_TRACE (ACE_TEXT ("test_large_inference"));

  knowledge.clear ();

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

  // build a large chain of simple reinforcements
   std::stringstream buffer;

  unsigned max_size = iterations > 1000 ? 1000 : iterations;
  unsigned actual_iterations = iterations > 1000 ? iterations / 1000 : 1;
  
  for (uint32_t i = 0; i < max_size; ++i)
  {
    buffer << "1 => ++.var1;";
  }

  timer.start ();

  // execute that chain of reinforcements
  for (uint32_t i = 0; i < actual_iterations; ++i)
    knowledge.evaluate (buffer.str (), 
      Madara::Knowledge_Engine::Eval_Settings (false, false, false));

  timer.stop ();
  timer.elapsed_time (measured);

  print (measured, knowledge.get (".var1"), iterations,
    "Large Inference: ");

  return measured;
}

uint64_t test_compiled_li (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     uint32_t iterations)
{
  ACE_TRACE (ACE_TEXT ("test_compiled_li"));

  knowledge.clear ();

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

  // build a large chain of simple reinforcements
   std::stringstream buffer;

  unsigned max_size = iterations > 1000 ? 1000 : iterations;
  unsigned actual_iterations = iterations > 1000 ? iterations / 1000 : 1;
  
  for (uint32_t i = 0; i < max_size; ++i)
  {
    buffer << "1 => ++.var1;";
  }

  Madara::Knowledge_Engine::Compiled_Expression ce;

  ce = knowledge.compile (buffer.str ());

  timer.start ();

  // execute that chain of reinforcements
  for (uint32_t i = 0; i < actual_iterations; ++i)
    knowledge.evaluate (ce, 
      Madara::Knowledge_Engine::Eval_Settings (false, false, false));

  timer.stop ();
  timer.elapsed_time (measured);

  print (measured, knowledge.get (".var1"), iterations,
    "Compiled LI: ");

  return measured;
}

/// Tests looped long inferences (1 => ++.var1)
uint64_t test_looped_li (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     uint32_t iterations)
{
  ACE_TRACE (ACE_TEXT ("test_looped_lr"));

  knowledge.clear ();

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

  // build a large chain of simple reinforcements
   std::string buffer;

  unsigned max_size = iterations > 10000 ? 10000 : iterations;
  unsigned actual_iterations = iterations > 10000 ? iterations / 10000 : 1;

  knowledge.set (".iterations", (Madara::Knowledge_Record::Integer) iterations);
  knowledge.set (".actual_iterations", (Madara::Knowledge_Record::Integer) actual_iterations);

  buffer = ".var2[0->.iterations) (1 => ++.var1)";

  Madara::Knowledge_Engine::Compiled_Expression ce;

  ce = knowledge.compile (buffer);

  timer.start ();

  // execute that chain of reinforcements
  knowledge.evaluate (ce, 
      Madara::Knowledge_Engine::Eval_Settings (false, false, false));

  timer.stop ();
  timer.elapsed_time (measured);

  print (measured, knowledge.get (".var1"), iterations,
    "Compiled Looped LR: ");

  return measured;
}

/// Tests logicals operators (&&, ||)
uint64_t test_optimal_inference (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     uint32_t iterations)
{
  ACE_TRACE (ACE_TEXT ("test_optimal_inference"));

  knowledge.clear ();

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

  Madara::Knowledge_Record::Integer var1 = 0;

  timer.start ();

  for (uint32_t i = 0; i < iterations; ++i)
  {
    // we use a global var that can be changed from the command
    // line to trick the compiler into not optimizing out this
    // loop
    if (conditional)
      ++var1;
  }

  timer.stop ();
  timer.elapsed_time (measured);

  print (measured, Madara::Knowledge_Record (var1), iterations,
    "Optimal Inference: ");

  return measured;
}

/// Tests logicals operators (&&, ||)
uint64_t test_optimal_assignment (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     uint32_t iterations)
{
  ACE_TRACE (ACE_TEXT ("test_optimal_reinforcement"));

  knowledge.clear ();

  // keep track of time
  ACE_hrtime_t measured = 0;
  ACE_High_Res_Timer timer;

  long var1 = 0;

  timer.start ();

  for (uint32_t i = 0; i < iterations; ++i)
  {
    // I'm going to have to admit defeat. I can't use __asm because
    // it's non-portable. The compiler will compile this away
    // with either a mov of the final value or imul (if I try using a step
    // to confuse the compiler). If I add in a print statement or something
    // else, the loop will of course be altered and time is wasted.

    // 
    // I understand that compilers don't care if we are trying to do 
    // performance testing of loops without resorting to volatile which
    // means drastic performance decrease (at least 1/3), but there should
    // be some way to portably force a compiler to NOT optimize away this
    // loop without causing speed decrease via printing, outputting to a file,
    // synchronizing, library calls, or using volatile on the accumulator
    //
    // 
      assignment (var1, (long)i);

    // the following works but is non-portable to non-VS
    // force compiler to do an operation here so it doesn't just
    // add all the iterations up and set var1 to that count
    //__asm 
    //{
    //  nop;
    //}
  }

  timer.stop ();
  timer.elapsed_time (measured);

  print (measured, Madara::Knowledge_Record (
    Madara::Knowledge_Record::Integer (var1)), iterations,
    "Optimal Reinforcement: ");

  return measured;
}

/// Tests logicals operators (&&, ||)
uint64_t test_optimal_reinforcement (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     uint32_t iterations)
{
  ACE_TRACE (ACE_TEXT ("test_optimal_reinforcement"));

  knowledge.clear ();

  // keep track of time
  ACE_hrtime_t measured = 0;
  ACE_High_Res_Timer timer;

  Madara::Knowledge_Record::Integer var1 = 0;

  timer.start ();

  for (uint32_t i = 0; i < iterations; ++i)
  {
    // I'm going to have to admit defeat. I can't use __asm because
    // it's non-portable. The compiler will compile this away
    // with either a mov of the final value or imul (if I try using a step
    // to confuse the compiler). If I add in a print statement or something
    // else, the loop will of course be altered and time is wasted.

    // 
    // I understand that compilers don't care if we are trying to do 
    // performance testing of loops without resorting to volatile which
    // means drastic performance decrease (at least 1/3), but there should
    // be some way to portably force a compiler to NOT optimize away this
    // loop without causing speed decrease via printing, outputting to a file,
    // synchronizing, library calls, or using volatile on the accumulator
    //
    // 
      ++var1;

    // the following works but is non-portable to non-VS
    // force compiler to do an operation here so it doesn't just
    // add all the iterations up and set var1 to that count
    //__asm 
    //{
    //  nop;
    //}
  }

  timer.stop ();
  timer.elapsed_time (measured);

  print (measured, Madara::Knowledge_Record (var1), iterations,
    "Optimal Reinforcement: ");

  return measured;
}

/// Tests C++ function inference
uint64_t test_volatile_inference (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     uint32_t iterations)
{
  ACE_TRACE (ACE_TEXT ("test_volatile_inference"));

  knowledge.clear ();

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

  volatile Madara::Knowledge_Record::Integer var1 = 0;

  timer.start ();

  for (uint32_t i = 0; i < iterations; ++i)
  {
    // guarded increment of the volatile variable
    if (conditional)
      ++var1;
  }

  timer.stop ();
  timer.elapsed_time (measured);

  print (measured, Madara::Knowledge_Record (var1), iterations,
    "Volatile Inference: ");

  return measured;
}

/// Tests C++ function inference
uint64_t test_volatile_reinforcement (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     uint32_t iterations)
{
  ACE_TRACE (ACE_TEXT ("test_volatile_reinforcement"));

  knowledge.clear ();
  Incrementer accumulator;

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

  volatile Madara::Knowledge_Record::Integer var1 = 0;

  timer.start ();

  for (uint32_t i = 0; i < iterations; ++i)
  {
    // increment the volatile variable
    ++var1;
  }

  timer.stop ();
  timer.elapsed_time (measured);

  print (measured, Madara::Knowledge_Record (var1), iterations,
    "Volatile Reinforcement: ");

  return measured;
}

/// Tests C++ function inference
uint64_t test_virtual_reinforcement (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     uint32_t iterations)
{
  ACE_TRACE (ACE_TEXT ("test_virtual_reinforcement"));

  knowledge.clear ();
  Incrementer accumulator;

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

  Base_Operation * var1 = new Increment_Operation ();

  timer.start ();

  for (uint32_t i = 0; i < iterations; ++i)
  {
    // increment the volatile variable
    var1->evaluate ();
  }

  timer.stop ();
  timer.elapsed_time (measured);
  
  print (measured, var1->get_value (), iterations,
    "Virtual Reinforcement: ");

  delete var1;

  return measured;
}

/// Tests C++ function inference
uint64_t test_volatile_assignment (
     Madara::Knowledge_Engine::Knowledge_Base & knowledge, 
     uint32_t iterations)
{
  ACE_TRACE (ACE_TEXT ("test_volatile_assignment"));

  knowledge.clear ();
  Incrementer accumulator;

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

  volatile Madara::Knowledge_Record::Integer var1 = 0;

  timer.start ();

  for (uint32_t i = 0; i < iterations; ++i)
  {
    // increment the volatile variable
    var1 = Madara::Knowledge_Record::Integer (i);
  }

  timer.stop ();
  timer.elapsed_time (measured);

  print (measured, Madara::Knowledge_Record (var1), iterations,
    "Volatile Assignment: ");

  return measured;
}


int parse_args (int argc, ACE_TCHAR * argv[])
{
  // options string which defines all short args
  ACE_TCHAR options [] = ACE_TEXT ("n:r:c:s:h");

  // create an instance of the command line args
  ACE_Get_Opt cmd_opts (argc, argv, options);

  // set up an alias for '-n' to be '--name'
  cmd_opts.long_option (ACE_TEXT ("help"), 'h', ACE_Get_Opt::NO_ARG);
  cmd_opts.long_option (ACE_TEXT ("iterations"), 'i', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("runs"), 'r', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("step"), 'i', ACE_Get_Opt::ARG_REQUIRED);
  cmd_opts.long_option (ACE_TEXT ("conditional"), 'r', ACE_Get_Opt::ARG_REQUIRED);
 
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
    case 'c':
      // thread number
      buffer.str ("");
      buffer << cmd_opts.opt_arg ();

      if (buffer.str () == "false")
        conditional = false;

      break;
    case 's':
      // step of the optimized function
      // we have to do this because the C++ compiler
      // will try to compile away the loop otherwise...
      // Very frustrating
      buffer.str ("");
      buffer << cmd_opts.opt_arg ();
      buffer >> step;
      break;
    case ':':
      ACE_ERROR_RETURN ((LM_ERROR, 
        ACE_TEXT ("ERROR: -%c requires an argument"), 
           cmd_opts.opt_opt ()), -2); 
    case 'h':
    default:
      ACE_DEBUG ((LM_DEBUG, "Program Summary for %s:\n\n\
      This stand-alone application runs a variety of tests to determine\n\
      performance on a host system. For a more comprehensive and\n\
      customizeable tests, see profile_architecture\n\n\
      -n (--iterations)  number of iterations      \n\
      -r (--runs)        number of runs            \n\
      -s (--step)        number of iterations      \n\
      -c (--conditional) false if guard==false     \n\
      -- author's note. The last two are only necessary \n\
      -- because C++ compilers are trying to opimize \n\
      -- away the loops we are trying to test \n\
      -h (--help)        print this menu           \n\n", argv[0]
      ));
      ACE_ERROR_RETURN ((LM_ERROR, 
        ACE_TEXT ("Returning from Help Menu\n")), -1); 
      break;
    }
  }

  return 0;
}

