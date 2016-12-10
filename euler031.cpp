/**
 * Problem 31 「硬貨の和」
 * イギリスでは硬貨はポンド£とペンスpがあり，一般的に流通している硬貨は以下の8種類である.
 *
 * 1p, 2p, 5p, 10p, 20p, 50p, £1 (100p) and £2 (200p).
 *
 * 以下の方法で£2を作ることが可能である．
 *
 * 1×£1 + 1×50p + 2×20p + 1×5p + 1×2p + 3×1p
 *
 * これらの硬貨を使って£2を作る方法は何通りあるか?
 */

#include <cstdint>
#include <iostream>

using uInt = std::uint_fast32_t;

/**
 * units[]の中身で合計値leftを作る組み合わせの個数を計算する
 * @param  units    組み合わせに使える硬貨の集合
 * @param  arr_size unitsの配列長
 * @param  left     （この呼び出し階層以下での残りの）合計値
 * @param  idx      unitsの注目インデックス
 * @param  count    今まで見つけた正解の組み合わせ個数
 * @return          この呼び出し階層までで見つけた正解の組み合わせ個数
 */
uInt get_patterns(
  const uInt units[],
  const uInt arr_size,
  uInt left,
  uInt idx = 0,
  uInt count = 0)
{
  // 対象の硬貨を使ってleft以下になるパターン数N
  // 例: left=200p, units[idx]=2p なら2p硬貨は0~100枚まで使える
  const uInt N = left / units[idx];
  if (units[idx] * N == left) { count++; }  // leftがunits[idx]で割り切れる => 正解の組み合わせがある
  for (uInt i = 0; i <= N; i++) {
    // 対象の硬貨 * 何枚使うか でこの階層の金額を計算
    const uInt cur = units[idx] * i;
    if (left > cur && idx + 1 < arr_size) {
      count = get_patterns(units, arr_size, left - cur, idx + 1, count);
    }
  }
  return count;
}

int main(void)
{
  uInt units[] = {200, 100, 50, 20, 10, 5, 2, 1};
  std::cout << "Euler031: " << get_patterns(units, 8, 200) << std::endl;
  return 0;
}
