#ifndef UTILITY_H
#define UTILITY_H

#include <vector>
#include <string>

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

    /// Split a string into tokens 
    void tokenizer (const ::std::string & input, 
      const ::std::vector< ::std::string> & splitters,
      ::std::vector< ::std::string> & tokens,
      ::std::vector< ::std::string> & pivot_list);
  }
}
#endif
