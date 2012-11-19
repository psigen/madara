/* -*- C++ -*- */
#ifndef _VISITOR_H_
#define _VISITOR_H_

namespace Madara
{
  namespace Expression_Tree
  {
    class Leaf_Node;
    class Variable_Node;
    class Variable_Decrement_Node;
    class Variable_Divide_Node;
    class Variable_Increment_Node;
    class Variable_Multiply_Node;
    class Variable_Compare_Node;
    class List_Node;
    class Composite_Negate_Node;
    class Composite_Predecrement_Node;
    class Composite_Preincrement_Node;
    class Composite_Not_Node;
    class Composite_Add_Node;
    class Composite_Both_Node;
    class Composite_Sequential_Node;
    class Composite_Function_Node;
    class Composite_For_Loop;
    class Composite_Implies_Node;
    class Composite_Assignment_Node;
    class Composite_And_Node;
    class Composite_Or_Node;
    class Composite_Equality_Node;
    class Composite_Inequality_Node;
    class Composite_Less_Than_Equal_Node;
    class Composite_Less_Than_Node;
    class Composite_Greater_Than_Equal_Node;
    class Composite_Greater_Than_Node;
    class Composite_Subtract_Node;
    class Composite_Divide_Node;
    class Composite_Multiply_Node;
    class Composite_Modulus_Node;
    class Composite_Return_Right_Node;

    /**
     * @class Visitor
     * @brief Abstract base class for all visitors to all classes that derive
     *        from @a Component_Node.
     *
     *        This class plays the role of the "visitor" in the Visitor
     *        pattern. 
     */
    class Visitor
    {
    public:
      /// Visit a Leaf_Node.
      virtual void visit (const Leaf_Node &node) = 0;

      /// Visit a Variable_Node.
      virtual void visit (const Variable_Node &node) = 0;
      
      /// Visit a Variable_Decrement_Node.
      virtual void visit (const Variable_Decrement_Node &node) = 0;
      
      /// Visit a Variable_Divide_Node.
      virtual void visit (const Variable_Divide_Node &node) = 0;
      
      /// Visit a Variable_Increment_Node.
      virtual void visit (const Variable_Increment_Node &node) = 0;
      
      /// Visit a Variable_Multiply_Node.
      virtual void visit (const Variable_Multiply_Node &node) = 0;
      
      /// Visit a Variable_Compare_Node.
      virtual void visit (const Variable_Compare_Node &node) = 0;
      
      /// Visit a List_Node.
      virtual void visit (const List_Node &node) = 0;

      /// Visit a Composite_Negate_Node.
      virtual void visit (const Composite_Negate_Node &node) = 0;

      /// Visit a Composite_Predecrement_Node.
      virtual void visit (const Composite_Predecrement_Node &node) = 0;

      /// Visit a Composite_Increment_Node.
      virtual void visit (const Composite_Preincrement_Node &node) = 0;

      /// Visit a Composite_Not_Node.
      virtual void visit (const Composite_Not_Node &node) = 0;

      /// Visit a Composite_Add_Node.
      virtual void visit (const Composite_Add_Node &node) = 0;

      /// Visit a Composite_Assignment_Node.
      virtual void visit (const Composite_Assignment_Node &node) = 0;

      /// Visit a Composite_And_Node.
      virtual void visit (const Composite_And_Node &node) = 0;

      /// Visit a Composite_Or_Node.
      virtual void visit (const Composite_Or_Node &node) = 0;

      /// Visit a Composite_Equality_Node.
      virtual void visit (const Composite_Equality_Node &node) = 0;

      /// Visit a Composite_Inequality_Node.
      virtual void visit (const Composite_Inequality_Node &node) = 0;

      /// Visit a Composite_Less_Than_Equal_Node.
      virtual void visit (const Composite_Greater_Than_Equal_Node &node) = 0;

      /// Visit a Composite_Less_Than_Node.
      virtual void visit (const Composite_Greater_Than_Node &node) = 0;

      /// Visit a Composite_Less_Than_Equal_Node.
      virtual void visit (const Composite_Less_Than_Equal_Node &node) = 0;

      /// Visit a Composite_Less_Than_Node.
      virtual void visit (const Composite_Less_Than_Node &node) = 0;

      /// Visit a Composite_Subtract_Node.
      virtual void visit (const Composite_Subtract_Node &node) = 0;

      /// Visit a Composite_Divide_Node.
      virtual void visit (const Composite_Divide_Node &node) = 0;

      /// Visit a Composite_Multiply_Node.
      virtual void visit (const Composite_Multiply_Node &node) = 0;

      /// Visit a Composite_Modulus_Node.
      virtual void visit (const Composite_Modulus_Node &node) = 0;

      /// Visit a Composite_Both_Node.
      virtual void visit (const Composite_Both_Node &node) = 0;

      /// Visit a Composite_Return_Right_Node.
      virtual void visit (const Composite_Return_Right_Node &node) = 0;
      
      /// Visit a Composite_Sequential_Node.
      virtual void visit (const Composite_Sequential_Node &node) = 0;
      
      /// Visit a Composite_Function_Node.
      virtual void visit (const Composite_Function_Node &node) = 0;
      
      /// Visit a Composite_Function_Node.
      virtual void visit (const Composite_For_Loop &node) = 0;

      /// Visit a Composite_Implies_Node.
      virtual void visit (const Composite_Implies_Node &node) = 0;

      /// No-op destructor to hold things together.
      virtual ~Visitor (void) = 0;
    };
  }
}

#endif /* _VISITOR_H_ */
