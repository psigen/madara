
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
    
    calls_["#expand_statement"] =
      "\n#expand (statement) or #expand_statement (statement):\n"
      "  Expands a statement such as 'var{.i}' into 'var0', assuming .i=0\n";

    calls_["#get_clock"] =
      "\n#get_clock () or #get_clock (variable):\n"
      "  Returns either the system clock or the variable clock. Note that\n"
      "  this is a Lamport clock value and not a wall time value.\n";

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

    calls_["#read_file"] =
      "\n#read_file (filename):\n"
      "  Reads a file and returns its contents.\n";
      
    calls_["#set_clock"] =
      "\n#set_clock (value) or #set_clock (variable, value):\n"
      "  Sets the system clock or a variable clock. The value should be\n"
      "  a non-zero integer (Lamport clock) and is used to determine which\n"
      "  variable values are rejected at the transport layer. Returns the\n"
      "  new clock value.\n";
      
    calls_["#size"] =
      "\n#size (value):\n"
      "  Returns the size of the variable or value.\n";
      
    calls_["#type"] =
      "\n#type (value):\n"
      "  Returns the integer type of the variable or value.\n"
      "  Supported types are:\n"
      "    INTEGER = 0\n"
      "    STRING = 1\n"
      "    DOUBLE = 2\n"
      "    UNKNOWN_FILE_TYPE = 3\n"
      "    XML = 4\n"
      "    TEXT_FILE = 5\n"
      "    IMAGE_JPEG = 50\n";
      
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
          i->first != "#expand")
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
