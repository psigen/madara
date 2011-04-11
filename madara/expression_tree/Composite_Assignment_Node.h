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
      /**
       * Constructor
       * @param   left   variable to assign the right expression to
       * @param   right  right expression to assign to the variable
       **/
      Composite_Assignment_Node (Component_Node *left, 
                          Component_Node *right);

      /**
       * Returns the printable character of the node
       * @return    value of the node
       **/
      virtual long long item (void) const;

      /** 
       * Prunes the expression tree of unnecessary nodes. 
       * @param     can_change   set to true if variable nodes are contained
       * @return    value of evaluated right expression
       **/
      virtual long long prune (bool & can_change);

      /** 
       * Evaluates the expression tree. 
       * @return    value of evaluated right expression
       **/
      virtual long long evaluate (void);

      /** 
       * Accepts a visitor subclassed from the Visitor class
       * @param    visitor   visitor instance to use
       **/
      virtual void accept (Visitor &visitor) const;
    };
  }
}

#endif //COMPOSITE_ASSIGNMENT_NODE_H
