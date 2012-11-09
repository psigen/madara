#ifndef _KNOWLEDGE_RECORD_CPP_
#define _KNOWLEDGE_RECORD_CPP_

#include "madara/knowledge_engine/Knowledge_Record.h"

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





#endif   // _KNOWLEDGE_RECORD_CPP_
