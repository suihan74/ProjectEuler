/**
 * Problem 23 「非過剰数和」
 * 完全数とは, その数の真の約数の和がそれ自身と一致する数のことである. たとえば, 28の真の約数の和は, 1 + 2 + 4 + 7 + 14 = 28 であるので, 28は完全数である.
 *
 * 真の約数の和がその数よりも少ないものを不足数といい, 真の約数の和がその数よりも大きいものを過剰数と呼ぶ.
 *
 * 12は, 1 + 2 + 3 + 4 + 6 = 16 となるので, 最小の過剰数である. よって2つの過剰数の和で書ける最少の数は24である.
 * 数学的な解析により, 28123より大きい任意の整数は2つの過剰数の和で書けることが知られている.
 * 2つの過剰数の和で表せない最大の数がこの上限よりも小さいことは分かっているのだが, この上限を減らすことが出来ていない.
 *
 * 2つの過剰数の和で書き表せない正の整数の総和を求めよ.
 */

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

using uInt = std::uint_fast64_t;

enum class DivisorsType
{
  UNKNOWN,
  ABUNDANT,
  NON_ABUNDANT,
};

/**
 * 対象の真の約数の和を計算（euler021で作ったもの）
 * @param  n 対象
 * @return   真の約数の和
 */
uInt proper_divisors_sum(uInt n)
{
  if (n < 4) { return n == 0 ? 0 : 1; }
  uInt sum = 0;
  const uInt max = n / 2 + 1;
  for (uInt i = 1; i < max; i++) {
    if (n % i == 0) {
      sum += i;
    }
  }
  return sum;
}

inline
DivisorsType probe_divisors_type(std::vector<DivisorsType> &dest, uInt num)
{
  if (dest.at(num) == DivisorsType::UNKNOWN) {
    dest.at(num) = num < proper_divisors_sum(num) ? DivisorsType::ABUNDANT : DivisorsType::NON_ABUNDANT;
  }
  return dest.at(num);
}

int main(void)
{
  const uInt MAX = 28123 + 1;
  std::vector<DivisorsType> abundants(MAX + 1, DivisorsType::UNKNOWN);

  // 説明にある通り，過剰数の和で表せる最小値は24である
  // そのため、1~23の和は予め計算する
  uInt sum = (23 + 1) * 23 / 2;
  for (uInt i = 24; i < MAX; i++) {
    bool passed = true;
    for (uInt a = 12; a + 11 < i; a++) {  // cond) a + 12 <= i; i-a<12 なる過剰数aは存在しないため
      // 2数が未使用の場合は過剰数かを判別する
      if (probe_divisors_type(abundants, a) == DivisorsType::ABUNDANT
       && probe_divisors_type(abundants, i - a) == DivisorsType::ABUNDANT)
      {
        passed = false;
        break;
      }
    }
    if (passed) { sum += i; }
  }
  std::cout << "Euler023: " << sum << std::endl;
  return 0;
}
