/**
 * Problem 77 「素数の和」
 * 10は素数の和として5通りに表すことができる:
 *
 * 7 + 3
 * 5 + 5
 * 5 + 3 + 2
 * 3 + 3 + 2 + 2
 * 2 + 2 + 2 + 2 + 2
 *
 * 素数の和としての表し方が5000通り以上になる最初の数を求めよ.
 */

#include <cstdint>
#include <iostream>
#include <vector>

#include "prime.h"

using uInt = std::uint_fast64_t;
using namespace Euler::Prime;

int main(void)
{
  std::vector<NumberType> table;
  for (uInt bound = 100; ; bound *= 10) {  // 十分な素数リストの長さが不明なのでぶん回す
    auto primes = make_primes_vector<uInt>(bound, &table);
    for (uInt target = 10; target < bound; target++) {
      // ここからはe031, e076と同様
      std::vector<uInt> memo(target + 1, 0);
      memo.at(0) = 1;
      for (const uInt prime : primes) {
        for (uInt i = prime; i < memo.size(); i++) {
          memo.at(i) += memo.at(i - prime);
        }
      }
      if (memo.at(target) >= 5000) {
        std::cout << "Euler077: " << target << std::endl;
        return 0;
      }
    }
  }
  return 0;
}

