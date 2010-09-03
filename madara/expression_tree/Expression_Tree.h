/* -*- C++ -*- */

#ifndef _EXPRESSION_TREE_H_
#define _EXPRESSION_TREE_H_

#include <string>
#include <stdexcept>
#include "madara/Refcounter.h"

#include "madara/expression_tree/Component_Node.h"

namespace Madara
{
  namespace Expression_Tree
  {
    // Forward declarations.
    class Iterator;
    class Expression_Tree_Const_Iterator;

    /**
     * @class Expression_Tree
     * @brief Interface for the Composite pattern that is used to contain
     *        all the operator and operand nodes in the expression tree. 
     * 
     *        Plays the role of the "abstraction" class in the Bridge
     *        pattern and delegates to the appropriate "implementor"
     *        that performs the expression tree operations.  Also
     *        defines the STL factory methods that create the desired
     *        begin and end iterators.
     */
    class Expression_Tree
    {
    public:

      /// Exception class for Invalid States exceptions
      class Invalid_Iterator : public ::std::domain_error
      {
      public:
        Invalid_Iterator(const ::std::string &message) : ::std::domain_error(message) {}
      }; 

      // Define a "trait"
      typedef long value_type;
      typedef Iterator iterator;
      typedef Expression_Tree_Const_Iterator const_iterator;

      /// Default ctor
      Expression_Tree (void);

      /// Ctor that takes a @a Node * that contains all the nodes in the
      /// expression tree.
      Expression_Tree (Component_Node *root, bool increase_count = false);

      // Copy ctor
      Expression_Tree (const Expression_Tree &t);

      /// Gain access to the underlying root pointer functions are useful
      /// to the iterators.
      Component_Node *get_root (void);

      /// Assignment operator.
      void operator= (const Expression_Tree &t);

      /// Dtor
      ~Expression_Tree (void);

      /// Check if tree is null tree.
      bool is_null (void) const;

      /// Return the item in the tree.
      int item (void) const;

      /// Prune the tree of unnecessary nodes. 
      /// Returns evaluation of the node and sets can_change appropriately.
      /// if this node can be changed, that means it shouldn't be pruned.
      int prune (void);

      /// Evaluates the node and its children. This does not prune any of
      /// the expression tree, and is much faster than the prune function
      int evaluate (void);

      /// Return the left child.
      Expression_Tree left (void);

      /// Return the right child.
      Expression_Tree right (void);

      /// Get an iterator pointing to the "beginning" of the expression
      /// tree relative to the requested @a traversal_order.
      iterator begin (const ::std::string &traversal_order);

      /// Get an iterator pointing to the "end" of the expression tree
      /// relative to the requested @a traversal_order.
      iterator end (const ::std::string &traversal_order);

      /// Get a const iterator pointing to the "beginning" of the
      /// expression tree relative to the requested @a traversal_order.
      const_iterator begin (const ::std::string &traversal_order) const;

      /// Get a const iterator pointing to the "end" of the expression
      /// tree relative to the requested @a traversal_order.
      const_iterator end (const ::std::string &traversal_order) const;

      /// Accept a visitor to perform some action on the Expression_Tree.
      void accept (Visitor &visitor) const;

    private:
      /// Pointer to actual implementation, i.e., the "bridge", which is
      /// reference counted to automate memory management. 
      Refcounter <Component_Node> root_;
    };

  }
}

#endif /* _EXPRESSION_TREE_H_ */
