#ifndef COMPOSITE_AND_NODE_H
#define COMPOSITE_AND_NODE_H


#include "madara/expression_tree/Composite_Binary_Node.h"
#include "madara/Globals.h"

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
      /**
       * Constructor
       * @param   left   left expression in addition
       * @param   right  right expression in addition
       **/
      Composite_And_Node (Component_Node *left, 
                          Component_Node *right);

      /**
       * Returns the printable character of the node
       * @return    value of the node
       **/
      virtual Madara::Knowledge_Record::VALUE_TYPE item (void) const;

      /** 
       * Prunes the expression tree of unnecessary nodes. 
       * @param     can_change   set to true if variable nodes are contained
       * @return    zero unless both left and right expressions 
       *            evaluate to non-zero. Right is not evaluated
       *            if left expression returns zero.
       **/
      virtual Madara::Knowledge_Record::VALUE_TYPE prune (bool & can_change);

      /** 
       * Evaluates the expression tree. 
       * @return    zero unless both left and right expressions 
       *            evaluate to non-zero. Right is not evaluated
       *            if left expression returns zero.
       **/
      virtual Madara::Knowledge_Record::VALUE_TYPE evaluate (void);

      /** 
       * Accepts a visitor subclassed from the Visitor class
       * @param    visitor   visitor instance to use
       **/
      virtual void accept (Visitor &visitor) const;
    };
  }
}


#endif // COMPOSITE_AND_NODE_H
