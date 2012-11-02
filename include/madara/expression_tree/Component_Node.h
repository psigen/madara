/* -*- C++ -*- */
#ifndef _COMPONENT_NODE_H_
#define _COMPONENT_NODE_H_

#include <string>
#include <stdexcept>
#include <stdint.h>
#include "madara/Globals.h"

namespace Madara
{
  namespace Expression_Tree
  {
    // Forward declaration.
    class Visitor;

    /**
     * @class Component_Node
     * @brief An abstract base class defines a simple abstract
     *        implementation of an expression tree node.
     *        
     * @see   See Composite_Unary_Node, Composite_Binary_Node, and Leaf Node
     *        for immediate subclasses of this class.
     */
    class Component_Node
    {
    public:
      /**
       * Destructor
       **/
      virtual ~Component_Node (void) = 0;

      /**
       * Returns the value of the node
       * @return    value of the node
       **/
      virtual Madara::Knowledge_Record::VALUE_TYPE item (void) const;

      /** 
       * Prunes the expression tree of unnecessary nodes. 
       * @param     can_change   set to true if variable nodes are contained
       * @return    value of current contained expression tree
       **/
      virtual Madara::Knowledge_Record::VALUE_TYPE prune (bool & can_change) = 0;

      /** 
       * Evaluates the expression tree. 
       * @return    value of current contained expression tree
       **/
      virtual Madara::Knowledge_Record::VALUE_TYPE evaluate (void) = 0;

      /** 
       * Returns the left expression. 
       * @return    a pointer to the left expression
       **/
      virtual Component_Node *left (void) const;

      /** 
       * Returns the right expression. 
       * @return    a pointer to the right expression
       **/
      virtual Component_Node *right (void) const;

      /** 
       * Accepts a visitor subclassed from the Visitor class
       * @param    visitor   visitor instance to use
       **/
      virtual void accept (Visitor &visitor) const;
    };
  }
}

#endif /* _COMPONENT_NODE_H_ */
