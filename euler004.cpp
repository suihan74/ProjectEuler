/**
 * Problem 4 「最大の回文積」
 * 左右どちらから読んでも同じ値になる数を回文数という.
 * 2桁の数の積で表される回文数のうち, 最大のものは 9009 = 91 × 99 である.
 * では, 3桁の数の積で表される回文数の最大値を求めよ.
 */

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>

using uInt = std::uint_fast32_t;

/**
 * 回文数かの判別
 * @brief 面倒っちいので，文字列化して反転→再度数値化．1の位が0なら絶対に違う
 * @param  num 検証対象の数値
 * @return     対象が回文数であるか否か
 */
bool isPalindrome(const uInt num)
{
  if (num % 10 == 0) { return false; }
  auto str = std::to_string(num);
  std::reverse(str.begin(), str.end());
  return std::stoul(str) == num;
}

int main(void)
{
  /*
   * 回文数: abccba → 100000a + 10000b + 1000c + 100c + 10b + a
   *                 = 100001a + 10010b + 1100c
   *                 = 11(9091a + 910b + 100c)
   * 三桁の2数のうち少なくとも片方は絶対に11で割り切れる数
   */
  uInt max = 0;
  for (uInt i = 999; i >= 100; --i) {
    for (uInt j = 990; j >= 110; j -= 11) {
      const uInt ij = i * j;
      if (ij > max && isPalindrome(ij)) {
        max = ij;
      }
    }
  }
  std::cout << "Euler004: " << max << std::endl;
  return 0;
}
