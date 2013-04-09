#ifndef _MADARA_KNOWLEDGE_BASE_INL_
#define _MADARA_KNOWLEDGE_BASE_INL_

#include <fstream>
#include "madara/Utility/Log_Macros.h"
#include "ace/streams.h"

/**
 * @file Knowledge_Base.inl
 * @author James Edmondson <james.r.edmondson@vanderbilt.edu>
 *
 * This file contains the inline functions for the Knowledge_Base_Impl class
 */

/**
  * Sets logging to STDERR (terminal). This is the default.
  * @param   clear_flags if true, clears all other logging options. If
  *                      false, retains previous logging options.
  **/
inline void Madara::Knowledge_Engine::Knowledge_Base::log_to_stderr (
  bool clear_flags)
{
  ACE_LOG_MSG->set_flags (ACE_Log_Msg::STDERR);
  if (clear_flags)
  {
    ACE_LOG_MSG->clr_flags (ACE_Log_Msg::OSTREAM);
    ACE_LOG_MSG->clr_flags (ACE_Log_Msg::SYSLOG);
  }
}

/**
  * Sets logging to a file
  *
  * @param   filename    path to the file that should contain log messages
  * @param   clear_flags if true, clears all other logging options. If
  *                      false, retains previous logging options.
  **/
inline void Madara::Knowledge_Engine::Knowledge_Base::log_to_file (
  const char * filename, bool clear_flags)
{
  // get the old message output stream and delete it
  ACE_OSTREAM_TYPE * output = new std::ofstream (filename);
  ACE_LOG_MSG->msg_ostream (output, true);
  ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);

  if (clear_flags)
  {
    ACE_LOG_MSG->clr_flags (ACE_Log_Msg::STDERR);
    ACE_LOG_MSG->clr_flags (ACE_Log_Msg::SYSLOG);
  }
}

/**
  * Sets logging to the system log
  * @param   prog_name   system logs require a program name. Use argv[0]
  *                      if unsure what to do with this.
  * @param   clear_flags if true, clears all other logging options. If
  *                      false, retains previous logging options
  **/
inline void Madara::Knowledge_Engine::Knowledge_Base::log_to_system_log (
  const char * prog_name, bool clear_flags)
{
  // open a new socket to the SYSLOG with madara set as logging agent
  ACE_LOG_MSG->open (prog_name, ACE_Log_Msg::SYSLOG, prog_name);

  if (clear_flags)
  {
    ACE_LOG_MSG->clr_flags (ACE_Log_Msg::STDERR);
    ACE_LOG_MSG->clr_flags (ACE_Log_Msg::OSTREAM);
  }
}



#endif   // _MADARA_KNOWLEDGE_BASE_INL_
