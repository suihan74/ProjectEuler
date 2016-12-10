/**
 * Problem 6 「二乗和の差」
 * 最初の10個の自然数について, その二乗の和は,
 * 1^2 + 2^2 + ... + 10^2 = 385
 *
 * 最初の10個の自然数について, その和の二乗は,
 * (1 + 2 + ... + 10)^2 = 3025
 *
 * これらの数の差は 3025 - 385 = 2640 となる.
 * 同様にして, 最初の100個の自然数について二乗の和と和の二乗の差を求めよ.
 */

#include <cstdint>
#include <iostream>

using uInt = std::uint_fast32_t;
const uInt N = 100;

int main(void)
{
  const uInt sum = (N + 1) * N / 2;
  const uInt pow_sum = N * (N + 1) * (2 * N + 1) / 6;
  std::cout << "Euler006: " << sum * sum - pow_sum << std::endl;
  return 0;
}
