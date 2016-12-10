/**
 * Problem 46 「もうひとつのゴールドバッハの予想」
 * Christian Goldbachは全ての奇合成数は平方数の2倍と素数の和で表せると予想した.
 *
 * 9 = 7 + 2×1^2
 * 15 = 7 + 2×2^2
 * 21 = 3 + 2×3^2
 * 25 = 7 + 2×3^2
 * 27 = 19 + 2×2^2
 * 33 = 31 + 2×1^2
 *
 * 後に, この予想は誤りであることが分かった.
 *
 * 平方数の2倍と素数の和で表せない最小の奇合成数はいくつか?
 */

#include <cstdint>
#include <iostream>
#include "prime.h"

using uInt = std::uint_fast32_t;
using namespace Euler::Prime;

/**
 * 奇合成数であるかを確認
 * @param num  対象の値
 * @return     奇数でかつ合成数の場合true
 */
inline
bool is_odd_composite(const uInt num)
{
  if (num % 2 == 0) { return false; }
  for (uInt i = 3; i * i <= num; i += 2) {
    if (num % i == 0) { return true; }
  }
  return false;
}

int main(void)
{
  for (uInt i = 1; ; i++) {
    if (!is_odd_composite(i)) { continue; }
    bool failed = true;
    for (uInt j = 1, tmp; (tmp = j * j * 2) < i; j++) {
      if (is_prime(i - tmp)) {
        failed = false;
        break;
      }
    }
    if (failed) {
      std::cout << "Euler046: " << i << std::endl;
      break;
    }
  }
  return 0;
}
