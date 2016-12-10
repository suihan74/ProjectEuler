/**
 * Problem 24 「辞書式順列」
 * 順列とはモノの順番付きの並びのことである. たとえば, 3124は数 1, 2, 3, 4 の一つの順列である.
 * すべての順列を数の大小でまたは辞書式に並べたものを辞書順と呼ぶ. 0と1と2の順列を辞書順に並べると
 *
 * 012 021 102 120 201 210
 * になる.
 *
 * 0,1,2,3,4,5,6,7,8,9からなる順列を辞書式に並べたときの100万番目はいくつか?
 */

#include <cmath>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <utility>

// 9,876,543,210 みたいな結果を取り得るので，32bitだと溢れる恐れがある
using uInt = std::uint_fast64_t;

/**
 * 階乗を計算
 * 試行可能範囲の算出に利用（nPn = n!より）
 * @param  n  階乗したい数
 * @return    n!
 */
constexpr
uInt factorial(uInt n)
{
  return n > 1 ? n * factorial(n - 1) : 1;
}

/**
 * 0~9の数字を一回ずつ使った順列のうち，index番目のものを取得する
 * @param INDEX        何番目の順列が欲しいか
 * @param used         0~9のうち現在試行で使用中の数字を記録するビット列
 * @param dic_counter  現在何番目までの順列を生成したかを表すカウンタ
 * @param current      この深度までに生成されてきた値
 * @param depth        再帰の深さ
 * @return  .first     生成した順列
 * @return  .second    生成した順列が辞書順で何番目のものか
 */
std::pair<uInt, uInt> make_permutation_recursive(
  const uInt INDEX,
  uInt used = 0,
  uInt dic_counter = 0,
  uInt current = 0,
  uInt depth = 0)
{
  for (uInt i = 0; i < 10; i++) {
    const uInt MASK = 1 << i;
    if (used & MASK) { continue; }
    const uInt PERM = current + i * std::pow(10, 9 - depth);
    if (depth == 9) {
      return std::make_pair(PERM, dic_counter + 1);
    }
    else {
      used |= MASK;
      auto p = make_permutation_recursive(INDEX, used, dic_counter, PERM, depth + 1);
      if (p.second == INDEX) { return std::move(p); }
      dic_counter = p.second;
      used &= ~MASK;
    }
  }
  return std::make_pair(0, dic_counter);
}

/**
 * 引数の範囲をチェックし，pairで返ってくる試行結果の必要部分だけ返す
 * @param INDEX  何番目の順列が欲しいか
 * @return       INDEX番目に見つけた順列
 * @throw std::out_of_range  INDEXが0または10!を超える場合，試行不可能なので例外送出
 */
inline
uInt make_permutation_at(const uInt INDEX)
{
  constexpr uInt FACT10 = factorial(10);
  if (INDEX == 0) { throw std::out_of_range("make_permutation_at(0) is not allowed"); }
  if (INDEX > FACT10) { throw std::out_of_range("make_permutation_at(): exceeded the calculable range"); }
  return make_permutation_recursive(INDEX).first;
}

int main(void)
{
  try {
    std::cout << "Euler024: " << make_permutation_at(1000000) << std::endl;
    return 0;
  }
  catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    return 1;
  }
}
