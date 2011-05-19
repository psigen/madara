
#include "madara/kats/Test_Framework.h"
#include "madara/utility/Log_Macros.h"

#include <sstream>
#include <iostream>
#include <assert.h>

/**
 * @file Test_Framework.cpp
 * @author James Edmondson <james.r.edmondson@vanderbilt.edu>
 *
 * This file contains the implementations for the Test_Framework class
 */

/// Constructor
Madara::KATS::Test_Framework::Test_Framework (
  const Madara::KATS::Settings & settings)
: knowledge_ (settings.host, settings)
{
  knowledge_.set (".madara.id", settings.id);
  knowledge_.set (".madara.processes", settings.processes);
}

/// Copy constructor
Madara::KATS::Test_Framework::Test_Framework (
  const Test_Framework & original)
: knowledge_ (original.knowledge_)
{
}

/// Destructor
Madara::KATS::Test_Framework::~Test_Framework ()
{
}

/// Retrieves a knowledge value
long long
Madara::KATS::Test_Framework::get (const ::std::string & key) const
{
  return knowledge_.get (key);
}

/// Sets a knowledge value to a specified value
int
Madara::KATS::Test_Framework::set (const ::std::string & key, long long value)
{
  return knowledge_.set (key, value);
}

/// Barriers on all processes until everyone is at the event
long long
Madara::KATS::Test_Framework::barrier (const std::string & event_name)
{
  // first, make sure the transport is activated or else
  // we will be here for a long time.
  knowledge_.activate_transport ();

  std::stringstream buffer;
  std::stringstream log_message;

  long long id = knowledge_.get (".madara.id");
  long long processes = knowledge_.get (".madara.processes");

  if (processes <= 1)
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "Test_Framework::barrier: .madara.processes <= 1."
      " Returning success.\n"));
    return 1;
  }

  // set our attribute to 1
  buffer << "((MADARA.BARRIER.";
  buffer << event_name;
  buffer << ".";
  buffer << id;
  buffer << " = 1)";

  // add logic to wait for all processes in the ring to set their
  // attribute to not-zero
  for (long long i = 0; i < processes; ++i)
  {
    buffer << " && MADARA.BARRIER.";
    buffer << event_name;
    buffer << ".";
    buffer << i;
  }

  // make an escape clause due to our usage of best_effort
  // if we see the attribute set globally, then move on because
  // we have missed a message, even though we know everyone has
  // arrived
  buffer << ") || MADARA.BARRIER.";
  buffer << event_name;

  log_message << "\nKATS::Test_Framework::barrier: started: ";
  log_message << buffer.str ();
  log_message << "\n";

  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
    DLINFO "%s", knowledge_.expand_statement (log_message.str ()).c_str ()));

  return knowledge_.wait (buffer.str ());
}

/// Attempts to send out all global knowledge
void
Madara::KATS::Test_Framework::sync (void)
{
  std::stringstream log_message;

  log_message.str ("");
  log_message << "KATS::Test_Framework::sync: Starting ";
  log_message << "\n";

  log (log_message.str (), MADARA_LOG_MAJOR_EVENT);

  // first, make sure the transport is activated or else
  // we will be here for a long time.
  knowledge_.activate_transport ();

  knowledge_.apply_modified ();

  log_message.str ("");
  log_message << "KATS::Test_Framework::sync: Finished ";
  log_message << "\n";

  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
    DLINFO "%s", knowledge_.expand_statement (log_message.str ()).c_str ()));
}

/// Syncs on all processes until everyone is at the event and
/// has the latest information
void
Madara::KATS::Test_Framework::barriered_sync (const std::string & event_name)
{
  std::stringstream buffer;
  std::stringstream log_message;

  log_message.str ("");
  log_message << "KATS::Test_Framework::barriered_sync: Starting ";
  log_message << event_name;
  log_message << "\n";

  log (log_message.str (), MADARA_LOG_MAJOR_EVENT);

  // first, make sure the transport is activated or else
  // we will be here for a long time.
  knowledge_.activate_transport ();

  buffer << "ENTER";
  buffer << event_name;
  barrier (event_name);

  knowledge_.apply_modified ();

  buffer.str ("");
  buffer << "LEAVE";
  barrier (event_name);

  log_message.str ("");
  log_message << "KATS::Test_Framework::barriered_sync: Finished ";
  log_message << event_name;
  log_message << "\n";

  MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
    DLINFO "%s", knowledge_.expand_statement (log_message.str ()).c_str ()));
}

/// Creates a testing event
int 
Madara::KATS::Test_Framework::event (const std::string & name, 
        const std::string & pre_condition,
        const std::string & post_condition,
        const std::string & fail_condition,
        const std::string & continue_condition,
        bool barrier_this_event, bool close_transport)
{
  if (name == "")
    return -1;

  // default result is 
  int result = 0;
  long long eval_result = 0;

  // first, make sure the transport is activated or else
  // we will be here for a long time. Only necessary
  // if we are 
  if (barrier_this_event)
    knowledge_.activate_transport ();

  // check if we should be aborting
  eval_result = knowledge_.evaluate (fail_condition);
  if (fail_condition == "" || eval_result == 0)
  {
    // check if we should be skipping over a pre and post condition
    eval_result = knowledge_.evaluate (continue_condition);

    if (continue_condition == "" || eval_result == 0)
    {
      // if the user wants us to barrier, then do so on the event
      if (barrier_this_event)
        barrier (name);

      // wait for the pre_condition to evaluate to non_zero (true)
      if (pre_condition != "")
        knowledge_.wait (pre_condition);

      // pre_condition has been met. Set the post_condition
      knowledge_.evaluate (post_condition);

      // update the knowledge base to show that the event succeeded
      std::stringstream buffer;
      buffer << ".event.";
      buffer << name;
      buffer << ".success";
      knowledge_.set (buffer.str (), 1);
    }
    else
    {
      // update the knowledge base to show that the event continued
      std::stringstream buffer;
      buffer << ".event.";
      buffer << name;
      buffer << ".continued";
      knowledge_.set (buffer.str (), 1);
    }
  }
  // we have to abort
  else
  {
    // update the knowledge base to show that the event failed
    std::stringstream buffer;
    buffer << ".event.";
    buffer << name;
    buffer << ".failed";
    knowledge_.set (buffer.str (), 1);

    result = -1;
  }

  // update the knowledge base to show that the event is done
  std::stringstream buffer;
  buffer << ".event.";
  buffer << name;
  buffer << ".done";
  knowledge_.set (buffer.str (), 1);

  // if we have a failed condition or if the user has
  // asked us to close the transport
  if (result || close_transport)
    knowledge_.close_transport ();

  if (result == -1)
  {
    dump ();
    exit (result);
  }

  return result;
}

/// Logs according to a severity level
void 
Madara::KATS::Test_Framework::log (const std::string & statement, unsigned int level)
{
  knowledge_.print (statement, level);
}

/// Dumps all knowledge to std::err
void Madara::KATS::Test_Framework::dump (unsigned int level)
{
  knowledge_.print_knowledge (level);
}
