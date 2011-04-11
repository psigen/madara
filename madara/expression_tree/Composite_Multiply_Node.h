/* -*- C++ -*- */
#ifndef _COMPOSITE_MULTIPLY_NODE_H
#define _COMPOSITE_MULTIPLY_NODE_H

namespace Madara
{
  namespace Expression_Tree
  {
    class Component_Node;
    class Composite_Binary_Node;
    class Visitor;

    /**
     * @class Composite_Multiply_Node
     * @brief A composite node that multiplies a left expression by a right
     *        expression
     */
    class Composite_Multiply_Node : public Composite_Binary_Node
    {
    public:
      /**
       * Constructor
       * @param   left   left expression
       * @param   right  right expression
       **/
      Composite_Multiply_Node (Component_Node *left,
                               Component_Node *right);

      /**
       * Destructor
       **/
      virtual ~Composite_Multiply_Node (void);

      /**
       * Returns the printable character of the node
       * @return    value of the node
       **/
      virtual long long item (void) const;

      /** 
       * Prunes the expression tree of unnecessary nodes. 
       * @param     can_change   set to true if variable nodes are contained
       * @return    value of left expression multiplied by right expression
       **/
      virtual long long prune (bool & can_change);

      /** 
       * Evaluates the expression tree. 
       * @return    value of left expression multiplied by right expression
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

#endif /* _COMPOSITE_MULTIPLY_NODE_H */
