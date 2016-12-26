/**
 * Problem 72 「分数の数え上げ」
 * nとdを正の整数として, 分数 n/d を考えよう. n<d かつ HCF(n,d)=1 のとき, 真既約分数と呼ぶ.
 *
 * d ≤ 8について真既約分数を大きさ順に並べると, 以下を得る:
 *
 * 1/8, 1/7, 1/6, 1/5, 1/4, 2/7, 1/3, 3/8, 2/5, 3/7, 1/2, 4/7, 3/5, 5/8, 2/3, 5/7, 3/4, 4/5, 5/6, 6/7, 7/8
 * この集合は21個の要素をもつことが分かる.
 *
 * d ≤ 1,000,000について, 真既約分数の集合は何個の要素を持つか?
 */

#include <cstdint>
#include <iostream>
#include <vector>

#include "prime.h"
#include "totient.h"

using namespace Euler::Prime;
using namespace Euler::Totient;
using uInt = std::uint_fast64_t;

int main(void)
{
  constexpr uInt D_BOUND = 1000000;
  std::vector<NumberType> nt_vec;
  make_primes_vector(D_BOUND, &nt_vec);

  // 分母dの真既約分数の個数 == dと互いに素なnの個数 == トーシェント数φ(d)
  uInt count = 0;
  for (uInt d = 2; d <= D_BOUND; d++) {
    count += totient(d, nt_vec);
  }
  std::cout << "Euler072: " << count << std::endl;
  return 0;
}
