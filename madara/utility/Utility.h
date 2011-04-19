#ifndef UTILITY_H
#define UTILITY_H

#include <vector>
#include <string>
#include "ace/SOCK_Acceptor.h"
#include "madara/MADARA_export.h"

namespace Madara
{
  namespace Utility
  {
    /// Convert string to uppercase
    MADARA_Export ::std::string & upper (::std::string & input);
    
    /// Convert string to lowercase 
    MADARA_Export ::std::string & lower (::std::string & input);

    /// Convert string to lowercase 
    MADARA_Export ::std::string & dds_topicify (::std::string & input);

    /// Strip all whitespace
    MADARA_Export ::std::string & strip_white_space (::std::string & input);

    /// Strip whitespace from front and end of string and also
    /// condense multiple whitespace into a single space
    MADARA_Export ::std::string & strip_extra_white_space (
      ::std::string & input);

    // split a key into a corresponding host and port
    MADARA_Export int split_hostport_identifier (const std::string & key, 
      std::string & host, std::string & port);

    // merge a host and port into a key
    MADARA_Export int merge_hostport_identifier (std::string & key, 
      const std::string & host, const std::string & port);

    // merge a host and port into a key
    MADARA_Export int merge_hostport_identifier (std::string & key, 
      const std::string & host, unsigned short u_port);

    /// Split a string into tokens 
    MADARA_Export void tokenizer (const ::std::string & input, 
      const ::std::vector< ::std::string> & splitters,
      ::std::vector< ::std::string> & tokens,
      ::std::vector< ::std::string> & pivot_list);

    /// Bind to an ephemeral port
    MADARA_Export int bind_to_ephemeral_port (ACE_SOCK_Acceptor & acceptor,
      unsigned short & port, bool increase_until_bound = true);

    /// Return file contents as a string
    MADARA_Export std::string file_to_string (const std::string & filename);

    /// Return the path of the provided file or directory
    MADARA_Export std::string extract_path (const std::string & name);
    
    /// Return the file name of the provided file or directory
    MADARA_Export std::string extract_filename (const std::string & name);

    /// Expand any environment variables in a string
    MADARA_Export std::string expand_envs (
      const std::string & source);

    /// grab an environment variable value (@see expand_envs)
    char * get_var (const std::string & source, size_t cur, size_t & end);


  }
}
#endif
