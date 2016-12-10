/**
 * Problem 41 「パンデジタル素数」
 * n桁パンデジタルであるとは, 1からnまでの数を各桁に1つずつ持つこととする.
 *
 * #下のリンク先にあるような数学的定義とは異なる
 *
 * 例えば2143は4桁パンデジタル数であり, かつ素数である. n桁（この問題の定義では9桁以下）パンデジタルな素数の中で最大の数を答えよ.
 */

#include <cmath>
#include <cstdint>
#include <iostream>
#include "prime.h"

using uInt = std::uint_fast64_t;
using namespace Euler::Prime;

/**
 * 1~depth_boundの数字を一回ずつ使ったパンデジタル素数のうち最大のものを探す
 * @param depth_bound  最大桁数
 * @param used         1~depth_boundのうち現在試行で使用中の数字を記録するビット列
 * @param current      この深度までに生成されてきた値
 * @param depth        再帰の深さ
 * @return             生成した順列 (失敗時: 0)
 */
uInt make_largest_pandigital_prime(
  const uInt depth_bound,
  const uInt used = 0,
  const uInt current = 0,
  const uInt depth = 0)
{
  if (depth_bound < 2) { return 0; }
  for (uInt i = depth_bound; i >= 1; --i) {
    const uInt MASK = 1 << i;
    if (used & MASK) { continue; }
    const uInt interim = current * 10 + i;
    if (depth + 1 == depth_bound) {
      return interim;
    }
    else {
      auto res = make_largest_pandigital_prime(depth_bound, used | MASK, interim, depth + 1);
      if (is_prime(res)) { return res; }
    }
  }
  return 0;
}


int main(void)
{
  for (uInt d = 9; d >= 1; --d) {
    const uInt result = make_largest_pandigital_prime(d);
    if (result) {
      std::cout << "Euler041: " << result << std::endl;
      break;
    }
  }
  return 0;
}
