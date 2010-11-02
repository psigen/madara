#ifndef UTILITY_H
#define UTILITY_H

#include <vector>
#include <string>
#include "ace/SOCK_Acceptor.h"

namespace Madara
{
  namespace Utility
  {
    /// Convert string to uppercase
    ::std::string & upper (::std::string & input);
    
    /// Convert string to lowercase 
    ::std::string & lower (::std::string & input);

    /// Strip all whitespace
    ::std::string & strip_white_space (::std::string & input);

    /// Strip whitespace from front and end of string and also
    /// condense multiple whitespace into a single space
    ::std::string & strip_extra_white_space (::std::string & input);

    // split a key into a corresponding host and port
    int split_hostport_identifier (const std::string & key, 
      std::string & host, std::string & port);

    // merge a host and port into a key
    int merge_hostport_identifier (std::string & key, 
      const std::string & host, const std::string & port);

    // merge a host and port into a key
    int merge_hostport_identifier (std::string & key, 
      const std::string & host, unsigned short u_port);

    /// Split a string into tokens 
    void tokenizer (const ::std::string & input, 
      const ::std::vector< ::std::string> & splitters,
      ::std::vector< ::std::string> & tokens,
      ::std::vector< ::std::string> & pivot_list);

    /// Bind to an ephemeral port
    int bind_to_ephemeral_port (ACE_SOCK_Acceptor & acceptor,
      unsigned short & port, bool increase_until_bound = true);

    std::string file_to_string (const std::string & filename);
  }
}
#endif
