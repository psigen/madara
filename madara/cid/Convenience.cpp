
#include <iostream>
#include <algorithm>
#include "Convenience.h"
#include "madara/utility/Log_Macros.h"

/**
 * Checks a solution for duplicate entries
 **/
bool
Madara::Cid::check_solution (Settings & settings)
{
  Deployment & solution = settings.solution;

  Solution_Map instances;
  bool multiple_entries = false;

  for (unsigned int i = 0; i < solution.size (); ++i)
  {
    ++instances[solution[i]];

    if (instances[solution[i]] > 1)
      multiple_entries = true;
  }

  if (multiple_entries)
  {
    for (unsigned int i = 0; i < solution.size (); ++i)
    {
      if (instances[solution[i]] > 1)
      {
        MADARA_DEBUG (MADARA_LOG_EMERGENCY, (LM_DEBUG, 
          DLINFO "Madara::Cid::check_solution:" \
          " ERROR: %u duplicate entries found for id %u." \
          " Please save a log and file issue at madara.googlecode.com.\n",
            instances[solution[i]], solution[i]));

        /**
         * to stop this from being called multiple times for the same entry,
         * set the instances[solution[i]] to 1.
         **/
        instances[solution[i]] = 1;
      }
    }
  }

  return multiple_entries;
}

/**
 * Generates a random, fully-connected network of latencies
 **/
void
Madara::Cid::generate_random_network (unsigned int size, Settings & settings)
{
  generate_random_network (size, settings.network_latencies);
}

/**
 * Generates a random, fully-connected network of latencies
 **/
void
Madara::Cid::generate_random_network (unsigned int size,
                                      LV_Vector & network_latencies)
{
#ifdef ENABLE_CID_LOGGING
  MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
    DLINFO "Madara::Cid::generate_random_network:" \
      " Generating random network of size %u\n",
      size));
#endif

  // allocate memory for the latencies
  if (network_latencies.size () != size)
    network_latencies.resize (size);

  for (unsigned int i = 0; i < size; ++i)
  {
#ifdef ENABLE_CID_LOGGING
    MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_DEBUG, 
      DLINFO "Madara::Cid::generate_random_network:" \
      " resizing network_latencies[%u]\n",
        i));
#endif

    if (network_latencies[i].size () != size)
      network_latencies[i].resize (size);

    for (unsigned int j = 0; j < size; ++j)
    {
#ifdef ENABLE_CID_LOGGING
      MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_DEBUG, 
        DLINFO "Madara::Cid::generate_random_network:" \
        " setting network_latencies[%u][%u]\n",
          i, j));
#endif

      if (network_latencies[i][j].second == 0)
      {
        Latency_Vector & source_latencies = network_latencies[i];
        Latency_Vector & dest_latencies = network_latencies[j];

        // make the latencies bidirectionally equivalent for realism
        source_latencies[j].first = j;
        source_latencies[j].second = rand () + 10;
        dest_latencies[i].first = i;
        dest_latencies[i].second = source_latencies[j].second;
      }
    }

#ifdef ENABLE_CID_LOGGING
    MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_DEBUG, 
      DLINFO "Madara::Cid::generate_random_network:" \
      " sorting network_latencies[%u]\n",
        i));
#endif

    // sort each row of the network_latencies in order of increasing latency
    std::sort (network_latencies[i].begin (), network_latencies[i].end (),
               Increasing_Latency);
  }

}

void
Madara::Cid::generate_random_solution (Settings & settings)
{
#ifdef ENABLE_CID_LOGGING
  MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
    DLINFO "Madara::Cid::generate_random_solution:" \
      " Generating random solution of size %u\n",
      settings.solution.size ()));
#endif

  for (unsigned int i = 0; i < settings.solution.size (); ++i)
  {
    settings.solution[i] = i;
    settings.solution_lookup[i] = i;
  }
}

void
Madara::Cid::generate_worst_solution (Settings & settings)
{
  settings.solution_lookup.clear ();

  if (settings.target_deployment.size () < 1)
    return;

  unsigned int start = 0;

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

  Latency_Vector & cur_averages = settings.network_averages[degree];
  Solution_Map & solution_lookup = settings.solution_lookup;
  Deployment & solution = settings.solution;
  Workflow & deployment = settings.target_deployment;

  for (unsigned int i = 0; 
    start < solution.size () && i < deployment.size (); ++i)
  {
    unsigned int actual = cur_averages.size () - i - 1;
    if (start == 0 ||
      solution_lookup.find (cur_averages[actual].first) == solution_lookup.end ())
    {    
#ifdef ENABLE_CID_LOGGING
      MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_DEBUG, 
      DLINFO "Madara::Cid::fill_by_highest_degree:" \
      " found solution[%u]=%u\n",
      start, cur_averages[i].first));
#endif

      solution_lookup[cur_averages[actual].first] = start;
      solution[start] = cur_averages[actual].first;
      ++start;
    }
  }
}

/**
 * Initializes attributes of a container to a specified size. Does
 * not adjust the target_deployment size.
 **/
void
Madara::Cid::init (unsigned int size, Settings & settings)
{
#ifdef ENABLE_CID_LOGGING
  MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
    DLINFO "Madara::Cid::init:" \
      " Resizing latencies, ids, and solution to size %u\n",
      size));
#endif

  settings.network_latencies.resize (size);
  settings.ids.resize (size);
  settings.solution.resize (size);

#ifdef ENABLE_CID_LOGGING
  MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
    DLINFO "Madara::Cid::init:" \
      " Resizing individual network latency entries to size %u\n",
      size));
#endif

  for (unsigned int i = 0; i < settings.network_latencies.size (); i++)
  {
    settings.network_latencies[i].resize (size);
  }
}


void
Madara::Cid::prepare_latencies (Settings & settings)
{
  for (unsigned int i = 0; i < settings.target_deployment.size (); ++i)
    prepare_latencies (settings, i);
}

void
Madara::Cid::prepare_latencies (Settings & settings, unsigned int node)
{
  prepare_latencies (settings.network_latencies, settings.network_averages,
    settings.target_deployment, node);
}

void
Madara::Cid::prepare_latencies (LV_Vector & network_latencies,
                                Averages_Map & network_averages,
                                Workflow & target_deployment,
                                unsigned int node)
{
#ifdef ENABLE_CID_LOGGING
  MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
    DLINFO "Madara::Cid::prepare_latencies:" \
      " Calculating degree\n"));
#endif

  // if user provided a bogus index into the target_deployment, return
  if (node >= target_deployment.size ())
    return;

  // if no outgoing degree, we need a best of the deployment size
  unsigned int degree = target_deployment[node].size () > 0 ?
    target_deployment[node].size () : network_latencies.size ();

#ifdef ENABLE_CID_LOGGING
  MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
    DLINFO "Madara::Cid::prepare_latencies:" \
      " Degree set to %u\n", 
      degree));
#endif

  // if we've already done this degree, continue to the next deployed element
  if (network_averages.find (degree) != network_averages.end ())
    return;

  // we're dealing with a std::map which has O(log n) lookup. Use ref.
  Latency_Vector & cur_averages = network_averages[degree];

  // make sure cur_averages has the right size
  if (cur_averages.size () != network_latencies.size ())
    cur_averages.resize (network_latencies.size ());

#ifdef ENABLE_CID_LOGGING
  MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
    DLINFO "Madara::Cid::prepare_latencies:" \
      " Entering main loop\n" 
      ));
#endif

  // loop through the network latency table and average the values
  for (unsigned int i = 0; i < network_latencies.size (); ++i)
  {
#ifdef ENABLE_CID_LOGGING
      MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_DEBUG, 
        DLINFO "Madara::Cid::prepare_latencies:" \
        " i=%u\n",
        i));
#endif

      // remember which id this average is for and reset the total to zero
      cur_averages[i].first = i;

      // for each element of the list, add the latency to the running total
      for (unsigned int j = 0; j < degree; ++j)
      {
#ifdef ENABLE_CID_LOGGING
        MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_DEBUG, 
          DLINFO "Madara::Cid::prepare_latencies:" \
          "   j=%u\n",
          j));
#endif

        // nothing to gain from averaging except less precision
        cur_averages[i].second += network_latencies[i][j].second;
      }

#ifdef ENABLE_CID_LOGGING
      MADARA_DEBUG (MADARA_LOG_DETAILED_TRACE, (LM_DEBUG, 
        DLINFO "Madara::Cid::prepare_latencies:" \
          " Dividing total by degree to give an average\n"));
#endif

  }

#ifdef ENABLE_CID_LOGGING
  MADARA_DEBUG (MADARA_LOG_EVENT_TRACE, (LM_DEBUG, 
    DLINFO "Madara::Cid::prepare_latencies:" \
    " Sorting averages. network_averages[%u]\n",
    degree));
#endif

  // sort the network averages in increasing order. Best average is smallest.
  std::sort (cur_averages.begin (), cur_averages.end (),
    Increasing_Latency);
}

