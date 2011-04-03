
#include "madara/kats/Test_Framework.h"

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

/// Barriers on all processes until everyone is at the event
long long
Madara::KATS::Test_Framework::barrier (const std::string & event_name)
{
  std::stringstream buffer;
  long long id = knowledge_.get (".madara.id");
  long long processes = knowledge_.get (".madara.processes");

  // set our attribute to 1
  buffer << "((madara";
  buffer << ".";
  buffer << event_name;
  buffer << ".";
  buffer << id;
  buffer << " = 1)";

  // add logic to wait for all processes in the ring to set their
  // attribute to not-zero
  for (long long i = 0; i < processes; ++i)
  {
    buffer << " && madara";
    buffer << ".";
    buffer << event_name;
    buffer << ".";
    buffer << i;
  }

  // make an escape clause due to our usage of best_effort
  // if we see the attribute set globally, then move on because
  // we have missed a message, even though we know everyone has
  // arrived
  buffer << ") || ";
  buffer << event_name;

  return knowledge_.wait (buffer.str ());
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
  assert (name != "");

  // default result is 
  int result = 0;
  long long eval_result = 0;

  // first, make sure the transport is activated or else
  // we will be here for a long time.
  knowledge_.activate_transport ();

  // check if we should be aborting
  eval_result = knowledge_.evaluate (fail_condition);
  if (eval_result != 0)
  {
    // check if we should be skipping over a pre and post condition
    eval_result = knowledge_.evaluate (continue_condition);

    if (eval_result != 0)
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

  return result;
}

/// Logs according to a severity level
void 
Madara::KATS::Test_Framework::log (int level, const std::string & statement)
{

}

/// Dumps all knowledge to std::err
void Madara::KATS::Test_Framework::dump (int level)
{

}
