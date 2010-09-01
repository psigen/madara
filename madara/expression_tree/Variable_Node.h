/* -*- C++ -*- */
#ifndef _VARIABLE_NODE_H_
#define _VARIABLE_NODE_H_

#include <string>
#include "madara/expression_tree/Component_Node.h"
#include "madara/knowledge_engine/Thread_Safe_Context.h"

namespace Madara
{
  namespace Expression_Tree
  {
    // Forward declarations.
    class Visitor;

    /**
     * @class Variable_Node
     * @brief Defines a terminal node of that references the current
     *        value stored in a variable.
     */

    class Variable_Node : public Component_Node
    {
    public:
      /// Ctor.
      Variable_Node (const ::std::string &key, 
        Madara::Knowledge_Engine::Thread_Safe_Context & context);

      /// Dtor.
      virtual ~Variable_Node (void);

      /// Return the item stored in the node.
      virtual int item (void) const;

      /// Return the item stored in the node.
      int set (const int & value);

      /// Atomically increment the variable.
      int inc (void);

      /// Atomically decrement the variable.
      int dec (void);

      /// Return the variable key.
      const std::string & key (void) const;

      /// Define the @a accept() operation used for the Visitor pattern.
      virtual void accept (Visitor &visitor) const; 

    private:
      /// Key for retrieving value of this variable.
      const ::std::string key_;

      /// Reference to context for variable retrieval
      Madara::Knowledge_Engine::Thread_Safe_Context & context_;
    };
  }
}
#endif /* _VARIABLE_NODE_H_ */
