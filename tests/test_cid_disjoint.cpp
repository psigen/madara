
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"

#include "madara/cid/Convenience.h"
#include "madara/cid/Genetic.h"
#include "madara/cid/Heuristic.h"
#include "madara/utility/Log_Macros.h"

void genetic_degree (Madara::Cid::Settings & settings)
{
  Madara::Cid::generate_worst_solution (settings);
  for (unsigned int i = 0; i < 400; ++i)
    Madara::Cid::ga_degree (settings, 5);
}

void genetic_naive (Madara::Cid::Settings & settings)
{
  Madara::Cid::generate_worst_solution (settings);
  for (unsigned int i = 0; i < 100; ++i)
    Madara::Cid::ga_naive (settings, 5);
}

void naive_cid_ga_naive (Madara::Cid::Settings & settings)
{  
  Madara::Cid::fill_by_highest_degree (settings);
  for (unsigned int i = 0; i < 400; ++i)
    Madara::Cid::ga_naive (settings, 5);
}

void naive_cid_ga_degree (Madara::Cid::Settings & settings)
{
  Madara::Cid::fill_by_highest_degree (settings);
  for (unsigned int i = 0; i < 100; ++i)
    Madara::Cid::ga_degree (settings, 5);
}

void cid_ga_naive (Madara::Cid::Settings & settings)
{
  Madara::Cid::approximate (settings);
  for (unsigned int i = 0; i < 400; ++i)
    Madara::Cid::ga_naive (settings, 5);
}

void cid_ga_degree (Madara::Cid::Settings & settings)
{
  Madara::Cid::approximate (settings);
  for (unsigned int i = 0; i < 100; ++i)
    Madara::Cid::ga_degree (settings, 5);
}

void hand_coded (Madara::Cid::Settings & settings)
{
  Madara::Cid::Workflow & deployment = settings.target_deployment;
  Madara::Cid::Averages_Map & averages = settings.network_averages;
  Madara::Cid::Solution_Map & lookup = settings.solution_lookup;

  unsigned int fill_starting = deployment.size ();
  unsigned int first_degree = deployment[0].size ();
  unsigned int second_degree = deployment[1].size ();

  if (first_degree > 1 && second_degree > 1)
  {
    // set node 1 to the best average latency from the highest degree
    settings.solution[deployment[0][0].first] = 
      averages[first_degree][0].first;

    lookup[averages[first_degree][0].first] = deployment[0][0].first;

    // set node 0 to either the best latency from degree 4 or to the
    // second best latency in 4, if solution[1] was the same
    if (averages[first_degree][0].first != averages[second_degree][0].first)
    {
      settings.solution[deployment[1][0].first] = 
        averages[second_degree][0].first;

      lookup[averages[second_degree][0].first] = deployment[1][0].first;
    }
    else
    {
      settings.solution[deployment[1][0].first] = 
        averages[first_degree][1].first;

      lookup[averages[first_degree][0].first] = deployment[1][0].first;
    }
  }
  else
  {
    fill_starting = 0;
  }

  Madara::Cid::fill_by_highest_degree (settings, fill_starting);
}

#define NUM_TESTS           9
#define PREPARATION         9
#define MIN_TEST_RANGE      0
#define MAX_TEST_RANGE      9

char * testnames [] = {
  "CID Heuristic",
  "Naive CID Heuristic",
  "Naive Genetic Algorithm",
  "Degreed Genetic Algorithm",
  "CID-NGA",
  "CID-DGA",
  "Naive CID-NGA",
  "Naive CID-DGA",
  "Random Deployment",
};


void (* test_impls []) (Madara::Cid::Settings &) = {
  Madara::Cid::approximate,
  Madara::Cid::fill_by_highest_degree,
  genetic_naive,
  genetic_degree,
  ::cid_ga_naive,
  ::cid_ga_degree,
  ::naive_cid_ga_naive,
  ::naive_cid_ga_degree,
  Madara::Cid::generate_worst_solution,

};

unsigned long long clocks [NUM_TESTS + 1];
unsigned long long latencies [NUM_TESTS];

void print_averages (std::ostream & output, 
                     Madara::Cid::Settings & settings)
{
  std::locale loc(""); 
  output.imbue (loc); 

  std::string divider (79, '=');

  std::vector <unsigned int> degrees;
  degrees.resize (settings.network_averages.size ());
  unsigned int size = settings.solution.size ();

  output << "\nPrinting averages by degree for tests.\n";
  output << divider << std::endl;

  Madara::Cid::Averages_Map & averages = settings.network_averages;

  unsigned int j = 0;

  for (Madara::Cid::Averages_Map::iterator i = averages.begin ();
                                           i != averages.end (); ++i)
  {
    if (i->first)
    {
      degrees[j++] = i->first;

      output << std::setw (15) << i->first;
    }
  }

  output << std::endl;

  for (unsigned int i = 0; i < size; ++i)
  {
    for (j = 0; j < degrees.size (); ++j)
    {
      if (averages[j].size () > i)
      {
        std::stringstream buffer;
        buffer << averages[j][i].first << "=" << averages[j][i].second;

        output << std::setw (15) << buffer.str ();
      }
      else
        output << std::setw (15) << "N/A";
    }

    output << std::endl;
  }
}

void print_stats (std::ostream & output, unsigned int size)
{
  std::string divider (79, '=');

  std::locale loc(""); 
  output.imbue (loc); 

  output << "\n Statistics for tests on size " << size << "\n";


  output << divider << "\n";

  output << std::setw (30) << "            " << 
            std::setw (25) << "Single Node (ns)" << 
            std::setw (24) << "Distributed (ns)" << "\n";

  output << divider << "\n";

  output << std::setw (30) << "  Prep time " << 
            std::setw (25) << clocks[PREPARATION] <<
            std::setw (24) << clocks[PREPARATION] / size << "\n\n";


  output << divider << "\n";
  output << std::setw (30) << " Technique" << 
            std::setw (25) << "Time (ns)" << 
            std::setw (24) << "Total Latency (us)" << "\n"; 

  output << divider << "\n";

  for (int i = MIN_TEST_RANGE; i < MAX_TEST_RANGE; ++i)
  {
    output << std::setw (30) << testnames[i] << 
              std::setw (25) << clocks[i] <<
              std::setw (24) << latencies[i] << "\n";
  }

  output << divider << "\n";
}

void prepare_latencies (Madara::Cid::Settings & settings)
{
  // high res timers provided by ACE
  ACE_hrtime_t measured;
  ACE_High_Res_Timer timer;

  timer.start ();

  // sort and average the latencies from each vertex
  Madara::Cid::prepare_latencies (settings);

  timer.stop ();
  timer.elapsed_time (measured);

  clocks[PREPARATION] = measured;
}

void print_solutions (std::ostream & output, Madara::Cid::Settings & settings,
       unsigned int test_type, unsigned int size)
{
  output << std::setw (30) << testnames[test_type];
  output << ":\n";

  for (unsigned int i = 0; i < settings.target_deployment.size (); ++i)
  {
    unsigned int degree = settings.target_deployment[i].size ();
    if (degree > 0)
      output << "  needed to solve solution[" 
             << settings.target_deployment[i][0].first
             << "] (degree=" << degree << ")\n";
  }

  for (unsigned int i = 0; i < size; ++i)
  {

    output << "  solution[" << i << "] is " << settings.solution[i] << "\n"; 
  }
  output << "\n";
}

void generate_fanout_latencies (unsigned int fan_outs,
                                Madara::Cid::Settings & settings)
{
  Madara::Cid::LV_Vector & latencies = settings.network_latencies;

  // for all nodes, set the initial latencies between all links to 1 second
  for (unsigned int i = 0; i < latencies.size (); ++i)
  {
    for (unsigned int j = 0; j < latencies[i].size (); ++j)
    {
      latencies[i][j].first = j;
      latencies[i][j].second = 1000000000;
    }
  }

  unsigned int size = latencies.size () / fan_outs;

  // now, setup the processes that are optimal for sending out info to 500 us
  for (unsigned int i = 0; i < fan_outs; ++i)
  {
    unsigned int source = i * size;
    unsigned int begin = i * size;
    unsigned int end = (i + 1) * size;

    // the last segment is the biggest
    if (i == fan_outs - 1)
      end = latencies.size () - 1;

    for (unsigned int j = begin; j < end; ++j)
    {
      latencies[source][j].second = 500000;
    }
  }

  for (unsigned int i = 0; i < latencies.size (); ++i)
    std::sort (latencies[i].begin (), latencies[i].end (),
      Madara::Cid::Increasing_Latency);


  //std::cerr << "\n\nPrinting latencies for size " << latencies.size () <<
  //             " and fanouts " << fan_outs << "\n\n";
  //Madara::Cid::print_latencies (settings);
}

void test_cid (unsigned int size, std::ostream & output)
{
  std::string test_divider (79, '*');

  output << "\n\nTesting deployment of size " << size << std::endl;

  Madara::Cid::Settings settings;

  // initialize network
  Madara::Cid::init (size, settings);

  // test even-sized fanouts from 1-4 source nodes targeting disjoint nodes
  for (unsigned int fan_outs = 1; fan_outs <= 4; ++fan_outs)
  {
    // create filename 
    std::stringstream filename;
    filename << getenv ("MADARA_ROOT");
    filename << "/configs/cid/deployments/test_cid/";
    filename << fan_outs << "_even_fans_disjoint.template";

    generate_fanout_latencies (fan_outs, settings);

    // notify user of current est
    output << test_divider << std::endl;
    std::cout << "  Testing " << fan_outs << " even-sized, disjoint fanouts\n";
    output << "  Testing " << fan_outs << " even-sized, disjoint fanouts\n";
    output << test_divider << std::endl;

    settings.solution.resize (size);
    Madara::Cid::read_deployment (settings, filename.str ());

    output << "  Preparing latencies\n";
    settings.network_averages.clear ();
    ::prepare_latencies (settings);


    // iterate through all tests on this constraint satisfaction problem
    for (unsigned int i = MIN_TEST_RANGE; i < MAX_TEST_RANGE; ++i)
    {
      settings.solution_lookup.clear ();

      // high res timers provided by ACE
      ACE_hrtime_t measured;
      ACE_High_Res_Timer timer;

      output << "  Approximating with " << testnames[i] << std::endl;
      std::cout << "    Approximating with " << testnames[i] << std::endl;


      timer.start ();

      // sort and average the latencies from each vertex
      test_impls [i] (settings);

      timer.stop ();
      timer.elapsed_time (measured);

      // save wall clock time that passed during the test
      clocks[i] = measured;

      // save the overall utility of the solution (less is better)
      latencies[i] = Madara::Cid::calculate_latency (settings);

      if (Madara::Cid::check_solution (settings))
        output << "  " << testnames[i] << " appears to be broken. " <<
                  " Please report this at madara.googlecode.com\n";

      output << "Solution was...\n";
      print_solutions (output, settings, i, size);
    }

    print_stats (output, size);
  } // end even-sized fanouts


}

void verify_algorithms (std::ostream & output)
{

  // *************************************************************

  #define CHECKED_ALGORITHMS      6
  #define WORST_LATENCY           0
  #define BEST_LATENCY            1
  #define CID                     2
  #define NAIVE_CID               3
  #define DEGREED_GA              4
  #define NAIVE_GA                5

  unsigned long long alg_latencies[CHECKED_ALGORITHMS] = {0,0,0,0,0,0};

  std::string divider (79, '*');
  output << divider << std::endl;
  output << "Verifying that the algorithms are correct.\n";
  output << divider << std::endl;

  Madara::Cid::Settings settings;

  Madara::Cid::init (10, settings);

  Madara::Cid::Workflow & deployment = settings.target_deployment;
  Madara::Cid::LV_Vector & network_latencies = settings.network_latencies;
  Madara::Cid::Latency_Vector & averages = settings.network_averages[10];
  Madara::Cid::Deployment & solution = settings.solution;
  Madara::Cid::Solution_Map & lookup = settings.solution_lookup;

  deployment.resize (10);
  deployment[0].resize (10);
  averages.resize (10);

  for (unsigned int i = 0; i < 10; ++i)
  {
    deployment[0][i].first = 0;
    deployment[0][i].second = i;
  }

  for (unsigned int i = 0; i < 10; ++i)
  {
    Madara::Cid::Latency_Vector & source_latencies = network_latencies[i];
    averages[i].first = i;

    for (unsigned int j = 0; j < 10; ++j)
    {
      if (network_latencies[i][j].second == 0)
      {
        Madara::Cid::Latency_Vector & dest_latencies = network_latencies[j];

        // make the latencies bidirectionally equivalent for realism
        source_latencies[j].first = j;
        source_latencies[j].second = j + 1;
        dest_latencies[i].first = i;
        dest_latencies[i].second = source_latencies[j].second;
      }
      averages[i].second += source_latencies[j].second;
    }
    averages[i].second /= 10;

    std::sort (source_latencies.begin (), source_latencies.end ());
  }

  output << "\nPrinting the deployment\n\n";
  output << "  ";

  for (unsigned int i = 0; i < 10; ++i)
  {
    std::stringstream buffer;
    buffer << "0->" << deployment[0][i].second;

    output << std::setw (5) << buffer.str ();

    if (i != 9)
      output << " ";
    else
      output << "\n";
  }

  output << "\nPrinting the network latencies\n\n  ";

  for (unsigned int i = 0; i < 10; ++i)
  {
    Madara::Cid::Latency_Vector & cur_latencies = network_latencies[i];

    for (unsigned int j = 0; j < 10; ++j)
    {
      output << "[" << i << "]" << "[" << j << "]=" <<
        std::setw (3) << cur_latencies[j].second;

      if (j % 5 == 4)
        output << "\n  ";
      else
        output << " ";
    }
  }

  output << "\nPrinting the averages\n";

  for (unsigned int i = 0; i < 10; ++i)
  {
    output << i << "->" << averages[i].second << " ";
  }

  output << "\n";

  Madara::Cid::generate_worst_solution (settings);

//  unsigned long long actual_latency, ideal_latency = 0, worst_latency;

  alg_latencies[WORST_LATENCY] = Madara::Cid::calculate_latency (settings);

  Madara::Cid::Latency_Vector & cur_latencies = network_latencies[0];

  for (unsigned int i = 0; i < 10; ++i)
  {
    alg_latencies[BEST_LATENCY] += cur_latencies[i].second;
  }

  output << "\nWorst case latency: " << alg_latencies[WORST_LATENCY] << "\n";
  output << "\nBest case latency: " << alg_latencies[BEST_LATENCY] << "\n";

  // Test CID

  Madara::Cid::generate_worst_solution (settings);
  lookup.clear ();

  Madara::Cid::approximate (settings);

  alg_latencies[CID] = 
    Madara::Cid::calculate_latency (settings);

  output << "Cid solution latency: " << alg_latencies[CID] << "\n";

  // Test Naive CID

  Madara::Cid::generate_worst_solution (settings);
  lookup.clear ();

  Madara::Cid::fill_by_highest_degree (settings);

  alg_latencies[NAIVE_CID] = Madara::Cid::calculate_latency (settings);

  output << "Naive Cid solution latency: " << alg_latencies[NAIVE_CID] << "\n";

  // Test Degreed-GA

  Madara::Cid::generate_worst_solution (settings);

  for (unsigned int i = 0; i < 10; ++i)
    Madara::Cid::ga_degree (settings, 5);

  alg_latencies[DEGREED_GA] = Madara::Cid::calculate_latency (settings);

  output << "Degreed GA solution latency: " << alg_latencies[DEGREED_GA] << "\n";

  // Test Naive-GA

  Madara::Cid::generate_worst_solution (settings);

  for (unsigned int i = 0; i < 10; ++i)
    Madara::Cid::ga_naive (settings, 5);

  alg_latencies[NAIVE_GA] = Madara::Cid::calculate_latency (settings);

  output << "Naive GA solution latency: " << alg_latencies[NAIVE_GA] << "\n";

  output << "The following algorithms are incorrect: \n";
  
  if (alg_latencies[WORST_LATENCY] != 100)
    output << "  generate_worst_solution\n";

  if (alg_latencies[CID] != 55)
    output << "  approximate (i.e. CID heuristic)\n";

  if (alg_latencies[NAIVE_CID] != 55)
    output << "  fill_by_highest_degree (i.e. Naive CID heuristic)\n";

  if (alg_latencies[NAIVE_GA] > 70)
    output << "  ga_naive (i.e. Naive Genetic Algorithm)\n";

  if (alg_latencies[DEGREED_GA] != 55)
    output << "  ga_degreed (i.e. Degreed Genetic Algorithm)\n";

  return;

  // *************************************************************

}

int main (int argc, char *argv[])
{
  MADARA_debug_level = 1;

  unsigned int begin = 20;
  unsigned int end = 100;
  unsigned int increment = 20;
  std::string output_file ("test_results.txt");

  for (int i = 1; i < argc; ++i)
  {
    std::string arg (argv[i]);
    std::stringstream buffer;

    if      (i + 1 < argc && arg == "-b" || arg == "--begin")
    {
      buffer << argv[i + 1];
      buffer >> begin;
      ++i;
    }
    else if (i + 1 < argc && arg == "-e" || arg == "--end")
    {
      buffer << argv[i + 1];
      buffer >> end;
      ++i;
    }
    else if (i + 1 < argc && arg == "-i" || arg == "--increment")
    {
      buffer << argv[i + 1];
      buffer >> increment;
      ++i;
    }
    else if (i + 1 < argc && arg == "-o" || arg == "--output-file")
    {
      buffer << argv[i + 1];
      buffer >> output_file;
      ++i;
    }
    else
    {
      buffer << "\nHelp for " << argv[0] << "\n\n";
      buffer << "  This application tests the CID heuristics and genetic\n";
      buffer << "  algorithms on disjoint deployments.\n\n";
      buffer << "Usage information for " << argv[0] << "\n\n";
      buffer << "  -b <value> || --begin <value>\n";
      buffer << "      First size to try. Default is 1000 (1,000)\n\n";
      buffer << "  -e <value> || --end <value>\n";
      buffer << "      Last size to try. Default is 10000 (10,000)\n\n";
      buffer << "  -h  || --help\n";
      buffer << "      Print this help menu\n\n";
      buffer << "  -i <value> || --increment <value>\n";
      buffer << "      Increment size by. Default is 1000 (1,000)\n\n";
      buffer << "  -o <filename> || --output-file <filename>\n";
      buffer << "      Print output to the filename."
             << " Default is test_results.txt\n\n";

      std::cout << buffer.str ();
      exit (0);
    }
  }

  std::ofstream output (output_file.c_str ());

  // use ACE real time scheduling class
  int prio  = ACE_Sched_Params::next_priority
    (ACE_SCHED_FIFO,
     ACE_Sched_Params::priority_max (ACE_SCHED_FIFO),
     ACE_SCOPE_THREAD);
  ACE_OS::thr_setprio (prio);

  srand ((unsigned int) time (0));

  //verify_algorithms (output);

  std::cout << "\nSaving results to " << output_file << "\n";

  for (unsigned int i = begin; i <= end; i += increment)
  {
    std::cout << "  Running tests for size " << i << "\n";
    test_cid (i, output);
  }

  output.close ();

  return 0;
}