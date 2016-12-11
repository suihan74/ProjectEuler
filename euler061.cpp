/**
 * Problem 61 「巡回図形数」
 * 三角数, 四角数, 五角数, 六角数, 七角数, 八角数は多角数であり, それぞれ以下の式で生成される.
 *
 * 三角数 P3,n=n(n+1)/2 1, 3, 6, 10, 15, ...
 * 四角数 P4,n=n2 1, 4, 9, 16, 25, ...
 * 五角数 P5,n=n(3n-1)/2  1, 5, 12, 22, 35, ...
 * 六角数 P6,n=n(2n-1)  1, 6, 15, 28, 45, ...
 * 七角数 P7,n=n(5n-3)/2  1, 7, 18, 34, 55, ...
 * 八角数 P8,n=n(3n-2)  1, 8, 21, 40, 65, ...
 * 3つの4桁の数の順番付きの集合 (8128, 2882, 8281) は以下の面白い性質を持つ.
 *
 * この集合は巡回的である. 最後の数も含めて, 各数の後半2桁は次の数の前半2桁と一致する
 * それぞれ多角数である: 三角数 (P3,127=8128), 四角数 (P4,91=8281), 五角数 (P5,44=2882) がそれぞれ別の数字で集合に含まれている
 * 4桁の数の組で上の2つの性質をもつはこの組だけである.
 * 三角数, 四角数, 五角数, 六角数, 七角数, 八角数が全て表れる6つの巡回する4桁の数からなる唯一の順序集合の和を求めよ.
 */

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <vector>
#include "polygonal_number.h"

using uInt = std::uint_fast32_t;
using namespace Euler::PolygonalNumber;

/**
 * numが集合の中にない多角数種であるかを確認する
 * @param num   判定したい数値
 * @param mask  既に集合に含まれている多角数種を表すビット列
 * @return      numが3~8の多角数ではない場合: 0
 *              numが集合に既に存在する多角数種である: 0
 *              numが集合に無い多角数種に含まれる: 集合中に含まれる多角数を表すビット列にnumの多角数種を加えたもの
 */
inline
uInt is_unused_polygonal_number(const uInt num, const uInt mask)
{
  uInt n = 0;
  for (;;) {  // 複数の多角数である場合があるのでそれらを全て判定する
    n = is_polygonal(num, n);
    if (!n) { return 0; }
    uInt b;
    if ((b = (1 << (n - 3))) & mask) { continue; }
    return mask | b;
  }
  return 0;
}

/**
 * 単に0b1111...なるビット列を作る
 * @param num  何個の1を立てるか
 * @return     0b111...(num個の1)
 */
inline
uInt make_mask1(uInt num)
{
  uInt res = 0;
  for (uInt i = 0; i < num; i++) {
    res |= 1 << i;
  }
  return res;
}

/**
 * 三角数~八画数が全て現れるように構成された巡回数集合を生成する
 * @param  SET_SIZE 何個の数の集合にするか（==三角数，四角数，……と続けて何角数まで網羅するか）
 * @param  set      順回数集合を表すベクタ
 * @param  depth    再帰の深さ
 * @param  mask     現在の再帰までで，集合の中にどの多角数種が現れているかを表すビット列
 * @return          題意を満たす巡回数集合が生成できた場合: true
 */
bool make_rounds_set_recursive(
  const uInt SET_SIZE,
  std::vector<uInt>& set,
  const uInt depth = 0,
  const uInt mask = 0)
{
  if (depth == 0) {
    set.clear();
    set.resize(SET_SIZE, 0);
  }

  // ひとつの再帰で2桁ずつ決定してくっつけていく
  // ex) aabb, bbcc, ccaa
  // depth = 0 で aa を，depth = 1 で bb を，depth = 2 で cc を決定
  // depth = 1, 2 のとき，aabb, bbcc を集合に加えて集合中でユニークな多角数種かを判定
  // さらに depth = 3 で ccaa 部分を作って判定
  for (uInt i = 10; i < 100; i++) {
    if (depth < SET_SIZE) {
      set.at(depth) = i * 100;
      if (depth > 0) {
        set.at(depth - 1) += i;
      }
    }

    uInt b = 0;
    if (depth == SET_SIZE) {
      set.at(depth - 1) += set.at(0) / 100;   // 上例でいうところの ccaa を作る作業
      if (!(b = is_unused_polygonal_number(set.at(depth - 1), mask))
       || (b ^ make_mask1(SET_SIZE)))
      {
        set.at(depth - 1) -= set.at(0) / 100;
        continue;
      }
      return true;
    }

    if ((depth == 0 || (b = is_unused_polygonal_number(set.at(depth - 1), mask)))
     && (make_rounds_set_recursive(SET_SIZE, set, depth + 1, b)))
    {
      return true;
    }
    set.at(depth) = 0;
    if (depth > 0) {
      set.at(depth - 1) -= i;
    }
  }
  return false;
}

int main(void)
{
  std::vector<uInt> set;
  make_rounds_set_recursive(6, set);
  // ちなみに三角数がふたつ表示されるが，8128は六角数でもある
  std::for_each(set.begin(), set.end(), [](uInt x) { std::cout << x << "(" << is_polygonal(x) << ")" << ","; });
  std::cout << std::endl;
  // ほしいのは集合内の数の和
  const uInt sum = std::accumulate(set.begin(), set.end(), uInt(0));
  std::cout << "Euler061: " << sum << std::endl;
  return 0;
}
