/**
 * Problem 51 「素数の桁置換」
 * *3の第1桁を置き換えることで, 13, 23, 43, 53, 73, 83という6つの素数が得られる.
 *
 * 56**3の第3桁と第4桁を同じ数で置き換えることを考えよう. この5桁の数は7つの素数をもつ最初の例である: 56003, 56113, 56333, 56443, 56663, 56773, 56993. よって, この族の最初の数である56003は, このような性質を持つ最小の素数である.
 *
 * 桁を同じ数で置き換えることで8つの素数が得られる最小の素数を求めよ. (注:連続した桁でなくても良い)
 */

#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>
#include <utility>
#include "prime.h"

using uInt = std::uint_fast64_t;
using namespace Euler::Prime;

std::pair<uInt, uInt> count_prime_variations(const uInt base, const std::vector<NumberType>& tbl)
{
  uInt max_count = 0;
  uInt min_prime = 0;
  const uInt digits = std::log10(base) + 1;
  uInt supplier = base;
  for (uInt i = 0; i < digits; i++) {
    // 現在の組（12**3とかのこと）での置換可能個数
    uInt count = 0;
    // 現在の組で生成できる最小の素数
    uInt min_prime_tmp = std::numeric_limits<uInt>::max();
    // 下の方から桁の数字に注目し，同じ数値の桁の値を全てnに置換する
    const uInt target = supplier % 10;
    supplier /= 10;
    for (uInt n = 0; n < 10; n++) {
      uInt base_tmp = base;
      for (uInt j = i; j < digits; j++) {
        const uInt scale = std::pow(10, j);
        if (base / scale % 10 != target) { continue; }
        const uInt scale_x10 = scale * 10;
        // 置換 -> ABB*CD -> ABB000 + n00 + CD -> ABBnCD
        const uInt base_top = base_tmp / scale_x10 * scale_x10 + n * scale;
        // 桁数が変化する場合は含めない
        if (base_top == 0) {
          base_tmp = 0;
          break;
        }
        base_tmp = base_top + base_tmp % scale;
      }
      if (tbl.at(base_tmp) == NumberType::PRIME) {
        if (base_tmp < min_prime_tmp) {
          min_prime_tmp = base_tmp;
        }
        count++;
      }
    }

    // 元の数から生成可能なパターンのうち，
    // 最も多くの素数を作れた個数と，そのパターンで生成できる最小の素数
    if (count > max_count) {
      min_prime = min_prime_tmp;
      max_count = count;
    }
  }
  return std::make_pair(min_prime, max_count);
}

int main(void)
{
  constexpr uInt VARIATION = 8;
  constexpr uInt MAX = 1000000;
  std::vector<NumberType> ptbl;
  auto primes = make_primes_vector(MAX, &ptbl);
  const uInt size = primes.size();

  for (uInt i = 0; i < size; i++) {
    const auto p = count_prime_variations(primes[i], ptbl);
    if (p.second == VARIATION) {
      std::cout << "Euler051: " << p.first << std::endl;
      return 0;
    }
  }

  return 0;
}



