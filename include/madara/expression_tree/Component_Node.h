/* -*- C++ -*- */
#ifndef _COMPONENT_NODE_H_
#define _COMPONENT_NODE_H_

#include <string>
#include <stdexcept>
#include "madara/knowledge_engine/Knowledge_Record.h"

// Because Microsoft decided to not support stdint.h
// from VS 2003 until VS 2010, we simply typedef the
// symbols that are supported in all MS products when
// we know we have a MS compiler

#ifdef _MSC_VER

typedef __int32             int32_t;
typedef unsigned __int32    uint32_t;
typedef __int64             int64_t;
typedef unsigned __int64    uint64_t;

#else   // !_MSC_VER

// otherwise, we do something more logical
#include <stdint.h>

#endif  // _MSC_VER


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
