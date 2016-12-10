/**
 * Problem 10 「素数の和」
 * 10以下の素数の和は 2 + 3 + 5 + 7 = 17 である.
 *
 * 200万以下の全ての素数の和を求めよ.
 */

#include <cstdint>
#include <iostream>
#include <vector>

using uInt = std::uint_fast64_t;
// 合成数か素数か
enum class NumberType
{
  PRIME,
  COMPOSITE,
};

/**
 * 2 ~ maxまでの間にある素数の和を返す
 * @param  max 上端（合成数でもよい）
 * @return     2 ~ maxまでの間にある素数の合計値
 */
uInt prime_sum(const uInt max)
{
  std::vector<NumberType> vec(max + 1, NumberType::PRIME);
  uInt sum = 0;
  for (uInt i = 2; i <= max; i++) {
    auto x = vec.begin() + i;
    if (*x == NumberType::COMPOSITE) { continue; }
    // 小さい方から順に見てるので，既に合成数判定されていなければ必ず素数
    sum += i;
    // 見つけた素数の倍数は全て合成数
    for (auto y = x + i; y < vec.end(); y += i) {
      *y = NumberType::COMPOSITE;
    }
  }
  return sum;
}


int main(void)
{
  std::cout << "Euler010: " << prime_sum(2000000) << std::endl;
  return 0;
}
