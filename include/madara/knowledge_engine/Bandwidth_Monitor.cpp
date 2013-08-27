#include "Bandwidth_Monitor.h"
#include "madara/utility/Utility.h"
#include "madara/utility/Log_Macros.h"

Madara::Knowledge_Engine::Bandwidth_Monitor::Bandwidth_Monitor (
  time_t window_in_secs)
  : utilization_ (0), window_ (window_in_secs)
{
}

Madara::Knowledge_Engine::Bandwidth_Monitor::Bandwidth_Monitor (
  const Bandwidth_Monitor & rhs)
  : messages_ (rhs.messages_), utilization_ (rhs.utilization_),
    window_ (rhs.window_)
{
}

Madara::Knowledge_Engine::Bandwidth_Monitor::~Bandwidth_Monitor ()
{
}

void
Madara::Knowledge_Engine::Bandwidth_Monitor::operator= (
  const Bandwidth_Monitor & rhs)
{
  Bandwidth_Guard guard (mutex_);
  if (this != &rhs)
  {
    messages_ = rhs.messages_;
    utilization_ = rhs.utilization_;
    window_ = rhs.window_;
  }
}

void
Madara::Knowledge_Engine::Bandwidth_Monitor::set_window (
  time_t window_in_secs)
{
  Bandwidth_Guard guard (mutex_);
  
  window_ = window_in_secs;
}

void
Madara::Knowledge_Engine::Bandwidth_Monitor::add (uint32_t size)
{
  Bandwidth_Guard guard (mutex_);
  
  utilization_ += size;

  time_t cur_time = update_utilization ();
  
  Bandwidth_Record entry (cur_time, size);
  
  messages_.push_back (entry);
}

void
Madara::Knowledge_Engine::Bandwidth_Monitor::add (
  time_t timestamp, uint32_t size)
{
  Bandwidth_Guard guard (mutex_);
  
  utilization_ += size;

  Bandwidth_Record entry (timestamp, size);

  update_utilization ();

  messages_.push_back (entry);
}

uint32_t
Madara::Knowledge_Engine::Bandwidth_Monitor::get_utilization (void)
{
  Bandwidth_Guard guard (mutex_);

  update_utilization ();

  return utilization_;
}

uint32_t
Madara::Knowledge_Engine::Bandwidth_Monitor::get_bytes_per_second (void)
{
  Bandwidth_Guard guard (mutex_);
  
  update_utilization ();

  return utilization_ / (uint32_t)window_;
}

void
Madara::Knowledge_Engine::Bandwidth_Monitor::clear (void)
{
  Bandwidth_Guard guard (mutex_);
  messages_.clear ();
  utilization_ = 0;
}

void
Madara::Knowledge_Engine::Bandwidth_Monitor::print_utilization (
  void)
{
  Bandwidth_Guard guard (mutex_);

  get_utilization ();

  MADARA_DEBUG (0, (LM_DEBUG,
    "Bandwidth: %d messages "
    "for %d bytes over %Qs window (%d B/s)\n", messages_.size (),
    utilization_, window_, get_bytes_per_second ()));
}


size_t
Madara::Knowledge_Engine::Bandwidth_Monitor::get_number_of_messages (
  void) const
{
  Bandwidth_Guard guard (mutex_);
  return messages_.size ();
}
