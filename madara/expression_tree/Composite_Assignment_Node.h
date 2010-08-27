#ifndef COMPOSITE_ASSIGNMENT_NODE_H
#define COMPOSITE_ASSIGNMENT_NODE_H


#include "madara/expression_tree/Composite_Binary_Node.h"

namespace Madara
{
  namespace Expression_Tree
  {
    // Forward declaration.
    class Component_Node;
    class Visitor;

    /**
     * @class Composite_Assignment_Node
     * @brief A composite node that allows for variable assignment.
     *        Left child should be a variable and right child should
     *        be an expression to evaluate
     */
    class Composite_Assignment_Node : public Composite_Binary_Node
    {
    public:
      /// Ctor
      Composite_Assignment_Node (Component_Node *left, 
                          Component_Node *right);

      /// Return the printable character stored in the node.
      virtual int item (void) const;

      /// Define the @a accept() operation used for the Visitor pattern.
      virtual void accept (Visitor &visitor) const;
    };
  }
}

#endif //COMPOSITE_ASSIGNMENT_NODE_H
