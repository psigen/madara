/* -*- C++ -*- */
#ifndef _LEAF_NODE_H_
#define _LEAF_NODE_H_

#include <string>
#include "madara/expression_tree/Component_Node.h"

namespace Madara
{
  namespace Expression_Tree
  {
    // Forward declarations.
    class Visitor;

    /**
     * @class Leaf_Node
     * @brief Defines a node that contains a long long value
     */

    class Leaf_Node : public Component_Node
    {
    public:
      /**
       * Constructor
       * @param   item    value of the node
       **/
      Leaf_Node (long long item);

      /**
       * Constructor
       * @param   item    value of the node
       **/
      Leaf_Node (const ::std::string &item);

      /**
       * Constructor
       * @param   item    value of the node
       **/
      Leaf_Node (const char *item);

      /**
       * Destructor
       **/
      virtual ~Leaf_Node (void);

      /**
       * Returns the printable value of the node
       * @return    value of the node
       **/
      virtual long long item (void) const;

      /** 
       * Prunes the expression tree of unnecessary nodes. 
       * @param     can_change   set to true if variable nodes are contained
       * @return    value of this node
       **/
      virtual long long prune (bool & can_change);

      /** 
       * Evaluates the expression tree. 
       * @return    value of this node
       **/
      virtual long long evaluate (void);

      /** 
       * Accepts a visitor subclassed from the Visitor class
       * @param    visitor   visitor instance to use
       **/
      virtual void accept (Visitor &visitor) const; 

    private:
      /// Integer value associated with the operand.
      long long item_;
    };

  }
}

#endif /* _LEAF_NODE_H_ */
