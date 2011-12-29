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
     * Checks a solution for duplicate entries
     * @param    settings    container for solution and solution_lookup
     **/
    MADARA_Export bool check_solution (Settings & settings);

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
     * Generates a random solution
     * @param    settings    container for solution and solution_lookup
     **/
    MADARA_Export void generate_random_solution (Settings & settings);

    /**
     * Generates the worst solution by applying the worst averages to the
     * deployment
     * @param    settings    container for solution and solution_lookup
     **/
    MADARA_Export void generate_worst_solution (Settings & settings);

    /**
     * Initializes attributes of a container to a specified size. Does
     * not adjust the target_deployment size.
     * @param       size         the number of nodes in the network
     * @param       settings     container to adjust
     **/
    MADARA_Export void init (unsigned int size, Settings & settings);

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
                            Workflow & target_deployment, unsigned int node);

    /**
     * Prints the latencies in the settings container
     * @param    settings    container for network_latencies 
     **/
    MADARA_Export void print_latencies (Settings & settings);

    /**
     * Processes a deployment from a string. The contents may use special
     * characters to ease deployment definition. Examples:
     * 
     * =====start of file======
     * 10 // define the number of processes in the deployment
     * 0 -> [size]  // this means process 0 is sending to all 10 processes
     * 0 -> [0,9] // this means the same thing
     * 0 -> [1,4)       // process 0 is sending to processes 1 through 3
     * 0 -> 1            // process 0 is sending to process 1
     * 0 -> [size / 4, size / 2]   // process 0 is sending to processes 2 through 5
     * =====end of file======
     * 
     * @param       settings     container for CID settings
     * @param       contents     string containing the deployment info.
     * @return      false if there was a problem with the contents
     **/
    MADARA_Export bool process_deployment (Settings & settings,
      const std::string & contents);

    /**
     * Reads the deployment from a file. @see process_deployment for
     * deployment file configuration information.
     * @param       filename     the name of the file to read
     * @return      true if the file existed and was read
     **/
    MADARA_Export bool read_deployment (Settings & settings,
      const std::string & filename);

  } // end Cid namespace
} // end Madara namespace

#endif // _CID_CONVENIENCE_H_