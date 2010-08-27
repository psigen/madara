#ifndef COMPOSITE_GREATER_THAN_EQUAL_NODE_H
#define COMPOSITE_GREATER_THAN_EQUAL_NODE_H


#include "madara/expression_tree/Composite_Binary_Node.h"

namespace Madara
{
  namespace Expression_Tree
  {
    // Forward declaration.
    class Component_Node;
    class Visitor;

    /**
     * @class Composite_Greater_Than_Equal_Node
     * @brief A composite node that compares left and right children
     *        for greater than or equal to
     */
    class Composite_Greater_Than_Equal_Node : public Composite_Binary_Node
    {
    public:
      /// Ctor
      Composite_Greater_Than_Equal_Node (Component_Node *left, 
                          Component_Node *right);

      /// Return the printable character stored in the node.
      virtual int item (void) const;

      /// Define the @a accept() operation used for the Visitor pattern.
      virtual void accept (Visitor &visitor) const;
    };
  }
}


#endif // COMPOSITE_GREATER_THAN_EQUAL_NODE_H