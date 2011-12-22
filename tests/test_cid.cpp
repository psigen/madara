
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"

#include "madara/cid/Convenience.h"
#include "madara/cid/Genetic.h"
#include "madara/cid/Heuristic.h"

void print_stats (std::ostream & output, unsigned int size,
  unsigned long long preparing_ns, unsigned long long solving_ns,
  unsigned long long naive_solving_ns, unsigned long long ga_naive_solving_ns,
  unsigned long long ga_degree_solving_ns,
  unsigned long long random_latency,
  unsigned long long naive_latency, unsigned long long cid_latency, 
  unsigned long long ga_naive_latency, unsigned long long ga_degree_latency)
{

  std::locale loc(""); 
  output.imbue (loc); 

  std::stringstream buffer;
  buffer.imbue (loc);
  buffer << "Solving deployment of size " << size;
  unsigned int dist = (51 - buffer.str ().size ()) / 2;

  output << "========================================================\n";
  output << std::setw (52 - dist) << buffer.str () << "\n";
  output << "========================================================\n";
  output << "            " << std::setw (20) << "Single Node (ns)" << std::setw (24) << "Distributed (ns)" << "\n";
  output << "  Prep time " << std::setw (20) << preparing_ns << std::setw (24) << (preparing_ns / size) << "\n";
  output << "========================================================\n";  
  output << " Technique" << std::setw (21) << "Time (ns)" << std::setw (24) << "Total Latency (us)" << "\n";  
  output << "   Initial" << std::setw (21) << 0 << std::setw (24) << random_latency << "\n";
  output << "       CID" << std::setw (21) << solving_ns << std::setw (24) << cid_latency << "\n";
  output << "     Naive" << std::setw (21) << naive_solving_ns << std::setw (24) << naive_latency << "\n";
  output << "  GA Naive" << std::setw (21) << ga_naive_solving_ns << std::setw (24) << ga_naive_latency << "\n";
  output << " GA Degree" << std::setw (21) << ga_degree_solving_ns << std::setw (24) << ga_degree_latency << "\n";
  output << "========================================================\n\n";
}

void test_cid (unsigned int size, std::ostream & output)
{

  Madara::Cid::Settings settings;

  // initialize network
  Madara::Cid::init (size, settings);
  Madara::Cid::generate_random_network (size, settings);

  // create the target workflow
  settings.target_deployment.resize (size);
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
  ACE_High_Res_Timer ga_naive_solving;
  ACE_High_Res_Timer ga_degree_solving;
  unsigned long long preparing_ns, solving_ns, naive_solving_ns,
    ga_naive_solving_ns, ga_degree_solving_ns;

  // sort the deployment by degree
  Madara::Cid::prepare_deployment (settings);

  preparing.start ();

  // sort and average the latencies from each vertex
  Madara::Cid::prepare_latencies (settings);

  preparing.stop ();
  preparing.elapsed_time (measured);

  preparing_ns = measured;

  solving.start ();

  Madara::Cid::approximate (settings);

  solving.stop ();
  solving.elapsed_time (measured);

  solving_ns = measured;

  unsigned long long cid_latency = 
    Madara::Cid::calculate_latency (settings);

  naive_solving.start ();

  Madara::Cid::fill_by_highest_degree (settings, 0);

  naive_solving.stop ();
  naive_solving.elapsed_time (measured);
  naive_solving_ns = measured;

  unsigned long long naive_latency = 
    Madara::Cid::calculate_latency (settings);

  for (unsigned int i = 0; i < settings.solution.size (); ++i)
    settings.solution[i] = i;

  unsigned long long random_latency = 
    Madara::Cid::calculate_latency (settings);

  ga_naive_solving.start ();

  // attempt up to 5 mutations, 400 times to improve
  for (unsigned int i = 0; i < 400; ++i)
    Madara::Cid::ga_naive (settings, 5);

  ga_naive_solving.stop ();
  ga_naive_solving.elapsed_time (measured);
  ga_naive_solving_ns = measured;

  unsigned long long ga_naive_latency = 
    Madara::Cid::calculate_latency (settings);

  for (unsigned int i = 0; i < settings.solution.size (); ++i)
    settings.solution[i] = i;

  ga_degree_solving.start ();

  // attempt up to 5 mutations, 10 times to improve
  for (unsigned int i = 0; i < 300; ++i)
    Madara::Cid::ga_degree (settings, 5);

  ga_degree_solving.stop ();
  ga_degree_solving.elapsed_time (measured);
  ga_degree_solving_ns = measured;

  unsigned long long ga_degree_latency = 
    Madara::Cid::calculate_latency (settings);

  print_stats (output, size, preparing_ns, solving_ns, naive_solving_ns, 
    ga_naive_solving_ns, ga_degree_solving_ns,
    random_latency,
    naive_latency, cid_latency, ga_naive_latency, ga_degree_latency);

  //output << "  Time required for a single node to prepare all latencies"
  //       << " with size (" << size << ") took " << preparing_ns
  //       << " ns" << std::endl;

  //output << "  Time required for " << size << 
  //          " nodes to prepare all latencies"
  //       << " with size (" << size << ") took " << (preparing_ns / size)
  //       << " ns" << std::endl;

  //output << "  Time required to naively solve by highest degree with size (" 
  //       << size << ") took " << naive_solving_ns
  //       << " ns" << std::endl;

  //output << "  Time required to solve deployment by degree with size (" 
  //       << size << ") took " << solving_ns
  //       << " ns" << std::endl;




  //Madara::Cid::Latency_Vector & cur_averages = 
  //  settings.network_averages[settings.target_deployment[0].size ()];

  //output << "  Network provided:\n"; 
  //for (unsigned int i = 0; i < size; ++i)
  //{
  //  output << "    " << cur_averages[i].first << 
  //    " had avg latency of " 
  //    << cur_averages[i].second << "\n";
  //}

  //output << "  Deployment request:\n"; 
  //for (unsigned int i = 0; i < settings.target_deployment.size (); ++i)
  //{
  //  if (settings.target_deployment[i].size () > 0)
  //  {
  //    output << "    " << settings.target_deployment[i][0].first << 
  //      " had degree of " << settings.target_deployment[i].size () <<
  //      " so set to " << settings.solution[i] << "\n";
  //  }
  //}

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

  output << "\n**Testing CID heuristic with 1,000->10,000 elements**\n\n";
  for (unsigned int i = 1000; i <= 10000; i += 1000)
    {
      //output << "Testing deployment of size " << i << std::endl;
      test_cid (i, std::cerr);
    }

  output.close ();

  return 0;
}