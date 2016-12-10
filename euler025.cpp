/**
 * Problem 25 「1000桁のフィボナッチ数」
 * フィボナッチ数列は以下の漸化式で定義される:
 *
 * Fn = Fn-1 + Fn-2, ただし F1 = 1, F2 = 1.
 * 最初の12項は以下である.
 *
 * F1 = 1
 * F2 = 1
 * F3 = 2
 * F4 = 3
 * F5 = 5
 * F6 = 8
 * F7 = 13
 * F8 = 21
 * F9 = 34
 * F10 = 55
 * F11 = 89
 * F12 = 144
 * 12番目の項, F12が3桁になる最初の項である.
 *
 * 1000桁になる最初の項の番号を答えよ.
 */

#include <cstdint>
#include <iostream>
#include <utility>
#include "largeint.h"

using uInt = std::uint_fast32_t;
using namespace Euler;

int main(void)
{
  LargeInt<uInt> f1 {1};    // F_(n-1)
  LargeInt<uInt> f2 {1};    // F_(n-2)

  uInt i = 2;
  while (f1.num_digits() < 1000) {
    i++;
    f2 += f1;
    std::swap(f1, f2);
  }

  std::cout << "Euler025: " << i << std::endl;
  return 0;
}
