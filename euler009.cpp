/**
 * Problem 9 「特別なピタゴラス数」
 * ピタゴラス数(ピタゴラスの定理を満たす自然数)とは a < b < c で以下の式を満たす数の組である.
 * a^2 + b^2 = c^2
 * 例えば, 3^2 + 4^2 = 9 + 16 = 25 = 5^2 である.
 *
 * a + b + c = 1000 となるピタゴラスの三つ組が一つだけ存在する.
 * これらの積 abc を計算しなさい.
 */

#include <cstdint>
#include <iostream>

using uInt = std::uint_fast32_t;

int main(void)
{
  /*
   * c = 1000 - a - b
   * a^2 + b^2 = (1000 - a - b)^2
   *           = 1000^2 + a^2 + b^2 - 2000a - 2000b + 2ab
   * 2000(a+b) - 2ab = 1000000
   * a < b < c より，a + b < 2b < 1000 - a → 2b < 1000 → b < 500, 同様に a < 500
  */
  for (uInt a = 1; a < 500; a++) {
    for (uInt b = a + 1; b < 500; b++) {
      if (1000 * (a + b) - a * b == 500000) {
        uInt abc = a * b * (1000 - a - b);
        std::cout << "Problem 9: " << abc << std::endl;
        return 0;
      }
    }
  }

  return 0;
}
