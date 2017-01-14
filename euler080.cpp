/**
 * Problem 80 「平方根の10進展開」
 * よく知られているように, 自然数の平方根が整数ではないならば, それは無理数である.
 *
 * そのような平方根の10進展開(decimal expansion)は繰り返しを持たない無限小数になる.
 *
 * 2の平方根は, 1.41421356237309504880...,であり, その頭から100桁の数字を合計すると475になる.
 *
 * 初めの100個の自然数の平方根のうち, 無理数について, それぞれの頭から100桁の数字を足した数の総和を求めよ.
 */

#include <algorithm>

#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>

#include "largeint.h"

using uInt = std::uint_fast64_t;
using LInt = Euler::LargeInt<uInt>;

/**
 * 開平法による平方数の桁ベクタを取得
 * @param n             √nのn
 * @param DIGITS_BOUND  何桁目まで求めるか
 * @return 桁の値ベクタ
 */
std::vector<uInt> extract_sqrt(uInt num, const uInt DIGITS_BOUND)
{
  const uInt N_DIGITS = std::log10(num) + 1;
  const uInt D_VEC_SIZE = N_DIGITS / 2 + (N_DIGITS % 2 == 0 ? 0 : 1);
  std::vector<uInt> digits(DIGITS_BOUND, 0);
  std::vector<uInt> result;
  result.reserve(DIGITS_BOUND);
  // 元の数を2桁ずつに区切る
  for (uInt i = 0; i * 2 < N_DIGITS; i++) {
    digits.at(D_VEC_SIZE - i - 1) = num % 100;
    num /= 100;
  }

  // 最初の1桁は特殊化
  const uInt n = digits.at(0);
  const uInt rt_n = std::sqrt(n);
  result.push_back(rt_n);

  LInt prev_n = n - rt_n * rt_n;
  LInt fact = rt_n + rt_n;
  for (uInt i = 1; i < DIGITS_BOUND; i++) {
    const LInt n = prev_n * 100 + digits.at(i);
    uInt j, best = 0;
    for (j = 1; j < 10; j++) {
      if ((fact * 10 + j) * j <= n) {
        best = j;
      }
      else {
        break;
      }
    }
    result.push_back(best);
    prev_n = n - (fact * 10 + best) * best;
    fact = fact * 10 + best * 2;
  }
  return result;
}

int main(void)
{
  uInt result = 0;
  for (uInt i = 2; i < 100; i++) {
    const double rt_i = std::sqrt(i);
    if (std::abs(rt_i - std::floor(rt_i)) < std::numeric_limits<double>::epsilon()) {
      continue;
    }
    auto digits = extract_sqrt(i, 100);
    const uInt x = std::accumulate(digits.begin(), digits.end(), uInt(0));
    result += x;
  }
  std::cout << "Euler080: " << result << std::endl;
  return 0;
}
