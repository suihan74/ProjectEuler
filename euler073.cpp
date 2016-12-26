/**
 * Problem 73 「ある範囲内の分数の数え上げ」
 * nとdを正の整数として, 分数 n/d を考えよう. n<d かつ HCF(n,d)=1 のとき, 真既約分数と呼ぶ.
 *
 * d ≤ 8 について既約分数を大きさ順に並べると, 以下を得る:
 *
 * 1/8, 1/7, 1/6, 1/5, 1/4, 2/7, 1/3, 3/8, 2/5, 3/7, 1/2, 4/7, 3/5, 5/8, 2/3, 5/7, 3/4, 4/5, 5/6, 6/7, 7/8
 * 1/3と1/2の間には3つの分数が存在することが分かる.
 *
 * では, d ≤ 12,000 について真既約分数をソートした集合では, 1/3 と 1/2 の間に何個の分数があるか?
 */

#include <cstdint>
#include <iostream>

using uInt = std::uint_fast32_t;

/**
 * aとbの最大公約数をユークリッドの互除法で計算する
 * @param a  自然数a
 * @param b  自然数b
 * @return   aとbの最大公約数
 */
template<typename uInt_t>
inline
uInt_t gcd(uInt_t a, uInt_t b)
{
  while (b) {
    auto tmp = a % b;
    a = b;
    b = tmp;
  }
  return a;
}

int main(void)
{
  constexpr uInt D_BOUND = 12000;
  constexpr double LOWER_BOUND = 1.0 / 3.0;
  constexpr double UPPER_BOUND = 1.0 / 2.0;
  uInt count = 0;
  for (uInt d = 2; d <= D_BOUND; d++) {
    for (uInt n = LOWER_BOUND * d; n < d; n++) {
      double frac = double(n) / d;
      if (frac <= LOWER_BOUND) { continue; } // epsilonとの比較に変えるべきか？
      if (frac >= UPPER_BOUND) { break; }
      if (gcd(n, d) == 1) {
        count++;
      }
    }
  }
  std::cout << "Euler073: " << count << std::endl;
  return 0;
}
