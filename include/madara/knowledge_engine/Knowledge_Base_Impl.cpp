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
#include "ace/OS_NS_sys_socket.h"

#ifdef _USE_OPEN_SPLICE_
  #include "madara/transport/splice/Splice_DDS_Transport.h"
  #include "madara/transport/splice/Inconsistent_Transport.h"
#endif // _USE_OPEN_SPLICE_

#ifdef _USE_NDDS_
  #include "madara/transport/ndds/NDDS_Transport.h"
#endif // _USE_NDDS_

#include <iostream>

Madara::Knowledge_Engine::Knowledge_Base_Impl::Knowledge_Base_Impl ()
: settings_ (), files_ (map_)
{
  activate_transport ();
  // no hope of transporting, so don't setup uniquehostport
}

Madara::Knowledge_Engine::Knowledge_Base_Impl::Knowledge_Base_Impl (
  const std::string & host, int transport)
: settings_ (), files_ (map_)
{
  // override default settings for the arguments
  settings_.type = transport;

  setup_uniquehostport (host);
  activate_transport ();
}

Madara::Knowledge_Engine::Knowledge_Base_Impl::Knowledge_Base_Impl (
  const std::string & host, int transport,
  const std::string & knowledge_domain)
: settings_ (), files_ (map_)
{
  // override default settings for the arguments
  settings_.type = transport;
  settings_.domains = knowledge_domain;

  setup_uniquehostport (host);
  activate_transport ();
}

Madara::Knowledge_Engine::Knowledge_Base_Impl::Knowledge_Base_Impl (
  const std::string & host, const Madara::Transport::Settings & config)
: settings_ (config), files_ (map_)
{
  setup_uniquehostport (host);
  if (!settings_.delay_launch)
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

    if (Madara::Utility::bind_to_ephemeral_port (
      unique_bind_, actual_host, port)
         == -1)
    {
      MADARA_ERROR (MADARA_LOG_TERMINAL_ERROR, (LM_ERROR, 
        DLINFO "Knowledge_Base_Impl::setup_uniquehostport:" \
        " unable to bind to any ephemeral port." \
        " Check firewall. ERRORNO = %d\n", ACE_OS::last_error ()));

      if (!settings_.never_exit)
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

size_t
Madara::Knowledge_Engine::Knowledge_Base_Impl::attach_transport (const std::string & id,
  Transport::Settings & settings)
{
  Madara::Transport::Base * transport (0);
  std::string originator (id);

  if (originator == "")
    originator = id_;

  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
    DLINFO "Knowledge_Base_Impl::attach_transport:" \
    " activating transport type %d\n", settings.type));
  if (settings.type == Madara::Transport::BROADCAST)
  {
    transport = new Madara::Transport::Broadcast_Transport (originator, map_,
      settings, true);
  }
  else if (settings.type == Madara::Transport::MULTICAST)
  {
    transport = new Madara::Transport::Multicast_Transport (originator, map_,
      settings, true);
  }
  else if (settings.type == Madara::Transport::SPLICE)
  {
  #ifdef _USE_OPEN_SPLICE_
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Knowledge_Base_Impl::activate_transport:" \
      " creating Open Splice DDS transport.\n",
      settings.type));

    transport = new Madara::Transport::Splice_DDS_Transport (originator, map_,
                        settings, true);
  #else
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Knowledge_Base_Impl::activate_transport:" \
      " project was not generated with opensplice=1. Transport is invalid.\n",
      settings.type));
  #endif
  }
  else if (settings.type == Madara::Transport::NDDS)
  {
  #ifdef _USE_NDDS_
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Knowledge_Base_Impl::activate_transport:" \
      " creating NDDS transport.\n",
      settings.type));

    transport = new Madara::Transport::NDDS_Transport (originator, map_,
                        settings, true);
  #else
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Knowledge_Base_Impl::activate_transport:" \
      " project was not generated with ndds=1. Transport is invalid.\n",
      settings.type));
  #endif
  }
  else if (settings.type == Madara::Transport::UDP)
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Knowledge_Base_Impl::activate_transport:" \
      " creating UDP transport.\n",
      settings.type));

    transport = new Madara::Transport::UDP_Transport (originator, map_,
      settings, true);
  }
  else if (settings.type == Madara::Transport::TCP)
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Knowledge_Base_Impl::activate_transport:" \
      " creating TCP transport.\n",
      settings.type));

    transport = new Madara::Transport::TCP_Transport (originator, map_,
      settings, true);
  }
  else if (settings.type == Madara::Transport::INCONSISTENT)
  {
  #ifdef _USE_OPEN_SPLICE_
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Knowledge_Base_Impl::activate_transport:" \
      " creating Inconsistent transport.\n",
      settings.type));

    transport = new Madara::Transport::Inconsistent_Transport (originator, map_,
                        settings, true);
  #else
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Knowledge_Base_Impl::activate_transport:" \
      " project was not generated with opensplice=1. Transport is invalid.\n",
      settings.type));
  #endif
  }
  else
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Knowledge_Base_Impl::activate_transport:" \
      " no transport was specified. Setting transport to null.\n",
      settings.type));
  }

  // if we have a valid transport, add it to the transports vector
  if (transport != 0)
    transports_.push_back (transport);

  return transports_.size ();
}

void
Madara::Knowledge_Engine::Knowledge_Base_Impl::activate_transport (void)
{
  if (transports_.size () == 0)
  {
    attach_transport (id_, settings_);
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
  if (transports_.size () > 0)
  {
    for (unsigned int i = 0; i < transports_.size (); ++i)
    {
      transports_[i]->close ();
      delete transports_[i];
      MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
        DLINFO "Knowledge_Base_Impl::close_transport:" \
        " transport has been closed.\n"));
    }
    transports_.clear ();
    ACE_OS::socket_fini ();
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
  
  send_modifieds ("Knowledge_Base_Impl:apply_modified");

  map_.unlock ();

  return ret;
}

int
Madara::Knowledge_Engine::Knowledge_Base_Impl::set (
  const std::string & key, 
  Madara::Knowledge_Record::Integer value,
  const Eval_Settings & settings)
{
  int result = map_.set (key, value, settings);

  send_modifieds ("Knowledge_Base_Impl:set", settings);

  return result;
}

int
Madara::Knowledge_Engine::Knowledge_Base_Impl::set (
  const Variable_Reference & variable, 
  Madara::Knowledge_Record::Integer value,
  const Eval_Settings & settings)
{
  int result = map_.set (variable, value, settings);

  send_modifieds ("Knowledge_Base_Impl:set", settings);

  return result;
}

int
Madara::Knowledge_Engine::Knowledge_Base_Impl::set_index (
  const std::string & key,
  size_t index,
  Madara::Knowledge_Record::Integer value,
  const Eval_Settings & settings)
{
  int result = map_.set_index (key, index, value, settings);
  
  send_modifieds ("Knowledge_Base_Impl:set_index", settings);

  return result;
}

int
Madara::Knowledge_Engine::Knowledge_Base_Impl::set_index (
  const Variable_Reference & variable,
  size_t index,
  Madara::Knowledge_Record::Integer value,
  const Eval_Settings & settings)
{
  int result = map_.set_index (variable, index, value, settings);
  
  send_modifieds ("Knowledge_Base_Impl:set_index", settings);

  return result;
}

int
Madara::Knowledge_Engine::Knowledge_Base_Impl::set (
  const std::string & key, 
  const Knowledge_Record::Integer * value,
  uint32_t size,
  const Eval_Settings & settings)
{
  int result = map_.set (key, value, size, settings);
  
  send_modifieds ("Knowledge_Base_Impl:set", settings);

  return result;
}

int
Madara::Knowledge_Engine::Knowledge_Base_Impl::set (
  const Variable_Reference & variable, 
  const Knowledge_Record::Integer * value,
  uint32_t size,
  const Eval_Settings & settings)
{
  int result = map_.set (variable, value, size, settings);
  
  send_modifieds ("Knowledge_Base_Impl:set", settings);

  return result;
}

int
Madara::Knowledge_Engine::Knowledge_Base_Impl::set (
  const std::string & key, 
  const std::vector <Knowledge_Record::Integer> & value,
  const Eval_Settings & settings)
{
  int result = map_.set (key, value, settings);
  
  send_modifieds ("Knowledge_Base_Impl:set", settings);

  return result;
}

int
Madara::Knowledge_Engine::Knowledge_Base_Impl::set (
  const Variable_Reference & variable, 
  const std::vector <Knowledge_Record::Integer> & value,
  const Eval_Settings & settings)
{
  int result = map_.set (variable, value, settings);
  
  send_modifieds ("Knowledge_Base_Impl:set", settings);

  return result;
}

int
Madara::Knowledge_Engine::Knowledge_Base_Impl::set (
  const std::string & key, 
  double value, 
  const Eval_Settings & settings)
{
  int result = map_.set (key, value, settings);
  
  send_modifieds ("Knowledge_Base_Impl:set", settings);

  return result;
}

int
Madara::Knowledge_Engine::Knowledge_Base_Impl::set (
  const Variable_Reference & variable, 
  double value, 
  const Eval_Settings & settings)
{
  int result = map_.set (variable, value, settings);
  
  send_modifieds ("Knowledge_Base_Impl:set", settings);

  return result;
}

int
Madara::Knowledge_Engine::Knowledge_Base_Impl::set_index (
  const std::string & key,
  size_t index,
  double value,
  const Eval_Settings & settings)
{
  int result = map_.set_index (key, index, value, settings);
  
  send_modifieds ("Knowledge_Base_Impl:set_index", settings);

  return result;
}

int
Madara::Knowledge_Engine::Knowledge_Base_Impl::set_index (
  const Variable_Reference & variable,
  size_t index,
  double value,
  const Eval_Settings & settings)
{
  int result = map_.set_index (variable, index, value, settings);
  
  send_modifieds ("Knowledge_Base_Impl:set_index", settings);

  return result;
}

int
Madara::Knowledge_Engine::Knowledge_Base_Impl::set (
  const std::string & key, 
  const double * value,
  uint32_t size,
  const Eval_Settings & settings)
{
  int result = map_.set (key, value, size, settings);
  
  send_modifieds ("Knowledge_Base_Impl:set", settings);

  return result;
}

int
Madara::Knowledge_Engine::Knowledge_Base_Impl::set (
  const Variable_Reference & variable, 
  const double * value,
  uint32_t size,
  const Eval_Settings & settings)
{
  int result = map_.set (variable, value, size, settings);
  
  send_modifieds ("Knowledge_Base_Impl:set", settings);

  return result;
}

int
Madara::Knowledge_Engine::Knowledge_Base_Impl::set (
  const std::string & key, 
  const std::vector <double> & value,
  const Eval_Settings & settings)
{
  int result = map_.set (key, value, settings);
  
  send_modifieds ("Knowledge_Base_Impl:set", settings);

  return result;
}

int
Madara::Knowledge_Engine::Knowledge_Base_Impl::set (
  const Variable_Reference & variable, 
  const std::vector <double> & value,
  const Eval_Settings & settings)
{
  int result = map_.set (variable, value, settings);
  
  send_modifieds ("Knowledge_Base_Impl:set", settings);

  return result;
}

int
Madara::Knowledge_Engine::Knowledge_Base_Impl::set (
  const std::string & key, 
  const std::string & value, 
  const Eval_Settings & settings)
{
  int result = map_.set (key, value, settings);
  
  send_modifieds ("Knowledge_Base_Impl:set", settings);

  return result;
}

int
Madara::Knowledge_Engine::Knowledge_Base_Impl::set (
  const Variable_Reference & variable, 
  const std::string & value, 
  const Eval_Settings & settings)
{
  int result = map_.set (variable, value, settings);
  
  send_modifieds ("Knowledge_Base_Impl:set", settings);

  return result;
}

/// Read a file into the knowledge base
int
Madara::Knowledge_Engine::Knowledge_Base_Impl::read_file (
  const std::string & key, const std::string & filename, 
        const Eval_Settings & settings)
{
  if (key == "")
    return -1;

  int result = map_.read_file (key, filename, settings);
  
  send_modifieds ("Knowledge_Base_Impl:read_file", settings);

  return result;
}

/// Read a file into the knowledge base
int
Madara::Knowledge_Engine::Knowledge_Base_Impl::read_file (
  const Variable_Reference & variable,
  const std::string & filename, 
  const Eval_Settings & settings)
{
  int result = map_.read_file (variable, filename, settings);
  
  send_modifieds ("Knowledge_Base_Impl:read_file", settings);

  return result;
}

Madara::Knowledge_Engine::Compiled_Expression
Madara::Knowledge_Engine::Knowledge_Base_Impl::compile (
  const std::string & expression)
{
  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Knowledge_Base_Impl::compile:" \
      " compiling %s\n", expression.c_str ()));

  return map_.compile (expression);
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
  
  send_modifieds ("Knowledge_Base_Impl:wait", settings);

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
    
    send_modifieds ("Knowledge_Base_Impl:wait", settings);

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
  
  send_modifieds ("Knowledge_Base_Impl:evaluate", settings);

  // print the post statement at highest log level (cannot be masked)
  if (settings.post_print_statement != "")
    map_.print (settings.post_print_statement, MADARA_LOG_EMERGENCY);

  map_.unlock ();

  return last_value;
}
