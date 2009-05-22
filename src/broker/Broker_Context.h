#ifndef BROKER_CONTEXT_H
#define BROKER_CONTEXT_H

#include <utility>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <iostream>

#include "ace/Guard_T.h"
#include "ace/Recursive_Thread_Mutex.h"


namespace Madara
  {
  
    typedef std::pair <std::string, int> LatencyPair;
    typedef std::map <std::string,int> PeerLatencyMap;
    typedef std::vector <LatencyPair> PeerLatencyVector;

    typedef std::vector <std::string> DeploymentCandidate;
    typedef std::map <int, std::map <int,bool>> Deployment;

    typedef std::map <std::string, bool> EliminationList;
    typedef std::map <int, bool> DeploymentEliminationList;

    // my lol heuristic
    typedef PeerLatencyVector TopNerds;


    static bool SortByPairValue (LatencyPair u, LatencyPair v)
    {
      return u.second < v.second;
    }


    typedef struct
    {
      Madara::PeerLatencyMap latencies;
      Madara::PeerLatencyVector vector;

      double classifier;
      int avg;
    } PeerLatency;

    typedef std::map <std::string,Madara::PeerLatency> BrokerMap;

    class Broker_Context
    {
    public:
      Broker_Context(int nerd_lookahead = 3, bool average_all = false);
      void read (const std::string& filename);
      void write (std::ostream& output);
      void write (const std::string& filename);
      void addLatency (const std::string& key, 
        const std::string & target, int latency);
      void addRequirement (int source, int target);
      void removeRequirement (int source, int target);
      void writeRequirements (std::ostream& output);
      void writeDeploymentCandidate (std::ostream& output, 
                              DeploymentCandidate & candidate);
      void writeRequirementsLegible (std::ostream& output);
      void writeRequirements (const std::string& filename);

      void clearRequirements ();

      const std::string& getHost ();
      void setHost (const std::string& host);
      const std::string& getPort ();
      void setPort (const std::string& port);

      bool peerExists (const std::string& key);
      bool peerExists (const std::string& host, u_short port);
      bool peerExists (const std::string& host, const std::string& port);

      Madara::DeploymentCandidate learnDeployment (void);

      unsigned int getNumKeys (void);
      std::string getKey (unsigned int pos);
      Madara::PeerLatency getLatencyMap (unsigned int pos);
      Madara::PeerLatency getLatencyMap (const std::string & key);

      int calculateUtility (DeploymentCandidate & candidate);

    private:
      std::string getNextLowestLatencyAvailable (
            const Madara::EliminationList & remainingNodes, 
            const Madara::PeerLatencyVector & sortedLatencies);
      typedef ACE_Guard<ACE_Recursive_Thread_Mutex> Context_Guard;
      void createEntry (const std::string&);

      int getHighestDegreeNode (Madara::Deployment & deployment,
        DeploymentEliminationList & remainingRanks);

      int averageLatency (Madara::PeerLatency & list, int degree);
      void generateBestGuess (Madara::DeploymentCandidate & best, 
                      Madara::TopNerds & nerds);
      void generateDeployment ( Madara::DeploymentCandidate & best, 
        Madara::DeploymentCandidate & last,
        Madara::TopNerds & nerds);

      Madara::BrokerMap map_;
      Madara::Deployment deployment_;
      int nerd_lookahead_;
      bool average_all_;
      int deployment_degree_;
      std::string host_;
      std::string port_;

      ACE_Recursive_Thread_Mutex mutex_;
    };

  }
#endif // BROKER_CONTEXT_H