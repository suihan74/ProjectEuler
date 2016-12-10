/**
 * Problem 60 「素数ペア集合」
 * 素数3, 7, 109, 673は非凡な性質を持っている. 任意の2つの素数を任意の順で繋げると, また素数になっている.
 * 例えば, 7と109を用いると, 7109と1097の両方が素数である. これら4つの素数の和は792である.
 * これは, このような性質をもつ4つの素数の集合の和の中で最小である.
 *
 * 任意の2つの素数を繋げたときに別の素数が生成される, 5つの素数の集合の和の中で最小のものを求めよ.
 */

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>
#include <iostream>
#include <numeric>
#include <utility>
#include "prime.h"

using uInt = std::uint_fast64_t;
using namespace Euler::Prime;
using uIntPair = std::pair<uInt, uInt>;

/**
 * 任意の自然数a,bを連結(aaa..bbb=aaabbb)した値が素数ならその値を返し，そうでないなら0を返す
 * @param a    任意の自然数
 * @param b    任意の自然数
 * @param tbl  素数テーブル（素数判定の計算時間短縮のため）
 * @return     a..bが素数のときa..bの値，素数ではないとき0
 */
inline
uInt prime_concat(uInt a, uInt b, const std::vector<NumberType>& tbl)
{
  const uInt ab = a * std::pow(10, uInt(std::log10(b) + 1)) + b;
  // 素数テーブルを使って可能な限り時間短縮を図る
  // 素数テーブル: 一度作ってしまえば（そしてどちらにしろ作る必要がある）素数判定はO(1)
  // is_prime: O(sqrt(ab))
  // ただしテーブルの最大値より大きい数（より時間がかかる）ではis_primeが使われるので気休め程度（実際実行時間に大差は無い）
  return ((ab < tbl.size() && tbl[ab] == NumberType::PRIME) || is_prime(ab)) ? ab : 0;
}

/**
 * 任意の組み合わせで連結素数化が可能な素数集合を生成し，それを構成する素数の和の最小値を返す
 * @param  set         素数集合を保持するベクタ
 * @param  primes      素数ベクタ
 * @param  tbl         素数判定テーブル
 * @param  DEPTH_BOUND 再帰回数の限度（==集合サイズ）
 * @param  idx         現在の再帰における探索開始インデックス
 * @param  depth       現在の再帰回数
 * @return             成功時，集合を構成する素数の和．失敗時0
 */
uInt make_set_recursive(
  std::vector<uInt>& set,
  const std::vector<uInt>& primes,
  const std::vector<NumberType>& tbl,
  const uInt DEPTH_BOUND,
  const uInt idx = 1,
  const uInt depth = 1)
{
  if (depth == 1) { set.clear(); set.reserve(DEPTH_BOUND); }
  const uInt size = primes.size();
  for (uInt i = idx; i < size; i++) {
    const auto p = primes[i];
    // 集合内の全ての素数に対して，左右どちら側から連結しても素数になる素数pを見つける
    const bool failed = std::any_of(set.begin(), set.end(), [p, &tbl](uInt x)
      {
        return !prime_concat(x, p, tbl) || !prime_concat(p, x, tbl);
      });
    if (failed) { continue; }

    set.push_back(p);
    if (depth == DEPTH_BOUND) {
      // 集合は小さい方から順番に作られるので，一番最初に見つけた集合の和が最小になる
      return std::accumulate(set.begin(), set.end(), uInt(0));
    }
    else {
      const uInt sum = make_set_recursive(set, primes, tbl, DEPTH_BOUND, i + 1, depth + 1);
      if (sum) {
        return sum;
      }
    }
    set.erase(set.end() - 1, set.end());
  }
  return 0;
}

/**
 * 題意に沿うSET_SIZE個のNUM_BOUND以下の素数でできた集合から作れる和の内最小の物を探す
 * @param SET_SIZE   素数集合の大きさ
 * @param NUM_BOUND  考慮する最大値
 * @return  集合が作れた場合，集合を構成する素数の和．失敗時0
 */
inline
uInt get_minsum_primeset(const uInt SET_SIZE, const uInt NUM_BOUND)
{
  std::vector<uInt> set;
  std::vector<NumberType> tbl;
  const auto primes = make_primes_vector<uInt>(NUM_BOUND, &tbl);
  return make_set_recursive(set, primes, tbl, SET_SIZE);
}

int main(void)
{
  constexpr uInt SET_SIZE = 5;
  // iは考慮する最大値．SET_SIZE==4のとき最小の和が792なので
  // SET_SIZE==5ではとりあえず1~3桁の素数の組み合わせからトライする
  for (uInt bound = 1000; ; bound *= 10) {
    const uInt result = get_minsum_primeset(SET_SIZE, bound);
    if (result) {
      std::cout << "Euler060: " << result << std::endl;
      break;
    }
  }
  return 0;
}
