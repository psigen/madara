/* -*- C++ -*- */
#ifndef _COMPOSITE_SUBTRACT_NODE_H_
#define _COMPOSITE_SUBTRACT_NODE_H_

#include "madara/knowledge_engine/Knowledge_Record.h"

namespace Madara
{
  namespace Expression_Tree
  {
    class Component_Node;
    class Visitor;

    /**
     * @class Composite_Subtract_Node
     * @brief A composite node that encompasses subtraction of a right
     *        expression from a left expression
     */
    class Composite_Subtract_Node : public Composite_Binary_Node
    {
    public:
      /**
       * Constructor
       * @param   left   left expression
       * @param   right  right expression
       **/
      Composite_Subtract_Node (Component_Node *left, Component_Node *right);

      /**
       * Destructor
       **/
      virtual ~Composite_Subtract_Node (void);

      /**
       * Returns the printable character of the node
       * @return    value of the node
       **/
      virtual Madara::Knowledge_Record::VALUE_TYPE item (void) const;

      /** 
       * Prunes the expression tree of unnecessary nodes. 
       * @param     can_change   set to true if variable nodes are contained
       * @return    value of right expression subtracted from left expression
       **/
      virtual Madara::Knowledge_Record::VALUE_TYPE prune (bool & can_change);

      /** 
       * Evaluates the expression tree. 
       * @return    value of right expression subtracted from left expression
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
#endif /* _COMPOSITE_SUBTRACT_NODE_H_ */
