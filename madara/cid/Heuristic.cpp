
/**
 * @file Heuristic.cpp
 * @author James Edmondson <james.r.edmondson@vanderbilt.edu>
 *
 * This file contains definitions for the CID heuristic used for approximating
 * a good deployment. The heuristic may be used in conjunction with the genetic
 * algorithm in @see cid/Genetic.h.
 */

#include <algorithm>

#include "Heuristic.h"

void
Madara::Cid::approximate (Settings & settings)
{
  approximate (settings.network_averages, 
    settings.target_deployment, settings.solution);
}

void
Madara::Cid::approximate (Latency_Vector & network_averages,
      LV_Vector & target_deployment, Deployment & solution)
{
  // if the solution has not been allocated
  if (solution.size () < target_deployment.size ())
    solution.resize (target_deployment.size ());

  // go down the deployment listing and insert the lowest averages
  for (unsigned int i = 0; i < target_deployment.size (); ++i)
  {
    solution[i] = network_averages[i].first;
  }

  /**
   * If we are allowed to not worry about the prep work, that leaves us with
   * all kinds of time to do other stuff--e.g., reaverage for the remaining
   * deployment items and solve for the remaining target deployment by its
   * degree and remaining items.
   **/
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