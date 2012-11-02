/* -*- C++ -*- */
#ifndef _COMPOSITE_ADD_NODE_H_
#define _COMPOSITE_ADD_NODE_H_

#include "madara/expression_tree/Composite_Binary_Node.h"
#include <stdint.h>

namespace Madara
{
  namespace Expression_Tree
  {
    // Forward declaration.
    class Component_Node;
    class Visitor;

    /**
     * @class Composite_Add_Node
     * @brief A composite node that encompasses addition of two expressions
     */
    class Composite_Add_Node : public Composite_Binary_Node
    {
    public:
      /**
       * Constructor
       * @param   left   left expression
       * @param   right  right expression
       **/
      Composite_Add_Node (Component_Node *left, 
                          Component_Node *right);

      /**
       * Returns the printable character of the node
       * @return    value of the node
       **/
      virtual Madara::Knowledge_Record::VALUE_TYPE item (void) const;

      /** 
       * Prunes the expression tree of unnecessary nodes. 
       * @param     can_change   set to true if variable nodes are contained
       * @return    value of left expression plus the value of the right one
       **/
      virtual Madara::Knowledge_Record::VALUE_TYPE prune (bool & can_change);

      /** 
       * Evaluates the expression tree. 
       * @return    value of left expression plus the value of the right one
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
#endif /* _COMPOSITE_ADD_NODE_H_ */
