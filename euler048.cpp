/**
 * Problem 48 「自身のべき乗(self powers)」
 * 次の式は, 1^1 + 2^2 + 3^3 + ... + 10^10 = 10405071317 である.
 *
 * では, 1^1 + 2^2 + 3^3 + ... + 1000^1000 の最後の10桁を求めよ.
 */

#include <cmath>
#include <cstdint>
#include <iostream>

using uInt = std::uint_fast64_t;

int main(void)
{
  constexpr uInt DIGITS = 10;
  constexpr uInt SCALE = std::pow(10, DIGITS);
  constexpr uInt LIMIT = 1000;
  uInt result = 0;
  for (uInt i = 1; i <= LIMIT; i++) {
    uInt x = i, y = i;
    for (uInt j = 1; j < i; j++) {
      x = (x * y) % SCALE;  // 必要なのは下から10桁だけなのでそれより大きい部分は捨てる
    }
    result = (result + x) % SCALE;
  }
  std::cout << "Euler048: " << result << std::endl;
  return 0;
}
