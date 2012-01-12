#include <ctype.h>
#include <stdlib.h>
#include "madara/utility/Utility.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "ace/INET_Addr.h"
#include "madara/utility/Log_Macros.h"
#include "ace/Default_Constants.h"

/// Convert string to uppercase
::std::string &
Madara::Utility::upper (::std::string &input)
{
  for (::std::string::iterator cur = input.begin ();
       cur != input.end (); ++cur)
    *cur = toupper (*cur);

  return input;
}

/// Convert string to lowercase 
::std::string & 
Madara::Utility::dds_topicify (::std::string & input)
{
  for (::std::string::iterator cur = input.begin ();
       cur != input.end (); ++cur)
  {
    // change periods to _
    if (*cur == '.')
      *cur = '_';
  }

  return input;
}

/// Convert string to lowercase 
::std::string &
Madara::Utility::lower (::std::string &input)
{
  for (::std::string::iterator cur = input.begin ();
       cur != input.end (); ++cur)
    *cur = tolower (*cur);

  return input;
}

/// Strip whitespace from front and end of string and also
/// condense multiple whitespace into a single space
::std::string &
Madara::Utility::strip_extra_white_space (::std::string & input)
{
  ::std::string::iterator cur = input.begin ();
  char prev = 0;

  for (::std::string::iterator eval = cur; 
    eval != input.end (); ++eval)
  {
    // if it isn't whitespace, then copy it over immediately
    if (*eval != ' ' && *eval != '\t' && *eval != '\n' && *eval != '\r')
    {
      prev = *cur = *eval;
      ++cur;
    }
    // if it is whitespace, only insert whitespace if the previous char
    // was non-whitespace
    else if (prev)
    {
      *cur = ' ';
      prev = 0;
      ++cur;
    }
  }

  // if the last char is actually whitespace, then move cur back one spot
  if (cur != input.end ())
  {
    --cur;
    if (*cur != ' ' && *cur != '\t' && *cur != '\n' && *cur != '\r')
      ++cur;
  }

  // erase everything from cur to end of input string
  if (cur != input.end ())
    input.erase (cur, input.end ());

  return input;
}


/// Strip all whitespace
::std::string &
Madara::Utility::strip_white_space (::std::string & input)
{
  ::std::string::iterator cur = input.begin ();
  char prev = 0;

  for (::std::string::iterator eval = cur; 
    eval != input.end (); ++eval)
  {
    // if it isn't whitespace, then copy it over immediately
    if (*eval != ' ' && *eval != '\t' && *eval != '\n' && *eval != '\r')
    {
      prev = *cur = *eval;
      ++cur;
    }
  }

  // erase everything from cur to end of input string
  if (cur != input.end ())
    input.erase (cur, input.end ());

  return input;
}

std::string &
Madara::Utility::strip_comments (std::string & input)
{
  std::stringstream source, dest;
  std::string cur;
  std::vector <std::string> splitters;
  splitters.resize (2);

  splitters[0] = "//";

  // place the input in the string stream
  source << input;
  
  while (std::getline (source, cur))
  {
    std::vector <std::string> tokens;
    std::vector <std::string> pivots;
    tokenizer (cur, splitters, tokens, pivots);

    if (tokens.size ())
    {
      dest << tokens[0];
      dest << "\n";
    }
  }

  input = dest.str ();
  return input;
}


/// Split a string into tokens 
void 
Madara::Utility::tokenizer (const ::std::string & input, 
    const ::std::vector< ::std::string> & splitters,
    ::std::vector< ::std::string> & tokens,
    ::std::vector< ::std::string> & pivots)
{
  ::std::string::size_type last = 0;
  ::std::string::size_type cur = 0;
  tokens.clear ();
  pivots.clear ();

  for (; cur < input.size (); ++cur)
  {
    for (::std::string::size_type i = 0; i < splitters.size (); ++i)
    {
      // if the splitter string length is greater than zero
      if (splitters[i].size () > 0)
      {
        // if the first char of the splitter string is equal to the char
        if (input[cur] == splitters[i][0])
        {
          ::std::string::size_type checker = cur;
          ::std::string::size_type j = 1;
          for (++checker; 
            checker < input.size () && j < splitters[i].size () &&
            input[checker] == splitters[i][j];
            ++j, ++checker);

          // we have found a splitter. Tokenize from last to splitter.
          if (j == splitters[i].size ())
          {
            // need to update this to only have as many pivots as tokens - 1
            pivots.push_back (input.substr (cur, j));

            if (cur - last >= splitters[i].size () - 1)
              tokens.push_back (input.substr (last, cur - last));
            else
              tokens.push_back ("");

            // we want last to point to the last valid token begin
            cur = checker - 1;
            last = checker;
          } // if found a splitter
        } // if first char == splitter first char
      } // if splitter length >= 1
    } // for splitters

  } // for chars

  if (last != cur)
  {
    tokens.push_back (input.substr (last, cur - last));
  }
}

// split a key into a corresponding host and port
int 
Madara::Utility::split_hostport_identifier (const std::string & key, 
    std::string & host, std::string & port)
{
  // delimeter is either a : or an @
  std::string::size_type delim = key.rfind (':');
  delim = delim == key.npos ? key.rfind ('@') : delim;

  // no delimeter found
  if (delim == key.npos)
    {
      host = key;
      port = "";
      
      return 1;
    }

  // otherwise, set the host and port appropriately
  host = key.substr (0, delim);
  port = key.substr (delim + 1, key.size () - delim);

  return 0;
}

// merge a host and port into a key
int 
Madara::Utility::merge_hostport_identifier (std::string & key, 
  const std::string & host, const std::string & port)
{
  key = host;
  key += ':';
  key += port;

  return 0;
}

// merge a host and ushort port into a key
int 
Madara::Utility::merge_hostport_identifier (std::string & key, 
  const std::string & host, unsigned short u_port)
{
  std::stringstream port_stream;
  port_stream << u_port;

  return merge_hostport_identifier (key, host, port_stream.str ());
}

/// Bind to an ephemeral port
int 
Madara::Utility::bind_to_ephemeral_port (ACE_SOCK_ACCEPTOR & acceptor,
              unsigned short & port, bool increase_until_bound)
{
  //ACE_DEBUG ((LM_DEBUG, 
  //    "(%P|%t) Binding starting with %d\n",
  //            port));
  // start with the initial port provided
  // increase port each time we don't properly bind
  
  for (ACE_INET_Addr addr (port); port < 65535; ++port, addr.set (port))
  {
    //ACE_DEBUG ((LM_DEBUG, 
    //  "(%P|%t) Attempting bind of %d\n",
    //  addr.get_port_number ()));
    // Output some debugging information so we know how far we had to go
    // to get a port
    // return correct if we are able to open the port
    if (acceptor.open (addr) != -1)
      return 0;     

    // failed to get port
    //ACE_DEBUG ((LM_DEBUG, 
    //          "(%P|%t) failed to acquire port %d: addr reporting %d\n",
    //          port, addr.get_port_number ()));
    if (!increase_until_bound)
      break;
  }

  return -1;
}

std::string
Madara::Utility::file_to_string (const std::string & filename)
{
  std::string line;
  std::stringstream buffer;

  std::ifstream file (filename.c_str ());

  // if the file was able to open
  if (file.is_open ())
  {
    // while there is still a line left in the file, read that line
    // into our stringstream buffer
    while (std::getline (file, line))
      buffer << line << "\n";
    file.close ();
  }

  return buffer.str ();
}


std::string
Madara::Utility::extract_path (const std::string & name)
{
  std::string::size_type start = 0;
  for (std::string::size_type i = 0; i < name.size (); ++i)
  {
    // check for directory delimiters and update start
    if (name[i] == '/' || name[i] == '\\')
    {
      // if they have supplied a directory with an
      // ending slash, then use the previous start
      if (i != name.size () - 1)
        start = i + 1;
    }
  }

  // return the substring from 0 with start number of elements
  return name.substr (0, start);
}

std::string
Madara::Utility::extract_filename (const std::string & name)
{
  std::string::size_type start = 0;
  for (std::string::size_type i = 0; i < name.size (); ++i)
  {
    // check for directory delimiters and update start
    if (name[i] == '/' || name[i] == '\\')
    {
      // if they have supplied a directory with an
      // ending slash, then use the previous start
      if (i != name.size () - 1)
        start = i + 1;
    }
  }

  // return the substring from start to the end of the filename
  return name.substr (start, name.size () - start);
}

/// Expand any environment variables in a string
std::string
Madara::Utility::expand_envs (const std::string & source)
{
  std::stringstream buffer;

  for (size_t i = 0; i < source.size (); ++i)
  {
    // environment variable must be larger than $()
    if (source[i] == '$' && i + 3 < source.size ())
    {
      char * value = get_var (source, i+2, i);
      if (value)
        buffer << value;
    }
    else
      buffer << source[i];
  }
  return buffer.str ();
}

/// grab an environment variable value (@see expand_envs)
char * 
Madara::Utility::get_var (const std::string & source, 
                          size_t cur, size_t & end)
{
  for (end = cur; end < source.size (); ++end)
  {
    if (source[end] == ')' || source[end] == '}')
    {
      return getenv (source.substr (cur, end - cur).c_str ());
    }
  }

  return getenv (source.substr (cur,source.size () - cur).c_str ());
}

std::string
Madara::Utility::clean_dir_name (const std::string & source)
{
  // define the characters we'll want to replace
#ifdef ACE_WIN32
  #define REPLACE_THIS    '/'
  #define REPLACE_WITH    '\\'
#else
  #define REPLACE_THIS    '\\'
  #define REPLACE_WITH    '/'
#endif

  std::string target (source);

  for (std::string::iterator i = target.begin (); i != target.end(); ++i)
  {
    if (*i == REPLACE_THIS)
      *i = REPLACE_WITH;
  }

  return target;
}