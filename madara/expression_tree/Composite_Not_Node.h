/* -*- C++ -*- */
#ifndef _COMPOSITE_NOT_NODE_H_
#define _COMPOSITE_NOT_NODE_H_

#include "madara/expression_tree/Composite_Unary_Node.h"

namespace Madara
{
  namespace Expression_Tree
  {
    class Component_Node;
    class Visitor;

    /**
     * @class Composite_Not_Node
     * @brief A composite node that logically nots a right expression.
     *        For instance, !0 == 1. !8 == 0.
     */
    class Composite_Not_Node : public Composite_Unary_Node
    {
    public:
      /**
       * Constructor
       * @param   right  right expression
       **/
      Composite_Not_Node (Component_Node *right);

      /**
       * Destructor
       **/
      virtual ~Composite_Not_Node (void);

      /**
       * Returns the printable character of the node
       * @return    value of the node
       **/
      virtual long long item (void) const;

      /** 
       * Prunes the expression tree of unnecessary nodes. 
       * @param     can_change   set to true if variable nodes are contained
       * @return    0 if right expression is non-zero and 1 otherwise.
       **/
      virtual long long prune (bool & can_change);

      /** 
       * Evaluates the expression tree. 
       * @return    0 if right expression is non-zero and 1 otherwise.
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

#endif /* _COMPOSITE_NEGATE_NODE_H_ */
