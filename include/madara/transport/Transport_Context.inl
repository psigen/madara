#ifndef _TRANSPORT_CONTEXT_INL_
#define _TRANSPORT_CONTEXT_INL_

#include "Transport_Context.h"

inline int64_t
Madara::Transport::Transport_Context::get_operation (void)
{
  return operation_;
}

inline uint32_t
Madara::Transport::Transport_Context::get_receive_bandwidth (void)
{
  return receive_bandwidth_;
}

inline uint32_t
Madara::Transport::Transport_Context::get_send_bandwidth (void)
{
  return send_bandwidth_;
}

inline void
Madara::Transport::Transport_Context::set_operation (
  int64_t operation)
{
  operation_ = operation;
}

inline void
Madara::Transport::Transport_Context::set_receive_bandwidth (
  uint32_t bandwidth)
{
  receive_bandwidth_ = bandwidth;
}

inline void
Madara::Transport::Transport_Context::set_send_bandwidth (
  uint32_t bandwidth)
{
  send_bandwidth_ = bandwidth;
}

#endif   // _TRANSPORT_CONTEXT_INL_