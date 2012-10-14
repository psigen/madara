

#ifndef _MADARA_GLOBALS_CPP_
#define _MADARA_GLOBALS_CPP_

#include "madara/Globals.h"

bool
Madara::Knowledge_Record::lt (VALUE_TYPE lhs, VALUE_TYPE rhs)
{
  return lhs < rhs;
}

bool
Madara::Knowledge_Record::lte (VALUE_TYPE lhs, VALUE_TYPE rhs)
{
  return lhs <= rhs;
}

bool
Madara::Knowledge_Record::eq (VALUE_TYPE lhs, VALUE_TYPE rhs)
{
  return lhs == rhs;
}

bool
Madara::Knowledge_Record::gt (VALUE_TYPE lhs, VALUE_TYPE rhs)
{
  return lhs > rhs;
}

bool  
Madara::Knowledge_Record::gte (VALUE_TYPE lhs, VALUE_TYPE rhs)
{
  return lhs >= rhs;
}





#endif
