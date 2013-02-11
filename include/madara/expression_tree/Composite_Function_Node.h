/* -*- C++ -*- */
#ifndef _COMPOSITE_FUNCTION_NODE_H_
#define _COMPOSITE_FUNCTION_NODE_H_

#include "madara/expression_tree/Composite_Ternary_Node.h"
#include "madara/Functions.h"
#include "madara/knowledge_engine/Thread_Safe_Context.h"

namespace Madara
{
  namespace Expression_Tree
  {
    class Component_Node;
    class Visitor;

    /**
     * @class Composite_Function_Node
     * @brief A composite node that calls a function
     */
    class Composite_Function_Node : public Composite_Ternary_Node
    {
    public:
      /**
       * Constructor
       * @param   name   function name
       * @param   right  right expression
       **/
      Composite_Function_Node (const std::string & name, 
        Madara::Knowledge_Engine::Thread_Safe_Context & context,
        const Component_Nodes & nodes);

      /**
       * Destructor
       **/
      virtual ~Composite_Function_Node (void);

      /**
       * Returns the printable character of the node
       * @return    value of the node
       **/
      virtual Madara::Knowledge_Record item (void) const;

      /** 
       * Prunes the expression tree of unnecessary nodes. 
       * @param     can_change   set to true if variable nodes are contained
       * @return    negation of the right expression
       **/
      virtual Madara::Knowledge_Record prune (bool & can_change);

      /** 
       * Evaluates the expression tree. 
       * @return    negation of the right expression
       **/
      virtual Madara::Knowledge_Record evaluate (
        const Madara::Knowledge_Engine::Knowledge_Update_Settings & settings);

      /** 
       * Accepts a visitor subclassed from the Visitor class
       * @param    visitor   visitor instance to use
       **/
      virtual void accept (Visitor &visitor) const;

    private:

      // function name
      const std::string name_;

      // variables context
      Madara::Knowledge_Engine::Thread_Safe_Context & context_;

      // function pointer
      Madara::Knowledge_Engine::Function * function_;
    };
  }
}

#endif /* _COMPOSITE_FUNCTION_NODE_H_ */
