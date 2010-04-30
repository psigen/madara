
// C++
#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <string>
#include "ace/OS.h"
#include "ace/Log_Msg.h"
#include "ace/SOCK_Connector.h"
#include "Agent_Messages.h"
#include "Broker_Context.h"
#include "Madara_Common.h"

void testDeploymentSize (int size, std::ostream & output)
{
  Madara::Broker_Context context;
  Madara::Deployment::Candidate deployment;
  ACE_hrtime_t start, end;
  unsigned int total;
  int i = 0, j = 0;

  std::vector <std::string> keys (size);

  keys.resize (size);

  char buf[18];
  strcpy (buf,"127.0.0.1:");

  for (i = 1; i < size; ++i)
    context.addRequirement (0, i);

  for (i = Madara::WORKER_PORT_START + 0, j = 0; j < size; ++i, ++j)
    {
      itoa (i, buf + 10, 10);
      keys[j] = buf;
    }

  for (i = 0; i < size; ++i)
    {
      for (j = 0; j < size; ++j)
        {
          context.addLatency (keys[i], keys[j], rand () % 100 + 10);
        }
    }

  //context.write (output);

  // keep track of time
  start = ACE_OS::gethrtime ();

  deployment = context.learnDeployment ();

  // keep track of time
  end = ACE_OS::gethrtime ();
  total = (end - start) / 1000;

  context.writeDeploymentCandidate (output, deployment);

  output << "Broadcast deployment (" << size << ") took " << total
         << " us" << std::endl;

}

int main (int argc, char *argv[])
{
  std::ofstream output ("test_results.txt");
  Madara::Broker_Context context;
  Madara::Deployment::Candidate deployment;
  ACE_hrtime_t start, end;
  unsigned int total;
  int i = 20;
  int count = 20;

  srand (time (NULL));
  
  for (i = 20; i <= 200; i += count)
    {
      std::cout << "Testing deployment of size " << i << std::endl;
      testDeploymentSize (i, std::cerr);
    }

  return 0;
}