
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "madara/cid/Convenience.h"

void check_fanout_deployment (unsigned int fanout, Madara::Cid::Settings & settings)
{
  unsigned int size = settings.solution.size () / fanout;

  for (unsigned int i = 0; i < fanout; ++i)
  {
    if (!(settings.target_deployment[i].size () == size || 
           (i == 0 && fanout == 3)))
    {
      std::cerr << " read_deployment failed on " << fanout << " fanouts\n.";
      std::cerr << " deployment[" << i << "] was " << 
        settings.target_deployment[i].size () << " instead of " << 
        size << "\n";
      return;
    }
  }
}

int main (int argc, char *argv[])
{
  // container for the deployment settings
  Madara::Cid::Settings settings;

  // create the filenames for the deployments to read
  std::string first_deployment (::getenv ("MADARA_ROOT"));
  std::string second_deployment (first_deployment);

  first_deployment += 
    "/configs/cid/deployments/10_procs_2_full_fans_disjoint.txt";

  second_deployment +=
    "/configs/cid/deployments/2_full_fans_disjoint.template";

  // read the first deployment
  if (!Madara::Cid::read_deployment (settings, first_deployment))
  {
    std::cerr << "ERROR: Deployment size is 0 for " 
      << first_deployment << "\n";
  }

  // the second deployment is a template that depends on solution.size ()
  settings.solution.resize (100);

  // read the second deployment
  if (!Madara::Cid::read_deployment (settings, second_deployment))
  {
    std::cerr << "ERROR: Deployment size is 0 for " 
      << second_deployment << "\n";
  }

  for (unsigned int fan_out = 1; fan_out < 4; ++fan_out)
  {
    // create filename 
    std::stringstream filename;
    filename << getenv ("MADARA_ROOT");
    filename << "/configs/cid/deployments/test_cid/";
    filename << fan_out << "_even_fans_disjoint.template";

    // notify user of current est
    std::cout << "  Testing " << filename.str () << "\n";

    Madara::Cid::read_deployment (settings, filename.str ());

    ::check_fanout_deployment (fan_out, settings);
  }

  return 0;
}
