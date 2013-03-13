#ifndef UTILITY_H
#define UTILITY_H

#include <vector>
#include <string>
#include "ace/SOCK_Acceptor.h"
#include "madara/MADARA_export.h"
#include "madara/utility/stdint.h"


namespace Madara
{
  namespace Utility
  {
    /**
     * Converts the string to upper. This function will modify and return the
     * input, so make a copy if needed.
     * @param     input         the string to change
     * @return    a reference to the modified input
     **/
    MADARA_Export std::string & upper (std::string & input);
    
    /**
     * Converts the string to lower. This function will modify and return the
     * input, so make a copy if needed.
     * @param     input         the string to change
     * @return    a reference to the modified input
     **/
    MADARA_Export std::string & lower (std::string & input);
    
    /**
     * Changes periods to underscores in compliance with OpenSplice needs.
     * This function will modify and return the input, so make a copy if
     * needed.
     * @param     input         the string to change
     * @return    a reference to the modified input
     **/
    MADARA_Export std::string & dds_topicify (std::string & input);
    
    /**
     * Strips all whitespace characters from a string. This function will
     * modify and return the input, so make a copy if needed.
     * @param     input         the string to change
     * @return    a reference to the modified input
     **/
    MADARA_Export std::string & strip_white_space (std::string & input);
    
    /**
     * Strips all comments (single-line and multi-line). This function will
     * modify and return the input, so make a copy if needed.
     * @param     input         the string to change
     * @return    a reference to the modified input
     **/
    MADARA_Export std::string & strip_comments (std::string & input);
    
    /**
     * Strips an unwanted character. This function will
     * modify and return the input, so make a copy if needed.
     * @param     input         the string to change
     * @param     unwanted      the character value to remove from input
     * @return    a reference to the modified input
     **/
    MADARA_Export std::string & string_remove (std::string & input,
      char unwanted);

    /**
     * Strips whitespace from front and end of string and also
     * condenses multiple whitespace into a single space. This function will
     * modify and return the input, so make a copy if needed.
     * @param     input         the string to change
     * @return    a reference to the modified input
     **/
    MADARA_Export std::string & strip_extra_white_space (
      std::string & input);
    
    /**
     * Splits a key of host:port into a corresponding host and port.
     * @param     key         a string containing a host:port
     * @param     host        the host in the key
     * @param     port        the port in the key
     * @return    1 if there was no ":". 0 if both host and port are set.
     **/
    MADARA_Export int split_hostport_identifier (const std::string & key, 
      std::string & host, std::string & port);
    
    /**
     * Merges a host and port into a host:port key
     * @param     key         a string containing a host:port
     * @param     host        the host in the key
     * @param     port        the port in the key
     * @return    0 if successful (always successful right now)
     **/
    MADARA_Export int merge_hostport_identifier (std::string & key, 
      const std::string & host, const std::string & port);
    
    /**
     * Merges a host and port into a host:port key
     * @param     key         a string containing a host:port
     * @param     host        the host in the key
     * @param     port        the port in the key
     * @return    0 if successful (always successful right now)
     **/
    MADARA_Export int merge_hostport_identifier (std::string & key, 
      const std::string & host, unsigned short u_port);
    
    /**
     * Splits an input string into tokens.
     * @param     input       a string to be split by splitters
     * @param     splitters   a vector of strings to look for in input
     * @param     tokens      the sections in between splitters
     * @param     pivot_list  the actual splitters that were found between
     *                        the tokens
     **/
    MADARA_Export void tokenizer (const std::string & input, 
      const ::std::vector< std::string> & splitters,
      ::std::vector< std::string> & tokens,
      ::std::vector< std::string> & pivot_list);
    
    /**
     * Binds to an ephemeral port
     * @param     acceptor    acceptor socket so caller can use the socket
     * @param     host        the host name of this device or computer
     * @param     port        starting port to try to bind to
     * @param     increase_until_bound   if true, increment port until success
     * @return    0 if successful bind. -1 if unsuccessful bind.
     **/
    MADARA_Export int bind_to_ephemeral_port (ACE_SOCK_Acceptor & acceptor,
      std::string & host, unsigned short & port,
      bool increase_until_bound = true);
    
    /**
     * Reads a file into a string
     * @param     filename    name of the file to read
     * @return    contents of the file. Null string if unsuccessful.
     **/
    MADARA_Export std::string file_to_string (const std::string & filename);

    /**
     * Extracts the path of a filename
     * @param     name        name of the file to extract path of
     * @return    the directory path of the file
     **/
    MADARA_Export std::string extract_path (const std::string & name);
    
    /**
     * Extracts the file name of an absolute or relative path
     * @param     name        name of the file to extract path of
     * @return    the file name
     **/
    MADARA_Export std::string extract_filename (const std::string & name);

    /**
     * Expands environment variables referenced in the string. The environment
     * variables must be specified as $(var) and not $var.
     * @param     source      the string to expand
     * @return    a string with environment variables expanded
     **/
    MADARA_Export std::string expand_envs (
      const std::string & source);

    /**
     * Helper function for Madara::Utility::expand_envs which retrieves an
     * environment variable.
     * @param     source      the string to expand
     * @param     cur         the current focus position of expand_vars
     * @param     end         the end of the variable
     * @return    the value of the environment variable
     **/
    char * get_var (const std::string & source, size_t cur, size_t & end);
    
    /**
     * Substitutes the appropriate directory delimiter, which may help
     * with portability between operating systems.
     * @param     target      the string to modify
     * @return    a more compliant directory path string
     **/
    MADARA_Export std::string clean_dir_name (const std::string & target);
    
    /**
     * Converts a host format uint64_t into big endian
     * @param     value      the value to convert
     * @return    the converted value
     **/
    MADARA_Export uint64_t endian_swap (uint64_t value);
    
    /**
     * Converts a host format signed Madara::Knowledge_Record::Integer into big endian
     * @param     value      the value to convert
     * @return    the converted value
     **/
    MADARA_Export int64_t endian_swap (int64_t value);
    
    /**
     * Converts a host format uint64_t into big endian
     * @param     value      the value to convert
     * @return    the converted value
     **/
    MADARA_Export uint32_t endian_swap (uint32_t value);
    
    /**
     * Converts a host format signed Madara::Knowledge_Record::Integer into big endian
     * @param     value      the value to convert
     * @return    the converted value
     **/
    MADARA_Export int32_t endian_swap (int32_t value);

    /**
     * Reads a file into a provided void pointer. The void pointer will point
     * to an allocated buffer that the user will need to delete.
     * @param    filename   the name of the file
     * @param    buffer     a buffer that contains the contents of the file
     * @param    size       the size of the allocated buffer (will change)
     * @param    add_zero_char  add a zero char to the end of the buffer
     * @return              zero if successful
     **/
    MADARA_Export int  read_file (const std::string & filename,
      void *& buffer, size_t & size, bool add_zero_char = false);
    
    /**
     * Writes a file with provided contents.
     * @param    filename   the name of the file
     * @param    buffer     a buffer that contains the contents of the file
     * @param    size       the size of the allocated buffer
     * @return              amount of bytes written (-1 if unsuccessful)
     **/
    ssize_t write_file (const std::string & filename,
      void * buffer, size_t size);
  }
}
#endif
