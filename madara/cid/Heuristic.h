#ifndef _CID_HEURISTIC_H_
#define _CID_HEURISTIC_H_

/**
 * @file Heuristic.h
 * @author James Edmondson <james.r.edmondson@vanderbilt.edu>
 *
 * This file contains declarations for the CID heuristic used for approximating
 * a good deployment. The heuristic may be used in conjunction with the genetic
 * algorithm in @see cid/Genetic.h.
 */

#include "Settings.h"
#include "madara/MADARA_export.h"

namespace Madara
{
  namespace Cid
  {

    /**
     * Approximates the solution with the CID heuristic
     * @param    settings    container for network averages, target
     *                       deployment, and generated solution
     *                       (@see Madara::Cid::Settings)
     **/
    MADARA_Export void approximate (Settings & settings);

    /**
     * Approximates the solution with the CID heuristic
     * @param    settings    container for network averages, target
     *                       deployment, and generated solution
     *                       (@see Madara::Cid::Settings)
     * @param    node        the deployment node we are approximating
     **/
    MADARA_Export void approximate (Settings & settings, unsigned int node);

    /**
     * Approximates the solution with the CID heuristic
     * @param    network_averages  sorted array of network averages
     *                             (@see Madara::Cid::Settings)
     * @param    target_deployment sorted target deployment by degree
     * @param    solution          results of heuristic
     * @param    solution_lookup   map of network ids to their deployments
     * @param    node        the deployment node we are approximating
     **/
    MADARA_Export void approximate (Averages_Map & network_averages, 
      LV_Vector & target_deployment, Deployment & solution, 
      Solution_Map & solution_lookup, unsigned int node);

    /**
     * Sorts the target deployment by degree to prepare it for the CID heuristic
     * @param    settings    container which contains the target_deployment
     **/
    MADARA_Export void prepare_deployment (Settings & settings);

    /**
     * Sorts the target deployment by degree to prepare it for the CID heuristic
     * @param    settings    container which contains the target_deployment
     **/
    MADARA_Export void prepare_deployment (LV_Vector & target_deployment);

    MADARA_Export void fill_by_highest_degree (Settings & settings,
      unsigned int start);
  } // end Cid namespace
} // end Madara namespace

#endif // _CID_HEURISTIC_H_
