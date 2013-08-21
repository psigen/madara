#ifndef   _MADARA_UTILITY_INL_
#define   _MADARA_UTILITY_INL_

#include "Utility.h"

template <typename T>
T Madara::Utility::bitmask_add (T mask, T values)
{
  return mask | values;
}
    
/**
  * Returns true if mask contains values
  **/
template <typename T>
bool Madara::Utility::bitmask_check (T mask, T values)
{
  return (mask & values) > 0;
}
    
/**
  * Removes values from a bit mask
  **/
template <typename T>
T Madara::Utility::bitmask_remove (T mask, T values)
{
  return mask & ~values;
}

#endif  // _MADARA_UTILITY_INL_