
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "madara/cid/Convenience.h"

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

  return 0;
}
