/**
 * Problem 29 「個別のべき乗」
 * 2 ≤ a ≤ 5 と 2 ≤ b ≤ 5について, ab を全て考えてみよう:
 *
 * 22=4, 23=8, 24=16, 25=32
 * 32=9, 33=27, 34=81, 35=243
 * 42=16, 43=64, 44=256, 45=1024
 * 52=25, 53=125, 54=625, 55=3125
 * これらを小さい順に並べ, 同じ数を除いたとすると, 15個の項を得る:
 *
 * 4, 8, 9, 16, 25, 27, 32, 64, 81, 125, 243, 256, 625, 1024, 3125
 *
 * 2 ≤ a ≤ 100, 2 ≤ b ≤ 100 で同じことをしたときいくつの異なる項が存在するか?
 */

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>
#include "largeint.h"

using uInt = std::uint_fast32_t;
using namespace Euler;

inline
LargeInt<uInt> large_pow(uInt a, uInt b)
{
  LargeInt<uInt> la(a);
  for (uInt c = 1; c < b; c++) {  // 累乗計算
    la *= a;
  }
  return std::move(la);
}

int main(void)
{
  std::vector<LargeInt<uInt>> v;
  v.reserve(99 * 99);
  for (int a = 2; a < 101; a++) {
    for (int b = 2; b < 101; b++) {
      v.push_back(large_pow(a, b));
    }
  }
  std::sort(v.begin(), v.end());
  auto end = std::unique(v.begin(), v.end());
  std::cout << "Euler029: " << end - v.begin() << std::endl;
  return 0;
}
