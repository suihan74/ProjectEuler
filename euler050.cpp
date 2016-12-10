/**
 * Problem 50 「連続する素数の和」
 * 素数41は6つの連続する素数の和として表せる:
 *
 * 41 = 2 + 3 + 5 + 7 + 11 + 13.
 * 100未満の素数を連続する素数の和で表したときにこれが最長になる.
 *
 * 同様に, 連続する素数の和で1000未満の素数を表したときに最長になるのは953で21項を持つ.
 *
 * 100万未満の素数を連続する素数の和で表したときに最長になるのはどの素数か?
 */

#include <cstdint>
#include <iostream>
#include "prime.h"

using uInt = std::uint_fast32_t;
using namespace Euler::Prime;

/**
 * ある数NUMが連続する素数の和で表せる場合，その和算の項数
 * @param  NUM    対象の数
 * @param  primes 素数リスト
 * @return        NUMが連続素数和で表せる場合その足し算の長さ，そうでない場合0
 */
uInt continuous_prime_sum_length(const uInt NUM, const std::vector<uInt>& primes)
{
  for (uInt base = 0; primes[base] * primes[base] <= NUM; base++) {
    for (uInt i = base, count = 0, sum = 0; sum + primes[i] <= NUM; i++) {
      sum += primes[i];
      count++;
      if (sum == NUM) { return count; }
    }
  }
  return 0;
}

int main(void)
{
  constexpr uInt MAX = 1000000;
  const auto primes = make_primes_vector(MAX);

  uInt best_len = 1, best_p = 0;
  for (auto p : primes) {
    const uInt len = continuous_prime_sum_length(p, primes);
    if (len > best_len) {
      best_p = p;
      best_len = len;
    }
  }
  std::cout << "Euler050: " << best_p
                            << " (length: " << best_len << ")"
                            << std::endl;
  return 0;
}
