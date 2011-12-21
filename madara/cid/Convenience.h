#ifndef _CID_CONVENIENCE_H_
#define _CID_CONVENIENCE_H_

/**
 * @file Convenience.h
 * @author James Edmondson <james.r.edmondson@vanderbilt.edu>
 *
 * This file contains declarations of convenience functions for developers
 * using the CID heuristic.
 */

#include "Settings.h"
#include "madara/MADARA_export.h"

namespace Madara
{
  namespace Cid
  {
    /**
     * Initializes attributes of a container to a specified size. Does
     * not adjust the target_deployment size.
     * @param       size         the number of nodes in the network
     * @param       settings     container to adjust
     **/
    MADARA_Export void init (unsigned int size, Settings & settings);

    /**
     * Generates a random, fully-connected network of latencies
     * @param       size         the number of nodes in the network
     * @param       settings     container for the network_latencies
     **/
    MADARA_Export void generate_random_network (unsigned int size,
      Settings & settings);

    /**
     * Generates a random, fully-connected network of latencies
     * @param       size                  the number of nodes in the network
     * @param       network_latencies     the generated network latencies
     **/
    MADARA_Export void generate_random_network (unsigned int size,
      LV_Vector & network_latencies);

    /**
     * Sorts the latencies in a network topology and averages them into a format
     * that can be used by CID. This function performs averages for all possible
     * degrees in the deployment.
     * @param    settings    container for network_latencies and network_averages
     *                       used by the method
     **/
    MADARA_Export void prepare_latencies (Settings & settings);
    
    /**
     * Sorts the latencies in a network topology and averages them into a format
     * that can be used by CID
     * @param    settings    container for network_latencies and network_averages
     *                       used by the method
     * @param    node        index of the deployment to use degree of
     **/
    MADARA_Export void prepare_latencies (Settings & settings,
      unsigned int node);
    
    /**
     * Sorts the latencies in a network topology and averages them into a format
     * that can be used by CID
     * @param    network_latencies    source latency matrix which will be sorted
     *                                (@see Madara::Cid::Settings)
     * @param    network_averages     averaged latencies from each row of  
     *                                network_latencies
     *                                (@see Madara::Cid::Settings)
     * @param    target_deployment    application workflow to approximate
     * @param    node                 index of the deployment to use degree of
     *                       
     **/
    MADARA_Export void prepare_latencies (LV_Vector & network_latencies,
                            Averages_Map & network_averages,
                            LV_Vector & target_deployment, unsigned int node);

  } // end Cid namespace
} // end Madara namespace

#endif // _CID_CONVENIENCE_H_