/**
 * Problem 14 「最長のコラッツ数列」 †
 * 正の整数に以下の式で繰り返し生成する数列を定義する.
 *
 * n → n/2 (n が偶数)
 *
 * n → 3n + 1 (n が奇数)
 *
 * 13からはじめるとこの数列は以下のようになる.
 *
 * 13 → 40 → 20 → 10 → 5 → 16 → 8 → 4 → 2 → 1
 * 13から1まで10個の項になる. この数列はどのような数字からはじめても最終的には 1 になると考えられているが, まだそのことは証明されていない(コラッツ問題)
 *
 * さて, 100万未満の数字の中でどの数字からはじめれば最長の数列を生成するか.
 *
 * 注意: 数列の途中で100万以上になってもよい
 */

#include <cstdint>
#include <iostream>
#include <numeric>

using uInt = std::uint_fast32_t;

uInt collatz_length(uInt n)
{
  uInt l = 1;
  while(n > 1) {
    n = n % 2 ? 3 * n + 1 : n / 2;
    l++;
  }
  return l;
}

int main(void)
{
  uInt max = 0, max_i = 0;
  for (uInt i = 1; i < 1000000; i++) {
    uInt l = collatz_length(i);
    if (l > max) {
      max = l;
      max_i = i;
    }
  }
  std::cout << "Euler014: " << max_i << " : length=" << max << std::endl;
  return 0;
}
