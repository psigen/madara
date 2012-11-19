/* -*- C++ -*- */
#ifndef _VARIABLE_COMPARE_NODE_H_
#define _VARIABLE_COMPARE_NODE_H_

#include <string>
#include <vector>

#include "madara/expression_tree/Component_Node.h"
#include "madara/knowledge_engine/Thread_Safe_Context.h"
#include "madara/knowledge_engine/Knowledge_Record.h"

namespace Madara
{
  namespace Expression_Tree
  {
    // Forward declarations.
    class Visitor;

    /**
     * @class Variable_Compare_Node
     * @brief Defines a terminal node of that references the current
     *        value stored in a variable.
     */

    class Variable_Compare_Node : public Component_Node
    {
    public:
      /// Ctor.
      Variable_Compare_Node (const std::string &key,
        Madara::Knowledge_Record value,
        int type, Component_Node * rhs,
        Madara::Knowledge_Engine::Thread_Safe_Context &context);

      enum Comparators
      {
        LESS_THAN = 0,
        LESS_THAN_EQUAL = 1,
        EQUAL = 2,
        GREATER_THAN = 3,
        GREATER_THAN_EQUAL = 4
      };

      /// Dtor.
      virtual ~Variable_Compare_Node (void);

      /// Return the item stored in the node.
      virtual Madara::Knowledge_Record item (void) const;

      /// Prune the tree of unnecessary nodes. 
      /// Returns evaluation of the node and sets can_change appropriately.
      /// if this node can be changed, that means it shouldn't be pruned.
      virtual Madara::Knowledge_Record prune (bool & can_change);

      /// Evaluates the node and its children. This does not prune any of
      /// the expression tree, and is much faster than the prune function
      virtual Madara::Knowledge_Record evaluate (void);

      /// Expands the key (if necessary). This allow for keys to be defined
      /// with other variables inserted (e.g. var{.id} with .id = 2 expands
      /// to var2)
      std::string expand_key (void) const;

      /// Return the variable key.
      const std::string & key (void) const;

      /// Define the @a accept() operation used for the Visitor pattern.
      virtual void accept (Visitor &visitor) const; 

    private:
      /// Key for retrieving value of this variable.
      const std::string key_;

      /// amount to increment by. Note that this can also do decrement.
      Madara::Knowledge_Record value_;
      
      /// holds a right hand side argument if it is not value_
      Component_Node * rhs_;

      Madara::Knowledge_Record * record_;

      Madara::Knowledge_Engine::Thread_Safe_Context & context_;

      /// comparison function
      int compare_type_;

      /// Expansion necessary
      bool key_expansion_necessary_;

      std::vector< std::string> splitters_;
      std::vector< std::string> tokens_;
      std::vector< std::string> pivot_list_;

      /// Reference to context for variable retrieval
    };
  }
}
#endif /* _VARIABLE_COMPARE_NODE_H_ */
