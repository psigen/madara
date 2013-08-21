#include "madara/transport/QoS_Transport_Settings.h"

Madara::Transport::QoS_Transport_Settings::QoS_Transport_Settings ()
  : Settings (), rebroadcast_ttl_ (0), rebroadcast_types_ (0)
{

}

Madara::Transport::QoS_Transport_Settings::QoS_Transport_Settings (
  const QoS_Transport_Settings & settings)
  : Settings (settings),
    rebroadcast_ttl_ (settings.rebroadcast_ttl_),
    rebroadcast_types_ (settings.rebroadcast_types_),
    trusted_peers_ (settings.trusted_peers_),
    banned_peers_ (settings.banned_peers_)
{
}

Madara::Transport::QoS_Transport_Settings::QoS_Transport_Settings (
  const Settings & settings)
  : Settings (settings),
    rebroadcast_ttl_ (0),
    rebroadcast_types_ (0)
{
  QoS_Transport_Settings * rhs = (QoS_Transport_Settings *) &settings;
  if (rhs)
  {
    rebroadcast_ttl_ = rhs->rebroadcast_ttl_;
    rebroadcast_types_ = rhs->rebroadcast_types_;
    trusted_peers_ = rhs->trusted_peers_;
    banned_peers_ = rhs->banned_peers_;
  }
}

Madara::Transport::QoS_Transport_Settings::~QoS_Transport_Settings ()
{

}

void
Madara::Transport::QoS_Transport_Settings::operator= (
  const QoS_Transport_Settings & rhs)
{
  if (this != &rhs)
  {
    Settings * lhs_base = (Settings *)this;
    Settings * rhs_base = (Settings *)&rhs;

    *lhs_base = *rhs_base;
    
    rebroadcast_ttl_ = rhs.rebroadcast_ttl_;
    rebroadcast_types_ = rhs.rebroadcast_types_;
    trusted_peers_ = rhs.trusted_peers_;
    banned_peers_ = rhs.banned_peers_;
  }
}

void
Madara::Transport::QoS_Transport_Settings::operator= (
  const Settings & rhs)
{
  if (this != &rhs)
  {
    Settings * lhs_base = (Settings *)this;
    Settings * rhs_base = (Settings *)&rhs;

    *lhs_base = *rhs_base;
  }
}

uint32_t
Madara::Transport::QoS_Transport_Settings::add_rebroadcastable_type (
  uint32_t type)
{
  return rebroadcast_types_ |= type;
}
      
uint32_t
Madara::Transport::QoS_Transport_Settings::remove_rebroadcastable_type (
  uint32_t type)
{
  // clear the type by taking the complement and logically anding
  return rebroadcast_types_ &= ~type;
}

bool
Madara::Transport::QoS_Transport_Settings::is_rebroadcastable_type (
  uint32_t type)
{
  // if the type is available, the logical and will return a non-zero value
  return (rebroadcast_types_ & type) > 0;
}

uint32_t
Madara::Transport::QoS_Transport_Settings::set_rebroadcast_ttl (uint32_t ttl)
{
  return rebroadcast_ttl_ = ttl;
}

uint32_t
Madara::Transport::QoS_Transport_Settings::get_rebroadcast_ttl (void)
{
  return rebroadcast_ttl_;
}

void
Madara::Transport::QoS_Transport_Settings::add_trusted_peer (
  const std::string & peer)
{
  banned_peers_.erase (peer);
  trusted_peers_[peer] = 1;
}

void
Madara::Transport::QoS_Transport_Settings::add_banned_peer (
  const std::string & peer)
{
  trusted_peers_.erase (peer);
  banned_peers_[peer] = 1;
}

bool
Madara::Transport::QoS_Transport_Settings::remove_trusted_peer (
  const std::string & peer)
{
  bool condition = false;
  if (trusted_peers_.find (peer) != trusted_peers_.end ())
  {
    trusted_peers_.erase (peer);
    condition = true;
  }
  return condition;
}

bool
Madara::Transport::QoS_Transport_Settings::remove_banned_peer (
  const std::string & peer)
{
  bool condition = false;
  if (banned_peers_.find (peer) != banned_peers_.end ())
  {
    banned_peers_.erase (peer);
    condition = true;
  }
  return condition;
}

bool
Madara::Transport::QoS_Transport_Settings::is_trusted (
  const std::string & peer) const
{
  bool condition = false;

  /**
   * There are three conditions when a peer is trusted. The
   * first is when the trusted_peers are everyone (empty set)
   * and the peer does not exist in the banned list. The
   * second is when trusted_peers contains the peer.
   **/

  if (trusted_peers_.size () == 0)
  {
    if (banned_peers_.find (peer) == banned_peers_.end ())
      condition = true;
  }
  else
  {
    condition = (trusted_peers_.find (peer) != trusted_peers_.end ());
  }

  return condition;
}
