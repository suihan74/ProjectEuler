/**
 * Problem 56 「もっとべき乗の数字和」
 * Googol (10^100)は非常に大きな数である: 1の後に0が100個続く. 100^100は想像を絶する. 1の後に0が200回続く. その大きさにも関わらず, 両者とも数字和 ( 桁の和 ) は1である.
 *
 * a, b < 100 について自然数 a^b を考える. 数字和の最大値を答えよ.
 */

#include <cstdint>
#include <iostream>
#include "largeint.h"

using uInt = std::uint_fast32_t;
using Euler::LargeInt;
using Euler::large_pow;

int main(void)
{
  uInt best_digits_sum = 0;
  for (uInt a = 1; a < 100; a++) {
    for (uInt b = 1; b < 100; b++) {
      auto num = large_pow(a, b);
      const uInt dsum = num.digits_sum();
      if (dsum > best_digits_sum) {
        best_digits_sum = dsum;
      }
    }
  }
  std::cout << "Euler056: " << best_digits_sum << std::endl;
  return 0;
}
