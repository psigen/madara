#ifndef AGENT_CPP
#define AGENT_CPP

#include <stdlib.h>
#include <string>

#include "ace/TP_Reactor.h"
#include "ace/SOCK_Connector.h"

#include "Agent.h"
#include "Agent_Context.h"
#include "Client_Acceptor.h"
#include "Madara_Common.h"

int
Madara::discoverPortOnHost(const std::string & host,
                           Agent_Context & context)
{
  // more than enough room for a stringenized port
  //char port_buf[16];
  u_short server_port;
  int ret = 1;
  std::string key;

  for (server_port = Madara::ACCEPTOR_PORT_START;
    context.peerExists (host, server_port) || 
    context.selfCheck (host, server_port); 
    ++server_port)
      {
        ret = 4;
      }


  // setup a connector to this potential new agent port
  ACE_SOCK_Connector connector;
  ACE_SOCK_Stream server;
  ACE_INET_Addr addr (server_port, host.c_str ());

  // attempt to connect to port. If it works, we've discovered an agent
  if (server_port = connector.connect (server, addr) != -1)
  {
    Madara::merge_key (key, host, server_port);
    context.addLatency (key, -1);

    server.close ();
    return 0;
  }

  return -1;
}

int 
Madara::bindToFirstAvailablePort (Client_Acceptor & peer_acceptor,
                           Agent_Context & context)
{
  // more than enough room for a stringenized port
  char port_buf[16];
  u_short server_port;

  // attempt to open a port from ACCEPTOR_PORT_START to the first available one
  for (server_port = Madara::ACCEPTOR_PORT_START;
       peer_acceptor.open (ACE_INET_Addr (server_port), 
                                ACE_Reactor::instance ()) == -1; ++server_port);

  // convert the server_port into string form for our context
  itoa (server_port, port_buf, 10);

  // update context for caller to know the bound port
  context.setPort (port_buf);

  return 0;
}

#endif