#include "madara/utility/Utility.h"
#include "madara/knowledge_engine/Knowledge_Base_Impl.h"
#include "madara/expression_tree/Interpreter.h"
#include "madara/expression_tree/Expression_Tree.h"
#include "madara/transport/udp/UDP_Transport.h"
#include "madara/transport/tcp/TCP_Transport.h"
#include "madara/transport/multicast/Multicast_Transport.h"
#include "madara/transport/broadcast/Broadcast_Transport.h"
#include "madara/utility/Log_Macros.h"

#include <sstream>

#include "ace/OS_NS_Thread.h"
#include "ace/High_Res_Timer.h"

#ifdef _USE_OPEN_SPLICE_
  #include "madara/transport/splice/Splice_DDS_Transport.h"
  #include "madara/transport/splice/Inconsistent_Transport.h"
#endif // _USE_OPEN_SPLICE_

#ifdef _USE_NDDS_
  #include "madara/transport/ndds/NDDS_Transport.h"
#endif // _USE_NDDS_

#include <iostream>

Madara::Knowledge_Engine::Knowledge_Base_Impl::Knowledge_Base_Impl ()
: settings_ (), files_ (map_), transport_ (0)
{
  activate_transport ();
  // no hope of transporting, so don't setup uniquehostport
}

Madara::Knowledge_Engine::Knowledge_Base_Impl::Knowledge_Base_Impl (
  const std::string & host, int transport)
: settings_ (), files_ (map_), transport_ (0)
{
  // override default settings for the arguments
  settings_.type = transport;

  setup_uniquehostport (host);
  activate_transport ();
}

Madara::Knowledge_Engine::Knowledge_Base_Impl::Knowledge_Base_Impl (
  const std::string & host, int transport,
  const std::string & knowledge_domain)
: settings_ (), files_ (map_), transport_ (0)
{
  // override default settings for the arguments
  settings_.type = transport;
  settings_.domains = knowledge_domain;

  setup_uniquehostport (host);
  activate_transport ();
}

Madara::Knowledge_Engine::Knowledge_Base_Impl::Knowledge_Base_Impl (
  const std::string & host, const Madara::Transport::Settings & config)
: settings_ (config), files_ (map_), transport_ (0)
{
  setup_uniquehostport (host);
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
  // placeholder for our ip address
  std::string actual_host;

  if (settings_.type != Madara::Transport::NO_TRANSPORT)
  {
    // start from 50k, which is just above the bottom of the user
    // definable port range (hopefully avoid conflicts with 49152-49999
    unsigned short port =  50000;

    if (Madara::Utility::bind_to_ephemeral_port (unique_bind_, actual_host, port)
         == -1)
    {
      MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, 
        DLINFO "Knowledge_Base_Impl::setup_uniquehostport:" \
        " unable to bind to any ephemeral port." \
        " Check firewall.\n"));
      exit (-1);
    }
   
    // if the user doesn't want us using the actual host, trust them with the
    // provided host
    if (host != "")
      actual_host = host;

    // we were able to bind to an ephemeral port
    Madara::Utility::merge_hostport_identifier (id_, actual_host, port);

    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Knowledge_Base_Impl::setup_uniquehostport:" \
      " unique bind to %s\n", id_.c_str ()));
  }
}

void
Madara::Knowledge_Engine::Knowledge_Base_Impl::activate_transport (void)
{
  if (!transport_)
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Knowledge_Base_Impl::activate_transport:" \
      " activating transport type %d\n", settings_.type));
    if (settings_.type == Madara::Transport::BROADCAST)
    {
      transport_ = new Madara::Transport::Broadcast_Transport (id_, map_,
        settings_, true);
    }
    else if (settings_.type == Madara::Transport::MULTICAST)
    {
      transport_ = new Madara::Transport::Multicast_Transport (id_, map_,
        settings_, true);
    }
    else if (settings_.type == Madara::Transport::SPLICE)
    {
    #ifdef _USE_OPEN_SPLICE_
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Knowledge_Base_Impl::activate_transport:" \
        " creating Open Splice DDS transport.\n",
        settings_.type));

      transport_ = new Madara::Transport::Splice_DDS_Transport (id_, map_,
                         settings_, true);
    #else
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Knowledge_Base_Impl::activate_transport:" \
        " project was not generated with opensplice=1. Transport is invalid.\n",
        settings_.type));

      exit (-2);
    #endif
    }
    else if (settings_.type == Madara::Transport::NDDS)
    {
    #ifdef _USE_NDDS_
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Knowledge_Base_Impl::activate_transport:" \
        " creating NDDS transport.\n",
        settings_.type));

      transport_ = new Madara::Transport::NDDS_Transport (id_, map_,
                         settings_, true);
    #else
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Knowledge_Base_Impl::activate_transport:" \
        " project was not generated with ndds=1. Transport is invalid.\n",
        settings_.type));

      exit (-2);
    #endif
    }
    else if (settings_.type == Madara::Transport::UDP)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Knowledge_Base_Impl::activate_transport:" \
        " creating UDP transport.\n",
        settings_.type));

      transport_ = new Madara::Transport::UDP_Transport (id_, map_,
        settings_, true);
    }
    else if (settings_.type == Madara::Transport::TCP)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Knowledge_Base_Impl::activate_transport:" \
        " creating TCP transport.\n",
        settings_.type));

      transport_ = new Madara::Transport::TCP_Transport (id_, map_,
        settings_, true);
    }
    else if (settings_.type == Madara::Transport::INCONSISTENT)
    {
    #ifdef _USE_OPEN_SPLICE_
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Knowledge_Base_Impl::activate_transport:" \
        " creating Inconsistent transport.\n",
        settings_.type));

      transport_ = new Madara::Transport::Inconsistent_Transport (id_, map_,
                         settings_, true);
    #else
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Knowledge_Base_Impl::activate_transport:" \
        " project was not generated with opensplice=1. Transport is invalid.\n",
        settings_.type));

      exit (-2);
    #endif
    }
    else
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Knowledge_Base_Impl::activate_transport:" \
        " no transport was specified. Setting transport to null.\n",
        settings_.type));

      transport_ = 0;
    }
  }
  else
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Knowledge_Base_Impl::activate_transport:" \
      " transport already activated. If you need" \
      " a new type, close transport first\n"));
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
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Knowledge_Base_Impl::close_transport:" \
      " transport has been closed.\n"));
  }
}

/**
 * Updates all global variables to current clock and then
 * sends them if a transport is available. This is useful
 * when trying to synchronize to late joiners (this process
 * will resend all global variables.
 **/
int
Madara::Knowledge_Engine::Knowledge_Base_Impl::apply_modified (void)
{
  // lock the context and apply modified flags and current clock to
  // all global variables
  map_.lock ();
  map_.apply_modified ();

  int ret = 0;

  if (transport_)
  {
    const Madara::Knowledge_Records & modified = map_.get_modified ();

    if (modified.size () > 0)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_DEBUG_INFO, (LM_DEBUG,
        DLINFO "Knowledge_Base_Impl::apply_modified:" \
        " sending %d updates\n", 
        modified.size ()));

      transport_->send_data (modified);
      map_.reset_modified ();
    }
  }
  else
  {
    ret = -1;
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Knowledge_Base_Impl::apply_modified:" \
      " not sending changes to knowledge.\n"));
  }

  map_.unlock ();

  return ret;
}

int
Madara::Knowledge_Engine::Knowledge_Base_Impl::set (
  const std::string & t_key, 
  Madara::Knowledge_Record::Integer value,
  const Eval_Settings & settings)
{
  // everything after this point is done on a string with at least 1 char
  std::string key = map_.expand_statement (t_key);

  if (key == "")
    return -1;

  int result = map_.set (key, value, settings);

  // only send an update if we have a transport, we have been asked to send
  // modifieds, and this is NOT a local key

  if (transport_ && !settings.delay_sending_modifieds)
  {
    const Madara::Knowledge_Records & modified = map_.get_modified ();

    if (modified.size () > 0)
    {
      transport_->send_data (modified);
      map_.reset_modified ();
    }
    else
    {
      MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
          DLINFO "Knowledge_Base_Impl::set:" \
          " no modifications to send during this set\n"));
    }
  }
  else
  {
    MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
        DLINFO "Knowledge_Base_Impl::set:" \
        " not sending knowledge mutations \n"));
  }


  return result;
}

int
Madara::Knowledge_Engine::Knowledge_Base_Impl::set (
  const std::string & t_key, 
  double value, 
  const Eval_Settings & settings)
{
  // everything after this point is done on a string with at least 1 char
  std::string key = map_.expand_statement (t_key);

  if (key == "")
    return -1;

  int result = map_.set (key, value, settings);

  // only send an update if we have a transport, we have been asked to send
  // modifieds, and this is NOT a local key
  
  if (transport_ && !settings.delay_sending_modifieds)
  {
    const Madara::Knowledge_Records & modified = map_.get_modified ();

    if (modified.size () > 0)
    {
      transport_->send_data (modified);
      map_.reset_modified ();
    }
    else
    {
      MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
          DLINFO "Knowledge_Base_Impl::set:" \
          " no modifications to send during this set\n"));
    }
  }
  else
  {
    MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
        DLINFO "Knowledge_Base_Impl::set:" \
        " not sending knowledge mutations \n"));
  }

  return result;
}

int
Madara::Knowledge_Engine::Knowledge_Base_Impl::set (
  const std::string & t_key, 
  const std::string & value, 
  const Eval_Settings & settings)
{
  // everything after this point is done on a string with at least 1 char
  std::string key = map_.expand_statement (t_key);

  if (key == "")
    return -1;

  int result = map_.set (key, value, settings);

  // only send an update if we have a transport, we have been asked to send
  // modifieds, and this is NOT a local key
  
  if (transport_ && !settings.delay_sending_modifieds)
  {
    const Madara::Knowledge_Records & modified = map_.get_modified ();

    if (modified.size () > 0)
    {
      transport_->send_data (modified);
      map_.reset_modified ();
    }
    else
    {
      MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
          DLINFO "Knowledge_Base_Impl::set:" \
          " no modifications to send during this set\n"));
    }
  }
  else
  {
    MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
        DLINFO "Knowledge_Base_Impl::set:" \
        " not sending knowledge mutations \n"));
  }


  return result;
}

Madara::Knowledge_Engine::Compiled_Expression
Madara::Knowledge_Engine::Knowledge_Base_Impl::compile (
  const std::string & expression)
{
  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Knowledge_Base_Impl::compile:" \
      " compiling %s\n", expression.c_str ()));

  Compiled_Expression ce;
  ce.logic = expression;
  ce.expression = interpreter_.interpret (map_, expression);

  return ce;
}

Madara::Knowledge_Record
Madara::Knowledge_Engine::Knowledge_Base_Impl::wait (const std::string & expression,
        const Wait_Settings & settings)
{
  Compiled_Expression compiled = compile (expression);
  return wait (compiled, settings);
}

Madara::Knowledge_Record
Madara::Knowledge_Engine::Knowledge_Base_Impl::wait (
  Compiled_Expression & ce, 
  const Wait_Settings & settings)
{
  ACE_High_Res_Timer timer;
  ACE_hrtime_t elapsed (0);
  ACE_hrtime_t maximum (0);
  timer.start ();

  // print the post statement at highest log level (cannot be masked)
  if (settings.pre_print_statement != "")
    map_.print (settings.pre_print_statement, MADARA_LOG_EMERGENCY);

  // lock the context
  map_.lock ();

  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Knowledge_Base_Impl::wait:" \
      " waiting on %s\n", ce.logic.c_str ()));

  // resulting tree of the expression
  //Madara::Expression_Tree::Expression_Tree tree = interpreter_.interpret (
  //  map_, expression);

  Madara::Knowledge_Record last_value = ce.expression.evaluate (settings);

  MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
      DLINFO "Knowledge_Base_Impl::wait:" \
      " completed first eval to get %s\n",
    last_value.to_string ().c_str ()));

  if (transport_ && !settings.delay_sending_modifieds)
  {
    const Madara::Knowledge_Records & modified = map_.get_modified ();

    if (modified.size () > 0)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_DEBUG_INFO, (LM_DEBUG,
        DLINFO "Knowledge_Base_Impl::wait:" \
        " sending %d updates\n", 
        modified.size ()));

      transport_->send_data (modified);
      map_.reset_modified ();
    }
    else
    {
      MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
          DLINFO "Knowledge_Base_Impl::wait:" \
          " no modifications to send during this wait\n"));
    }
  }
  else
  {
    MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
        DLINFO "Knowledge_Base_Impl::wait:" \
        " not sending knowledge mutations \n"));
  }

  map_.unlock ();

  ACE_Time_Value poll_frequency;

  if (settings.max_wait_time > 0 && last_value.is_false ())
  {
    ACE_Time_Value max_tv;
    poll_frequency.set (settings.poll_frequency);

    max_tv.set (settings.max_wait_time);
    maximum = max_tv.sec () * 1000000000;
    maximum += max_tv.usec () * 1000;

    timer.stop ();
    timer.elapsed_time (elapsed);

  }

  // wait for expression to be true
  while (!last_value.to_integer () &&
    (settings.max_wait_time < 0 || maximum > elapsed))
  {
    MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
        DLINFO "Knowledge_Base_Impl::wait:" \
        " elapsed is %Q and max is %Q (poll freq is %f)\n",
        elapsed, maximum, settings.poll_frequency));

    MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
        DLINFO "Knowledge_Base_Impl::wait:" \
        " last value didn't result in success\n"));

    // Unlike the other wait statements, we allow for a time based wait.
    // To do this, we allow a user to specify a 
    if (settings.max_wait_time > 0)
      ACE_OS::sleep (poll_frequency);
    else
      map_.wait_for_change (true);

    // relock - basically we need to evaluate the tree again, and
    // we can't have a bunch of people changing the variables as 
    // while we're evaluating the tree.
    map_.lock ();
    last_value = ce.expression.evaluate (settings);

    if (transport_ && !settings.delay_sending_modifieds)
    {
      const Madara::Knowledge_Records & modified = map_.get_modified ();

      if (modified.size () > 0)
      {
        MADARA_DEBUG (MADARA_LOG_MAJOR_DEBUG_INFO, (LM_DEBUG,
          DLINFO "Knowledge_Base_Impl::wait:" \
          " sending %d updates\n", 
          modified.size ()));

        transport_->send_data (modified);
        map_.reset_modified ();
      }
      else
      {
        MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
            DLINFO "Knowledge_Base_Impl::wait:" \
            " no modifications to send during this wait\n"));
      }
    }
    else
    {
      MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
          DLINFO "Knowledge_Base_Impl::wait:" \
          " no modifications to send during this wait\n"));
    }
    
    map_.unlock ();
    map_.signal ();
    timer.stop ();
    timer.elapsed_time (elapsed);
  } // end while (!last)

  // print the post statement at highest log level (cannot be masked)
  if (settings.post_print_statement != "")
    map_.print (settings.post_print_statement, MADARA_LOG_EMERGENCY);

  // release the context lock
  //map_.unlock ();
  return last_value;
}

void
Madara::Knowledge_Engine::Knowledge_Base_Impl::add_rule (const std::string & expression_copy)
{
  std::string expression (expression_copy);
  Madara::Utility::strip_white_space (expression);

  evaluate (expression);

  rules_.push_back (expression);
}

Madara::Knowledge_Record
Madara::Knowledge_Engine::Knowledge_Base_Impl::evaluate (
  const std::string & expression,
  const Eval_Settings & settings)
{
  Compiled_Expression compiled = compile (expression);
  return evaluate (compiled, settings);
}

Madara::Knowledge_Record
Madara::Knowledge_Engine::Knowledge_Base_Impl::evaluate (
  Compiled_Expression & ce,
  const Eval_Settings & settings)
{
  Madara::Knowledge_Record last_value;

  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Knowledge_Base_Impl::evaluate:" \
        " evaluating %s.\n", ce.logic.c_str ()));

  // iterators and tree for evaluation of interpreter results
  //Madara::Expression_Tree::Expression_Tree tree;

  // print the post statement at highest log level (cannot be masked)
  if (settings.pre_print_statement != "")
    map_.print (settings.pre_print_statement, MADARA_LOG_EMERGENCY);

  // lock the context from being updated by any ongoing threads
  map_.lock ();

  // interpret the current expression and then evaluate it
  //tree = interpreter_.interpret (map_, expression);
  last_value = ce.expression.evaluate (settings);

  // if we have a transport and we've been asked to send modified knowledge
  // to any interested parties...
  if (transport_ && !settings.delay_sending_modifieds)
  {
    const Madara::Knowledge_Records & modified = map_.get_modified ();

    if (modified.size () > 0)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_DEBUG_INFO, (LM_DEBUG,
        DLINFO "Knowledge_Base_Impl::evaluate:" \
        " sending %d updates\n", 
        modified.size ()));

      transport_->send_data (modified);
      map_.reset_modified ();
    }
  }

  // print the post statement at highest log level (cannot be masked)
  if (settings.post_print_statement != "")
    map_.print (settings.post_print_statement, MADARA_LOG_EMERGENCY);

  map_.unlock ();

  return last_value;
}

void
Madara::Knowledge_Engine::Knowledge_Base_Impl::print_rules (
  unsigned int level) const
{
  MADARA_TRACE (ACE_TEXT ("Knowledge_Base_Impl::print_rules"));
  
  MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_INFO,
        "Rules in Knowledge Base:\n"));

  for (Knowledge_Rules::const_iterator i = rules_.begin ();
       i != rules_.end (); ++i)
  {
    MADARA_DEBUG (level, (LM_INFO,
        "%s\n", (*i).c_str ()));
  }
}

// Defines a function
void Madara::Knowledge_Engine::Knowledge_Base_Impl::define_function (
  const std::string & name, VALUE_TYPE (*func) (Function_Arguments &, Variables &))
{
  map_.define_function (name, func);
}
