#include "Arguments.h"
#include "Generic_Filters.h"
#include <sstream>

const char * operation_types [] = {
  "IDLE_OPERATION",
  "SENDING_OPERATION",
  "RECEIVING_OPERATION",
  "REBROADCASTING_OPERATION"
};


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

Madara::Knowledge_Record
Madara::Filters::log_args (Knowledge_Engine::Function_Arguments & args,
                           Knowledge_Engine::Variables & vars)
{
  Madara::Knowledge_Record result;

  if (args.size () == Madara::Filters::TOTAL_ARGUMENTS)
  {
    // return the first arg for further processing by other filters
    result = args[0];

    std::stringstream buffer;
    buffer << "Filter args:\n";
    
    // [0] Record
    buffer << "  [0:Record]:            " ;
    buffer << args[0].to_string ();
    buffer << "\n";
    
    // [1] Record
    buffer << "  [1:Record Name]:       " ;
    buffer << args[1].to_string ();
    buffer << "\n";
    
    // [2] Operation Type
    buffer << "  [2:Operation Type]:    " ;
    buffer << operation_types [args[2].to_integer ()];
    buffer << "\n";
    
    // [3] Send Bandwidth
    buffer << "  [3:Send Bandwidth]:    " ;
    buffer << args[3].to_integer ();
    buffer << " B/s\n";
    
    // [4] Receive Bandwidth
    buffer << "  [4:Receive Bandwidth]: " ;
    buffer << args[4].to_integer ();
    buffer << " B/s\n";
    
    // [5] Update Time
    buffer << "  [5:Update Time]:       " ;
    buffer << args[5].to_integer ();
    buffer << "\n";
    
    // [6] Current Time
    buffer << "  [6:Current Time]:      " ;
    buffer << args[6].to_integer ();
    buffer << "\n";
    
    // [7] Knowledge Domain
    buffer << "  [7:Knowledge Domain]:  " ;
    buffer << args[7].to_string ();
    buffer << "\n";

    vars.print (buffer.str (), 0);
  }

  return result;
}
