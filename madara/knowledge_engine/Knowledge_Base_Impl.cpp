#include "madara/utility/Utility.h"
#include "madara/knowledge_engine/Knowledge_Base_Impl.h"
#include "madara/expression_tree/Interpreter.h"
#include "madara/expression_tree/Expression_Tree.h"
#include "madara/transport/TCP_Transport.h"
#include "ace/Log_Msg.h"

#include <sstream>


#ifdef _USE_OPEN_SPLICE_
  #include "madara/transport/Splice_DDS_Transport.h"
#endif // _USE_OPEN_SPLICE_

#include <iostream>

Madara::Knowledge_Engine::Knowledge_Base_Impl::Knowledge_Base_Impl ()
: transport_ (0), transport_type_ (0), files_ (map_)
{
  //setup_splitters ();
  activate_transport ();
  // no hope of transporting, so don't setup uniquehostport
}

Madara::Knowledge_Engine::Knowledge_Base_Impl::Knowledge_Base_Impl (
  const std::string & host, int transport)
: transport_type_ (transport), files_ (map_)
{
  setup_uniquehostport (host);
//  setup_splitters ();    // only used for debugging
  activate_transport ();
}

Madara::Knowledge_Engine::Knowledge_Base_Impl::Knowledge_Base_Impl (
  const std::string & host, int transport,
  const std::string & knowledge_domain)
: transport_type_ (transport), domain_name_ (knowledge_domain), files_ (map_)
{
  setup_uniquehostport (host);
//  setup_splitters ();   // only used for debugging
  activate_transport ();
}

Madara::Knowledge_Engine::Knowledge_Base_Impl::~Knowledge_Base_Impl ()
{
  close_transport ();
  unique_bind_.close ();
}

void
Madara::Knowledge_Engine::Knowledge_Base_Impl::setup_uniquehostport (
  const std::string & host)
{
  // start from 50k, which is just above the bottom of the user
  // definable port range (hopefully avoid conflicts with 49152-49999
  unsigned short port =  50000;

  if (Madara::Utility::bind_to_ephemeral_port (unique_bind_, port) == -1)
  {
    ACE_DEBUG ((LM_DEBUG, 
      "MADARA ERROR: Unable to bind to any ephemeral port\n"));
    return;
  }
 
  // we were able to bind to an ephemeral port
  Madara::Utility::merge_hostport_identifier (id_, host, port);

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Unique bind to %s\n", id_.c_str ()));

}

void
Madara::Knowledge_Engine::Knowledge_Base_Impl::setup_splitters (void)
{
  //statement_splitters_.push_back (";");

  //implies_splitters_.push_back ("=>");
  //implies_splitters_.push_back ("->");

  //assignment_splitters_.push_back ("=");

  //conditional_splitters_.push_back ("&&");
  //conditional_splitters_.push_back ("||");

  //comparison_splitters_.push_back ("==");
  //comparison_splitters_.push_back ("!=");
  //comparison_splitters_.push_back (">=");
  //comparison_splitters_.push_back ("<=");
  //comparison_splitters_.push_back ("<");
  //comparison_splitters_.push_back (">");
}

void
Madara::Knowledge_Engine::Knowledge_Base_Impl::activate_transport (void)
{
  if (transport_type_ == OPEN_SPLICE_TRANSPORT)
  {
  #ifdef _USE_OPEN_SPLICE_
    transport_ = new Madara::Transport::Splice_DDS_Transport (id_, map_,
    Madara::Transport::Splice_DDS_Transport::RELIABLE, true, domain_name_);
  #endif
  }
  else if (transport_type_ == TCP_TRANSPORT)
  {
    transport_ = new Madara::Transport::TCP_Transport (id_, map_,
      Madara::Transport::TCP_Transport::RELIABLE);
  }
  else
  {
    transport_ = 0;
  }
}

void
Madara::Knowledge_Engine::Knowledge_Base_Impl::close_transport (void)
{
  if (transport_)
  {
    transport_->close ();
    delete transport_;
    transport_ = 0;
  }
}


int
Madara::Knowledge_Engine::Knowledge_Base_Impl::set (const ::std::string & key, 
                                               long value, bool send_modifieds)
{
  // everything after this point is done on a string with at least 1 char
  int result = map_.set (key, value);

  // only send an update if we have a transport, we have been asked to send
  // modifieds, and this is NOT a local key
  if (result == 0 && transport_ && send_modifieds && 
      key.size () > 0 && key[0] != '.')
  {
    map_.lock ();

    /// generate a new clock time and set our variable's clock to
    /// this new clock
    long cur_clock = map_.inc_clock ();
    map_.set_clock (key, cur_clock);

    /// now send the data and reset the modified state
    transport_->send_data (key, value);
    map_.reset_modified (key);

    /// unlock the knowledge map
    map_.unlock ();
  }

  return result;
}


int
Madara::Knowledge_Engine::Knowledge_Base_Impl::wait (const ::std::string & expression, 
                                                bool send_modifieds)
{
  // lock the context
  map_.lock ();

  // resulting tree of the expression
  Madara::Expression_Tree::Expression_Tree tree = interpreter_.interpret (
    map_, expression);

  int last_value = tree.evaluate ();

  if (transport_ && send_modifieds)
  {
    Madara::String_Vector modified;
    map_.get_modified (modified);
    std::stringstream string_builder;
    if (modified.size () > 0)
    {
      /// generate a new clock time and set our variable's clock to
      /// this new clock
      long cur_clock = map_.inc_clock ();
      long quality = 0;

      for (Madara::String_Vector::const_iterator k = modified.begin ();
             k != modified.end (); ++k)
      {
        map_.set_clock (*k, cur_clock);
        long cur_quality = map_.get_write_quality (*k);

        // every knowledge update via multiassignment has the quality
        // of the highest update. This is to ensure consistency for
        // updating while also providing quality indicators for sensors,
        // actuators, controllers, etc.
        if (cur_quality > quality)
          quality = cur_quality;

        string_builder << *k << " = " << map_.get (*k) << " ; ";
          //transport_->send_data (*k, map_.get (*k));
      }

      transport_->send_multiassignment (string_builder.str (), quality);
      map_.reset_modified ();
    }
  }

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

    if (transport_ && send_modifieds)
    {
      Madara::String_Vector modified;
      map_.get_modified (modified);
      std::stringstream string_builder;
      if (modified.size () > 0)
      {
        /// generate a new clock time and set our variable's clock to
        /// this new clock
        long cur_clock = map_.inc_clock ();

        for (Madara::String_Vector::const_iterator k = modified.begin ();
               k != modified.end (); ++k)
        {
          map_.set_clock (*k, cur_clock);
          string_builder << *k << " = " << map_.get (*k) << " ; ";
          //transport_->send_data (*k, map_.get (*k));
        }

        transport_->send_multiassignment (string_builder.str ());
        map_.reset_modified ();
      }
    }

    map_.signal ();
  }

  // release the context lock
  map_.unlock ();
  return 0;
}

void
Madara::Knowledge_Engine::Knowledge_Base_Impl::add_rule (const ::std::string & expression_copy)
{
  ::std::string expression (expression_copy);
  Madara::Utility::strip_white_space (expression);

  evaluate (expression);

  rules_.push_back (expression);
}

int
Madara::Knowledge_Engine::Knowledge_Base_Impl::evaluate (
  const ::std::string & expression, bool send_modifieds)
{
  int last_value = 0;
  // strip the incoming expression of white spaces
  //::std::string expression (expression_copy);
  //Madara::Utility::strip_white_space (expression);

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Evaluting %s\n", expression.c_str ()));

  // iterators and tree for evaluation of interpreter results
  Madara::Expression_Tree::Expression_Tree tree;

  // lock the context from being updated by any ongoing threads
  map_.lock ();

  // interpret the current expression and then evaluate it
  tree = interpreter_.interpret (map_, expression);
  last_value = tree.evaluate ();

  // if we have a transport and we've been asked to send modified knowledge
  // to any interested parties...
  if (transport_ && send_modifieds)
  {
    Madara::String_Vector modified;
    map_.get_modified (modified);
    std::stringstream string_builder;

    /// generate a new clock time and set our variable's clock to
    /// this new clock
    long cur_clock = map_.inc_clock ();

    for (Madara::String_Vector::const_iterator k = modified.begin ();
         k != modified.end (); ++k)
    {
      map_.set_clock (*k, cur_clock);
      string_builder << *k << " = " << map_.get (*k) << " ; ";
    }

    if (modified.size () > 0)
      transport_->send_multiassignment (string_builder.str ());
    map_.reset_modified ();
  }

  map_.unlock ();

  return last_value;
}

void
Madara::Knowledge_Engine::Knowledge_Base_Impl::print_rules (void) const
{
  ACE_TRACE (ACE_TEXT ("print_rules"));
  
  ::std::cout << "\nRules in Knowledge Base:\n";
  for (Knowledge_Rules::const_iterator i = rules_.begin ();
       i != rules_.end (); ++i)
  {
    ACE_DEBUG ((LM_INFO, "(%P|%t) %s\n", (*i).c_str ()));
  }
}

void
Madara::Knowledge_Engine::Knowledge_Base_Impl::test(const long & iterations)
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
    transport_->send_data (keys[i % 4], i);
  }
}
