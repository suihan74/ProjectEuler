/**
 * Problem 5 「最小の倍数」
 * 2520 は 1 から 10 の数字の全ての整数で割り切れる数字であり, そのような数字の中では最小の値である.
 * では, 1 から 20 までの整数全てで割り切れる数字の中で最小の正の数はいくらになるか.
 */

#include <cstdint>
#include <iostream>

using uInt = std::uint_fast64_t;

int main(void)
{
  /* 説明例の検証 */
  constexpr uInt max_pre = 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9 * 10;
  // 9と10で割り切れる最初の数が90，少なくともこれよりは大きい
  for (uInt i = 90; i < max_pre; i += 2) { // 少なくとも偶数
    // この4パターンで割り切れるとき，自動的に2, 3, 4, 5, 6でも割り切れる
    if (!(i % 10 || i % 9 || i % 8 || i % 7)) {
      std::cout << "Euler005(pre1~10): " << i << std::endl;
      break;
    }
  }
  /* 本題 */
  constexpr uInt max = max_pre * 11 * 12 * 13 * 14 * 15 * 16 * 17 * 18 * 19 * 20;
  for (uInt i = 190; i < max; i += 2) {
    if (!(i % 20 ||   // 2, 4, 5, 10, 20
          i % 19 ||   // 19
          i % 18 ||   // 3, 6, 9, 18
          i % 17 ||   // 17
          i % 16 ||   // 8, 16
          i % 14 ||   // 7, 14
          i % 13 ||   // 13
          i % 11))
    {
      std::cout << "Euler005(1~20): " << i << std::endl;
      break;
    }
  }

  return 0;
}
