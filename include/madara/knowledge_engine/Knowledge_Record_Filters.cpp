#include "Knowledge_Record_Filters.h"
#include "madara/utility/Utility.h"
#include "madara/utility/Log_Macros.h"

Madara::Knowledge_Engine::Knowledge_Record_Filters::Knowledge_Record_Filters ()
  : context_ (0)
{
}

Madara::Knowledge_Engine::Knowledge_Record_Filters::Knowledge_Record_Filters (
  const Knowledge_Record_Filters & filters)
  : filters_ (filters.filters_), context_ (filters.context_)
{
}

Madara::Knowledge_Engine::Knowledge_Record_Filters::~Knowledge_Record_Filters ()
{
}

void
Madara::Knowledge_Engine::Knowledge_Record_Filters::operator= (
  const Knowledge_Record_Filters & rhs)
{
  if (this != &rhs)
  {
    filters_ = rhs.filters_;
    context_ = rhs.context_;
  }
}

void
Madara::Knowledge_Engine::Knowledge_Record_Filters::add (uint32_t types,
        Knowledge_Record (*function) (Function_Arguments &, Variables &))
{
  if (function != 0)
  {
    // start with 1st bit, check every bit until types is 0
    for (uint32_t cur = 1; types > 0; cur <<= 1)
    {
      // if current is set in the bitmask
      if (Madara::Utility::bitmask_check (types, cur))
      {
        // remove the filter list from the type cur
        filters_[cur].push_back (Function (function));
      }

      // remove the current flag from the types
      types = Madara::Utility::bitmask_remove (types, cur);
    }
  }
}

void
Madara::Knowledge_Engine::Knowledge_Record_Filters::attach (
  Thread_Safe_Context * context)
{
  context_ = context;
}

void
Madara::Knowledge_Engine::Knowledge_Record_Filters::clear (uint32_t types)
{
  // start with 1st bit, check every bit until types is 0
  for (uint32_t cur = 1; types > 0; cur <<= 1)
  {
    // if current is set in the bitmask
    if (Madara::Utility::bitmask_check (types, cur))
    {
      // remove the filter list from the type cur
      filters_.erase (cur);
    }

    // remove the current flag from the types
    types = Madara::Utility::bitmask_remove (types, cur);
  }
}

void
Madara::Knowledge_Engine::Knowledge_Record_Filters::print_num_filters (
  void) const
{
  MADARA_DEBUG (0, (LM_DEBUG,
    "Printing Knowledge Record Filter Chains by Type...\n"));

  for (Filter_Map::const_iterator i = filters_.begin ();
       i != filters_.end (); ++i)
  {
    MADARA_DEBUG (0, (LM_DEBUG,
      "%d = %d chained filters\n", i->first, i->second.size ()));
  }
}


Madara::Knowledge_Record
Madara::Knowledge_Engine::Knowledge_Record_Filters::filter (
  const Knowledge_Record & input,
  const std::string & name) const
{
  // grab the filter chain entry for the type
  uint32_t type = input.type ();
  Filter_Map::const_iterator type_match = filters_.find (type);
  Knowledge_Record result (input);

  // if there are filters for this type
  if (type_match != filters_.end ())
  {
    const Filter_Chain & chain = type_match->second;
    Function_Arguments arguments;
    Variables variables;
    variables.context_ = context_;
    
    for (Filter_Chain::const_iterator i = chain.begin ();
         i != chain.end (); ++i)
    {
      /**
       * arguments vector is modifiable by filter, so we have to
       * resize every filter call to make sure we have adequate space
       **/

      if (name == "")
      {
        arguments.resize (1);
      }
      else
      {
        // second argument is the variable name, if applicable
        arguments.resize (2);
        arguments[1].set_value (name);
      }

      // setup arguments to the function
      arguments[0] = result;

      // if the function is not zero
      if (i->extern_unnamed_)
      {
        result = i->extern_unnamed_ (arguments, variables);
      }
    }
  }

  return result;
}


size_t
Madara::Knowledge_Engine::Knowledge_Record_Filters::get_number_of_filtered_types (
  void) const
{
  return filters_.size ();
}
