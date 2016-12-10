/**
 * Problem 12 「高度整除三角数」
 * 三角数の数列は自然数の和で表わされ, 7番目の三角数は 1 + 2 + 3 + 4 + 5 + 6 + 7 = 28 である. 三角数の最初の10項は:
 *
 * 1, 3, 6, 10, 15, 21, 28, 36, 45, 55, ...
 * となる.
 *
 * 最初の7項について, その約数を列挙すると, 以下のとおり.
 *
 *  1: 1
 *  3: 1,3
 *  6: 1,2,3,6
 * 10: 1,2,5,10
 * 15: 1,3,5,15
 * 21: 1,3,7,21
 * 28: 1,2,4,7,14,28
 *
 * これから, 7番目の三角数である28は, 5個より多く約数をもつ最初の三角数であることが分かる.
 *
 * では, 500個より多く約数をもつ最初の三角数はいくつか
 */

#include <cstdint>
#include <iostream>
#include <vector>

using uInt = std::uint_fast64_t;

/**
 * 与えられた値の約数を数える
 * @param  num 対象の値
 * @return     約数の個数
 */
uInt count_factors(const uInt num)
{
  if (num < 2) { return num; }  // 0と1の約数は自明
  uInt count = 2;               // 1とnumは絶対に約数である
  uInt trials = num;
  for (uInt i = 2; i < trials; i++) {
    // num % i == 0 のとき，i * j = num なるjが存在するので，2つ分数える
    // 値jは再度iで探索する必要が無く，iの探索終了点を更新する
    // （より後で見つけたjは必ず現在の終了点よりも小さくなる）
    // 28: 1,2,4, | 7,14,28 → 最初の1,2,4を見つけた時点で7,14,28は数えなくてもいい
    if (num % i == 0) {
      trials = num / i;
      count += 2;
    }
  }
  return count;
}


int main(void)
{
  uInt num = 0;
  for (uInt i = 0; ; i++) {
    num += i;
    const uInt f = count_factors(num);
    if (f > 500) {
      std::cout << "Euler012: " << num << " : factors=" << f << std::endl;
      break;
    }
  }
  return 0;
}
