/**
 * Problem 57 「平方根の近似分数」
 * 2の平方根は無限に続く連分数で表すことができる.
 *
 * √ 2 = 1 + 1/(2 + 1/(2 + 1/(2 + ... ))) = 1.414213...
 * 最初の4回の繰り返しを展開すると以下が得られる.
 *
 * 1 + 1/2 = 3/2 = 1.5
 * 1 + 1/(2 + 1/2) = 7/5 = 1.4
 * 1 + 1/(2 + 1/(2 + 1/2)) = 17/12 = 1.41666...
 * 1 + 1/(2 + 1/(2 + 1/(2 + 1/2))) = 41/29 = 1.41379...
 *
 * 次の3つの項は99/70, 239/169, 577/408である. 第8項は1393/985である. これは分子の桁数が分母の桁数を超える最初の例である.
 *
 * 最初の1000項を考えたとき, 分子の桁数が分母の桁数を超える項はいくつあるか?
 */

#include <cmath>
#include <cstdint>
#include <iostream>
#include "largeint.h"

using uInt = std::uint_fast32_t;
using namespace Euler;
using LInt = LargeInt<uInt>;

// n_(i-1) + 2 * d_(i-1)
// n_(i-1) + d_(i-1)
int main(void)
{
  uInt count = 0;
  LInt n = 1, d = 1;
  for (uInt i = 0; i < 1000; i++) {
    // 分子の桁数が分母より大きいものを数える
    LInt n_next = n + d * 2;
    LInt d_next = n + d;
    n = std::move(n_next);
    d = std::move(d_next);
    if (std::log10(n) > std::log10(d)) {
      count++;
    }
  }
  std::cout << "Euler057: " << count << std::endl;
  return 0;
}
