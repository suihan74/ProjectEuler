
#include <cstdint>
#include <iostream>
#include <utility>

// 計算途中ででかい数になるため
#include "largeint.h"

using uInt = std::uint_least32_t;
using LInt = Euler::LargeInt<uInt>;
using Fraction = std::pair<LInt, LInt>;  // first: 分子, second: 分母

/**
 * ネイピア数の連分数を構成するa_kを計算する
 * @param idx  何番目のaが欲しいか
 * @return     a_(idx)
 */
inline
uInt napiers_term_at(uInt idx)
{
  if (idx == 0) { return 2; }
  const uInt term = (idx - 1) / 3 + 1;
  const uInt pos = (idx - 1) % 3;
  return pos == 1 ? (2 * term) : 1;
}

int main(void)
{
  constexpr uInt DEPTH_BOUND = 100;  // 項目は1始まりで数える（1項目, 2項目, ...）
  Fraction frac = std::make_pair(1, 1);
  if (DEPTH_BOUND <= 1) {
    frac.first = napiers_term_at(0);
  }
  else {
    frac.second = napiers_term_at(DEPTH_BOUND - 1);
  }

  // 地道に分数計算
  for (uInt i = 2; i <= DEPTH_BOUND; i++) {
    const uInt a = napiers_term_at(DEPTH_BOUND - i);
    frac.first += frac.second * a;
    if (i == DEPTH_BOUND) { continue; }
    // 1/(fr.f/fr.s) => fr.s/fr.f
    std::swap(frac.first, frac.second);
  }

  std::cout << "Euler065: " << frac.first.digits_sum() << std::endl;

  return 0;
}
