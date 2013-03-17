
#include "madara/utility/Log_Macros.h"
#include "madara/expression_tree/Leaf_Node.h"
#include "madara/expression_tree/System_Call_Read_File.h"
#include "madara/expression_tree/Visitor.h"


Madara::Expression_Tree::System_Call_Read_File::System_Call_Read_File (
  Madara::Knowledge_Engine::Thread_Safe_Context & context,
  const Component_Nodes & nodes)
  : System_Call_Node (context, nodes)
{

}

// Dtor
Madara::Expression_Tree::System_Call_Read_File::~System_Call_Read_File (void)
{
}

Madara::Knowledge_Record
Madara::Expression_Tree::System_Call_Read_File::item (void) const
{
  return Madara::Knowledge_Record::Integer (nodes_.size ());
}

/// Prune the tree of unnecessary nodes. 
/// Returns evaluation of the node and sets can_change appropriately.
/// if this node can be changed, that means it shouldn't be pruned.
Madara::Knowledge_Record
Madara::Expression_Tree::System_Call_Read_File::prune (bool & can_change)
{
  // user can always change a function, and we have no control over
  // what it does. Consequently, a function node cannot be pruned out
  // under any situation
  can_change = true;
  
  Madara::Knowledge_Record result;

  if (nodes_.size () > 0)
  {
    bool arg_can_change = false;
    result = nodes_[0]->prune (arg_can_change);
    
    if (!arg_can_change && dynamic_cast <Leaf_Node *> (nodes_[0]) == 0)
    {
      delete nodes_[0];
      nodes_[0] = new Leaf_Node (result);
    }
  }
  else
  {
    MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_ERROR, 
      "KARL COMPILE ERROR: System call read_file"
      " requires a filename to read.\n"));
  }

  return result;
}

/// Evaluates the node and its children. This does not prune any of
/// the expression tree, and is much faster than the prune function
Madara::Knowledge_Record 
Madara::Expression_Tree::System_Call_Read_File::evaluate (
const Madara::Knowledge_Engine::Knowledge_Update_Settings & settings)
{
  Knowledge_Record return_value;

  if (nodes_.size () > 0)
  {
    // copying strings wastes execution time, so we hold the Knowledge_Record
    // instead of the resulting string filename.
    Knowledge_Record filename_eval = nodes_[0]->evaluate (settings);

    MADARA_DEBUG (MADARA_LOG_MINOR_EVENT, (LM_DEBUG, 
      "System call read_file is attempting to open %s.\n",
      filename_eval.to_string ().c_str ()));

    if (0 != return_value.read_file (filename_eval.to_string ()))
    {
      MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_DEBUG, 
        "KARL ERROR: System call read_file could not open %s.\n",
        filename_eval.to_string ().c_str ()));
    }
  }
  else
  {
    MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_DEBUG, 
      "KARL RUNTIME ERROR: System call read_file requires"
      " a filename to read.\n"));
  }

  return return_value;
}

// accept a visitor
void 
Madara::Expression_Tree::System_Call_Read_File::accept (
  Madara::Expression_Tree::Visitor &visitor) const
{
  visitor.visit (*this);
}
