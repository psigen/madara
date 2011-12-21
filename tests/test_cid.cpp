
#include <fstream>
#include <iostream>

#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"

#include "madara/cid/Heuristic.h"
#include "madara/cid/Convenience.h"

void test_cid (unsigned int size, std::ostream & output)
{
  output << "  Entering test cid\n";

  Madara::Cid::Settings settings;

  output << "  Initializing settings for " << size << " elements\n";

  Madara::Cid::init (size, settings);

  output << "  Generating random network of " << size << " latencies\n";

  Madara::Cid::generate_random_network (size, settings);

  output << "  Generating deployment with big broadcaster to " <<
            size << " nodes\n";

  settings.target_deployment.resize (size);
  // second element of the deployment is a broadcasting node to everyone
  settings.target_deployment[1].resize (size);
  for (unsigned int i = 0; i < size; ++i)
  {
    settings.target_deployment[1][i].first = 1;
    settings.target_deployment[1][i].second = i;
  }

  // first element of the deployment sends stuff to 0-3
  settings.target_deployment[0].resize (4);
  for (unsigned int i = 0; i < 4; ++i)
  {
    settings.target_deployment[0][i].first = 0;
    settings.target_deployment[0][i].second = i;
  }

  // keep track of time
  ACE_hrtime_t measured;
  ACE_High_Res_Timer preparing;
  ACE_High_Res_Timer solving;
  ACE_High_Res_Timer naive_solving;
  unsigned long long preparing_ns, solving_ns, naive_solving_ns;

  output << "  Preparing deployment\n";

  // sort the deployment by degree
  Madara::Cid::prepare_deployment (settings);

  output << "  Preparing latencies\n";

  preparing.start ();

  // sort and average the latencies from each vertex
  Madara::Cid::prepare_latencies (settings);

  preparing.stop ();
  preparing.elapsed_time (measured);

  preparing_ns = measured;

  output << "  Solving for network latencies and degrees in deployment\n";

  solving.start ();

  Madara::Cid::approximate (settings);

  solving.stop ();
  solving.elapsed_time (measured);

  solving_ns = measured;

  output << "  Solving naively with the highest degree in deployment\n";

  naive_solving.start ();

  Madara::Cid::fill_by_highest_degree (settings, 0);

  naive_solving.stop ();
  naive_solving.elapsed_time (measured);
  naive_solving_ns = measured;


  output << "  Time required for a single node to prepare all latencies"
         << " with size (" << size << ") took " << preparing_ns
         << " ns" << std::endl;

  output << "  Time required for " << size << 
            " nodes to prepare all latencies"
         << " with size (" << size << ") took " << (preparing_ns / size)
         << " ns" << std::endl;

  output << "  Time required to naively solve by highest degree with size (" 
         << size << ") took " << naive_solving_ns
         << " ns" << std::endl;

  output << "  Time required to solve deployment by degree with size (" 
         << size << ") took " << solving_ns
         << " ns" << std::endl;


  Madara::Cid::Latency_Vector & cur_averages = 
    settings.network_averages[settings.target_deployment[0].size ()];

  //output << "  Network provided:\n"; 
  //for (unsigned int i = 0; i < size; ++i)
  //{
  //  output << "    " << cur_averages[i].first << 
  //    " had avg latency of " 
  //    << cur_averages[i].second << "\n";
  //}

  output << "  Deployment request:\n"; 
  for (unsigned int i = 0; i < settings.target_deployment.size (); ++i)
  {
    if (settings.target_deployment[i].size () > 0)
    {
      output << "    " << settings.target_deployment[i][0].first << 
        " had degree of " << settings.target_deployment[i].size () <<
        " so set to " << settings.solution[i] << "\n";
    }
  }

}

int main (int argc, char *argv[])
{
  std::ofstream output ("test_results.txt");

  // use ACE real time scheduling class
  int prio  = ACE_Sched_Params::next_priority
    (ACE_SCHED_FIFO,
     ACE_Sched_Params::priority_max (ACE_SCHED_FIFO),
     ACE_SCOPE_THREAD);
  ACE_OS::thr_setprio (prio);

  srand ((unsigned int) time (0));

  output << "\n**CID heuristic with std::sort**\n";
  for (unsigned int i = 1000; i <= 10000; i += 1000)
    {
      output << "Testing deployment of size " << i << std::endl;
      test_cid (i, output);
    }

  output.close ();

  return 0;
}