#ifndef _MADARA_KNOWLEDGE_BASE_FILES_H_
#define _MADARA_KNOWLEDGE_BASE_FILES_H_

/**
 * @file Files.h
 * @author James Edmondson <james.r.edmondson@vanderbilt.edu>
 *
 * This file contains the Knowledge_Base Files class
 */

#include <string>
#include "madara/knowledge_engine/Thread_Safe_Context.h"

namespace Madara
{
  namespace Knowledge_Engine
  {
    /**
     * @class Files
     * @brief This class provides file repo capabilities to the knowledge base
     */
    class Files
    {
    public:
      /**
       * Constructor
       */
      Files (Thread_Safe_Context & map);

      /**
       * Read a file into the knowledge base
       * @param file_name           file to read
       * @param knowledge_key       key to store the file into
       */
      int read_file (const std::string & file_name, 
                     const std::string & knowledge_key);
      
      /**
       * Write a file from the knowledge base to a specified location
       * @param file_name           file to write to
       * @param knowledge_key       key to read the file from
       */
      int write_file (const std::string & file_name, 
                      const std::string & knowledge_key);

      
      /**
       * Read a policy into the knowledge base
       * @param policy_file         file to read the policy from
       * @param policy_key          key to read the policy into
       */
      int read_policy (const std::string & policy_key, 
                       const std::string & policy_file);

      /**
       * Shape a file according to a shaping policy
       * @param source_key          file that needs to be shaped
       * @param target_key          target key to save the shaped file into
       * @param policy_key          policy to use for shaping the file
       */
      int shape_file (const std::string & source_key, 
                      const std::string & target_key, 
                      const std::string & policy_key);

    private:
      // folder to store files into
      const static std::string files_folder_;

      // folder to store policies into
      const static std::string policies_folder_;

      // handle to the knowledge base
      Thread_Safe_Context & map_;
    };
  }
}


#endif