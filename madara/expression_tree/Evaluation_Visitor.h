/* -*- C++ -*- */
#ifndef _EVALUATION_VISITOR_H_
#define _EVALUATION_VISITOR_H_

#include "madara/expression_tree/Visitor.h"
#include "madara/utility/LStack.h"

namespace Madara
{
  namespace Expression_Tree
  {
    /**
     * @class Evaluation_Visitor
     * @brief This plays the role of a visitor for evaluating
     *        nodes in an expression tree that is being iterated in
     *        post-order fashion (and does not work correctly with any
     *        other iterator).
     */
    class Evaluation_Visitor : public Visitor
    {
    public:
      /// Visit a Leaf_Node.
      virtual void visit (const Leaf_Node &node);

      /// Visit a Variable_Node.
      virtual void visit (const Variable_Node &node);

      /// Visit a Composite_Negate_Node.
      virtual void visit (const Composite_Negate_Node &node);

      /// Visit a Composite_Predecrement_Node.
      virtual void visit (const Composite_Predecrement_Node &node);
      
      /// Visit a Composite_Preincrement_Node.
      virtual void visit (const Composite_Preincrement_Node &node);
      
      /// Visit a Composite_Not_Node.
      virtual void visit (const Composite_Not_Node &node);

      /// Visit a Composite_Add_Node.
      virtual void visit (const Composite_Add_Node &node);

      /// Visit a Composite_Assignment_Node.
      virtual void visit (const Composite_Assignment_Node &node);

      /// Visit a Composite_And_Node.
      virtual void visit (const Composite_And_Node &node);

      /// Visit a Composite_Or_Node.
      virtual void visit (const Composite_Or_Node &node);

      /// Visit a Composite_Equality_Node.
      virtual void visit (const Composite_Equality_Node &node);

      /// Visit a Composite_Inequality_Node.
      virtual void visit (const Composite_Inequality_Node &node);

      /// Visit a Composite_Less_Than_Equal_Node.
      virtual void visit (const Composite_Greater_Than_Equal_Node &node);

      /// Visit a Composite_Less_Than_Node.
      virtual void visit (const Composite_Greater_Than_Node &node);

      /// Visit a Composite_Less_Than_Equal_Node.
      virtual void visit (const Composite_Less_Than_Equal_Node &node);

      /// Visit a Composite_Less_Than_Node.
      virtual void visit (const Composite_Less_Than_Node &node);

      /// Visit a Composite_Subtract_Node.
      virtual void visit (const Composite_Subtract_Node &node);

      /// Visit a Composite_Divide_Node.
      virtual void visit (const Composite_Divide_Node &node);

      /// Visit a @a Composite_Multiply_Node.
      virtual void visit (const Composite_Multiply_Node &node);

      /// Visit a @a Composite_Modulus_Node.
      virtual void visit (const Composite_Modulus_Node &node);

      /// Print the total of the evaluation.
      int total (void);

      /// Resets the evaluation to it can be reused.
      void reset (void);

    private:
      /// Stack used for temporarily storing evaluations.
      Madara::Utility::LStack<int> stack_;
    };  
  }
}

#endif /* _VISITOR_H_ */
