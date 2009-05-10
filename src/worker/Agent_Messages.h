#ifndef AGENT_MESSAGES_H
#define AGENT_MESSAGES_H


namespace Madara
  {
    class Agent_Context;

    enum size_t
      {
        AGENT_PING = 1,
        AGENT_DISCOVER = 2,
        AGENT_LATENCY_QUERY_RESPONSE = 3,
        AGENT_LATENCY_DESCRIBE = 10,
        AGENT_DUMP_CONTEXT = 20,
        BROKER_LATENCY_QUERY = 100
      };

    typedef struct
    {
      unsigned int size;
      unsigned int type;
      char host[16];
      unsigned int port;
    } Agent_Ping;

    typedef struct
    {
      char host[16];
      unsigned int port;
      unsigned int latency;
    } Agent_Latency;

    void fillPingFromContext (Agent_Ping & ping, 
                              Agent_Context & context);
  }
#endif