/**
 * Problem 15 「格子経路」
 * 2×2 のマス目の左上からスタートした場合, 引き返しなしで右下にいくルートは 6 つある.
 *
 * では, 20×20 のマス目ではいくつのルートがあるか.
 */

#include <cstdint>
#include <iostream>
#include <numeric>
#include <vector>

using uInt = std::uint_fast64_t;

/**
 * 階乗で表された分子分母を"適当に"約分
 * 掛け算を小さくしたいだけなので必ずしも完璧に約分しきらない（この問題では十分）
 * @param  numerator   分子の階乗を表すベクタ（ex) 5!なら{5,4,3,2,1}）
 * @param  denominator 分母の階乗を表すベクタ
 * @return             分子が分母で割り切れる場合その値（割り切れない場合0）
 */
uInt reduce_factorial(std::vector<uInt> &numerator, std::vector<uInt> &denominator)
{
  for (auto &n : numerator) {
    for (auto &d : denominator) {
      if (d > 1 && n % d == 0) {
        n /= d;
        d = 1;
      }
    }
  }
  const auto multi = [](uInt init, uInt x) { return init * x; };
  uInt n = std::accumulate(numerator.begin(), numerator.end(), static_cast<uInt>(1), multi);
  uInt d = std::accumulate(denominator.begin(), denominator.end(), static_cast<uInt>(1), multi);
  return n % d ? 0 : n / d;
}

int main(void)
{
  // 40! / (20!20!) = 40C20 = (40*39*...*21) / (20*19*...*1)
  // 32bitではそのまま計算すると分子も分母も大きすぎる（分子は64bitでも足りない）
  std::vector<uInt> numerator(20), denominator(20);
  std::iota(numerator.begin(), numerator.end(), 21);
  std::iota(denominator.begin(), denominator.end(), 1);
  std::cout << "Euler015: " << reduce_factorial(numerator, denominator) << std::endl;
  return 0;
}
