#ifndef __POLYGONAL_NUMBER_H__
#define __POLYGONAL_NUMBER_H__

#include <cmath>
#include <limits>

namespace Euler
{
  /**
   * 与えられた浮動小数点数numが実質的に整数であるかを確認する
   * @param num 対象の数値
   * @return  numの小数点以下が実質的に0であると言えるときtrue
   */
  template <typename Real, typename Int>
  inline
  bool is_int(const Real num)
  {
    constexpr auto REAL_EPS = std::numeric_limits<Real>::epsilon();
    return std::abs(num - Real(Int(num))) < REAL_EPS;
  }

namespace PolygonalNumber
{
  /**
   * 与えた数が三角数か確認
   * @param num 確認したい数
   * @return    numが三角数のときtrue
   */
  template <typename Int_t>
  inline
  bool is_trianguler(const Int_t num)
  {
    // n * (n + 1) / 2 = Tn
    // n ^ 2 + n - 2 * Tn = 0
    // n = (-1 + sqrt(1 + 8 * Tn)) / 2
    const auto nf = (std::sqrt(1 + 8 * num) - 1) / 2;
    return is_int<decltype(nf), Int_t>(nf);
  }

  /**
   * 与えた数が五角数か確認
   * @param num 確認したい数
   * @return    numが四角数のときtrue
   */
  template <typename Int_t>
  inline
  bool is_square(const Int_t num)
  {
    // n^2 = Sn
    // n = sqrt(Sn) が整数のときtrue
    const auto nf = std::sqrt(num);
    return is_int<decltype(nf), Int_t>(nf);
  }

  /**
   * 与えた数が五角数か確認
   * @param num 確認したい数
   * @return    numが五角数のときtrue
   */
  template <typename Int_t>
  inline
  bool is_pentagonal(const Int_t num)
  {
    // 解の公式を使って，五角数num(=Pn)を生成する自然数nがあるか確認する
    // n * (3 * n - 1) / 2 = Pn
    // 3 * n^2 - n - 2 * Pn = 0
    // n = (1 + sqrt(1 + 24 * Pn)) / 6 が整数のときtrue
    const auto nf = (1 + std::sqrt(1 + 24 * num)) / 6;
    return is_int<decltype(nf), Int_t>(nf);
  }

  /**
   * 与えた数が六角数か確認
   * @param num 確認したい数
   * @return    numが六角数のときtrue
   */
  template <typename Int_t>
  inline
  bool is_hexagonal(const Int_t num)
  {
    // n * (2 * n - 1) = Hn
    // 2 * n ^ 2 - n - Hn = 0
    // n = (1 + sqrt(1 + 8 * Hn)) / 4 が整数のときtrue
    const auto nf = (1 + std::sqrt(1 + 8 * num)) / 4;
    return is_int<decltype(nf), Int_t>(nf);
  }

  /**
   * 与えた数が七角数か確認
   * @param num 確認したい数
   * @return    numが七角数のときtrue
   */
  template <typename Int_t>
  inline
  bool is_heptagonal(const Int_t num)
  {
    // (5 * n ^ 2 - 3 * n) / 2 = Hn
    // 5 * n ^ 2 - 3 * n - 2 * Hn = 0
    // n = (3 + sqrt(9 + 40 * Hn)) / 10 が整数のときtrue
    const auto nf = (3 + std::sqrt(9 + 40 * num)) / 10;
    return is_int<decltype(nf), Int_t>(nf);
  }

  /**
   * 与えた数が八角数か確認
   * @param num 確認したい数
   * @return    numが八角数のときtrue
   */
  template <typename Int_t>
  inline
  bool is_octagonal(const Int_t num)
  {
    // (3 * n ^ 2 - 2 * n) = On
    // 3 * n ^ 2 - 2 * n - On = 0
    // n = (1 + sqrt(1 + 3 * On)) / 3 が整数のときtrue
    const auto nf = (1 + std::sqrt(1 + 3 * num)) / 3;
    return is_int<decltype(nf), Int_t>(nf);
  }

}
}

#endif
