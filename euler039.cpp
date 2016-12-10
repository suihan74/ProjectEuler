/**
 * Problem 39 「整数の直角三角形」
 * 辺の長さが {a,b,c} と整数の3つ組である直角三角形を考え, その周囲の長さを p とする. p = 120のときには3つの解が存在する:
 *
 * {20,48,52}, {24,45,51}, {30,40,50}
 *
 * p ≤ 1000 のとき解の数が最大になる p はいくつか?
 */

#include <cstdint>
#include <iostream>

using uInt = std::uint_fast32_t;

/*
a^2 + b^2 = c^2
a + b + c = p  ->  c = p - a - b
a^2 + b^2 = (p - a - b)^2
          = p^2 + a^2 + b^2 - 2pa - 2pb + 2ab
p^2 - 2(pa + pb -ab) = 0
*/

int main(void)
{
  uInt max_count = 0;
  uInt max_p = 0;
  for (uInt p = 1; p <= 1000; p++) {
    uInt count = 0;
    for (uInt a = 1; a < p; a++) {
      for (uInt b = 1; a + b < p; b++) {
        if (p * p - 2 * (p * (a + b) - a * b) == 0) {
          count++;
        }
      }
    }
    if (count > max_count) {
      max_count = count;
      max_p = p;
    }
  }
  std::cout << "Euler039: " << max_p << std::endl;
  return 0;
}
