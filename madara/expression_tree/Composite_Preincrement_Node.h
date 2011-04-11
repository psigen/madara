/* -*- C++ -*- */
#ifndef _COMPOSITE_PREINCREMENT_NODE_H_
#define _COMPOSITE_PREINCREMENT_NODE_H_

#include "madara/expression_tree/Composite_Unary_Node.h"

namespace Madara
{
  namespace Expression_Tree
  {
    class Component_Node;
    class Visitor;

    /**
     * @class Composite_Preincrement_Node
     * @brief A composite node that increments a right expression. If
     *        right expression is a variable, then the variable will
     *        be updated with the new value.
     */
    class Composite_Preincrement_Node : public Composite_Unary_Node
    {
    public:
      /**
       * Constructor
       * @param   right  right expression
       **/
      Composite_Preincrement_Node (Component_Node *right);

      /**
       * Destructor
       **/
      virtual ~Composite_Preincrement_Node (void);

      /**
       * Returns the printable character of the node
       * @return    value of the node
       **/
      virtual long long item (void) const;

      /** 
       * Prunes the expression tree of unnecessary nodes. 
       * @param     can_change   set to true if variable nodes are contained
       * @return    one added to the value returned by the right expression
       **/
      virtual long long prune (bool & can_change);

      /** 
       * Evaluates the expression tree. 
       * @return    one added to the value returned by the right expression
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

#endif /* _COMPOSITE_PREINCREMENT_NODE_H_ */
