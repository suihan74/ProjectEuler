/**
 * Problem 38 「パンデジタル倍数」
 * 192 に 1, 2, 3 を掛けてみよう.
 *
 * 192 × 1 = 192
 * 192 × 2 = 384
 * 192 × 3 = 576
 *
 * 積を連結することで1から9の パンデジタル数 192384576 が得られる. 192384576 を 192 と (1,2,3) の連結積と呼ぶ.
 *
 * 同じようにして, 9 を 1,2,3,4,5 と掛け連結することでパンデジタル数 918273645 が得られる. これは 9 と (1,2,3,4,5) との連結積である.
 *
 * 整数と (1,2,...,n) (n > 1) との連結積として得られる9桁のパンデジタル数の中で最大のものはいくつか?
 */

#include <cmath>
#include <cstdint>
#include <iostream>
#include "pandigital.h"

using uInt = std::uint_fast32_t;
using namespace Euler::Pandigital;

int main(void)
{
  uInt max = 0;
  // j_max = n > 2 より，少なくとも2つの数の連結を作ればよい
  // x = i * j は単調増加なので [i*1][i*2] -> [5桁][4桁] になるパターンは無く，
  // 最大でも [i*1][i*2] -> [4桁][5桁] まで考えれば十分
  for (uInt i = 2; i <= 9876; i++) {
    uInt x = i;
    uInt bits = used_num_bits(x);
    if (!bits) { continue; }
    // i*1, i*2, ..., i*n (n>2)  -> [i*1][i*2]...[i*n] といった具合に連結
    for (uInt j = 2; x < 100000000; j++) {
      const uInt next = i * j;
      const uInt next_bits = used_num_bits(next);
      if (!next_bits || is_overlap(bits, next_bits)) { break; }
      x = next + x * std::pow(10, (uInt)std::log10(next) + 1);
      bits |= next_bits;
    }

    if (x >= 123456789 && pandigital_test(bits) && x > max) {
      max = x;
    }
  }
  std::cout << "Euler038: " << max << std::endl;
  return 0;
}
