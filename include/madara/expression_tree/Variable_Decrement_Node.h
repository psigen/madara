/* -*- C++ -*- */
#ifndef _VARIABLE_DECREMENT_NODE_H_
#define _VARIABLE_DECREMENT_NODE_H_

#include <string>
#include <vector>

#include "madara/expression_tree/Component_Node.h"
#include "madara/expression_tree/Variable_Node.h"
#include "madara/expression_tree/Composite_Array_Reference.h"
#include "madara/knowledge_engine/Thread_Safe_Context.h"
#include "madara/knowledge_engine/Knowledge_Record.h"

namespace Madara
{
  namespace Expression_Tree
  {
    // Forward declarations.
    class Visitor;

    /**
     * @class Variable_Decrement_Node
     * @brief Composite node that subtracts a variable by some right hand side
     */

    class Variable_Decrement_Node : public Component_Node
    {
    public:
      /// Ctor.
      Variable_Decrement_Node (Component_Node * lhs,
        Madara::Knowledge_Record value,
        Component_Node * rhs,
        Madara::Knowledge_Engine::Thread_Safe_Context &context);

      /// Dtor.
      virtual ~Variable_Decrement_Node (void);

      /// Return the item stored in the node.
      virtual Madara::Knowledge_Record item (void) const;

      /// Prune the tree of unnecessary nodes. 
      /// Returns evaluation of the node and sets can_change appropriately.
      /// if this node can be changed, that means it shouldn't be pruned.
      virtual Madara::Knowledge_Record prune (bool & can_change);

      /// Evaluates the node and its children. This does not prune any of
      /// the expression tree, and is much faster than the prune function
      virtual Madara::Knowledge_Record evaluate (
        const Madara::Knowledge_Engine::Knowledge_Update_Settings & settings);

      /// Define the @a accept() operation used for the Visitor pattern.
      virtual void accept (Visitor &visitor) const; 

    private:
      /// variable holder
      Variable_Node * var_;

      /// variable index holder
      Composite_Array_Reference * array_;

      /// amount to increment by. Note that this can also do decrement.
      Madara::Knowledge_Record value_;
      
      /// holds a right hand side argument if it is not value_
      Component_Node * rhs_;

      Madara::Knowledge_Engine::Thread_Safe_Context & context_;
    };
  }
}
#endif /* _VARIABLE_DECREMENT_NODE_H_ */
