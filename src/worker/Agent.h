#ifndef AGENT_H
#define AGENT_H

#include "Agent_Context.h"
#include "Client_Acceptor.h"

namespace Madara
{
  int bindToFirstAvailablePort (Client_Acceptor & peer_acceptor,
                           Agent_Context & context);
  int discoverPortOnHost(const std::string & host,
                           Agent_Context & context);
}

#endif