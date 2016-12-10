/**
 * Problem 7 「10001番目の素数」
 * 素数を小さい方から6つ並べると 2, 3, 5, 7, 11, 13 であり, 6番目の素数は 13 である.
 *
 * 10 001 番目の素数を求めよ.
 */

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>
#include "prime.h"

using uInt = std::uint_fast64_t;
using namespace Euler::Prime;

int main(void)
{
  try {
    std::cout << "Euler007: " << get_prime_at(10001ul) << std::endl;
  }
  catch (const std::exception &e) {
    std::cout << "Error: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
