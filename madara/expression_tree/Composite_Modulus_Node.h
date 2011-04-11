/* -*- C++ -*- */
#ifndef _COMPOSITE_MODULUS_NODE_H
#define _COMPOSITE_MODULUS_NODE_H

namespace Madara
{
  namespace Expression_Tree
  {
    class Component_Node;
    class Composite_Binary_Node;
    class Visitor;

    /**
     * @class Composite_Modulus_Node
     * @brief A composite node that divides a left expression by a right
     *        expression and returns the remainder of the division
     */
    class Composite_Modulus_Node : public Composite_Binary_Node
    {
    public:
      /**
       * Constructor
       * @param   left   left expression
       * @param   right  right expression
       **/
      Composite_Modulus_Node (Component_Node *left,
                               Component_Node *right);

      /**
       * Destructor
       **/
      virtual ~Composite_Modulus_Node (void);

      /**
       * Returns the printable character of the node
       * @return    value of the node
       **/
      virtual long long item (void) const;

      /** 
       * Prunes the expression tree of unnecessary nodes. 
       * @param     can_change   set to true if variable nodes are contained
       * @return    remainder of left expression divided by right expression.
       *            If right expression is zero, then a compile error is 
       *            logged and the process exits.
       **/
      virtual long long prune (bool & can_change);

      /** 
       * Evaluates the expression tree. 
       * @return    remainder of left expression divided by right expression.
       *            If right expression is zero, then a compile error is 
       *            logged and the process exits.
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

#endif /* _COMPOSITE_MODULUS_NODE_H */
