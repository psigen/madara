
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

void
Madara::Cid::approximate (Settings & settings)
{
  for (unsigned int i = 0; i < settings.target_deployment.size (); ++i)
    approximate (settings.network_averages, 
      settings.target_deployment, settings.solution,
      settings.solution_lookup, i);
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
  // if the solution has not been allocated
  if (solution.size () < target_deployment.size ())
    solution.resize (target_deployment.size ());

  /**
   * Some things to remember:
   * node = the node you are trying to solve
   * degree = target_deployment[node].size ()
   * network_averages[node] = sorted list of averages for that degree
   *                          remember that this is a map and to use a ref
   * This entire function is for solving 1 node and that is solution[node]
   **/

  unsigned int degree = target_deployment[node].size ();
  Latency_Vector & cur_averages = network_averages[degree];

  for (unsigned int i = 0; i < cur_averages.size (); ++i)
  {
    /**
     * If this is the first node in the deployment, or if we haven't tried
     * this solution before, then we have a match
     **/

    if (node == 0 || 
      solution_lookup.find (cur_averages[i].first) == solution_lookup.end ())
    {
      solution_lookup[cur_averages[i].first] = node;
      solution[node] = cur_averages[i].first;
      break;
    }
  }
}

void
Madara::Cid::fill_by_highest_degree (Settings & settings, unsigned int start)
{
  if (settings.target_deployment.size () < 1)
    return;

  // if the solution has not been allocated
  if (settings.solution.size () < settings.target_deployment.size ())
    settings.solution.resize (settings.target_deployment.size ());

  unsigned int degree = settings.target_deployment[0].size ();

  Latency_Vector & cur_averages = settings.network_averages[degree];
  
  for (unsigned int i = 0; i < settings.target_deployment.size (); ++i)
  {
    settings.solution[start] = cur_averages[i].first;
    ++start;
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
  std::sort (target_deployment.begin (), target_deployment.end (),
    Decreasing_Size);
}