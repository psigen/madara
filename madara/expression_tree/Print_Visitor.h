/* -*- C++ -*- */
#ifndef _PRINT_VISITOR_H_
#define _PRINT_VISITOR_H_

#include "madara/expression_tree/Visitor.h"

namespace Madara
{
  namespace Expression_Tree
  {
    /**
     * @class Print_Visitor
     * @brief This class serves as a visitor for printing the contents of
     *        nodes to @a std::cout.
     */

    class Print_Visitor : public Visitor
    {
    public:
      /// Visits a Leaf_Node and prints it contents to @a std::cout.
      virtual void visit (const Leaf_Node &node);

      /// Visit a Variable_Node.
      virtual void visit (const Variable_Node &node);

      /// Visit a Composite_Negate_Node and prints its contents to @a std::cout.
      virtual void visit (const Composite_Negate_Node &node);
      
      /// Visit a Composite_Predecrement_Node and prints its contents to @a std::cout.
      virtual void visit (const Composite_Predecrement_Node &node);
      
      /// Visit a Composite_Preincrement_Node and prints its contents to @a std::cout.
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

      /// Visit a @a Composite_Subtract_Node and prints its contents to @a std::cout.
      virtual void visit (const Composite_Subtract_Node &node);
      
      /// Visit a @a Composite_Divide_Node and prints its contents to @a std::cout.
      virtual void visit (const Composite_Divide_Node &node);
      
      /// visit function - prints Composite_Multiply_Node contents to std::cout
      virtual void visit (const Composite_Multiply_Node &node);
    };  
  }
}
#endif /* _PRINT_VISITOR_H_ */
