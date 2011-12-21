
#include <algorithm>
#include "Convenience.h"

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
  // allocate memory for the latencies
  if (network_latencies.size () != size)
    network_latencies.resize (size);

  for (unsigned int i = 0; i < size; ++i)
  {
    if (network_latencies[i].size () != size)
      network_latencies[i].resize (size);

    for (unsigned int j = 0; j < size; ++j)
    {
      network_latencies[i][j].first = j;
      network_latencies[i][j].second = rand () % 100 + 10;
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
  settings.network_latencies.resize (size);
  settings.ids.resize (size);
  settings.network_averages.resize (size);
  settings.solution.resize (size);

  for (unsigned int i = 0; i < settings.network_latencies.size (); i++)
  {
    settings.network_latencies[i].resize (size);
  }
}


void
Madara::Cid::prepare_latencies (Settings & settings, unsigned int node)
{
  prepare_latencies (settings.network_latencies, settings.network_averages,
    settings.target_deployment, node);
}

void
Madara::Cid::prepare_latencies (LV_Vector & network_latencies,
                                Latency_Vector & network_averages,
                                LV_Vector & target_deployment,
                                unsigned int node)
{
  // allocate room if necessary
  if (network_latencies.size () != network_averages.size ())
    network_averages.resize (network_latencies.size ());

  // if user provided a bogus index into the target_deployment, return
  if (node >= target_deployment.size ())
    return;

  // loop through the network latency table and sort and average the values
  for (unsigned int i = 0; i < network_latencies.size (); ++i)
  {
      // sort each row of the network_latencies in order of increasing latency
      std::sort (network_latencies[i].begin (), network_latencies[i].end (),
                 Increasing_Latency);

      // remember which id this average is for and reset the total to zero
      network_averages[i].first = i;
      network_averages[i].second = 0;

      // for each element of the list, add the latency to the running total
      for (unsigned int j = 0; j < target_deployment[node].size (); ++j)
      {
        network_averages[i].second += network_latencies[i][j].second;
      }

      // average the total by the number of latencies we added
      network_averages[i].second /= target_deployment[node].size ();
  }

  // sort the network averages in increasing order. Best average is smallest.
  std::sort (network_averages.begin (), network_averages.end (),
    Increasing_Latency);
}
