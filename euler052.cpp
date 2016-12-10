/**
 * Problem 52 「置換倍数」
 * 125874を2倍すると251748となる. これは元の数125874と順番は違うが同じ数を含む.
 *
 * 2x, 3x, 4x, 5x, 6x が x と同じ数を含むような最小の正整数 x を求めよ.
 */

#include <cstdint>
#include <iostream>
#include "pandigital.h"

using uInt = std::uint_fast32_t;
using namespace Euler::Pandigital;

inline
uInt make_bits(const uInt num)
{
  return used_num_bits<uInt, 0, true>(num);
}

int main(void)
{
  for (uInt i = 1; ; i++) {
    const auto bits_x1 = make_bits(i);
    bool success = true;
    for (uInt j = 2; j < 7; j++) {
      if (bits_x1 != make_bits(i * j)) {
        success = false;
        break;
      }
    }
    if (success) {
      std::cout << "Euler052: " << i << std::endl;
      break;
    }
  }
  return 0;
}
