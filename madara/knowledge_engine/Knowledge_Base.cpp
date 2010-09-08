#include "madara/utility/Utility.h"
#include "madara/knowledge_engine/Knowledge_Base.h"
#include "madara/expression_tree/Interpreter.h"
#include "madara/expression_tree/Evaluation_Visitor.h"
#include "madara/expression_tree/Print_Visitor.h"
#include "madara/expression_tree/Expression_Tree.h"
#include "madara/expression_tree/Iterator.h"

#ifdef _USE_OPEN_SPLICE_
  #include "madara/transport/Splice_DDS_Transport.h"
#endif // _USE_OPEN_SPLICE_

#include <iostream>

Madara::Knowledge_Engine::Knowledge_Base::Knowledge_Base ()
: transport_ (0)
{
  statement_splitters_.push_back (";");

  implies_splitters_.push_back ("=>");
  implies_splitters_.push_back ("->");

  assignment_splitters_.push_back ("=");

  conditional_splitters_.push_back ("&&");
  conditional_splitters_.push_back ("||");

  comparison_splitters_.push_back ("==");
  comparison_splitters_.push_back ("!=");
  comparison_splitters_.push_back (">=");
  comparison_splitters_.push_back ("<=");
  comparison_splitters_.push_back ("<");
  comparison_splitters_.push_back (">");

#ifdef _USE_OPEN_SPLICE_
  transport_ = new Madara::Transport::Splice_DDS_Transport (map_,
    Madara::Transport::Splice_DDS_Transport::RELIABLE);
#endif

}

Madara::Knowledge_Engine::Knowledge_Base::~Knowledge_Base ()
{
  if (transport_)
  {
    transport_->close ();
    delete transport_;
    transport_ = 0;
  }
}

int
Madara::Knowledge_Engine::Knowledge_Base::get (const ::std::string & key) const
{
  return map_.get (key);
}

void
Madara::Knowledge_Engine::Knowledge_Base::set (const ::std::string & key, int value)
{
  map_.set (key, value);

  // only try to send this update if the key is valid, is not local ('.var') and
  // the transport is valid
  if (key.length () > 0 && key[0] != '.' && transport_)
  {
    transport_->send (key, value);
  }
}

int
Madara::Knowledge_Engine::Knowledge_Base::wait (const ::std::string & expression)
{
  // lock the context
  map_.lock ();

  // Interpreter and resulting tree of the expression
  Madara::Expression_Tree::Interpreter interpreter; 
  Madara::Expression_Tree::Expression_Tree tree = interpreter.interpret (
    map_, expression);

  // optimize the tree
  int last_value = tree.prune ();

  // wait for expression to be true
  while (!last_value)
  {
    // we need the context to do an additional release. If release
    // the context lock, we may have an update event happen
    // that we cannot be signalled on - which could lead to
    // permanent, unnecessary deadlock
    map_.wait_for_change (true);

    // relock - basically we need to evaluate the tree again, and
    // we can't have a bunch of people changing the variables as 
    // while we're evaluating the tree.
    map_.lock ();
    last_value = tree.evaluate ();
    map_.signal ();
  }

  // release the context lock
  map_.unlock ();
  return 0;
}

void
Madara::Knowledge_Engine::Knowledge_Base::add_rule (const ::std::string & expression_copy)
{
  ::std::string expression (expression_copy);
  Madara::Utility::strip_white_space (expression);

  evaluate (expression);

  rules_.push_back (expression);
}

int
Madara::Knowledge_Engine::Knowledge_Base::evaluate (
  const ::std::string & expression_copy)
{
  return evaluate (expression_copy, true);
}

int
Madara::Knowledge_Engine::Knowledge_Base::evaluate (
  const ::std::string & expression_copy, bool send_modifieds)
{
  int last_value = 0;
  // strip the incoming expression of white spaces
  ::std::string expression (expression_copy);
  Madara::Utility::strip_white_space (expression);

  // Interpreter and visitors
  Madara::Expression_Tree::Interpreter interpreter;
  //Madara::Expression_Tree::Evaluation_Visitor eval_visitor;
  Madara::Expression_Tree::Print_Visitor print_visitor;

  // Statements and pivots resulting from splitting the ";"
  ::std::vector <::std::string> statements;
  ::std::vector <::std::string> statements_pivots;

  // Split the expression according to ";"
  Madara::Utility::tokenizer (expression, this->statement_splitters_, 
                      statements, statements_pivots);

  map_.lock ();

  // For each resulting statement, evaluate
  for (::std::vector<::std::string>::size_type i = 0; i < statements.size (); ++i)
  {
    ACE_DEBUG ((LM_DEBUG, "Evaluating %s\n", statements[i].c_str ()));

    // expressions are separated by implications
    ::std::vector <::std::string> expressions;
    ::std::vector <::std::string> implications;

    // iterators and tree for evaluation of interpreter results
    Madara::Expression_Tree::Expression_Tree tree;

    // split the current statement according to implications
    Madara::Utility::tokenizer (statements[i], this->implies_splitters_, 
                      expressions, implications);

    for (::std::vector<::std::string>::size_type j = 0; j == 0 || 
            (last_value != 0 && j < expressions.size ()); ++j)
    {
      // interpret the current expression
      tree = interpreter.interpret (map_, expressions[j]);

      // optimize the tree
      tree.prune ();
      last_value = tree.evaluate ();

      // reset the eval_visitor
      //eval_visitor.reset ();

 
      //ACE_DEBUG ((LM_DEBUG, "\nPrinting the resulting tree:\n"));     
      //for (Madara::Expression_Tree::Expression_Tree::iterator iter = tree.begin ("in-order");
      //  iter != tree.end ("in-order"); ++iter)
      //  (*iter).accept (print_visitor);
      //ACE_DEBUG ((LM_DEBUG, "\n"));  

      // iterate over the resulting tree
      //for (Madara::Expression_Tree::Expression_Tree::iterator iter = tree.begin ("post-order");
      //  iter != tree.end ("post-order"); ++iter)
      //  (*iter).accept (eval_visitor);

    }
  }

  if (transport_ && send_modifieds)
  {
    Madara::String_Vector modified;
    map_.get_modified (modified);

    for (Madara::String_Vector::const_iterator k = modified.begin ();
         k != modified.end (); ++k)
    {
      transport_->send (*k, map_.get (*k));
    }
    map_.reset_modified ();
  }

  map_.unlock ();

  return last_value;
}

void
Madara::Knowledge_Engine::Knowledge_Base::print_rules (void) const
{
  ACE_TRACE (ACE_TEXT ("print_rules"));
  
  ::std::cout << "\nRules in Knowledge Base:\n";
  for (Knowledge_Rules::const_iterator i = rules_.begin ();
       i != rules_.end (); ++i)
  {
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s\n", (*i).c_str ()));
  }
}

void
Madara::Knowledge_Engine::Knowledge_Base::print_knowledge (void) const
{
  ACE_DEBUG ((LM_DEBUG, "\nKnowledge in Knowledge Base:\n"));
  map_.print ();
}

void
Madara::Knowledge_Engine::Knowledge_Base::clear (void)
{
  map_.clear ();
  rules_.clear ();
}

void
Madara::Knowledge_Engine::Knowledge_Base::test(const long & iterations)
{
  if (!transport_)
    return;

  char * keys [] = {
    "bob",
    "bill",
    "jack",
    "thomas" };

  for (long i = 0; i < iterations; ++i)
  {
    transport_->send (keys[i % 4], i);
  }
}

void
Madara::Knowledge_Engine::Knowledge_Base::clear_rules (void)
{
  rules_.clear ();
}

void
Madara::Knowledge_Engine::Knowledge_Base::clear_map (void)
{
  map_.clear ();
}
