#ifndef COMPOSITE_BOTH_NODE_H
#define COMPOSITE_BOTH_NODE_H


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
     * @class Composite_Both_Node
     * @brief A composite node that evaluates both left and right
     *        expressions regardless of their evaluations
     */
    class Composite_Both_Node : public Composite_Binary_Node
    {
    public:
      /**
       * Constructor
       * @param   left   left expression
       * @param   right  right expression
       **/
      Composite_Both_Node (Component_Node *left, 
                          Component_Node *right);

      /**
       * Returns the printable character of the node
       * @return    value of the node
       **/
      virtual Madara::Knowledge_Record::VALUE_TYPE item (void) const;

      /** 
       * Prunes the expression tree of unnecessary nodes. 
       * @param     can_change   set to true if variable nodes are contained
       * @return    the maximum of the value returned by the
       *            left and right expressions
       **/
      virtual Madara::Knowledge_Record::VALUE_TYPE prune (bool & can_change);

      /** 
       * Evaluates the expression tree. 
       * @return    the maximum of the value returned by the
       *            left and right expressions
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


#endif // COMPOSITE_BOTH_NODE_H
