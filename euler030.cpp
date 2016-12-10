/**
 * Problem 30 「各桁の5乗」
 * 驚くべきことに, 各桁を4乗した数の和が元の数と一致する数は3つしかない.
 *
 * 1634 = 14 + 64 + 34 + 44
 * 8208 = 84 + 24 + 04 + 84
 * 9474 = 94 + 44 + 74 + 44
 * ただし, 1=14は含まないものとする. この数たちの和は 1634 + 8208 + 9474 = 19316 である.
 *
 * 各桁を5乗した数の和が元の数と一致するような数の総和を求めよ.
 */

#include <cstdint>
#include <cmath>
#include <iostream>

using uInt = std::uint_fast32_t;

inline
bool equals_powers_sum(uInt num, uInt power)
{
  uInt n = num;
  uInt sum = 0;
  while (n) {
    const uInt scale = std::pow(10, static_cast<uInt>(std::log10(n)));
    const uInt digit = n / scale;
    sum += std::pow(digit, power);
    if (sum > num) { return false; }
    n -= digit * scale;
  }
  return sum == num;
}

int main(void)
{
  uInt sum = 0;
  // 6桁x9^5=345294 で6桁ではこれ以上には存在しない
  // 7桁x9^5=413343 で最大の組でも6桁になってしまうので7桁には存在しない(8桁以降も同様)
  for (uInt i = 2; i < 345295; i++) {
    if (equals_powers_sum(i, 5)) {
      std::cout << i << std::endl;
      sum += i;
    }
  }
  std::cout << "Euler030: " << sum << std::endl;
  return 0;
}
