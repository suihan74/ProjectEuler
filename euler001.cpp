/**
 * Problem 1 「3と5の倍数」
 * 10未満の自然数のうち, 3 もしくは 5 の倍数になっているものは 3, 5, 6, 9 の4つがあり, これらの合計は 23 になる.
 * 同じようにして, 1000 未満の 3 か 5 の倍数になっている数字の合計を求めよ.
 */

#include <cstdint>
#include <iostream>

using Int = std::uint_fast32_t;
constexpr Int UPPER_BOUND = 1000;

int main(void)
{
  Int sum = 0;
  for (Int i = 3; i < UPPER_BOUND; i++) {
    if (i % 3 == 0 || i % 5 == 0) {
      sum += i;
    }
  }
  std::cout << "Euler001: " << sum << std::endl;

  return 0;
}
