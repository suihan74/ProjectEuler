#ifndef __PANDIGITAL_H__
#define __PANDIGITAL_H__

namespace Euler
{
namespace Pandigital
{
  /**
   * 1~9までの数値が使用されていることをチェックするための値を生成
   * ex) uInt x = make_test_bits(5);   // x = 0b11111
   * @param d 1にするビット数
   * @return  dビット1が並んだ数値
   */
  template <typename Int>
  constexpr
  Int make_test_bits(Int d)
  {
    return d == 1 ? 1 : (make_test_bits(d - 1) | 1 << (d - 1));
  }

  /**
   * 与えられたビット列bitsの全ビットが1であるか確認
   * @param bits    8桁のビット列（ある数値の全桁に含まれる1~9までの数値を表す）
   * @return        与えられたbitsがパンデジタル数から得られたビット列のときtrue
   */
  template <typename Int, Int n = 9>
  inline
  bool pandigital_test(Int bits)
  {
    constexpr Int TEST = make_test_bits(n);
    return !(bits ^ TEST);
  }

  /**
   * ビット列a, bの各ビットがひとつでも重複して1になっているかを確認
   * @param a パンデジタル数判別のためのビット列
   * @param b パンデジタル数判別のためのビット列
   * @return  aとbのビットがひとつ以上同時に1になっている
   */
  template <typename Int>
  inline
  bool is_overlap(Int a, Int b)
  {
    return (a & b);
  }

  /**
   * 引数である非負整数nに使用されている1~9の数字を表すビット列を返す
   * 0または重複を発見時には0を返す
   * ex) 127 -> 0b001000011
   * @param  n 検査対象の正の整数
   * @return   nに含まれる数値を表すビット列（エラー時: 0）
   */
  template <typename Int, Int BASE = 1, bool PERMIT_OVERLAP = false>
  Int used_num_bits(Int n)
  {
    Int res = 0;
    while (n) {
      const Int digit = n % 10;
      if (digit < BASE) { return 0; }

      const Int b = 1 << (digit - BASE);
      if (!PERMIT_OVERLAP && is_overlap(res, b)) { return 0; }

      res |= b;
      n /= 10;
    }
    return res;
  }

}}

#endif
