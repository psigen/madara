#include "madara/utility/Utility.h"
#include "madara/knowledge_engine/Knowledge_Base_Impl.h"
#include "madara/expression_tree/Interpreter.h"
#include "madara/expression_tree/Expression_Tree.h"
#include "madara/transport/tcp/TCP_Transport.h"
#include "madara/utility/Log_Macros.h"

#include <sstream>

#include "ace/OS_NS_Thread.h"
#include "ace/High_Res_Timer.h"

#ifdef _USE_OPEN_SPLICE_
  #include "madara/transport/splice/Splice_DDS_Transport.h"
#endif // _USE_OPEN_SPLICE_

#ifdef _USE_NDDS_
  #include "madara/transport/ndds/NDDS_Transport.h"
#endif // _USE_NDDS_

#include <iostream>

Madara::Knowledge_Engine::Knowledge_Base_Impl::Knowledge_Base_Impl ()
: files_ (map_), transport_ (0)
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
  // start from 50k, which is just above the bottom of the user
  // definable port range (hopefully avoid conflicts with 49152-49999
  unsigned short port =  50000;

  if (Madara::Utility::bind_to_ephemeral_port (unique_bind_, port) == -1)
  {
    MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, 
      DLINFO "Knowledge_Base_Impl::setup_uniquehostport:" \
      " unable to bind to any ephemeral port." \
      " Check firewall.\n"));
    exit (-1);
  }
 
  // we were able to bind to an ephemeral port
  Madara::Utility::merge_hostport_identifier (id_, host, port);

  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
    DLINFO "Knowledge_Base_Impl::setup_uniquehostport:" \
    " unique bind to %s\n", id_.c_str ()));
}

void
Madara::Knowledge_Engine::Knowledge_Base_Impl::activate_transport (void)
{
  if (!transport_)
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Knowledge_Base_Impl::activate_transport:" \
      " activating transport type %d\n", settings_.type));

    if (settings_.type == Madara::Transport::SPLICE)
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
    else if (settings_.type == Madara::Transport::TCP)
    {
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Knowledge_Base_Impl::activate_transport:" \
        " creating TCP transport.\n",
        settings_.type));

      transport_ = new Madara::Transport::TCP_Transport (id_, map_,
        Madara::Transport::TCP_Transport::RELIABLE);
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
    Madara::String_Vector modified;
    map_.get_modified (modified);
    std::stringstream string_builder;
    if (modified.size () > 0)
    {
      /// grab the clock time that was created with the apply_modified call
      unsigned long long cur_clock = map_.get_clock ();
      unsigned long quality = 0;

      for (Madara::String_Vector::const_iterator k = modified.begin ();
             k != modified.end (); ++k)
      {
        map_.set_clock (*k, cur_clock);
        unsigned long cur_quality = map_.get_write_quality (*k);

        // every knowledge update via multiassignment has the quality
        // of the highest update. This is to ensure consistency for
        // updating while also providing quality indicators for sensors,
        // actuators, controllers, etc.
        if (cur_quality > quality)
          quality = cur_quality;

        string_builder << *k << " = " << map_.get (*k) << " ; ";
      }

      transport_->send_multiassignment (string_builder.str (), quality);
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
Madara::Knowledge_Engine::Knowledge_Base_Impl::set (const ::std::string & key, 
                          long long value, bool send_modifieds)
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
    unsigned long long cur_clock = map_.inc_clock ();
    map_.set_clock (key, cur_clock);

    /// now send the data and reset the modified state
    transport_->send_data (key, value);
    map_.reset_modified (key);

    /// unlock the knowledge map
    map_.unlock ();
  }

  return result;
}


long long
Madara::Knowledge_Engine::Knowledge_Base_Impl::wait (const ::std::string & expression, 
                                                bool send_modifieds)
{
  // return an error message
  if (expression == "")
    return 0;

  // lock the context
  map_.lock ();

  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Knowledge_Base_Impl::wait:" \
      " waiting on %s\n", expression.c_str ()));

  // resulting tree of the expression
  Madara::Expression_Tree::Expression_Tree tree = interpreter_.interpret (
    map_, expression);

  long long last_value = tree.evaluate ();

  MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
      DLINFO "Knowledge_Base_Impl::wait:" \
      " completed first eval to get %d\n",
    last_value));

  if (transport_ && send_modifieds)
  {
    Madara::String_Vector modified;
    map_.get_modified (modified);
    std::stringstream string_builder;
    if (modified.size () > 0)
    {
      /// generate a new clock time and set our variable's clock to
      /// this new clock
      unsigned long long cur_clock = map_.inc_clock ();
      unsigned long quality = 0;

      for (Madara::String_Vector::const_iterator k = modified.begin ();
             k != modified.end (); ++k)
      {
        map_.set_clock (*k, cur_clock);
        unsigned long cur_quality = map_.get_write_quality (*k);

        // every knowledge update via multiassignment has the quality
        // of the highest update. This is to ensure consistency for
        // updating while also providing quality indicators for sensors,
        // actuators, controllers, etc.
        if (cur_quality > quality)
          quality = cur_quality;

        string_builder << *k << " = " << map_.get (*k) << " ; ";
          //transport_->send_data (*k, map_.get (*k));
      }

      MADARA_DEBUG (MADARA_LOG_MAJOR_DEBUG_INFO, (LM_DEBUG,
        DLINFO "Knowledge_Base_Impl::wait:" \
        " will be sending %s\n", 
        string_builder.str ().c_str ()));

      transport_->send_multiassignment (string_builder.str (), quality);
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

  // wait for expression to be true
  while (!last_value)
  {
    MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
        DLINFO "Knowledge_Base_Impl::wait:" \
        " last value didn't result in success\n"));

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
        unsigned long long cur_clock = map_.inc_clock ();
        unsigned long quality = 0;

        for (Madara::String_Vector::const_iterator k = modified.begin ();
               k != modified.end (); ++k)
        {
          map_.set_clock (*k, cur_clock);
          unsigned long cur_quality = map_.get_write_quality (*k);

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
      else
      {
        MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
            DLINFO "Knowledge_Base_Impl::wait:" \
            " no modifications to send during this wait\n"));
      }
    }

    map_.signal ();
  }

  // release the context lock
  map_.unlock ();
  return last_value;
}

long long
Madara::Knowledge_Engine::Knowledge_Base_Impl::wait (const ::std::string & expression, 
                                                const Wait_Settings & settings)
{
  // return an error message
  if (expression == "")
    return 0;

  ACE_High_Res_Timer timer;
  ACE_hrtime_t elapsed;
  ACE_hrtime_t maximum;
  timer.start ();

  // print the post statement at highest log level (cannot be masked)
  if (settings.pre_print_statement != "")
    map_.print (settings.pre_print_statement, MADARA_LOG_EMERGENCY);

  // lock the context
  map_.lock ();

  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Knowledge_Base_Impl::wait:" \
      " waiting on %s\n", expression.c_str ()));

  // resulting tree of the expression
  Madara::Expression_Tree::Expression_Tree tree = interpreter_.interpret (
    map_, expression);

  long long last_value = tree.evaluate ();

  MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
      DLINFO "Knowledge_Base_Impl::wait:" \
      " completed first eval to get %d\n",
    last_value));

  if (transport_ && settings.send_modifieds)
  {
    Madara::String_Vector modified;
    map_.get_modified (modified);
    std::stringstream string_builder;
    if (modified.size () > 0)
    {
      /// generate a new clock time and set our variable's clock to
      /// this new clock
      unsigned long long cur_clock = map_.inc_clock ();
      unsigned long quality = 0;

      for (Madara::String_Vector::const_iterator k = modified.begin ();
             k != modified.end (); ++k)
      {
        map_.set_clock (*k, cur_clock);
        unsigned long cur_quality = map_.get_write_quality (*k);

        // every knowledge update via multiassignment has the quality
        // of the highest update. This is to ensure consistency for
        // updating while also providing quality indicators for sensors,
        // actuators, controllers, etc.
        if (cur_quality > quality)
          quality = cur_quality;

        string_builder << *k << " = " << map_.get (*k) << " ; ";
          //transport_->send_data (*k, map_.get (*k));
      }

      MADARA_DEBUG (MADARA_LOG_MAJOR_DEBUG_INFO, (LM_DEBUG,
        DLINFO "Knowledge_Base_Impl::wait:" \
        " will be sending %s\n", 
        string_builder.str ().c_str ()));

      transport_->send_multiassignment (string_builder.str (), quality);
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

  ACE_Time_Value poll_frequency;

  if (!last_value)
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
  while (!last_value &&
    (settings.max_wait_time > 0 && maximum > elapsed))
  {
    MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
        DLINFO "Knowledge_Base_Impl::wait:" \
        " elapsed is %Q and max is %Q\n",
        elapsed, maximum));

    MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
        DLINFO "Knowledge_Base_Impl::wait:" \
        " last value didn't result in success\n"));

    // Unlike the other wait statements, we allow for a time based wait.
    // To do this, we allow a user to specify a 
    ACE_OS::sleep (poll_frequency);

    // relock - basically we need to evaluate the tree again, and
    // we can't have a bunch of people changing the variables as 
    // while we're evaluating the tree.
    map_.lock ();
    last_value = tree.evaluate ();

    if (transport_ && settings.send_modifieds)
    {
      Madara::String_Vector modified;
      map_.get_modified (modified);
      std::stringstream string_builder;
      if (modified.size () > 0)
      {
        /// generate a new clock time and set our variable's clock to
        /// this new clock
        unsigned long long cur_clock = map_.inc_clock ();
        unsigned long quality = 0;

        for (Madara::String_Vector::const_iterator k = modified.begin ();
               k != modified.end (); ++k)
        {
          map_.set_clock (*k, cur_clock);
          unsigned long cur_quality = map_.get_write_quality (*k);

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
      else
      {
        MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
            DLINFO "Knowledge_Base_Impl::wait:" \
            " no modifications to send during this wait\n"));
      }
    }

    map_.signal ();
    timer.stop ();
    timer.elapsed_time (elapsed);
  } // end while (!last)

  // print the post statement at highest log level (cannot be masked)
  if (settings.post_print_statement != "")
    map_.print (settings.post_print_statement, MADARA_LOG_EMERGENCY);

  // release the context lock
  map_.unlock ();
  return last_value;
}

void
Madara::Knowledge_Engine::Knowledge_Base_Impl::add_rule (const ::std::string & expression_copy)
{
  ::std::string expression (expression_copy);
  Madara::Utility::strip_white_space (expression);

  evaluate (expression);

  rules_.push_back (expression);
}

long long
Madara::Knowledge_Engine::Knowledge_Base_Impl::evaluate (
  const ::std::string & expression, bool send_modifieds)
{
  if (expression == "")
    return 0;

  long long last_value = 0;

  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Knowledge_Base_Impl::wait:" \
        " evaluting %s\n", expression.c_str ()));

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
    unsigned long long cur_clock = map_.inc_clock ();
    unsigned long quality = 0;

    for (Madara::String_Vector::const_iterator k = modified.begin ();
         k != modified.end (); ++k)
    {
      map_.set_clock (*k, cur_clock);
      unsigned long cur_quality = map_.get_write_quality (*k);

      // every knowledge update via multiassignment has the quality
      // of the highest update. This is to ensure consistency for
      // updating while also providing quality indicators for sensors,
      // actuators, controllers, etc.
      if (cur_quality > quality)
        quality = cur_quality;

      string_builder << *k << " = " << map_.get (*k) << " ; ";
    }

    if (modified.size () > 0)
      transport_->send_multiassignment (string_builder.str (), quality);
    map_.reset_modified ();
  }

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
