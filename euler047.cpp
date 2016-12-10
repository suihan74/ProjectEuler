/**
 * Problem 47 「異なる素因数」
 * それぞれ2つの異なる素因数を持つ連続する2つの数が最初に現れるのは:
 *
 * 14 = 2 × 7
 * 15 = 3 × 5
 *
 * それぞれ3つの異なる素因数を持つ連続する3つの数が最初に現れるのは:
 *
 * 644 = 2^2 × 7 × 23
 * 645 = 3 × 5 × 43
 * 646 = 2 × 17 × 19
 *
 * 最初に現れるそれぞれ4つの異なる素因数を持つ連続する4つの数を求めよ. その最初の数はいくつか?
 */

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>
#include <iterator>

using uInt = std::uint_fast32_t;

// 一度素因数の数がわかった数をキャッシュする
std::vector<std::vector<uInt>> factors_cache;

uInt count_prime_factors(const uInt num)
{
  uInt n = num;
  if (factors_cache.size() <= num) {
    factors_cache.resize(num * 2, {});
  }
  auto& factors = factors_cache.at(num);
  while (n > 1) {
    for (uInt i = 2; i <= n; i++) {
      if (n % i == 0) {
        n /= i;
        factors.push_back(i);
        break;
      }
    }

    // 計算中に以前扱った数が出てきたら以後の探索をスキップする
    const auto& cache = factors_cache.at(n);
    if (!cache.empty()) {
      std::copy(cache.cbegin(), cache.cend(), std::back_inserter(factors));
      std::sort(factors.begin(), factors.end());
      break;
    }
  }

  // 644の例のように重複した因数がある場合もあるのでその分を除いて数える
  return std::unique(factors.begin(), factors.end()) - factors.begin();
}

int main(void)
{
  constexpr uInt LENGTH = 4;
  for (uInt i = 2, j; ; i++) {
    for (j = 0; j < LENGTH; j++) {
      if (count_prime_factors(i + LENGTH - j - 1) != LENGTH) {
        i += LENGTH - j - 1;
        break;
      }
    }
    if (j == LENGTH) {
      std::cout << "Euler047: " << i << std::endl;
      break;
    }
  }
  return 0;
}
