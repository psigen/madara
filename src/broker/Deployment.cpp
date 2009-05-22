#ifndef DEPLOYMENT_CPP
#define DEPLOYMENT_CPP

#include "Deployment.h"

Madara::Deployment::Deployment 
Madara::Deployment::read (std::string filename, int & num_ranks)
{
  Madara::Deployment::Deployment deployment;
  std::ifstream input (filename.c_str ());

  if (input.is_open ())
    {
      // read the total number of ranks required for the deployment
      input >> num_ranks;

      while (input)
        {
          // read each deployment requirement
          std::string operation;
          int source;
          int target;

          input >> source;
          input >> operation;
          input >> target;

          // if the op is <->, then links need to be optimized
          // for bidirectional links between target/source
          if (operation == "<->")
            {
              add (deployment, source, target);
              add (deployment, target, source);
            }
          // if the op is ->, then links need to be optimized
          // for unidirectional link between source to target
          else if (operation == "->")
            {
              add (deployment, source, target);
            }
          // if the op is <-, then links need to be optimized
          // for unidirectional link between target to source
          // this is a a convenience mechanism and is somewhat
          // counterintuitive to a deployment file human reader
          else if (operation == "<-")
            {
              add (deployment, target, source);
            }
        }
    }

  return deployment;
}

::size_t
Madara::Deployment::add (Madara::Deployment::Deployment & deployment,
                         int source, int target)
{
  ::size_t size = 0;

  if (source != target)
    {
      deployment[source][target] = true;

      size = deployment[source].size ();
    }

  return size;
}

void
Madara::Deployment::write (Madara::Deployment::Deployment & deployment, 
                           std::ostream& output)
{
  for (Madara::Deployment::Deployment::iterator i = deployment.begin (); 
       i != deployment.end (); ++i)
    {
      std::map <int, bool>::iterator j = i->second.begin ();
      for (; j != i->second.end (); ++j)
        output << "  " << i->first << " -> " <<  j->first << "\n";
    }
}

#endif