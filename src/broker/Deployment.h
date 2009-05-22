#ifndef DEPLOYMENT_H
#define DEPLOYMENT_H

#include <string>
#include <map>

namespace Madara
{

  namespace Deployment
    {

      typedef std::vector <std::string> DeploymentCandidate;
      typedef std::map <int, std::map <int,bool>> Deployment;
      Deployment read (std::string filename);

    }
}

#endif