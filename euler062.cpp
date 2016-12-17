/**
 * Problem 62 「立方数置換」
 * 立方数 41063625 (345^3) は, 桁の順番を入れ替えると2つの立方数になる: 56623104 (384^3) と 66430125 (405^3) である.
 * 41063625は, 立方数になるような桁の置換をちょうど3つもつ最小の立方数である.
 *
 * 立方数になるような桁の置換をちょうど5つもつ最小の立方数を求めよ.
 */

#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <string>

using uInt = std::uint_fast64_t; // 32bitだと溢れる

int main(void)
{
  // 予め上限を決めて，立方数とその各桁に含まれる数字と文字列化・ソートしたもののペアでできた配列を作っておく
  constexpr uInt MAX = 10000;
  std::array<std::pair<uInt, std::string>, MAX> arr;
  for (uInt i = 0; i < MAX; i++) {
    const uInt i3 = i * i * i;
    auto str = std::to_string(i3);
    std::sort(str.begin(), str.end());
    arr.at(i) = std::make_pair(i3, std::move(str));
  }

  for (uInt i = 0; i < MAX; i++) {
    uInt count = 1;   // 0ではなく1なのは，予め自分自身をカウントしているため
    for (uInt j = i + 1; j < MAX; j++) {
      if (arr[i].second == arr[j].second) {  // ソート後文字列が一致すれば置換で表せる
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
