

#include "ace/OS.h"
#include "ace/Log_Msg.h"
#include "ace/SOCK_Connector.h"

#include "Agent_Messages.h"
#include "Madara_Common.h"
#include "Query_Latencies.h"


int Madara::query_latencies_by_key (
  Madara::Broker_Context& context, std::string key)
{
  Agent_Ping query;
  Agent_Ping response;
  Agent_Latency * latencies = 0;

  ::size_t num_latencies = 0;
  ::ssize_t result = 0;

  std::string host;
  std::string port;
  std::string target;

  // split the user provided key into a host and port
  Madara::split_key (key, host, port);
  
  u_short server_port     = ACE_OS::atoi (port.c_str ());

  // create an address that ACE can use from our host/port
  ACE_INET_Addr addr (server_port, host.c_str ());

  ACE_SOCK_Stream worker;
  ACE_SOCK_Connector connector;

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Attempting worker latency query (host %p, port %d)\n", 
    host.c_str (), server_port));
  
  if (connector.connect (worker, addr) == -1)
    {
      ACE_DEBUG ((LM_DEBUG, "%p\n", "open"));
      return result;
    }

  query.size = 0;
  query.type = Madara::BROKER_LATENCY_QUERY;

  // send a latency query to the worker at the host/port
  result = worker.send_n (&query, sizeof (query));

  if (result == -1)
    {
      ACE_DEBUG ((LM_DEBUG, "Problem sending query to %p\n", key.c_str ()));
      return result;
    }

  // recv reply back

  result = worker.recv_n (&response, sizeof(response));

  if (result == -1)
    {
      ACE_DEBUG ((LM_DEBUG, 
        "Madara::QLBK. Never heard back from %p\n", key.c_str ()));
      return result;
    }

  if (response.type == Madara::AGENT_LATENCY_QUERY_RESPONSE)
    {
      num_latencies = response.size;

      latencies = (Madara::Agent_Latency *)
        malloc (sizeof (Agent_Latency) * num_latencies);

      result = worker.recv_n (latencies, sizeof (Agent_Latency) * num_latencies);

      if (result == -1)
        {
          ACE_DEBUG ((LM_DEBUG, 
            "Madara::QLBK. Failed while receiving latencies from %p\n", 
            key.c_str ()));
          return result;
        }

      for (::size_t i = 0; i < num_latencies; ++i)
        {
          Madara::merge_key (target, latencies[i].host, latencies[i].port);
          context.addLatency (key, target, latencies[i].latency);
        }
    }
  
  return 0;
}