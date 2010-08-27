#ifndef COMPOSITE_OR_NODE_H
#define COMPOSITE_OR_NODE_H


#include "madara/expression_tree/Composite_Binary_Node.h"

namespace Madara
{
  namespace Expression_Tree
  {
    // Forward declaration.
    class Component_Node;
    class Visitor;

    /**
     * @class Composite_Or_Node
     * @brief A composite node that performs a logical or
     */
    class Composite_Or_Node : public Composite_Binary_Node
    {
    public:
      /// Ctor
      Composite_Or_Node (Component_Node *left, 
                          Component_Node *right);

      /// Return the printable character stored in the node.
      virtual int item (void) const;

      /// Define the @a accept() operation used for the Visitor pattern.
      virtual void accept (Visitor &visitor) const;
    };
  }
}


#endif // COMPOSITE_OR_NODE_H
