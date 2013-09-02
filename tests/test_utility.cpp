
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <assert.h>
#include <iomanip>
#include <algorithm>

#include "madara/utility/Utility.h"

void handle_arguments (int argc, char ** argv)
{
  for (int i = 1; i < argc; ++i)
  {
    std::string arg1 (argv[i]);
    
    if (argc > 1)
    {
      std::cout <<
        "\nProgram Summary for " << argv[0] << ":\n\n" \
        "  This stand-alone application tests functions from the utility\n"
        "  functions included in the MADARA distribution.\n"
        "\n";
      exit (0);
    }
  }
}

void test_endian_swap (void)
{
  int64_t ll_value1 (1), ll_temp (0), neg_result (0);
  uint64_t ull_value1 (1), ull_temp (0);
  int64_t ll_value_neg (-1);
  
  int32_t l_value1 (1), l_temp (0), l_neg_result (0);
  uint32_t ul_value1 (1), ul_temp (0);
  int32_t l_value_neg (-1);
  std::cout << "\n********* 64 bit endian swap *************\n\n";
  std::cout << "64 bit endian_swap: Before swap:\n";
  std::cout << "     ll_value1 = " << ll_value1 << "\n";
  std::cout << "    ull_value1 = " << ull_value1 << "\n";
  std::cout << "  ll_value_neg = " << ll_value_neg << "\n";
  std::cout << "       ll_temp = " << ll_temp << "\n";
  std::cout << "      ull_temp = " << ull_temp << "\n";
  std::cout << "    neg_result = " << neg_result << "\n";

  ll_temp = Madara::Utility::endian_swap (ll_value1);
  ull_temp = Madara::Utility::endian_swap (ull_value1);
  neg_result = Madara::Utility::endian_swap (ll_value_neg);
  
  std::cout << "64 bit endian_swap: After swap to temps:\n";
  std::cout << "     ll_value1 = " << ll_value1 << "\n";
  std::cout << "    ull_value1 = " << ull_value1 << "\n";
  std::cout << "  ll_value_neg = " << ll_value_neg << "\n";
  std::cout << "       ll_temp = " << ll_temp << "\n";
  std::cout << "      ull_temp = " << ull_temp << "\n";
  std::cout << "    neg_result = " << neg_result << "\n";
  
  ll_temp = Madara::Utility::endian_swap (ll_temp);
  ull_temp = Madara::Utility::endian_swap (ull_temp);
  neg_result = Madara::Utility::endian_swap (neg_result);
  
  std::cout << "64 bit endian_swap: After temps swapped themselves:\n";
  std::cout << "     ll_value1 = " << ll_value1 << "\n";
  std::cout << "    ull_value1 = " << ull_value1 << "\n";
  std::cout << "  ll_value_neg = " << ll_value_neg << "\n";
  std::cout << "       ll_temp = " << ll_temp << "\n";
  std::cout << "      ull_temp = " << ull_temp << "\n";
  std::cout << "    neg_result = " << neg_result << "\n";
  
  std::cout << "\n********* 32 bit endian swap *************\n\n";
  
  std::cout << "32 bit endian_swap: Before swap:\n";
  std::cout << "     l_value1 = " << l_value1 << "\n";
  std::cout << "    ul_value1 = " << ul_value1 << "\n";
  std::cout << "  l_value_neg = " << l_value_neg << "\n";
  std::cout << "       l_temp = " << l_temp << "\n";
  std::cout << "      ul_temp = " << ul_temp << "\n";
  std::cout << "  l_neg_result = " << l_neg_result << "\n";

  l_temp = Madara::Utility::endian_swap (l_value1);
  ul_temp = Madara::Utility::endian_swap (ul_value1);
  l_neg_result = Madara::Utility::endian_swap (l_value_neg);
  
  std::cout << "32 bit endian_swap: After swap to temps:\n";
  std::cout << "     l_value1 = " << l_value1 << "\n";
  std::cout << "    ul_value1 = " << ul_value1 << "\n";
  std::cout << "  l_value_neg = " << l_value_neg << "\n";
  std::cout << "       l_temp = " << l_temp << "\n";
  std::cout << "      ul_temp = " << ul_temp << "\n";
  std::cout << "  l_neg_result = " << l_neg_result << "\n";
  
  l_temp = Madara::Utility::endian_swap (l_temp);
  ul_temp = Madara::Utility::endian_swap (ul_temp);
  l_neg_result = Madara::Utility::endian_swap (l_neg_result);
  
  std::cout << "32 bit endian_swap: After temps swapped themselves:\n";
  std::cout << "     l_value1 = " << l_value1 << "\n";
  std::cout << "    ul_value1 = " << ul_value1 << "\n";
  std::cout << "  l_value_neg = " << l_value_neg << "\n";
  std::cout << "       l_temp = " << l_temp << "\n";
  std::cout << "      ul_temp = " << ul_temp << "\n";
  std::cout << "  l_neg_result = " << l_neg_result << "\n";
  

}

void
test_version (void)
{
  std::string human_readable_version = Madara::Utility::get_version ();
  uint32_t uint_version = Madara::Utility::get_uint_version ();
  std::string converted_version =
    Madara::Utility::to_string_version (uint_version);
  
  std::cout << "\n********* Testing version functions *************\n\n";
  std::cout << "Version in VERSION.txt is " << human_readable_version << ".\n";
  std::cout << "Version from get_uint_version is " << uint_version << ".\n";
  std::cout << "Converted uint_version is " << converted_version << ".\n";

  if (human_readable_version == converted_version)
    std::cout << "Current version conversion is a SUCCESS.\n";
  else
    std::cout << "Current version conversion is a FAIL.\n";
}

int main (int argc, char ** argv)
{
  handle_arguments (argc, argv);
  
  test_version ();
  test_endian_swap ();

  return 0;
}