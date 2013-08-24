#include "Generic_Filters.h"


Madara::Knowledge_Record
Madara::Filters::discard (Knowledge_Engine::Function_Arguments & args,
              Knowledge_Engine::Variables & vars)
{
  // this works because a default record is UNCREATED
  return Madara::Knowledge_Record ();
}

Madara::Knowledge_Record
Madara::Filters::discard_nonprimitives (Knowledge_Engine::Function_Arguments & args,
              Knowledge_Engine::Variables & vars)
{
  Madara::Knowledge_Record result;

  if (args.size () > 0)
  {
    /**
     * Alternatively, we could check type () for each
     * defined type, but this will check that we are not
     * a file type
     **/
    if (!args[0].is_file_type ())
    {
      result = args[0];
    }
  }

  /**
   * At the transport layer, we should check for stripped types
   * and remove them from the list of knowledge records
   **/
  return result;
}


Madara::Knowledge_Record
Madara::Filters::discard_nonfiles (Knowledge_Engine::Function_Arguments & args,
              Knowledge_Engine::Variables & vars)
{
  Madara::Knowledge_Record result;

  if (args.size () > 0)
  {
    /**
     * Alternatively, we could check type () for each
     * defined type, but this will check that we are
     * a file type
     **/
    if (args[0].is_file_type ())
    {
      result = args[0];
    }
  }

  /**
   * At the transport layer, we should check for stripped types
   * and remove them from the list of knowledge records
   **/
  return result;
}
