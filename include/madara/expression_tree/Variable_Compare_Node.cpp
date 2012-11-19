#include "madara/expression_tree/Visitor.h"
#include "madara/expression_tree/Variable_Compare_Node.h"
#include "madara/utility/Utility.h"

#include "madara/utility/Log_Macros.h"
#include <string>
#include <sstream>

Madara::Expression_Tree::Variable_Compare_Node::Variable_Compare_Node (
  const std::string &key, Madara::Knowledge_Record value, int compare_type,
  Component_Node * rhs, Madara::Knowledge_Engine::Thread_Safe_Context &context)
: key_ (key), value_ (value), rhs_ (rhs), compare_type_ (compare_type),
  record_ (0), context_ (context), key_expansion_necessary_ (false)
{
  // this key requires expansion. We do the compilation and error checking here
  // as the key shouldn't change, and this allows us to only have to do this
  // once
  if (key.find ("{") != key.npos)
  {
    unsigned int count = 1;
    key_expansion_necessary_ = true;
    splitters_.push_back ("{");
    splitters_.push_back ("}");

    Utility::tokenizer (key, splitters_, tokens_, pivot_list_);

    if (pivot_list_.size () % 2 != 0)
    {
      MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
        "\nKARL COMPILE ERROR: matching braces not found in %s\n",
        key.c_str ()));
      exit (-1);
    }

    // check for braces that are not properly closed
    std::vector< std::string>::const_iterator pivot = pivot_list_.begin ();

    for (; count < pivot_list_.size () && pivot != pivot_list_.end (); 
           pivot+=2, count+=2)
    {
      if (pivot_list_[count-1] != "{" || pivot_list_[count] != "}")
      {
        MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
          "\nKARL COMPILE ERROR: matching braces not found in %s\n",
          key.c_str ()));
        exit (-1);
      }
    }
  }
  // no variable expansion necessary. Create a hard link to the record_->
  // this will save us lots of clock cycles each variable access or
  // mutation.
  else
  {
    record_ = context_.get_record (key);
  }
}

Madara::Expression_Tree::Variable_Compare_Node::~Variable_Compare_Node ()
{
  // do not clean up record_. Let the context clean that up.
}

std::string
Madara::Expression_Tree::Variable_Compare_Node::expand_key (void) const
{
  if (key_expansion_necessary_)
  {
    unsigned int count = 0;

    // add the first token into a string builder
    std::stringstream builder;
    std::vector< std::string>::const_iterator token = tokens_.begin ();
    builder << *token;

    // move the token to the next in the list.
    for (++token, ++count; token != tokens_.end (); ++token, ++count)
    {
      if (*token != "")
      {
        // is the current token a variable to lookup?
        if (count < pivot_list_.size () 
          && pivot_list_[count] == "}")
        {
          builder << context_.get (*token);
        }
        else
        {
          builder << *token;
        }
      }
    }

    return builder.str ();
  }
  // if there was no need to expand the key, just return
  // the key
  else
    return key_;
}


void
Madara::Expression_Tree::Variable_Compare_Node::accept (Visitor &visitor) const
{
  visitor.visit (*this);
}

Madara::Knowledge_Record
Madara::Expression_Tree::Variable_Compare_Node::item () const
{
  if (record_)
    return *record_;
  else
    return context_.get (expand_key ());
}

/// Prune the tree of unnecessary nodes. 
/// Returns evaluation of the node and sets can_change appropriately.
/// if this node can be changed, that means it shouldn't be pruned.
Madara::Knowledge_Record
Madara::Expression_Tree::Variable_Compare_Node::prune (bool & can_change)
{
  // a variable is one of very few nodes that can change over time and
  // cannot be pruned
  can_change = true;

  // we could call item(), but since it is virtual, it incurs unnecessary
  // overhead.
  if (record_)
    return *record_;
  else
    return context_.get (expand_key ());
}

/// Evaluates the node and its children. This does not prune any of
/// the expression tree, and is much faster than the prune function
Madara::Knowledge_Record 
Madara::Expression_Tree::Variable_Compare_Node::evaluate (void)
{
  Madara::Knowledge_Record * lhs = record_;

  // if we don't have a static record, get the dynamic one
  if (!lhs)
    lhs = context_.get_record (expand_key ());

  if (rhs_)
  {
    if      (compare_type_ == LESS_THAN)
      return *lhs < rhs_->evaluate ();
    else if (compare_type_ == LESS_THAN_EQUAL)
      return *lhs <= rhs_->evaluate ();
    else if (compare_type_ == EQUAL)
      return *lhs == rhs_->evaluate ();
    else if (compare_type_ == GREATER_THAN_EQUAL)
      return *lhs >= rhs_->evaluate ();
    else
      return *lhs > rhs_->evaluate ();
  }
  else
  {
    if      (compare_type_ == LESS_THAN)
      return *lhs < value_;
    else if (compare_type_ == LESS_THAN_EQUAL)
      return *lhs <= value_;
    else if (compare_type_ == EQUAL)
      return *lhs == value_;
    else if (compare_type_ == GREATER_THAN_EQUAL)
      return *lhs >= value_;
    else
      return *lhs > value_;
  }
}

const std::string &
Madara::Expression_Tree::Variable_Compare_Node::key () const
{
  return key_;
}

