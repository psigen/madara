#include "madara/transport/ndds/NDDS_Transport.h"
#include "madara/transport/ndds/NDDS_Transport_Read_Thread.h"
#include "madara/utility/Log_Macros.h"
#include "madara/knowledge_engine/Update_Types.h"
#include "madara/utility/Utility.h"

#include <iostream>
#include <sstream>

const char * Madara::Transport::NDDS_Transport::topic_names_[] = {
  "MADARA_KaRL_Data",
  "MADARA_KaRL_Control"
};

const char * Madara::Transport::NDDS_Transport::partition_ = "Madara_knowledge";

Madara::Transport::NDDS_Transport::NDDS_Transport (
  const std::string & id,
  Madara::Knowledge_Engine::Thread_Safe_Context & context, 
  const Settings & config, bool launch_transport)
  : Madara::Transport::Base (config), 
  id_ (id), context_ (context)
{
  if (launch_transport)
    setup ();
}
Madara::Transport::NDDS_Transport::~NDDS_Transport ()
{
  close ();
}

void
Madara::Transport::NDDS_Transport::close (void)
{
  this->invalidate_transport ();

  thread_ = 0;

  this->shutting_down_ = false;
}

int
Madara::Transport::NDDS_Transport::reliability (void) const
{
  return this->settings_.reliability;
}

int
Madara::Transport::NDDS_Transport::reliability (const int & setting)
{
  return this->settings_.reliability = setting;
}

int
Madara::Transport::NDDS_Transport::setup (void)
{
  this->is_valid_ = false;

  //thread_ = new Madara::Transport::NDDS_Read_Thread (id_, context_, 
  //  update_reader_);
  
  this->validate_transport ();

  return 0;
}

long
Madara::Transport::NDDS_Transport::send_data (const std::string & key, 
                                               const long long & value)
{
  // check to see if we are shutting down
  long ret = this->check_transport ();
  if (-1 == ret)
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "NDDS_Transport::send_data: transport is shutting down")); 
    return ret;
  }
  else if (-2 == ret)
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "NDDS_Transport::send_data: transport is not valid")); 
    return ret;
  }
  
  return 0;
}

long
Madara::Transport::NDDS_Transport::send_multiassignment (
  const std::string & expression, unsigned long quality)
{
  // check to see if we are shutting down
  long ret = this->check_transport ();
  if (-1 == ret)
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "NDDS_Transport::send_multiassignment: transport is shutting down")); 
    return ret;
  }
  else if (-2 == ret)
  {
    MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
      DLINFO "NDDS_Transport::send_multiassignment: transport is not valid")); 
    return ret;
  }
  

  return 0;
}
