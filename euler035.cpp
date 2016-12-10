/**
 * Problem 35 「巡回素数」
 * 197は巡回素数と呼ばれる. 桁を回転させたときに得られる数 197, 971, 719 が全て素数だからである.
 *
 * 100未満には巡回素数が13個ある: 2, 3, 5, 7, 11, 13, 17, 31, 37, 71, 73, 79, および97である.
 *
 * 100万未満の巡回素数はいくつあるか?
 */

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>
#include "prime.h"

using uInt = std::uint_fast32_t;
using namespace Euler::Prime;

/**
 * 値が既にベクタに存在するかを確認
 * @param vec  対象ベクタ
 * @param num  探す値
 * @return     vecにnumが既に存在する場合true
 */
inline
bool find(std::vector<uInt>& vec, uInt num)
{
  return std::find(vec.begin(), vec.end(), num) != vec.end();
}

/**
 * 巡回素数を収集
 * @param  num    チェック対象の値
 * @param  table  numが巡回素数のとき，numの巡回素数すべてを記録するベクタ
 */
bool check_circular_prime(uInt num, std::vector<uInt>* table)
{
  if (!is_prime(num) || find(*table, num)) { return false; }
  const uInt digits = std::log10(num);
  // numから得られる巡回素数を一時記録するベクタ
  std::vector<uInt> circulars;
  circulars.reserve(digits + 1);
  circulars.push_back(num);
  for (uInt i = 0; i < digits; i++) {
    const uInt d = num % 10;
    num = num / 10 + d * std::pow(10, digits);
    if (!is_prime(num)) { return false; }
    circulars.push_back(num);
  }
  // 11とか同じ値が複数入る場合があるので，重複を除いておく
  std::sort(circulars.begin(), circulars.end());
  auto end = std::unique(circulars.begin(), circulars.end());
  // numから得られた巡回素数全てを記録（もう一度数えないようにする）
  std::copy(circulars.begin(), end, std::back_inserter(*table));
  return true;
}

int main(void)
{
  std::vector<uInt> circular_primes;
  for (uInt i = 2; i < 1000000; i++) {
    check_circular_prime(i, &circular_primes);
  }
  std::cout << "Euler035: " << circular_primes.size() << std::endl;
  return 0;
}
