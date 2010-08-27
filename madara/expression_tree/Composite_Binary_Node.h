/* -*- C++ -*- */
#ifndef _COMPOSITE_BINARY_NODE_H_
#define _COMPOSITE_BINARY_NODE_H_

#include <string>

#include "madara/expression_tree/Composite_Unary_Node.h"

namespace Madara
{
  namespace Expression_Tree
  {
    /**
     * @class Composite_Binary_Node
     * @brief Defines a left and right node (via inheritance).
     */
    class Composite_Binary_Node : public Composite_Unary_Node
    {
    public:
      /// Ctor
      Composite_Binary_Node (Component_Node *left,
                             Component_Node *right);

      /// Dtor
      virtual ~Composite_Binary_Node (void);

      /// Return the left child.
      virtual Component_Node *left (void) const;

    private:
      /// left child
      Component_Node *left_;
    };
  }
}

#endif /* _COMPOSITE_BINARY_NODE_H_ */
