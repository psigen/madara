
#include "madara/utility/Log_Macros.h"
#include "madara/expression_tree/Leaf_Node.h"
#include "madara/expression_tree/System_Call_Print_System_Calls.h"
#include "madara/expression_tree/Visitor.h"
#include "madara/knowledge_engine/Thread_Safe_Context.h"

// a map of all the system calls to their help information.
Madara::Expression_Tree::System_Calls_Help
Madara::Expression_Tree::System_Call_Print_System_Calls::calls_;

Madara::Expression_Tree::System_Call_Print_System_Calls::System_Call_Print_System_Calls (
  Madara::Knowledge_Engine::Thread_Safe_Context & context,
  const Component_Nodes & nodes)
  : System_Call_Node (context, nodes)
{

}

// Dtor
Madara::Expression_Tree::System_Call_Print_System_Calls::~System_Call_Print_System_Calls (void)
{
}

Madara::Knowledge_Record
Madara::Expression_Tree::System_Call_Print_System_Calls::item (void) const
{
  return Madara::Knowledge_Record::Integer (1);
}

/// Prune the tree of unnecessary nodes. 
/// Returns evaluation of the node and sets can_change appropriately.
/// if this node can be changed, that means it shouldn't be pruned.
Madara::Knowledge_Record
Madara::Expression_Tree::System_Call_Print_System_Calls::prune (bool & can_change)
{
  // user can always change a function, and we have no control over
  // what it does. Consequently, a function node cannot be pruned out
  // under any situation
  can_change = true;
  
  Madara::Knowledge_Record result;

  if (nodes_.size () == 1)
  {
    bool arg_can_change = false;
    result = nodes_[0]->prune (arg_can_change);
    
    if (!arg_can_change && dynamic_cast <Leaf_Node *> (nodes_[0]) == 0)
    {
      delete nodes_[0];
      nodes_[0] = new Leaf_Node (result);
    }
  }
  else if (nodes_.size () != 0)
  {
    MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_DEBUG, 
      "KARL COMPILE ERROR: System call print_system_calls takes either 0"
      " or 1 arguments.\n"));
  }

  // if calls hasn't been initialized yet, fill the list of system calls
  if (calls_.size () == 0)
  {
    calls_["#delete_var"] =
      "\n#delete_var (var) or #delete_variable (var):\n"
      "  Deletes a variable named var from the knowledge base\n";
    
    calls_["#delete_variable"] =
      "\n#delete_var (var) or #delete_variable (var):\n"
      "  Deletes a variable named var from the knowledge base\n";

    calls_["#eval"] =
      "\n#eval (expression) or #evaluate (expression):\n"
      "  Evaluates the KaRL expression and returns a result. Works similarly\n"
      "  to the Knowledge_Base::evaluate function except this function\n"
      "  inherits the Knowledge_Update_Settings from the eval call.\n";

    calls_["#evaluate"] =
      "\n#eval (expression) or #evaluate (expression):\n"
      "  Evaluates the KaRL expression and returns a result. Works similarly\n"
      "  to the Knowledge_Base::evaluate function except this function\n"
      "  inherits the Knowledge_Update_Settings from the eval call.\n";
    
    calls_["#expand"] =
      "\n#expand (statement) or #expand_statement (statement):\n"
      "  Expands a statement such as 'var{.i}' into 'var0', assuming .i=0\n";
    
    calls_["#expand_env"] =
      "\n#expand_env (statement) or #expand_envs (statement):\n"
      "  Expands a statement such as 'env_var=$(VAR)' into\n"
      "  env_var=VAR CONTENTS, assuming VAR has been set in the OS.\n";
    
    calls_["#expand_envs"] =
      "\n#expand_env (statement) or #expand_envs (statement):\n"
      "  Expands a statement such as 'env_var=$(VAR)' into\n"
      "  env_var=VAR CONTENTS, assuming VAR has been set in the OS.\n";
    
    calls_["#expand_statement"] =
      "\n#expand (statement) or #expand_statement (statement):\n"
      "  Expands a statement such as 'var{.i}' into 'var0', assuming .i=0\n";
    
    calls_["#fragment"] =
      "\n#fragment (arg, first, last):\n"
      "  Returns a fragment of the arg, startin at first byte and extending\n"
      "  to last byte.\n";

    calls_["#get_clock"] =
      "\n#get_clock () or #get_clock (variable):\n"
      "  Returns either the system clock or the variable clock. Note that\n"
      "  this is a Lamport clock value and not a wall time value.\n";
    
    calls_["#get_time"] =
      "\n#get_time ():\n"
      "  Returns wall clock time on the local machine in nanoseconds.\n";

    calls_["#log_level"] =
      "\n#log_level () or #log_level (value):\n"
      "  Returns the logging level or sets the logging level. The logging\n"
      "  level determines the granularity of MADARA log messages. The value\n"
      "  should be an integer from 0-10.\n"
      "  0: Error messages and emergency information. Default setting.\n"
      "  1. Terminal errors\n"
      "  2. Non-terminal errors\n"
      "  3. Questionable state (MADARA can keep going)\n"
      "  4. Warnings\n"
      "  3. Errors that may cause slight instability\n"
      "  4. Event trace\n"
      "  5. Major events\n"
      "  6. Minor events\n"
      "  7. Event traces\n"
      "  8. Major debug info\n"
      "  9. Log trace\n"
      " 10. Detailed trace\n";

    calls_["#print"] =
      "\n#print (statement):\n"
      "  Prints a statement that can contain MADARA KaRL variables.\n"
      "  Variables are expanding to include their values where applicable.\n"
      "  For instance, 'var{.id}' would result in printing 'var0' if .id\n"
      "  was set to 0. The same statement would print 'variable' if .id\n"
      "  was set to 'iable'.\n";
    
    calls_["#print_system_calls"] =
      "\n#print_system_calls () or #print_system_call (system_call):\n"
      "  Prints help for all system calls or a specific system call.\n";
    
    calls_["#rand_double"] =
      "\n#rand_double (floor = 0.0, ceiling = 1.0, update_srand = true):\n"
      "  Generates a random double from a uniform distribution.\n";
      
    calls_["#rand_int"] =
      "\n#rand_int (floor = 0, ceiling = RAND_MAX, update_srand = true):\n"
      "  Generates a random integer from a uniform distribution.\n";
      
    calls_["#read_file"] =
      "\n#read_file (filename) or #read_file (filename, type):\n"
      "  Reads a file and returns its contents. Default is to determine\n"
      "  file type by filname extension. Explicit type can be a string\n"
      "  or an integer of the following values:\n"
      "    'xml' = 16\n"
      "    'text' = 32\n"
      "    'jpeg' = 256\n";
      
    calls_["#set_clock"] =
      "\n#set_clock (value) or #set_clock (variable, value):\n"
      "  Sets the system clock or a variable clock. The value should be\n"
      "  a non-zero integer (Lamport clock) and is used to determine which\n"
      "  variable values are rejected at the transport layer. Returns the\n"
      "  new clock value.\n";
      
    calls_["#set_precision"] =
      "\n#set_precision (value) or #precision (value):\n"
      "  Sets the precision that is used to convert doubles to strings.\n"
      "  This is a fixed precision (e.g., 5 for pi would be 3.14159).\n"
      "  Negative value resets to default precision.\n";
      
    calls_["#size"] =
      "\n#size (value):\n"
      "  Returns the size of the variable or value.\n";
       
    calls_["#sleep"] =
      "\n#sleep (double time_to_sleep in s):\n"
      "  Sleeps for at least the specified time and returns "
      " time spent sleeping in seconds.\n";
      
    calls_["#to_buffer"] =
      "\n#to_buffer (value) or #buffer (value):\n"
      "  Converts the value to an unsigned char array.\n";
      
    calls_["#to_double"] =
      "\n#to_double (value) or #double (value):\n"
      "  Converts the value to a double.\n";
      
    calls_["#to_doubles"] =
      "\n#to_doubles (value) or #doubles (value):\n"
      "  Converts the value to an array of doubles.\n";
      
    calls_["#to_host_dirs"] =
      "\n#to_host_dirs (directory):\n"
      "  Returns the host appropriate directory structure, e.g.,.\n"
      "  files/file.txt to files\file.txt on Windows.\n";
      
    calls_["#to_integer"] =
      "\n#to_integer (value) or #integer (value):\n"
      "  Converts the value to an integer.\n";
      
    calls_["#to_integers"] =
      "\n#to_integers (value) or #integers (value):\n"
      "  Converts the value to an array of integers.\n";
      
    calls_["#to_string"] =
      "\n#to_string (value, delimiter) or #string (value, delimiter):\n"
      "  Converts the value to an ASCII string. Delimiter is an optional\n"
      "  string sequence that separates elements of an integer or double"
      " array.\n";
      
    calls_["#type"] =
      "\n#type (value):\n"
      "  Returns the integer type of the variable or value.\n"
      "  Supported types are:\n"
      "    UNINITIALIZED = 0\n"
      "    INTEGER = 1\n"
      "    STRING = 2\n"
      "    DOUBLE = 4\n"
      "    UNKNOWN_FILE_TYPE = 8\n"
      "    XML = 16\n"
      "    TEXT_FILE = 32\n"
      "    INTEGER_ARRAY = 64\n"
      "    DOUBLE_ARRAY = 128\n"
      "    IMAGE_JPEG = 256\n";
      
    calls_["#write_file"] =
      "\n#write_file (filename, value):\n"
      "  Writes the value to a file. Supports all MADARA types.\n"
      "  @see #type.\n";
      
  }

  return result;
}

/// Evaluates the node and its children. This does not prune any of
/// the expression tree, and is much faster than the prune function
Madara::Knowledge_Record 
Madara::Expression_Tree::System_Call_Print_System_Calls::evaluate (
const Madara::Knowledge_Engine::Knowledge_Update_Settings & settings)
{
  Knowledge_Record return_value;

  if (nodes_.size () == 1)
  {
    MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
      "System call print_system_calls is printing help.\n"));

    context_.print (
      calls_[nodes_[0]->evaluate (settings).to_string ()], 0);

    return Madara::Knowledge_Record::Integer (1);
  }
  else if (nodes_.size () == 0)
  {
    MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
      "System call print_system_calls is printing help.\n"));

    for (System_Calls_Help::const_iterator i = calls_.begin ();
         i != calls_.end (); ++i)
    {
      if (i->first != "#delete_var" && i->first != "#eval" && 
        i->first != "#expand" && i->first != "#expand_env")
        context_.print (i->second, 0);
    }
  }
  else
  {
    MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_DEBUG, 
      "KARL RUNTIME ERROR: System call print_system_calls takes either 0"
      " or 1 arguments.\n"));
  }

  return return_value;
}

// accept a visitor
void 
Madara::Expression_Tree::System_Call_Print_System_Calls::accept (
  Madara::Expression_Tree::Visitor &visitor) const
{
  visitor.visit (*this);
}
