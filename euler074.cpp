/**
 * Problem 74 「桁の階乗による連鎖」
 * 145は各桁の階乗の和が145と自分自身に一致することで有名である.
 *
 * 1! + 4! + 5! = 1 + 24 + 120 = 145
 *
 * 169の性質はあまり知られていない. これは169に戻る数の中で最長の列を成す. このように他の数を経て自分自身に戻るループは3つしか存在しない.
 *
 * 169 → 363601 → 1454 → 169
 * 871 → 45361 → 871
 * 872 → 45362 → 872
 *
 * どのような数からスタートしてもループに入ることが示せる.
 *
 * 例を見てみよう.
 *
 * 69 → 363600 → 1454 → 169 → 363601 (→ 1454)
 * 78 → 45360 → 871 → 45361 (→ 871)
 * 540 → 145 (→ 145)
 *
 * 69から始めた場合, 列は5つの循環しない項を持つ. また100万未満の数から始めた場合最長の循環しない項は60個であることが知られている.
 *
 * 100万未満の数から開始する列の中で, 60個の循環しない項を持つものはいくつあるか?
 */

#include <cstdint>
#include <iostream>
#include <set>

using uInt = std::uint_fast32_t;

/**
 * 階乗を計算
 * @param n   n!のn
 * @return n!
 */
constexpr
uInt factorial(uInt n)
{
  return n < 2 ? 1 : n * factorial(n - 1);
}

/** 0!,1!,...,9!をコンパイル時に計算 */
constexpr uInt FACTS[] = {
  factorial(0), factorial(1), factorial(2), factorial(3), factorial(4),
  factorial(5), factorial(6), factorial(7), factorial(8), factorial(9)
};

/**
 * 各桁の数値を階乗して足した数を返す
 * @param  cur 自然数
 * @return     curから生成される次の数
 */
inline
uInt next(uInt cur)
{
  uInt result = 0;
  while (cur) {
    result += FACTS[cur % 10];
    cur /= 10;
  }
  return result;
}

int main(void)
{
  constexpr uInt BOUND = 60;
  uInt count = 0;
  for (uInt i = 2; i < 1000000; i++) {
    std::set<uInt> used;  // 循環しない項を収集（今回は順番はどうでもいいのでstd::set）
    for (uInt j = 0, cur = i; j < BOUND; j++) {
      used.insert(cur);
      cur = next(cur);
      if (used.find(cur) != used.end()) { break; }
    }
    if (used.size() == BOUND) {
      count++;
    }
  }
  std::cout << "Euler074: " << count << std::endl;
  return 0;
}
