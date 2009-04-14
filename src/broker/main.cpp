
// C++
#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <string>
#include "ace/OS.h"
#include "ace/Log_Msg.h"
#include "ace/SOCK_Connector.h"
#include "../Madara/Agent_Messages.h"
#include "Broker_Context.h"

void testDeploymentSize (int size, std::ostream & output)
{
  Madara::Broker_Context context;
  Madara::DeploymentCandidate deployment;
  ACE_hrtime_t start, end;
  unsigned int total;
  int i = 0, j = 0;

  std::vector <std::string> keys (size);

  char buf[16];

  for (i = 1; i < size; ++i)
    context.addRequirement (0, i);

  for (i = 0; i < size; ++i)
    {
      itoa (i, buf, 10);
      keys[i] = buf;
    }

  for (i = 0; i < size; ++i)
    {
      for (j = 0; j < size; ++j)
        {
          context.addLatency (keys[i], keys[j], rand () % 100 + 10);
        }
    }

  // keep track of time
  start = ACE_OS::gethrtime ();

  deployment = context.learnDeployment ();

  // keep track of time
  end = ACE_OS::gethrtime ();
  total = (end - start) / 1000;

  output << "Broadcast deployment (" << size << ") took " << total
         << " us" << std::endl;

}

int main (int argc, char *argv[])
{
  std::ofstream output ("test_results.txt");
  Madara::Broker_Context context;
  Madara::DeploymentCandidate deployment;
  ACE_hrtime_t start, end;
  unsigned int total;
  int i = 20;
  int count = 20;

  srand (time (NULL));
  
  for (i = 20; i <= 10000; i += count)
    {
      std::cout << "Testing deployment of size " << i << std::endl;
      testDeploymentSize (i, output);
    }

  return 0;
}