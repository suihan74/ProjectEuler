/**
 * Problem 43 「部分文字列被整除性」
 * 数1406357289は0から9のパンデジタル数である (0から9が1度ずつ現れるので). この数は部分文字列が面白い性質を持っている.
 *
 * d1を上位1桁目, d2を上位2桁目の数とし, 以下順にdnを定義する. この記法を用いると次のことが分かる.
 *
 * d2d3d4=406 は 2 で割り切れる
 * d3d4d5=063 は 3 で割り切れる
 * d4d5d6=635 は 5 で割り切れる
 * d5d6d7=357 は 7 で割り切れる
 * d6d7d8=572 は 11 で割り切れる
 * d7d8d9=728 は 13 で割り切れる
 * d8d9d10=289 は 17 で割り切れる
 * このような性質をもつ0から9のパンデジタル数の総和を求めよ.
 */

#include <cmath>
#include <cstdint>
#include <iostream>
#include <stdexcept>

using uInt = std::uint_fast64_t;

constexpr uInt PRIMES[] = { 2, 3, 5, 7, 11, 13, 17 };

/**
 * 0 ~ depth_bound のパンデジタル数のうち'部分文字列被整除性'をもったものを順番に得る
 * @param depth_bound  最大桁数
 * @param prev         前回の値（初回では0を与える）
 * @param used         0 ~ depth_boundのうち現在試行で使用中の数字を記録するビット列
 * @param current      この深度までに生成されてきた値
 * @param depth        再帰の深さ
 * @return             生成した順列 (無効時: 0)
 */
uInt next_divisible_pandigital(
  const uInt depth_bound,
  const uInt prev = 0,
  const uInt used = 0,
  const uInt current = 0,
  const uInt depth = 0)
{
  if (depth_bound < 2 || depth_bound > 9) { throw std::out_of_range("next_divisible_pandigital: invalid depth_bound 'd': d<2 || d>9"); }
  // 前回の値から再帰を復元する
  // d1 == 0 のときだけ実際の桁数が変化する（ex: 0123 -> 123）ので修正する
  const uInt init = (depth == 0 && (uInt)std::log10(prev) == 8) ? 0
                  : prev / uInt(std::pow(10, depth_bound - depth)) % 10;
  for (uInt i = init; i <= depth_bound; i++) {
    const uInt MASK = 1 << i;
    if (used & MASK) { continue; }
    const uInt interim = current * 10 + i;

    // 条件 d2d3d4 % 2 == 0 ...など，構成する各三桁が指定の素数で割り切れるものだけ考慮する
    if (depth > 2 && (interim % 1000) % PRIMES[depth - 3] != 0) {
      continue;
    }

    // 最初の一回は前回の値と同じ値をとるので，そこでは無効な結果を返す
    if (depth == depth_bound) {
      return interim > prev ? interim : 0;
    }
    else {
      // 前回の値を使うのは最初の一回だけでよく，その後も渡してしまうとinitに同じ値が入り続けるので止す
      auto res = next_divisible_pandigital(depth_bound, (i == init ? prev : 0), used | MASK, interim, depth + 1);
      if (res) { return res; }
    }
  }
  return 0;
}

int main(void)
{
  uInt prev = 0;
  uInt sum = 0;
  for (uInt i = 0; ; i++) {
    prev = next_divisible_pandigital(9, prev);
    if (!prev) { break; }
    sum += prev;
  }
  std::cout << "Euler043: " << sum << std::endl;
  return 0;
}
