/* -*- C++ -*- */
#ifndef _COMPOSITE_MODULUS_NODE_H
#define _COMPOSITE_MODULUS_NODE_H

namespace Madara
{
  namespace Expression_Tree
  {
    class Component_Node;
    class Composite_Binary_Node;
    class Visitor;

    /**
     * @class Composite_Modulus_Node
     * @brief A composite node containing left and right
     *        children. The meaning of this node is left * right
     */
    class Composite_Modulus_Node : public Composite_Binary_Node
    {
    public:
      /// Ctor
      Composite_Modulus_Node (Component_Node *left,
                               Component_Node *right);

      /// Dtor
      virtual ~Composite_Modulus_Node (void);

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

#endif /* _COMPOSITE_MODULUS_NODE_H */
