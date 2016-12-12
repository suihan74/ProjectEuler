/**
 * Problem 62 「立方数置換」
 * 立方数 41063625 (345^3) は, 桁の順番を入れ替えると2つの立方数になる: 56623104 (384^3) と 66430125 (405^3) である.
 * 41063625は, 立方数になるような桁の置換をちょうど3つもつ最小の立方数である.
 *
 * 立方数になるような桁の置換をちょうど5つもつ最小の立方数を求めよ.
 */

#include <array>
#include <cstdint>
#include <iostream>

using uInt = std::uint_fast64_t;           // 32bitだと溢れる
using DigitsArray = std::array<uInt, 10>;  // 0~9だけ数えればいいので配列サイズはたかだか10個

/**
 * 正の整数numの各桁に現れる数字それぞれの個数を連想配列に記録して返す
 * @param  num 対象の数値
 * @return     numの各桁に含まれる数字がそれぞれ何個ずつ含まれているかを記した配列
 */
DigitsArray count_digits_num(uInt num)
{
  DigitsArray arr;
  arr.fill(0);
  do {
    const uInt d = num % 10;
    num /= 10;
    arr[d]++;
  } while (num);
  return arr;
}

/**
 * 二つの数値に対応する使用数字配列が一致するかを確認する
 * @param arr_a  ある数num_aの桁に含まれる数字の個数を記録した配列
 * @param arr_b  ある数num_bの桁に含まれる数字の個数を記録した配列
 */
inline
bool equals_digits_num(const DigitsArray& arr_a, const DigitsArray& arr_b)
{
  constexpr auto SIZE = arr_a.size();
  for (uInt i = 0; i < SIZE; i++) {
    if (arr_a[i] != arr_b[i]) { return false; }
  }
  return true;
}

int main(void)
{
  // 予め上限を決めて，立方数とその各桁に含まれる数字の個数記録のペアでできた配列を作っておく
  constexpr uInt MAX = 10000;
  std::array<std::pair<uInt, DigitsArray>, MAX> arr;
  for (uInt i = 0; i < MAX; i++) {
    const uInt i3 = i * i * i;
    arr.at(i) = std::make_pair(i3, count_digits_num(i3));
  }

  for (uInt i = 0; i < MAX; i++) {
    uInt count = 1;   // 0ではなく1なのは，予め自分自身をカウントしているため
    for (uInt j = i + 1; j < MAX; j++) {
      if (equals_digits_num(arr[i].second, arr[j].second)) {
        count++;
      }
    }
    if (count == 5) {
      std::cout << "Euler062: " << arr[i].first << std::endl;
      break;
    }
  }

  return 0;
}
