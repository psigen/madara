
#include <algorithm>
#include "Genetic.h"
#include "Heuristic.h"


void
Madara::Cid::ga_naive (Settings & settings, unsigned int max_mutations)
{
  // how many mutations are we going to try?
  unsigned int actual_mutations = rand () % max_mutations + 1;

  // create an exact copy of the solution vector
  Deployment current;
  current.resize (settings.solution.size ());
  std::copy (settings.solution.begin (), settings.solution.end (), 
    current.begin ());

  unsigned long long orig_latency = calculate_latency (settings);

  for (unsigned int i = 0; i < actual_mutations; ++i)
  {
    // generate some candidates for mutating
    unsigned int candidate1 = rand () % settings.solution.size ();
    unsigned int candidate2 = rand () % settings.solution.size ();

    // loop until we have two different candidates
    while (candidate1 == candidate2)
      candidate1 = rand () % settings.solution.size ();

    // attempt the swap
    std::swap (current[candidate1], current[candidate2]);

    unsigned long long new_latency = calculate_latency (
      settings.network_latencies, settings.target_deployment, current);
   
    if (new_latency < orig_latency)
    {
      std::copy (current.begin (), current.end (), settings.solution.begin ());
      for (unsigned int i = 0; i < settings.solution.size (); ++i)
      {
        if (settings.solution_lookup[settings.solution[i]] != i)
          settings.solution_lookup[settings.solution[i]] = i;
      }
      return;
    }
  }
}

void Madara::Cid::ga_degree (Settings & settings, unsigned int max_mutations)
{
  if (settings.solution.size () < 2)
    return;

  // how many mutations are we going to try?
  unsigned int actual_mutations = rand () % max_mutations + 1;

  // create an exact copy of the solution vector
  Deployment current;
  current.resize (settings.solution.size ());
  std::copy (settings.solution.begin (), settings.solution.end (), 
    current.begin ());

  unsigned long long orig_latency = calculate_latency (settings);

  unsigned int num_degreed_nodes = 0;
  LV_Vector & deployment = settings.target_deployment;
  Averages_Map & averages = settings.network_averages;
  Solution_Map solution_lookup = settings.solution_lookup;

  // count the number of degreed nodes in the deployment.
  for (unsigned int i = 0; i < deployment.size (); ++i)
  {
    if (deployment[i].size () == 0)
      break;

    ++num_degreed_nodes;
  }

  for (unsigned int i = 0; i < actual_mutations; ++i)
  {
    /**
     * 1/5 times, choose a naive solution.
     * 4/5 times, choose to swap someone from the high degreed candidates
     **/
    int tactic = rand () % 5;
    unsigned int candidate1;
    unsigned int candidate2;
    
    if (tactic == 4)
    {
      // go with a naive solution
      candidate1 = rand () % settings.solution.size ();
      candidate2 = rand () % settings.solution.size ();

      // loop until we have two different candidates
      while (candidate1 == candidate2)
        candidate1 = rand () % settings.solution.size ();
    }
    else
    {
      unsigned int choice = rand () % num_degreed_nodes;
      // use degree and latency information to form ideal candidates
      candidate1 = deployment[choice][0].first;
      
      Latency_Vector & cur_averages = 
        settings.network_averages[deployment[choice].size ()];
  
      /**
       * the other candidate can be from a range:
       * 0-2 : top num_degreed_nodes
       * 3 : top 10%
       * 4 : top 25%
       **/
      int candidate_type = rand () % 5;
      unsigned int range;
      if (candidate_type <= 2)
      {
        // choose candidate2 from the top num_degreed_nodes
        range = cur_averages.size () >= num_degreed_nodes ?
           num_degreed_nodes : cur_averages.size (); 

        candidate2 = solution_lookup[cur_averages[rand () % range].first];
      }
      else if (candidate_type == 3)
      {
        // choose candidate2 from the top 10%
        range = cur_averages.size () / 10; 
        
        candidate2 = solution_lookup[cur_averages[rand () % range].first];
      }
      else
      {
        // choose candidate2 from the top 25%
        range = cur_averages.size () / 4; 
        
        candidate2 = solution_lookup[cur_averages[rand () % range].first];
      }
      // loop until we have two different candidates
      while (candidate1 == candidate2)
        candidate2 = solution_lookup[cur_averages[rand () % range].first];
    }
    // attempt the swap
    std::swap (current[candidate1], current[candidate2]);

    solution_lookup[current[candidate1]] = candidate1;
    solution_lookup[current[candidate2]] = candidate2;

    unsigned long long new_latency = calculate_latency (
      settings.network_latencies, settings.target_deployment, current);

    if (new_latency < orig_latency)
    {
      std::copy (current.begin (), current.end (), settings.solution.begin ());
      settings.solution_lookup = solution_lookup;
      return;
    }
  }
}
