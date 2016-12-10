/**
 * Problem 58 「螺旋素数」
 * 1から始めて, 以下のように反時計回りに数字を並べていくと, 辺の長さが7の渦巻きが形成される.
 *
 * 37  36  35  34  33  32  31
 * 38  17  16  15  14  13  30
 * 39  18  5   4   3   12  29
 * 40  19  6   1   2   11  28
 * 41  20  7   8   9   10  27
 * 42  21  22  23  24  25  26
 * 43  44  45  46  47  48  49
 * 面白いことに, 奇平方数が右下の対角線上に出現する. もっと面白いことには, 対角線上の13個の数字のうち, 8個が素数である. ここで割合は8/13 ≈ 62%である.
 *
 * 渦巻きに新しい層を付け加えよう. すると辺の長さが9の渦巻きが出来る. 以下, この操作を繰り返していく. 対角線上の素数の割合が10%未満に落ちる最初の辺の長さを求めよ.
 */

#include <cstdint>
#include <iostream>
#include "prime.h"

using uInt = std::uint_fast32_t;
using namespace Euler::Prime;

inline
uInt count_spiral_corners_prime(uInt level)
{
  // 右上方向に注目すると，1,9,25,49,... と初項1,公差2の等差数列の2乗になる
  // 1x1になる部分だけ四隅が無いので，そこだけ特殊化する
  if (level == 0) { return 0; }
  // 1x1を考えないので，初項3,公差2の等差数列の2乗
  const uInt N = 2 * level + 1;
  // 他の三隅の値は，その階層がNxNとするとN-1ずつ引いていった値になる
  // -> N*N, N*N - (N - 1), N*N - 2 * (N - 1), N*N - 3 * (N - 1);
  return (uInt)is_prime(N * N)
       + (uInt)is_prime(N * N - N + 1)
       + (uInt)is_prime(N * N - 2 * N + 2)
       + (uInt)is_prime(N * N - 3 * N + 3);
}

int main(void)
{
  uInt primes = 0;
  uInt corners = 1;
  for (uInt i = 1; ; i++) {
    corners += 4;
    primes += count_spiral_corners_prime(i);
    if (primes * 10 < corners) {
      // 辺の長さ => 1,3,5,7... => 初項1(i=0),公差2 => 2 * i + 1
      std::cout << "Euler058: " << (2 * i + 1) << std::endl;
      break;
    }
  }
  return 0;
}
