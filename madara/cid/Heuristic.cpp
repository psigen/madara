
/**
 * @file Heuristic.cpp
 * @author James Edmondson <james.r.edmondson@vanderbilt.edu>
 *
 * This file contains definitions for the CID heuristic used for approximating
 * a good deployment. The heuristic may be used in conjunction with the genetic
 * algorithm in @see cid/Genetic.h.
 */

#include <algorithm>
#include <iostream>

#include "Heuristic.h"
#include "madara/utility/Log_Macros.h"

void
Madara::Cid::approximate (Settings & settings)
{
  for (unsigned int i = 0; i < settings.target_deployment.size (); ++i)
  {
    if (settings.target_deployment[i].size ())
    {
// see Settings.h
#ifdef ENABLE_CID_LOGGING
      MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
        DLINFO "Madara::Cid::approximate:" \
          " calling approximate (averages, ..., %u)\n",
          i));
#endif

      approximate (settings.network_averages, 
        settings.target_deployment, settings.solution,
        settings.solution_lookup, i);
    }
    else
    {
#ifdef ENABLE_CID_LOGGING
      MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
        DLINFO "Madara::Cid::approximate:" \
          " calling fill_from_solution_map for i=%u\n",
          i));
#endif

      fill_from_solution_map (settings);
      i = settings.target_deployment.size ();
    }
  }
}

void
Madara::Cid::approximate (Settings & settings, unsigned int node)
{
  approximate (settings.network_averages, 
    settings.target_deployment, settings.solution,
    settings.solution_lookup, node);
}

void
Madara::Cid::approximate (Averages_Map & network_averages,
      LV_Vector & target_deployment, Deployment & solution, 
      Solution_Map & solution_lookup, unsigned int node)
{
  /**
   * Some things to remember:
   * node = the node you are trying to solve
   * degree = target_deployment[node].size ()
   * network_averages[node] = sorted list of averages for that degree
   *                          remember that this is a map and to use a ref
   * This entire function is for solving 1 node and that is solution[node]
   **/

  Latency_Vector & current_flow = target_deployment[node];
  unsigned int degree = current_flow.size ();
  Latency_Vector & cur_averages = network_averages[degree];
  unsigned int source;

  if (current_flow.size ())
  {
    source = current_flow[0].first;

#ifdef ENABLE_CID_LOGGING
    MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_DEBUG, 
      DLINFO "Madara::Cid::approximate:" \
        " degree=%u, source=%u\n",
        degree, source));
#endif

    for (unsigned int i = 0; i < cur_averages.size (); ++i)
    {
      /**
       * If this is the first node in the deployment, or if we haven't tried
       * this solution before, then we have a match
       **/

      if (node == 0 || 
        solution_lookup.find (cur_averages[i].first) == solution_lookup.end ())
      {
        MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_DEBUG, 
          DLINFO "Madara::Cid::approximate:" \
          " degree=%u, source=%u, i=%u: found solution for %u\n",
            degree, source, i, cur_averages[i].first));

        solution_lookup[cur_averages[i].first] = source;
        solution[source] = cur_averages[i].first;
        break;
      }
    }
  }
}

unsigned long long
Madara::Cid::calculate_latency (Settings & settings)
{
  return calculate_latency (settings.network_latencies,
    settings.target_deployment, settings.solution);
}

unsigned long long
Madara::Cid::calculate_latency (LV_Vector & latencies, LV_Vector & workflow,
                                Deployment & solution)
{
  unsigned long long total_latency = 0;

  for (unsigned int i = 0; i < workflow.size (); ++i)
  {
    Latency_Vector & current = workflow[i];

    if (current.size () > 0)
    {
      /**
       * source = the solution index number of the source of the directed edge
       * dest = the solution index number of the dest of the directed edge
       * solution[source] = the network id of the source
       * solution[dest] = the network id of the destination
       **/
      unsigned int & source = current[0].first;
      Latency_Vector source_latencies = latencies[solution[source]];

      for (unsigned int j = 0; j < current.size (); ++j)
      {
#ifdef ENABLE_CID_LOGGING
        MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_DEBUG, 
          DLINFO "Madara::Cid::calculate_latency:" \
          " workflow[%u]: adding current[%u].second to total_latency (%Q)\n",
            i, j, total_latency));
#endif

        unsigned int & dest = current[j].second;
        total_latency += source_latencies [solution[dest]].second;
      }
    }
  }

#ifdef ENABLE_CID_LOGGING
  MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
    DLINFO "Madara::Cid::calculate_latency:" \
    " total_latency for deployment is %Q\n",
      total_latency));
#endif

  return total_latency;
}

void
Madara::Cid::fill_by_highest_degree (Settings & settings)
{
  fill_by_highest_degree (settings, 0);
}

void
Madara::Cid::fill_by_highest_degree (Settings & settings, unsigned int start)
{
  if (settings.target_deployment.size () < 1)
    return;

  // if the solution has not been allocated
  if (settings.solution.size () < settings.target_deployment.size ())
    settings.solution.resize (settings.target_deployment.size ());

  // old version which looked at highest degree in deployment
  // unsigned int degree = settings.target_deployment[0].size ();

  /**
   * New version looks at the degree of the deployment size. The logic
   * behind this decision is that we'll prefer to place unused nodes that
   * have the lowest latency in case of conflicts with other workflows. These
   * nodes are more likely to fit within a heuristic's good destination ids
   **/
  unsigned int degree = settings.network_latencies.size ();

  if (degree == 0 || degree == 1)
    MADARA_DEBUG (MADARA_LOG_ERROR, (LM_DEBUG, 
      DLINFO "Madara::Cid::fill_by_highest_degree:" \
      " ERROR: highest degree equals %u\n",
        degree));

  Latency_Vector & cur_averages = settings.network_averages[degree];
  Solution_Map & solution_lookup = settings.solution_lookup;
  Deployment & solution = settings.solution;
  LV_Vector & deployment = settings.target_deployment;

  for (unsigned int i = 0; 
    start < solution.size () && i < deployment.size (); ++i)
  {
    if (start == 0 ||
      solution_lookup.find (cur_averages[i].first) == solution_lookup.end ())
    {    
#ifdef ENABLE_CID_LOGGING
      MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_DEBUG, 
      DLINFO "Madara::Cid::fill_by_highest_degree:" \
      " found solution[%u]=%u\n",
      start, cur_averages[i].first));
#endif

      solution_lookup[cur_averages[i].first] = start;
      solution[start] = cur_averages[i].first;
      ++start;
    }
  }
}

void
Madara::Cid::fill_from_solution_map (Settings & settings)
{
  // setup some references for simplicity
  LV_Vector & deployment = settings.target_deployment;
  Solution_Map & lookup = settings.solution_lookup;
  Deployment & solution = settings.solution;

  // iterate until we find a 0 degree deployment node
  for (unsigned int i = 0; i < deployment.size (); ++i)
  {
    Latency_Vector & source_flow = deployment[i];
    if (source_flow.size () > 0)
    {
      unsigned int & source = source_flow[0].first;
      unsigned int & source_id = solution[source];
      Latency_Vector & latencies = settings.network_latencies[source_id];
      
      unsigned int k = 0;

      for (unsigned int j = 0; j < source_flow.size (); ++j)
      {
        unsigned int & dest = source_flow[j].second;
        unsigned int & dest_id = solution[dest];

        Solution_Map::iterator found = lookup.find (dest_id);

        // if we can't find the id, we need to place our best latency endpoint
        if (found == lookup.end () || found->second != j)
        {
          for (; k < latencies.size (); ++k)
          {
            // if we find a winner, place it and break back to j loop
            if (lookup.find (latencies[k].first) == lookup.end ())
            {
#ifdef ENABLE_CID_LOGGING
              MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_DEBUG, 
              DLINFO "Madara::Cid::fill_from_solution_map:" \
              " found solution[%u]=%u\n",
              dest_id, latencies[k].first));
#endif

              dest_id = latencies[k].first;
              lookup[latencies[k].first] = dest_id;
              ++k;
              break;
            }
          } // end for k
        } // end couldn't find dest_id
      } // end for j
    } // end if the degree was greater than 0
    else
    {
      // fill the remaining in with arbitrary ids
      fill_by_highest_degree (settings);
      i = deployment.size ();
    }
  }
}

void
Madara::Cid::prepare_deployment (Settings & settings)
{
  prepare_deployment (settings.target_deployment);
}

void
Madara::Cid::prepare_deployment (LV_Vector & target_deployment)
{
#ifdef ENABLE_CID_LOGGING
  MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
  DLINFO "Madara::Cid::prepare_deployment:" \
  " sorting the target_deployment\n"));
#endif

  std::sort (target_deployment.begin (), target_deployment.end (),
    Decreasing_Size);
}
