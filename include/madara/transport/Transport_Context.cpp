#include "Transport_Context.h"
#include "madara/utility/Utility.h"
#include "madara/utility/Log_Macros.h"

Madara::Transport::Transport_Context::Transport_Context (int64_t operation,
        uint32_t receive_bandwidth, uint32_t send_bandwidth,
        uint64_t message_time, uint64_t current_time)
  : operation_ (operation),
    receive_bandwidth_ (receive_bandwidth),
    send_bandwidth_ (send_bandwidth),
    message_time_ (message_time),
    current_time_ (current_time)
{
}

Madara::Transport::Transport_Context::Transport_Context (
  const Transport_Context & rhs)
  : operation_ (rhs.operation_),
  receive_bandwidth_ (rhs.receive_bandwidth_),
  send_bandwidth_ (rhs.send_bandwidth_),
  message_time_ (rhs.message_time_),
  current_time_ (rhs.current_time_)
{
}

Madara::Transport::Transport_Context::~Transport_Context ()
{
}

void
Madara::Transport::Transport_Context::operator= (
  const Transport_Context & rhs)
{
  if (this != &rhs)
  {
    operation_ = rhs.operation_;
    send_bandwidth_ = rhs.send_bandwidth_;
    receive_bandwidth_ = rhs.receive_bandwidth_;
    message_time_ = rhs.message_time_;
    current_time_ = rhs.current_time_;
  }
}
