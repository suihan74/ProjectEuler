/**
 * Problem 49 「素数数列」
 * 項差3330の等差数列1487, 4817, 8147は次の2つの変わった性質を持つ.
 *
 * (i)3つの項はそれぞれ素数である.
 * (ii)各項は他の項の置換で表される.
 * 1, 2, 3桁の素数にはこのような性質を持った数列は存在しないが, 4桁の増加列にはもう1つ存在する.
 *
 * それではこの数列の3つの項を連結した12桁の数を求めよ.
 */

#include <cstdint>
#include <iostream>
#include "prime.h"
#include "pandigital.h"

using uInt = std::uint_fast32_t;
using namespace Euler::Prime;
using namespace Euler::Pandigital;
// パンデジタル数関係ないけど「値にどの数字が使用されているか」を調べるのに使える

inline
uInt make_num_bits(const uInt num)
{
  // Problem 38で作ったused_num_bitsを拡張して，0~9でかつ同じ数字の重複使用を許すようにしている
  return used_num_bits<uInt, 0, true>(num);
}

int main(void)
{
  for (uInt i = 1000; i < 10000; i++) {
    if (!is_prime(i)) { continue; }
    const uInt bi = make_num_bits(i);
    for (uInt diff = 1, j, k; (k = (j = i + diff) + diff) < 10000; diff++) {
      const uInt bj = make_num_bits(j);
      const uInt bk = make_num_bits(k);
      // 素数判定は比較的遅いので，先に使用数字で候補を絞ると速くなる
      // そのため，条件文はこの順番で書く必然性がある
      if (bi == bj && bi == bk && i != 1487 && is_prime(j) && is_prime(k)) {
        std::cout << "Euler049: " << i << j << k << std::endl;
      }
    }
  }
  return 0;
}
