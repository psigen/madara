#ifndef COMPOSITE_GREATER_THAN_NODE_H
#define COMPOSITE_GREATER_THAN_NODE_H


#include "madara/expression_tree/Composite_Binary_Node.h"

namespace Madara
{
  namespace Expression_Tree
  {
    // Forward declaration.
    class Component_Node;
    class Visitor;

    /**
     * @class Composite_Greater_Than_Node
     * @brief A composite node that compares left and right children
     *        for greater than
     */
    class Composite_Greater_Than_Node : public Composite_Binary_Node
    {
    public:
      /**
       * Constructor
       * @param   left   left expression
       * @param   right  right expression
       **/
      Composite_Greater_Than_Node (Component_Node *left, 
                          Component_Node *right);

      /**
       * Returns the printable character of the node
       * @return    value of the node
       **/
      virtual long long item (void) const;

      /** 
       * Prunes the expression tree of unnecessary nodes. 
       * @param     can_change   set to true if variable nodes are contained
       * @return    0 if left expression is less than or equal to right 
       *            expression and 1 otherwise
       **/
      virtual long long prune (bool & can_change);

      /** 
       * Evaluates the expression tree. 
       * @return    0 if left expression is less than or equal to right 
       *            expression and 1 otherwise
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


#endif // COMPOSITE_GREATER_THAN_NODE_H
