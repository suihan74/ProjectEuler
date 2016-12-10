/**
 * Problem 20 「各位の数字の和 2」
 * n × (n - 1) × ... × 3 × 2 × 1 を n! と表す.
 *
 * 例えば, 10! = 10 × 9 × ... × 3 × 2 × 1 = 3628800 となる.
 * この数の各桁の合計は 3 + 6 + 2 + 8 + 8 + 0 + 0 = 27 である.
 *
 * では, 100! の各位の数字の和を求めよ.
 *
 * 注: Problem 16 も各位の数字の和に関する問題です。解いていない方は解いてみてください。
 */

#include <cstdint>
#include <iostream>
#include "largeint.h"

using uInt = std::uint_fast32_t;
using namespace Euler;

int main(void)
{
  LargeInt<uInt> result{1};
  for (uInt i = 2; i <= 100; i++) {
    result *= i;
  }

  std::cout << result.str() << std::endl;
  std::cout << "Euler020: " << result.digits_sum() << std::endl;
  return 0;
}
