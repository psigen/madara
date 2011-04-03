/* -*- C++ -*- */
#ifndef _LEAF_NODE_H_
#define _LEAF_NODE_H_

#include <string>
#include "madara/expression_tree/Component_Node.h"

namespace Madara
{
  namespace Expression_Tree
  {
    // Forward declarations.
    class Visitor;

    /**
     * @class Leaf_Node
     * @brief Defines a terminal node of type integer. This node inherits
     *        from Node and so has no children.
     */

    class Leaf_Node : public Component_Node
    {
    public:
      /// Ctor.
      Leaf_Node (long long item);

      /// Ctor.
      Leaf_Node (const ::std::string &item);

      /// Ctor.
      Leaf_Node (const char *item);

      /// Dtor.
      virtual ~Leaf_Node (void);

      /// Return the item stored in the node.
      virtual long long item (void) const;

      /// Prune the tree of unnecessary nodes. 
      /// Returns evaluation of the node and sets can_change appropriately.
      /// if this node can be changed, that means it shouldn't be pruned.
      virtual long long prune (bool & can_change);

      /// Evaluates the node and its children. This does not prune any of
      /// the expression tree, and is much faster than the prune function
      virtual long long evaluate (void);

      /// Define the @a accept() operation used for the Visitor pattern.
      virtual void accept (Visitor &visitor) const; 

    private:
      /// Integer value associated with the operand.
      long long item_;
    };

  }
}

#endif /* _LEAF_NODE_H_ */
