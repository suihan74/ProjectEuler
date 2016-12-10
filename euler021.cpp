/**
 * Problem 21 「友愛数」
 * d(n) を n の真の約数の和と定義する. (真の約数とは n 以外の約数のことである. )
 * もし, d(a) = b かつ d(b) = a (a ≠ b のとき) を満たすとき, a と b は友愛数(親和数)であるという.
 *
 * 例えば, 220 の約数は 1, 2, 4, 5, 10, 11, 20, 22, 44, 55, 110 なので d(220) = 284 である.
 * また, 284 の約数は 1, 2, 4, 71, 142 なので d(284) = 220 である.
 *
 * それでは10000未満の友愛数の和を求めよ.
 */

#include <cstdint>
#include <iostream>

using uInt = std::uint_fast32_t;

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

int main(void)
{
  uInt sum = 0;
  for (uInt a = 1; a < 10000; a++) {
    uInt b = proper_divisors_sum(a);
    if (a != b && proper_divisors_sum(b) == a) {
      sum += a;
    }
  }

  std::cout << "Euler021: " << sum << std::endl;
  return 0;
}
