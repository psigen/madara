
// C++
#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <string>
#include "ace/OS.h"
#include "ace/Log_Msg.h"
#include "ace/SOCK_Connector.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"
#include "Agent_Messages.h"
#include "Broker_Context.h"
#include "Madara_Common.h"

typedef std::pair <int, int> LatencyRecord;
typedef std::vector <LatencyRecord> Latencies;
typedef std::vector <Latencies>  AllLatencies;
typedef std::vector <int> ActualDeployment;

static bool SortByPairValue (const LatencyRecord & u, const LatencyRecord & v)
{
  return u.second < v.second;
}

static bool SortBySize (const Latencies & u, const Latencies & v)
{
  return u.size () > v.size ();
}

void testDeploymentSize (int size, std::ostream & output)
{
  Madara::Broker_Context context;
  Madara::Deployment::Candidate deployment;

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

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

  timer.start ();

  deployment = context.learnDeployment ();

  // keep track of time

  timer.stop ();
  timer.elapsed_time (measured);
  unsigned long long nanoseconds = measured;


  context.writeDeploymentCandidate (output, deployment);

  output << "Broadcast deployment (" << size << ") took " << nanoseconds
         << " ns" << std::endl;

}

void testRawHeuristic (int size, std::ostream & output)
{

  // latencies
  AllLatencies allLatencies;
  Latencies averages;
  AllLatencies deployment;
  ActualDeployment solution;

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer averaging;
  ACE_High_Res_Timer sorting;
  ACE_High_Res_Timer solving;
  unsigned long long averaging_ns, sorting_ns, solving_ns;


  allLatencies.resize (size);
  averages.resize (size);
  deployment.resize (size);


  // allocate memory to hold individual latencies
  // and then set up random latencies between 10-110
  for (int i = 0; i < size; ++i)
  {
    allLatencies[i].resize (size);

    for (int j = 0; j < size; ++j)
    {
      allLatencies[i][j].first = j;
      allLatencies[i][j].second = rand () % 100 + 10;
    }

    std::sort (allLatencies[i].begin (), allLatencies[i].end (), SortByPairValue);
  }

  // second element of the deployment is a broadcasting node to everyone
  deployment[1].resize (size);
  for (int i = 0; i < size; ++i)
  {
    deployment[1][i].first = 1;
    deployment[1][i].second = i;
  }

  // first element of the deployment sends stuff to 0-3
  deployment[0].resize (4);
  for (int i = 0; i < 4; ++i)
  {
    deployment[0][i].first = 0;
    deployment[0][i].second = i;
  }

  // sort the deployment by degree
  std::sort (deployment.begin (), deployment.end (), SortBySize);

  averaging.start ();

  // average the latencies
  for (int i = 0; i < size; ++i)
  {
    averages[i].first = i;
    averages[i].second = 0;
    for (int j = 0; j < size; ++j)
    {
      averages[i].second += allLatencies[i][j].second;
    }
    averages[i].second /= size;
  }

  averaging.stop ();
  averaging.elapsed_time (measured);

  averaging_ns = measured;


  sorting.start ();

  std::sort (averages.begin (), averages.end (), SortByPairValue);

  sorting.stop ();
  sorting.elapsed_time (measured);

  sorting_ns = measured;



  solving.start ();

  // for each process that a user wants deployed, start with
  // largest
  for (int i; i < deployment.size (); ++i)
  {
    solution[i] = averages[i].first;
  }

  solving.stop ();
  solving.elapsed_time (measured);

  solving_ns = measured;


  output << "  Time required to average all latencies with size (" 
         << size << ") took " << averaging_ns
         << " ns" << std::endl;

  output << "  Time required to sort all degree latencies with size (" 
         << size << ") took " << sorting_ns
         << " ns" << std::endl;

  output << "  Time required to solve deployment by degree with size (" 
         << size << ") took " << solving_ns
         << " ns" << std::endl;


  output << "  Network provided:\n"; 
  for (int i = 0; i < size; ++i)
  {
    output << "    " << averages[i].first << " had avg latency of " 
       << averages[i].second << "\n";
  }

  output << "  Deployment request:\n"; 
  for (int i = 0; i < size; ++i)
  {
    if (deployment[i].size () > 0)
    {
      output << "    " << deployment[i][0].first << " had degree of " 
        << deployment[i].size () << " so set to " << averages[i].first << "\n";
    }
  }

}

int main (int argc, char *argv[])
{
  std::ofstream output ("test_results.txt");
  Madara::Broker_Context context;
  Madara::Deployment::Candidate deployment;
  int i = 20;
  int count = 1000;

  // use ACE real time scheduling class
  int prio  = ACE_Sched_Params::next_priority
    (ACE_SCHED_FIFO,
     ACE_Sched_Params::priority_max (ACE_SCHED_FIFO),
     ACE_SCOPE_THREAD);
  ACE_OS::thr_setprio (prio);

  srand (time (0));
  
  for (i = 1000; i <= 4000; i += count)
    {
      output << "Testing deployment of size " << i << std::endl;
      testRawHeuristic (i, output);
    }

  output.close ();

  return 0;
}