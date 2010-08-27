#include "ctype.h"
#include "madara/utility/Utility.h"
#include <iostream>

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



/// Split a string into tokens 
void 
Madara::Utility::tokenizer (const ::std::string & input, 
    const ::std::vector<::std::string> & splitters,
    ::std::vector<::std::string> & tokens,
    ::std::vector<::std::string> & pivots)
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
              tokens.push_back (input.substr (last, cur - last));

                 
            last = cur = checker;
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