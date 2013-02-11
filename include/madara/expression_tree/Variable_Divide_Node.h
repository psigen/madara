/* -*- C++ -*- */
#ifndef _VARIABLE_DIVIDE_NODE_H_
#define _VARIABLE_DIVIDE_NODE_H_

#include <string>
#include <vector>

#include "madara/expression_tree/Component_Node.h"
#include "madara/knowledge_engine/Thread_Safe_Context.h"
#include "madara/knowledge_engine/Knowledge_Record.h"

namespace Madara
{
  namespace Expression_Tree
  {
    // Forward declarations.
    class Visitor;

    /**
     * @class Variable_Divide_Node
     * @brief Composite node that divides a variable by some right hand side
     */

    class Variable_Divide_Node : public Component_Node
    {
    public:
      /// Ctor.
      Variable_Divide_Node (const std::string &key,
        Madara::Knowledge_Record value,
        Component_Node * rhs,
        Madara::Knowledge_Engine::Thread_Safe_Context &context);

      /// Dtor.
      virtual ~Variable_Divide_Node (void);

      /// Return the item stored in the node.
      virtual Madara::Knowledge_Record item (void) const;

      /// Atomically increment the variable.
      inline Madara::Knowledge_Record compute (
        const Madara::Knowledge_Engine::Knowledge_Update_Settings & settings)
      {
        Madara::Knowledge_Record * record = record_;

        // if we don't have a static record, get the dynamic one
        if (!record)
          record = context_.get_record (expand_key ());

        // notice that we assume the context is locked
        // check if we have the appropriate write quality
        if (!settings.always_overwrite &&
            record->write_quality < record->quality)
          return *record;

        // cheaper to read than write, so check to see if
        // we actually need to update quality and status
        if (record->write_quality != record->quality)
          record->quality = record->write_quality;

        if (!rhs_)
          *record /= this->value_;
        else
          *record /= rhs_->item ();

        if (key_[0] != '.' && !settings.treat_globals_as_locals)
        {
          context_.mark_modified (key_, *record);
        }

        context_.signal ();
        return *record;
      }

      /// Prune the tree of unnecessary nodes. 
      /// Returns evaluation of the node and sets can_change appropriately.
      /// if this node can be changed, that means it shouldn't be pruned.
      virtual Madara::Knowledge_Record prune (bool & can_change);

      /// Evaluates the node and its children. This does not prune any of
      /// the expression tree, and is much faster than the prune function
      virtual Madara::Knowledge_Record evaluate (
        const Madara::Knowledge_Engine::Knowledge_Update_Settings & settings);

      /// Expands the key (if necessary). This allow for keys to be defined
      /// with other variables inserted (e.g. var{.id} with .id = 2 expands
      /// to var2)
      std::string expand_key (void) const;

      /// Return the variable key.
      const std::string & key (void) const;

      /// Define the @a accept() operation used for the Visitor pattern.
      virtual void accept (Visitor &visitor) const; 

    private:
      /// Key for retrieving value of this variable.
      const std::string key_;

      /// amount to increment by. Note that this can also do decrement.
      Madara::Knowledge_Record value_;
      
      /// holds a right hand side argument if it is not value_
      Component_Node * rhs_;

      Madara::Knowledge_Record * record_;

      Madara::Knowledge_Engine::Thread_Safe_Context & context_;

      /// Expansion necessary
      bool key_expansion_necessary_;

      std::vector< std::string> splitters_;
      std::vector< std::string> tokens_;
      std::vector< std::string> pivot_list_;

      /// Reference to context for variable retrieval
    };
  }
}
#endif /* _VARIABLE_DIVIDE_NODE_H_ */
