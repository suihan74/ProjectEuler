/**
 * Problem 45 「三角数, 五角数, 六角数」
 * 三角数, 五角数, 六角数は以下のように生成される.
 *
 * 三角数 Tn=n(n+1)/2 1, 3, 6, 10, 15, ...
 * 五角数 Pn=n(3n-1)/2  1, 5, 12, 22, 35, ...
 * 六角数 Hn=n(2n-1)  1, 6, 15, 28, 45, ...
 *
 * T285 = P165 = H143 = 40755であることが分かる.
 *
 * 次の三角数かつ五角数かつ六角数な数を求めよ.
 */

#include <iostream>
#include <cmath>
#include <cstdint>
#include <limits>
#include "polygonal_number.h"

using uInt = std::uint_fast64_t;
using Real = double;

using namespace Euler::PolygonalNumber;

// 五角数
// 解の公式を使って，五角数num(=Pn)を生成する自然数nがあるか確認する
// n * (3 * n - 1) / 2 = Pn
// 3 * n^2 - n - 2 * Pn = 0
// n = (1 + sqrt(1 + 24 * Pn)) / 6 が整数のときtrue

// 三角数
// n * (n + 1) / 2 = Tn
// n ^ 2 + n - 2 * Tn = 0
// n = (-1 + sqrt(1 + 8 * Tn)) / 2

// 六角数
// n * (2 * n - 1) = Hn
// 2 * n ^ 2 - n - Hn = 0
// n = (1 + sqrt(1 + 8 * Hn)) / 4
//
// P_n+1 - P_n = ((n+1)(3n-1) - n(3n-1)) /2 = (3n - 1)/2
// T_n+1 - T_n = ((n+1)(n+2) - n(n+1)) /2 = n + 1
// H_n+1 - H_n = (n+1)(2n+1) - n(2n-1) = 2n^2 + 3n + 1
//
// Tn,Pn,Hn を比べても解の公式はどれも同じくらいのめんどさ
// そこで，隣接項が最も大きく離れている六角数で値を計算して，
// 他二つの条件を満たすか確認する

int main(void)
{
  for (uInt n = 144; ; n++) {
    const uInt h = n * (2 * n - 1);
    if (is_trianguler(h) && is_pentagonal(h)) {
      std::cout << "Euler045: " << h << std::endl;
      break;
    }
  }
  return 0;
}
