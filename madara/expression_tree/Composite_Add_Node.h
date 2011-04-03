/* -*- C++ -*- */
#ifndef _COMPOSITE_ADD_NODE_H_
#define _COMPOSITE_ADD_NODE_H_

#include "madara/expression_tree/Composite_Binary_Node.h"

namespace Madara
{
  namespace Expression_Tree
  {
    // Forward declaration.
    class Component_Node;
    class Visitor;

    /**
     * @class Composite_Add_Node
     * @brief A composite node containing left and right
     *        children. The meaning of this node is left + right
     */
    class Composite_Add_Node : public Composite_Binary_Node
    {
    public:
      /// Ctor
      Composite_Add_Node (Component_Node *left, 
                          Component_Node *right);

      /// Return the printable character stored in the node.
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
    };
  }
}
#endif /* _COMPOSITE_ADD_NODE_H_ */
