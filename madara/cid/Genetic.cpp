
#include <algorithm>
#include <iostream>

#include "Genetic.h"
#include "Heuristic.h"
#include "madara/utility/Log_Macros.h"


void
Madara::Cid::ga_naive (Settings & settings, unsigned int max_mutations)
{
  // how many mutations are we going to try?
  unsigned int actual_mutations = rand () % max_mutations + 1;

#ifdef ENABLE_CID_LOGGING
  MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
    DLINFO "Madara::Cid::ga_naive:" \
    " creating an exact copy of the solution vector\n"));
#endif

  // create an exact copy of the solution vector
  Deployment current;
  current.resize (settings.solution.size ());
  std::copy (settings.solution.begin (), settings.solution.end (), 
    current.begin ());

#ifdef ENABLE_CID_LOGGING
  MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
    DLINFO "Madara::Cid::ga_naive:" \
    " calculating latency for the solution so far\n"));
#endif

  unsigned long long orig_latency = calculate_latency (settings);

  for (unsigned int i = 0; i < actual_mutations; ++i)
  {
    // generate some candidates for mutating
    unsigned int candidate1 = rand () % settings.solution.size ();
    unsigned int candidate2 = rand () % settings.solution.size ();

#ifdef ENABLE_CID_LOGGING
    MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_DEBUG, 
      DLINFO "Madara::Cid::ga_naive:" \
      " initial candidates: %u, %u\n",
      candidate1, candidate2));
#endif

    // loop until we have two different candidates
    while (candidate1 == candidate2)
      candidate1 = rand () % settings.solution.size ();

#ifdef ENABLE_CID_LOGGING
    MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_DEBUG, 
      DLINFO "Madara::Cid::ga_naive:" \
      " final candidates: %u, %u\n",
      candidate1, candidate2));
#endif

    // attempt the swap
    std::swap (current[candidate1], current[candidate2]);

    unsigned long long new_latency = calculate_latency (
      settings.network_latencies, settings.target_deployment, current);

#ifdef ENABLE_CID_LOGGING
    MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_DEBUG, 
      DLINFO "Madara::Cid::ga_naive:" \
      " latency improvement: %Q->%Q\n",
      orig_latency, new_latency));
#endif

    if (new_latency < orig_latency)
    {
#ifdef ENABLE_CID_LOGGING
      MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
        DLINFO "Madara::Cid::ga_naive:" \
        " latency improvement: %Q->%Q. Copying solution.\n",
        orig_latency, new_latency));
#endif

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

#ifdef ENABLE_CID_LOGGING
  MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
    DLINFO "Madara::Cid::ga_degree:" \
    " entering main function\n"));
#endif

  // create an exact copy of the solution vector
  Deployment current;
  current.resize (settings.solution.size ());
  std::copy (settings.solution.begin (), settings.solution.end (), 
    current.begin ());

  unsigned long long orig_latency = calculate_latency (settings);

  unsigned int num_degreed_nodes = 0;
  Workflow & deployment = settings.target_deployment;
  Averages_Map & averages = settings.network_averages;
  Solution_Map solution_lookup;

#ifdef ENABLE_CID_LOGGING
  MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
    DLINFO "Madara::Cid::ga_degree:" \
    " copying initial solution\n"));
#endif

  for (unsigned int i = 0; i < current.size (); ++i)
  {
    solution_lookup[current[i]] = i;
  }

#ifdef ENABLE_CID_LOGGING
  MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
    DLINFO "Madara::Cid::ga_degree:" \
    " counting the degreed nodes in the deployment\n"));
#endif

  // count the number of degreed nodes in the deployment.
  for (unsigned int i = 0; i < deployment.size (); ++i)
  {
    if (deployment[i].size () == 0)
      break;

    ++num_degreed_nodes;
  }

#ifdef ENABLE_CID_LOGGING
  MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
    DLINFO "Madara::Cid::ga_degree:" \
    " Attempting up to %u mutations to find better solution\n",
    max_mutations));
#endif

  for (unsigned int i = 0; i < max_mutations; ++i)
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
#ifdef ENABLE_CID_LOGGING
      MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_DEBUG, 
        DLINFO "Madara::Cid::ga_degree:" \
        " naively choosing candidates\n"));
#endif

      // go with a naive solution
      candidate1 = rand () % settings.solution.size ();
      candidate2 = rand () % settings.solution.size ();

      if (settings.solution.size () > 0)
      {
        // loop until we have two different candidates
        while (candidate1 == candidate2)
          candidate1 = rand () % settings.solution.size ();
      }
    }
    else
    {
#ifdef ENABLE_CID_LOGGING
      MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_DEBUG, 
        DLINFO "Madara::Cid::ga_degree:" \
        " choosing degree-informed candidates\n"));
#endif

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
      if (cur_averages.size () < 50)
      {
        range = cur_averages.size () / 10 + 1;
        candidate2 = solution_lookup[cur_averages[rand () % range].first];
      }
      else if (candidate_type <= 2)
      {
        if (num_degreed_nodes < 5)
        {
          range = cur_averages.size () / 20;
        }
        else
        {
          range = num_degreed_nodes;
        }

#ifdef ENABLE_CID_LOGGING
        MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_DEBUG, 
          DLINFO "Madara::Cid::ga_degree:" \
          " choosing from top %u candidates\n",
          range));
#endif

        candidate2 = solution_lookup[cur_averages[rand () % range].first];
      }
      else if (candidate_type == 3)
      {
        // choose candidate2 from the top 10%
        range = cur_averages.size () / 10; 

#ifdef ENABLE_CID_LOGGING
        MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_DEBUG, 
          DLINFO "Madara::Cid::ga_degree:" \
          " choosing from top %u candidates\n",
          range));
#endif

        candidate2 = solution_lookup[cur_averages[rand () % range].first];
      }
      else
      {
        // choose candidate2 from the top 25%
        range = cur_averages.size () / 4; 

#ifdef ENABLE_CID_LOGGING
        MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_DEBUG, 
          DLINFO "Madara::Cid::ga_degree:" \
          " choosing from top %u candidates\n",
          range));
#endif

        candidate2 = solution_lookup[cur_averages[rand () % range].first];
      }

#ifdef ENABLE_CID_LOGGING
      MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_DEBUG, 
        DLINFO "Madara::Cid::ga_degree:" \
        " initial candidates: %u, %u\n",
        candidate1, candidate2));
#endif

      // loop until we have two different candidates
      while (range >= 2 && candidate1 == candidate2)
        candidate2 = solution_lookup[cur_averages[rand () % range].first];
    }

#ifdef ENABLE_CID_LOGGING
    MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_DEBUG, 
      DLINFO "Madara::Cid::ga_degree:" \
      " final candidates: %u, %u\n",
      candidate1, candidate2));
#endif

    // attempt the swap
    if (candidate1 != candidate2)
    {
      std::swap (current[candidate1], current[candidate2]);
      solution_lookup[current[candidate1]] = candidate1;
      solution_lookup[current[candidate2]] = candidate2;

      unsigned long long new_latency = calculate_latency (
        settings.network_latencies, deployment, current);

#ifdef ENABLE_CID_LOGGING
      MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_DEBUG, 
        DLINFO "Madara::Cid::ga_degree:" \
        " latency: %Q->%Q\n",
        orig_latency, new_latency));
#endif

      if (new_latency < orig_latency)
      {
#ifdef ENABLE_CID_LOGGING
        MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_DEBUG, 
          DLINFO "Madara::Cid::ga_degree:" \
          " updating solution to current\n",
          orig_latency, new_latency));
#endif

        std::copy (current.begin (), current.end (), settings.solution.begin ());
        for (unsigned int j = 0; j < settings.solution.size (); ++j)
        {
          if (settings.solution_lookup[settings.solution[j]] != j)
            settings.solution_lookup[settings.solution[j]] = j;
        }
        return;
      }
    }
  }
}
