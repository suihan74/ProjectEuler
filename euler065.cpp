
#include <cstdint>
#include <iostream>
#include <utility>

#include "continued_fraction.h"
#include "largeint.h"

using uInt = std::uint_fast32_t;
using LInt = Euler::LargeInt<uInt>;
using Fraction = std::pair<LInt, LInt>;  // first: 分子, second: 分母
using namespace Euler::ContinuedFraction;

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
