/**
 * Problem 34 「桁の階乗」
 * 145は面白い数である. 1! + 4! + 5! = 1 + 24 + 120 = 145となる.
 *
 * 各桁の数の階乗の和が自分自身と一致するような数の和を求めよ.
 *
 * 注: 1! = 1 と 2! = 2 は総和に含めてはならない
 */

#include <cstdint>
#include <iostream>

using uInt = std::uint_fast32_t;

/**
 * 階乗をコンパイル時に計算
 * @param n   階乗する数n
 * @return    nの階乗n!
 */
constexpr
uInt factorial(uInt n)
{
  return n < 2 ? 1 : n * factorial(n - 1);
}

// 0!~9!を毎回計算するのは非効率なので，コンパイル時に計算して配列に突っ込んでおく
constexpr uInt factorial_array[] = {
  factorial(0), factorial(1), factorial(2), factorial(3), factorial(4),
  factorial(5), factorial(6), factorial(7), factorial(8), factorial(9)
};

/**
 * 与えられた引数numの各桁の階乗和がnum自身と一致したらnumを返す．そうでなければ0を返す
 * @param  num 対象の数
 * @return     各桁の階乗和がnum自身に一致するときnum，そうでないなら0
 */
uInt equals_digit_factorials_sum_itself(const uInt num)
{
  uInt n = num;
  uInt sum = 0;
  while (n) {
    sum += factorial_array[n % 10];
    if (sum > num) { return 0; }
    n /= 10;
  }
  return sum == num ? num : 0;
}

int main(void)
{
  // 8桁の各桁の階乗和の最大値は 8 * 9! = 2,903,040 (7桁) で，全ての8桁の数はこれより大きい
  // つまり，7桁の階乗和の最大値 7 * 9! まで見れば十分である
  // また，1!,2! は考慮しない
  uInt sum = 0;
  for (uInt i = 3; i <= factorial_array[9] * 7; i++) {
    sum += equals_digit_factorials_sum_itself(i);
  }
  std::cout << "Euler034: " << sum << std::endl;
  return 0;
}
