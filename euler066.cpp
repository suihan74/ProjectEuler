
#include <cstdint>
#include <iostream>

#include "largeint.h"
#include "continued_fraction.h"

using uInt = std::uint_fast64_t;
using namespace Euler::ContinuedFraction;
using Euler::LargeInt;

int main(void)
{
  uInt best_d = 0;
  LargeInt<uInt> best_x(0);
  uInt i = 2;
  for (uInt d = 2; d <= 1000; d++) {
    if (d == i * i) { i++; continue; }
    const auto a_vec = make_cfrac_factors(d);
    // 題意を満たす最小のxとyは，連分数の要素を用いた漸化式から計算することができる
    // これは，x^2 - D * y^2 = 1 --> x^2 = 1 + D * y^2 --> x^2 = D * y^2
    // --> sqrt(D) = x / y
    // ……と近似できるためである．
    //
    // x(0) = 1, x(1) = a(0), x(n+1) = a(n) * x(n) + x(n - 1)
    // y(0) = 0, y(1) = 1,    y(n+1) = a(n) * y(n) + y(n - 1)
    LargeInt<uInt> x_1(1);
    LargeInt<uInt> x(a_vec.at(0));
    const uInt size = a_vec.size() - 1;
    // 漸化式は，ベクタの長さで割った余りが-1と合同になる奇数nまで繰り返す
    for (uInt n = 0; n % 2 == 0 || n % size != size - 1; n++) {
      const uInt a = a_vec.at(1 + n % size);
      LargeInt<uInt> x_tmp = std::move(x);
      x = x_tmp * a + x_1;
      x_1 = std::move(x_tmp);
    }
    if (x > best_x) {
      best_d = d;
      best_x = std::move(x);
    }
  }

  std::cout << "Euler066: " << best_d << " (x = " << best_x << ")" << std::endl;
  return 0;
}
