#ifndef __CONTINUED_FRACTION_H__
#define __CONTINUED_FRACTION_H__

#include <cmath>
#include <vector>

namespace Euler
{
namespace ContinuedFraction
{
  /**
   * nを表す正則連分数を構成する要素aのベクタを生成する
   * aはa_0からループ部分の最初の終端まで収集する
   * @param n  任意の自然数
   * @return   nを表す連分数の要素a
   */
  template <typename Int_t>
  std::vector<Int_t> make_cfrac_factors(Int_t n)
  {
    const auto rt_n = std::sqrt(n);
    // a_0, a_1, a_2, ..., を格納するためのベクタ
    std::vector<Int_t> a_vec;
    // a_0のときだけ特殊化
    // a_0, (sqrt(N) - b) / c;  b == a, c == 1
    Int_t a = rt_n;
    Int_t b = a;
    Int_t c = 1;
    a_vec.push_back(a);

    // 説明より，a_1から繰り返すことは確定しているっぽいので，
    // a_1とともに生成される連分数のパーツを記録しておく
    Int_t b_1 = 0;
    Int_t c_1 = 0;
    for (;;) {
      // abcの計算順は変えちゃ駄目
      c = (n - b * b) / c;
      a = (rt_n + b) / c;
      b = a * c - b;
      if (c_1 == 0) {  // a_1の時の分数のパーツを記録
        b_1 = b;
        c_1 = c;
      }
      else if (b == b_1 && c == c_1) { // ループ点発見
        break;
      }
      a_vec.push_back(a);
    }
    return a_vec;
  }

  /**
   * ネイピア数の連分数を構成するa_kを計算する
   * @param idx  何番目のaが欲しいか
   * @return     a_(idx)
   */
  template <typename Int_t>
  inline
  Int_t napiers_term_at(Int_t idx)
  {
    if (idx == 0) { return 2; }
    const auto term = (idx - 1) / 3 + 1;
    const auto pos = (idx - 1) % 3;
    return pos == 1 ? (2 * term) : 1;
  }

}
}

#endif
