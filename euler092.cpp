/**
 * Problem 92 「桁の2乗による連鎖」
 * 各桁の2乗を足し合わせて新たな数を作ることを, 同じ数が現れるまで繰り返す.
 *
 * 例えば
 *
 * 44 → 32 → 13 → 10 → 1 → 1
 * 85 → 89 → 145 → 42 → 20 → 4 → 16 → 37 → 58 → 89
 *
 * のような列である. どちらも1か89で無限ループに陥っている.
 * 驚くことに, どの数から始めても最終的に1か89に到達する.
 *
 * では, 10,000,000より小さい数で89に到達する数はいくつあるか.
 */

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <vector>

using uInt = std::uint_fast32_t;

/**
 * ある数numの各桁の2乗の和を計算する
 * @param num 対象の数
 * @return    numの各桁の数の2乗の和
 */
inline
uInt digits_pow_sum(uInt num)
{
  uInt res = 0;
  while (num) {
    const uInt d = num % 10;
    res += d * d;
    num /= 10;
  }
  return res;
}

/**
 * 与えられた数に対して題意に沿った連鎖を繰り返し,
 * 連鎖がループする数値を見つける
 * @param  num   最初の数
 * @param  loops 過去のループを記録するマップ
 * @return       ループが発生した時点での数値
 */
uInt find_loop_point(uInt num, std::unordered_map<uInt, std::vector<uInt>>& loops)
{
  std::vector<uInt> memo;
  while (std::find(memo.begin(), memo.end(), num) == memo.end()) {
    auto iter = loops.find(num);
    if (iter != loops.end()) {
      // 過去に発見済みのループに突入
      for (uInt i : iter->second) {
        if (std::find(memo.begin(), memo.end(), i) != memo.end()) {
          // 今回の連鎖中に発見した値を既存ループ中に見つけたらそこで終了
          memo.erase(memo.begin(), std::find(memo.begin(), memo.end(), i));
          loops.insert(std::make_pair(i, std::move(memo)));
          return i;
        }
        memo.push_back(i);
      }
      loops.insert(std::make_pair(num, iter->second));
      return num;
    }
    else {
      // メモに値を記録しながら連鎖を続ける
      memo.push_back(num);
      num = digits_pow_sum(num);
    }
  }
  memo.erase(memo.begin(), std::find(memo.begin(), memo.end(), num));
  loops.insert(std::make_pair(num, std::move(memo)));
  return num;
}

int main(void)
{
  std::unordered_map<uInt, std::vector<uInt>> loops;
  uInt count = 0;
  for (uInt i = 1; i < 10000000; i++) {
    uInt lp = find_loop_point(i, loops);
    // 「89に到達する」==「ループ部分に89を含む」
    auto iter = loops.at(lp);
    if (std::find(iter.begin(), iter.end(), 89) != iter.end()) {
      count++;
    }
  }
  std::cout << "Euler092: " << count << std::endl;
  return 0;
}
