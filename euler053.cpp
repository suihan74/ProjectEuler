/**
 * Problem 53 「組み合わせ選択」
 * 12345から3つ選ぶ選び方は10通りである.
 *
 * 123, 124, 125, 134, 135, 145, 234, 235, 245, 345.
 * 組み合わせでは, 以下の記法を用いてこのことを表す: 5C3 = 10.
 *
 * 一般に, r ≤ n について nCr = n!/(r!(n-r)!) である. ここで, n! = n×(n−1)×...×3×2×1, 0! = 1 と階乗を定義する.
 *
 * n = 23 になるまで, これらの値が100万を超えることはない: 23C10 = 1144066.
 *
 * 1 ≤ n ≤ 100 について, 100万を超える nCr は何通りあるか?
 */

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>

using uInt = std::uint_fast32_t;
using Real = double;

/**
 * インデックス 0 ~ MAX に対してそれぞれ 0! ~ MAX!をざっと計算して格納したベクタを返す
 * @param MAX  階乗ベクタに含める最大値
 * @return     階乗ベクタ（浮動小数なので大きい数ほど不正確）
 */
std::vector<Real> make_factorial_vector(const uInt MAX)
{
  std::vector<Real> vec { 1 };
  vec.reserve(MAX + 1);
  for (uInt i = 1; i <= MAX; i++) {
    vec.push_back(*vec.rbegin() * i);
  }
  return vec;
}

/**
 * nCrをざっと計算する
 * @param n  nCrのn
 * @param r  nCrのr
 * @param factorials  階乗ベクタ
 * @return nCrの大体の値（大きい数ほど不正確）
 */
inline
Real nCr(const uInt n, uInt r, const std::vector<Real>& factorials)
{
  if (n < 1) { throw std::out_of_range("nCr(n,r) : 'n' must be 1 or over"); }
  Real num = factorials.at(n);
  Real del = factorials.at(r) * factorials.at(n - r);
  return num / del;
}

int main(void)
{
  const auto vec = make_factorial_vector(100);
  uInt count = 0;
  for (uInt n = 1; n <= 100; n++) {
    for (uInt r = 0; r <= n; r++) {
      if (nCr(n, r, vec) > 1000000) {
        count++;
      }
    }
  }
  std::cout << "Euler053: " << count << std::endl;
  return 0;
}
