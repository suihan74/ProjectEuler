/**
 * Problem 3 「最大の素因数」
 * 13195 の素因数は 5, 7, 13, 29 である.
 * 600851475143 の素因数のうち最大のものを求めよ.
 */

#include <cstdint>
#include <iostream>
#include <cmath>

using uInt = std::uint_fast64_t;
constexpr uInt INPUT = 600851475143;

int main(void)
{
  uInt num = INPUT;
  for (uInt i = 2; num > 1; i++) {
    if (num % i == 0) {
      std::cout << i << std::endl;
      num /= i;
    }
  }

  return 0;
}
