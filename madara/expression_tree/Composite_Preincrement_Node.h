/* -*- C++ -*- */
#ifndef _COMPOSITE_PREINCREMENT_NODE_H_
#define _COMPOSITE_PREINCREMENT_NODE_H_

#include "madara/expression_tree/Composite_Unary_Node.h"

namespace Madara
{
  namespace Expression_Tree
  {
    class Component_Node;
    class Visitor;

    /**
     * @class Composite_Preincrement_Node
     * @brief A composite node containing only a right child. 
     *        The meaning of this node is ++right, eg ++5 == 6.
     *        If used with a variable, serves as a permanent increase.
     */
    class Composite_Preincrement_Node : public Composite_Unary_Node
    {
    public:
      /// Ctor
      Composite_Preincrement_Node (Component_Node *right);

      /// Dtor
      virtual ~Composite_Preincrement_Node (void);

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

#endif /* _COMPOSITE_PREINCREMENT_NODE_H_ */
