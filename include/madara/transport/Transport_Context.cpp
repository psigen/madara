#include "Transport_Context.h"
#include "madara/utility/Utility.h"
#include "madara/utility/Log_Macros.h"

Madara::Transport::Transport_Context::Transport_Context (int64_t operation,
        uint32_t receive_bandwidth, uint32_t send_bandwidth)
  : operation_ (operation),
    receive_bandwidth_ (receive_bandwidth),
    send_bandwidth_ (send_bandwidth)
{
}

Madara::Transport::Transport_Context::Transport_Context (
  const Transport_Context & rhs)
  : operation_ (rhs.operation_),
  receive_bandwidth_ (rhs.receive_bandwidth_),
  send_bandwidth_ (rhs.send_bandwidth_)
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
  }
}
