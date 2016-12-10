/**
 * Problem 33 「桁消去分数」
 * 49/98は面白い分数である.「分子と分母からそれぞれ9を取り除くと, 49/98 = 4/8 となり, 簡単な形にすることができる」と経験の浅い数学者が誤って思い込んでしまうかもしれないからである. (方法は正しくないが，49/98の場合にはたまたま正しい約分になってしまう．)
 *
 * 我々は 30/50 = 3/5 のようなタイプは自明な例だとする.
 *
 * このような分数のうち, 1より小さく分子・分母がともに2桁の数になるような自明でないものは, 4個ある.
 *
 * その4個の分数の積が約分された形で与えられたとき, 分母の値を答えよ.
 */

#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>
#include <numeric>
#include <utility>
#include <vector>

using uInt = std::uint_fast32_t;
using Real = double;

/**
 * 不動点小数の等価比較演算
 * （そのまま==でやると場合によっては誤差が出たり出なかったり）
 */
inline
bool real_equals(Real a, Real b)
{
  return std::abs(a - b) < std::numeric_limits<Real>::epsilon();
}

/**
 * 分数を表すペア
 * ex) 'Fraction<uInt> f(1, 2)' は '1/2' を表す
 */
template<class T>
using Fraction = std::pair<T, T>;

// 総積算用関数
const auto productOp = [](uInt init, uInt x) { return init * x; };
/**
 * 総積算
 * そのまま書くと何だかくどくなって嫌なのでラップしただけ
 * @param vec  対象ベクタ
 * @param init 初期値
 */
inline
uInt product(const std::vector<uInt>& vec, uInt init)
{
  return std::accumulate(vec.cbegin(), vec.cend(), init, productOp);
}

/**
 * 素因数分解
 * ex) 18 -> 2 * 3 * 3
 * @param src  元の値（上例でいうところの 18）
 * @return     分解結果（上例でいうところの 2,3,3）
 */
std::vector<uInt> factorize(uInt src)
{
  std::vector<uInt> factors;
  for (uInt i = 2; i <= src; i++) {
    if (src % i == 0) {
      src /= i;
      factors.push_back(i);
      if (src == 1) {
        break;
      }
      else {
        i--;
      }
    }
  }
  return factors;
}

/**
 * 約分した分数を返す
 * ex) 4/16 -> 1/4
 * @param src 元の分数
 * @return 約分したあとの分数
 */
Fraction<uInt> reduceFraction(const Fraction<uInt>& src)
{
  auto nfactors = factorize(src.first);
  auto dfactors = factorize(src.second);
  for (auto &n : nfactors) {
    for (auto &d : dfactors) {
      if (n == d) {
        n = d = 1;
        break;
      }
    }
  }
  return std::make_pair(product(nfactors, 1), product(dfactors, 1));
}

int main(void)
{
  std::vector<Fraction<uInt>> targets;
  targets.reserve(4);

  // 自明でないケースを探す
  for (uInt i = 1; i < 10; i++) {      // 分子 i
    for (uInt j = 1; j < 10; j++) {    // 分母 j
      if (i == j) { continue; }
      for (uInt k = 1; k < 10; k++) {  // 消去する値 k
        if (k == j || k == i) {    // 自明なケース
          continue;
        }
        Real reduced = static_cast<Real>(i) / j;
        Real ik = i * 10 + k;
        Real kj = k * 10 + j;
        Real ki = k * 10 + i;
        Real jk = j * 10 + k;
        if ((ik < kj && real_equals(ik / kj, reduced))    // ik/kj
         || (ki < jk && real_equals(ki / jk, reduced)))   // ki/jk
        {
          targets.emplace_back(i, j);
        }
      }
    }
  }

  // 答え
  uInt numerator = 1;
  uInt denominator = 1;
  for (auto &f : targets) {
    std::cout << f.first << "/" << f.second << std::endl;
    numerator *= f.first;
    denominator *= f.second;
  }
  auto answer = reduceFraction(std::make_pair(numerator, denominator));
  std::cout << "Euler033: " << answer.second << std::endl;
  return 0;
}
