/**
 * Problem 37 「切り詰め可能素数」
 * 3797は面白い性質を持っている. まずそれ自身が素数であり, 左から右に桁を除いたときに全て素数になっている (3797, 797, 97, 7). 同様に右から左に桁を除いたときも全て素数である (3797, 379, 37, 3).
 *
 * 右から切り詰めても左から切り詰めても素数になるような素数は11個しかない. 総和を求めよ.
 *
 * 注: 2, 3, 5, 7を切り詰め可能な素数とは考えない.
 */

#include <cmath>
#include <cstdint>
#include <iostream>
#include "prime.h"

using uInt = std::uint_fast32_t;
using namespace Euler::Prime;

/**
 * 右向きに切り詰め（便宜的に右シフトと呼ぶ）する過程で現れるすべての数が素数か確認
 * ex) 233 -> 23 -> 2
 * @param  num 対象の数値
 * @return     右シフトの過程で現れる数値が全て素数
 */
bool is_rshift_prime(uInt num)
{
  while (num) {
    num /= 10;
    if (num > 0 && !is_prime(num)) { return false; }
  }
  return true;
}

/**
 * 左向きに切り詰めする過程で現れるすべての数が素数か確認
 * ex) 233 -> 33(3と11の合成数)
 * @param  num 対象の数値
 * @return     左シフトの過程で現れる数値が全て素数
 */
bool is_lshift_prime(uInt num)
{
  while (num) {
    const uInt scale = std::pow(10, (uInt)std::log10(num));
    num -= num / scale * scale;
    if (num > 0 && !is_prime(num)) { return false; }
  }
  return true;
}

int main(void)
{
  uInt count = 0;
  uInt sum = 0;
  for (uInt i = 11; count < 11; i++) {
    if (is_prime(i) && is_rshift_prime(i) && is_lshift_prime(i)) {
      count++;
      sum += i;
    }
  }
  std::cout << "Euler037: " << sum << std::endl;
  return 0;
}
