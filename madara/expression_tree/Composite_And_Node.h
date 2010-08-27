#ifndef COMPOSITE_AND_NODE_H
#define COMPOSITE_AND_NODE_H


#include "madara/expression_tree/Composite_Binary_Node.h"

namespace Madara
{
  namespace Expression_Tree
  {
    // Forward declaration.
    class Component_Node;
    class Visitor;

    /**
     * @class Composite_And_Node
     * @brief A composite node that performs a logical and
     */
    class Composite_And_Node : public Composite_Binary_Node
    {
    public:
      /// Ctor
      Composite_And_Node (Component_Node *left, 
                          Component_Node *right);

      /// Return the printable character stored in the node.
      virtual int item (void) const;

      /// Define the @a accept() operation used for the Visitor pattern.
      virtual void accept (Visitor &visitor) const;
    };
  }
}


#endif // COMPOSITE_AND_NODE_H
