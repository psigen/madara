#ifndef _CID_SETTINGS_H_
#define _CID_SETTINGS_H_

/**
 * @file Heuristic.h
 * @author James Edmondson <james.r.edmondson@vanderbilt.edu>
 *
 * This file contains declarations for the CID heuristic used for approximating
 * a good deployment. The heuristic may be used in conjunction with the genetic
 * algorithm in @see cid/Genetic.h.
 */

#include <string>
#include <vector>
#include <map>

namespace Madara
{
  namespace Cid
  {
    // Uncomment if you would like CID logging enabled
    //#define ENABLE_CID_LOGGING

    /**
     * Container class for a link established within a deployment
     **/
    class Link
    {
    public:
      unsigned int target;
      unsigned int length;

      /**
       * Default constructor
       **/
      Link () : target (0), length (0)
      {  
      }
    };

    /**
     * A vector of @see Link
     **/
    typedef std::map <unsigned int, Link>            Link_Map;

    /**
     * A container for all links from a source
     **/
    class Links
    {
    public:
      unsigned int source;
      unsigned int degree;
      Link_Map dest;
    };

    /**
     * A pairing of a target (first) and a latency (second)
     **/
    typedef std::pair <unsigned int, unsigned long long>   Latency_Record;

    /**
     * A vector of @see Latency_Record
     **/
    typedef std::vector <Latency_Record>             Latency_Vector;

    /**
     * A vector of @see Latency_Vector
     **/
    typedef std::vector <Latency_Vector>             LV_Vector;

    /**
     * A directed edge has a source and destination pair
     **/
    typedef std::pair <unsigned int, unsigned int>   Directed_Edge;

    /**
     * A vector of @see Directed_Edge
     **/
    typedef std::vector <Directed_Edge>              Directed_Edges;

    /**
     * A vector of @see Directed_Edge
     **/
    typedef std::vector <Directed_Edges>             Workflow;

    /**
     * A lookup for all paths in the deployment.
     **/
    typedef std::vector <Links>                      Paths;

    /**
     * A list of deployment ids which correspond to individuals
     * in a @see LV_Vector
     **/
    typedef std::vector <unsigned int>               Deployment;

    /**
     * A list of latencies, mostly used for averages
     **/
    typedef std::vector <unsigned int>               Latencies;
    
    /**
     * A map of degree to the average latencies for that degree
     */
    typedef std::map <unsigned int, Latency_Vector>  Averages_Map;

    /**
     * A map of the solution which allows reverse lookup by identifier
     **/
    typedef std::map <unsigned int, unsigned int>    Solution_Map;

    /**
     * A mapping of ids to string names. Using strings natively in the
     * algorithm would result high overhead (orders of magnitude) even
     * if we used "const std::string &" wherever possible. A map of strings
     * requires O(m) for each comparison within the std::map tree per access
     * of the key (and this is compounded when attempting to sort).
     **/
    typedef std::vector <std::string>     Identifiers;

    /**
     * Comparator for a list of increasing latency records
     **/
    static bool Increasing_Latency (
      const Latency_Record & u, const Latency_Record & v)
    {
      return u.second < v.second;
    }

    /**
     * Comparator for a list of increasing latency records
     **/
    static bool Increasing_Id (
      const Latency_Record & u, const Latency_Record & v)
    {
      return u.first < v.first;
    }

    /**
     * Comparator for a list of decreasing sizes of latency vectors
     **/
    static bool Decreasing_Size_Directed_Edges (
      const Directed_Edges & u, const Directed_Edges & v)
    {
      return u.size () > v.size ();
    }

    /**
     * Comparator for a list of decreasing sizes of number of links
     **/
    static bool Decreasing_Size_Link_Map (
      const Link_Map & u, const Link_Map & v)
    {
      return u.size () > v.size ();
    }

    /**
     * Comparator for a list of decreasing sizes of number of links
     **/
    static bool Decreasing_Size_And_Degrees_Links (
      const Links & u, const Links & v)
    {
      return u.dest.size () >= v.dest.size () && u.degree > v.degree;
    }

    /**
     * Comparator for an increasing path length
     **/
    static bool Increasing_Path_Length (
      const Link & u, const Link & v)
    {
      return u.length < v.length;
    }

    /**
     * Comparator for an increasing target
     **/
    static bool Increasing_Target (
      const Link & u, const Link & v)
    {
      return u.target < v.target;
    }

    /**
     * Comparator for an increasing source
     **/
    static bool Increasing_Source (
      const Links & u, const Links & v)
    {
      return u.source < v.source;
    }

    ///**
    // * Comparator for a list of increasing integers
    // **/
    //static bool Increasing (
    //  const int & u, const int & v)
    //{
    //  return u < v;
    //}

    ///**
    // * Comparator for a list of decreasing integers
    // **/
    //static bool Decreasing (
    //  const int & u, const int & v)
    //{
    //  return v < u;
    //}

    /**
     * A public container for CID operations
     **/
    struct Settings
    {
      /**
       * Mapping of network unique ids to human-readables. These
       * ids correspond to an entry in the network_latencies table.
       * e.g.,
       *
       * ids[0] == "192.168.5.1:30000"
       * ids[1] == "192.168.5.2:30000"
       *
       *    or (if you prefer)
       *
       * ids[0] == "John's PC"
       * ids[1] == "Mike's PC"
       **/
      Identifiers   ids;

      /**
       * These are all of the network latencies between ids. They
       * should have each element sorted with Increasing_Latency so
       * this array can be used directly for populating solutions.
       *
       * ids[0] == "192.168.5.1"
       * ids[1] == "192.168.5.2"
       * ids[2] == "192.168.5.3"
       * ids[3] == "192.168.5.4"
       *
       * Given the above example ids, here is an example table and
       * what it means:
       *
       * [0][0][1,180]   192.168.5.1 => 192.168.5.2     180 us
       *    [1][2,50]    192.168.5.1 => 192.168.5.3      50 us
       *    [2][3,70]    192.168.5.1 => 192.168.5.4      70 us
       * [1][0][0,180]   192.168.5.2 => 192.168.5.1     180 us
       *    [1][2,60]    192.168.5.2 => 192.168.5.3      60 us
       *    [2][3,50]    192.168.5.2 => 192.168.5.4      50 us
       * [2][0][0,50]    192.168.5.3 => 192.168.5.1      50 us
       *    [1][1,60]    192.168.5.3 => 192.168.5.2      60 us
       *    [2][3,80]    192.168.5.3 => 192.168.5.4      80 us
       * [3][0][0,70]    192.168.5.4 => 192.168.5.1      70 us
       *    [1][1,50]    192.168.5.4 => 192.168.5.2      50 us
       *    [2][2,80]    192.168.5.4 => 192.168.5.3      80 us
       **/
      LV_Vector     network_latencies;

      /**
       * These are the averages for the network_latencies vector
       * e.g. if the network_latencies were the following:
       *
       * [0][0][1,180]   192.168.5.1 => 192.168.5.2     180 us
       *    [1][2,50]    192.168.5.1 => 192.168.5.3      50 us
       *    [2][3,70]    192.168.5.1 => 192.168.5.4      70 us
       * [1][0][0,180]   192.168.5.2 => 192.168.5.1     180 us
       *    [1][2,60]    192.168.5.2 => 192.168.5.3      60 us
       *    [2][3,50]    192.168.5.2 => 192.168.5.4      50 us
       * [2][0][0,50]    192.168.5.3 => 192.168.5.1      50 us
       *    [1][1,60]    192.168.5.3 => 192.168.5.2      60 us
       *    [2][3,80]    192.168.5.3 => 192.168.5.4      80 us
       * [3][0][0,70]    192.168.5.4 => 192.168.5.1      70 us
       *    [1][1,50]    192.168.5.4 => 192.168.5.2      50 us
       *    [2][2,80]    192.168.5.4 => 192.168.5.3      80 us
       *
       * Then the network averages array should be:
       *
       * [0] = 100
       * [1] =  96
       * [2] =  63
       * [3] =  66
       **/
      Averages_Map         network_averages;

      /**
       * This is the target workflow that is being optimized. Note that the
       * ordering doesn't matter, e.g., 
       *
       * [0][1][1,4]            [1][0][0,1]
       * [0][1][1,2]            [1][0][0,2]
       *                and
       * [0][0][0,1]            [1][1][1,4]
       * [0][0][0,2]            [1][1][1,2]
       * 
       * are the same target deployment. Each of them are asking to create
       * a deployment that seeks to optimize links for component 0 and 1 where
       * 
       *        0->1    and     1->2
       *        0->2            1->4
       *
       * which equates to a visual of this:
       *
       *              2
       *           /  
       *        0     |
       *           \    
       *              1  - 4
       *              
       **/
      Workflow     target_deployment;

      /**
       * A listing of all paths
       **/
      Paths        paths;

      /**
       * This is the solution to the target_deployment according
       * to the averages in network_averages. These integers should
       * be mapped to the ids listing for user-readable deployments.
       * e.g., if the target deployment was
       * 
       * [0][1][1,4]            [1][0][0,1]
       * [0][1][1,2]            [1][0][0,2]
       *
       * The solution would require placement of 0, 1, 2, and 4 at least.
       * The result for 3 is arbitrary since it is not indicated as important.
       * Consequently, our solution should have 5 elements and a mapping
       * can be made to the ids array
       **/
      Deployment    solution;

      /**
       * The solution_lookup allows for reverse lookup by network identifier
       * to find its corresponding deployment solution id. This is used by
       * the Madara::Cid::approximate function to check if a network id
       * has already been assigned to one of the other components in the
       * deployment.
       **/
      Solution_Map  solution_lookup;
    };
  }
}

#endif // _CID_SETTINGS_H_
