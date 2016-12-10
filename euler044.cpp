/**
 * Problem 44 「五角数」
 * 五角数は Pn = n(3n-1)/2 で生成される. 最初の10項は
 *
 * 1, 5, 12, 22, 35, 51, 70, 92, 117, 145, ...
 *
 * である.
 *
 * P4 + P7 = 22 + 70 = 92 = P8 である. しかし差 70 - 22 = 48 は五角数ではない.
 *
 * 五角数のペア Pj と Pk について, 差と和が五角数になるものを考える. 差を D = |Pk - Pj| と書く. 差 D の最小値を求めよ.
 */

#include <iostream>
#include <cmath>
#include <cstdint>
#include <limits>

using uInt = std::uint_fast64_t;
using Real = double;

constexpr auto REAL_EPS = std::numeric_limits<Real>::epsilon();

/**
 * n番目の五角数を生成
 * @param n  インデックス（n==0のとき計算途中で溢れる気がするけど結果は正常に戻るみたいなのでとりあえず無視）
 * @return   n番目の五角数
 */
inline
uInt pentagon_number(const uInt n)
{
  return n * (3 * n - 1) / 2;
}

/**
 * 与えた数が五角数か確認
 * @param num 確認したい数
 * @return    numが五角数のときtrue
 */
inline
bool is_pentagon(const uInt num)
{
  // 解の公式を使って，五角数num(=Pn)を生成する自然数nがあるか確認する
  // n * (3 * n - 1) / 2 = Pn
  // 3 * n^2 - n - 2 * Pn = 0
  // n = (1 + sqrt(1 + 24 * Pn)) / 6 が整数のときtrue
  const Real nf = (1 + std::sqrt(1 + 24 * num)) / 6;
  return std::abs(nf - Real(uInt(nf))) < REAL_EPS;
}

int main(void)
{
  constexpr auto MAX = std::numeric_limits<uInt>::max();  // 途中で見つかることはわかっちゃいるが
  for (uInt j = 1; j < MAX; j++) {
    const auto pj = pentagon_number(j);
    for (uInt k = 1; k < j; k++) {
      const auto pk = pentagon_number(k);
      const auto diff = pj - pk;
      if (is_pentagon(pj + pk) && is_pentagon(diff)) {
        // Pj が大きくなるほど Pk との差 D は大きくなる
        // j - k が大きくなるほど D は大きくなる
        // 以上二点より，j が小さい方から探して最初に見つけた D が最小値である
        std::cout << "Euler044: " << diff << std::endl;
        return 0;
      }
    }
  }
  return 0;
}
