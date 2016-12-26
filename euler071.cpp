/**
 * Problem 71 「順序分数」
 * nとdを正の整数として, 分数 n/d を考えよう. n<d かつ HCF(n,d)=1 のとき, 真既約分数と呼ぶ.
 *
 * d ≤ 8について既約分数を大きさ順に並べると, 以下を得る:
 *
 * 1/8, 1/7, 1/6, 1/5, 1/4, 2/7, 1/3, 3/8, 2/5, 3/7, 1/2, 4/7, 3/5, 5/8, 2/3, 5/7, 3/4, 4/5, 5/6, 6/7, 7/8
 * 3/7のすぐ左の分数は2/5である.
 *
 * d ≤ 1,000,000について真既約分数を大きさ順に並べたとき, 3/7のすぐ左の分数の分子を求めよ.
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
  constexpr uInt D_BOUND = 1000000;
  constexpr double THRESHOULD = 3.0 / 7.0;
  double best_frac = 0.0;
  uInt best_n = 0;
  for (uInt d = 2; d <= D_BOUND; d++) {
    // 現在の最良解より3/7に近い部分だけ調べることでかなり速くなる
    // 実際のところbest_fracの値で分岐させなくても正答は出るが，一応定義ではnは正の整数なので厳密にしておく
    const uInt begin = best_frac > 0.0 ? uInt(best_frac * d) : 1;
    for (uInt n = begin; n < d; n++) {
      double frac = double(n) / d;
      if (frac >= THRESHOULD) { break; }  // epsilonとの比較に変えるべきか？
      if (frac > best_frac && gcd(n, d) == 1) {
        best_frac = frac;
        best_n = n;
      }
    }
  }
  std::cout << "Euler071: " << best_n << std::endl;
  return 0;
}
