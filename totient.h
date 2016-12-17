#ifndef __TOTIENT_H__
#define __TOTIENT_H__

#include "prime.h"

namespace Euler
{
namespace Totient
{

  /**
   * φ(n) = n・Π(1 - 1/p) ; pはnの素因数
   * 上式によるトーシェント関数計算の過程
   * @param dest ここまでの途中結果
   * @param n    n（の途中結果）
   * @param i    素因数（の候補）
   */
  template <typename Int_t>
  inline
  void product_prime_factor(double& dest, Int_t& n, Int_t i)
  {
    if (n % i == 0) {
      dest *= 1.0 - 1.0 / i;
      do { n /= i; } while (n % i == 0);
    }
  }

  /**
   * トーシェント関数φ(n)（nと互いに素な数の個数を得る）
   * @param  n      任意の自然数
   * @param  nt_vec 素数・複素数判定ベクタ
   * @return        nと互いに素な関係にある数の個数
   */
  template <typename Int_t>
  inline
  Int_t totient(Int_t n, const std::vector<Euler::Prime::NumberType>& nt_vec)
  {
    if (nt_vec.at(n) == Euler::Prime::NumberType::PRIME) { return n - 1; }

    // φ(n) = n・Π(1 - 1/p) ; pはnの素因数
    // 上式によりトーシェント関数を計算

    double result = n;
    for (Int_t i = 2; i <= 3; i++) {
      product_prime_factor(result, n, i);
    }
    for (Int_t i = 5; i <= n / i; i += 6) {
      product_prime_factor(result, n, i);
      product_prime_factor(result, n, i + 2);
    }
    if (n > 1) {
      result *= 1.0 - 1.0 / n;
    }
    return static_cast<Int_t>(result);
  }
}
}

#endif
