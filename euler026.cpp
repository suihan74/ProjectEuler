/**
 * Problem 26 「逆数の循環節 その1」
 * 単位分数とは分子が1の分数である. 分母が2から10の単位分数を10進数で表記すると次のようになる.
 *
 * 1/2 = 0.5
 * 1/3 = 0.(3)
 * 1/4 = 0.25
 * 1/5 = 0.2
 * 1/6 = 0.1(6)
 * 1/7 = 0.(142857)
 * 1/8 = 0.125
 * 1/9 = 0.(1)
 * 1/10 = 0.1
 *
 * 0.1(6)は 0.166666... という数字であり, 1桁の循環節を持つ. 1/7 の循環節は6桁ある.
 *
 * d < 1000 なる 1/d の中で小数部の循環節が最も長くなるような d を求めよ.
 */

#include <algorithm>
#include <stdexcept>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

using uInt = std::uint_fast32_t;

/**
 * ある自然数の逆数 1/d の小数部の循環節の長さを計算
 * @param  d  1/d なる対象の数
 * @return    循環節の長さ（非循環小数の場合0）
 */
std::size_t length_recurring_cycle(uInt d)
{
  if (d == 0) { throw std::logic_error("division by zero"); }
  std::vector<uInt> num_history;
  uInt numerator = 10;
  for (;;) {
    while (numerator < d) { numerator *= 10; }

    // 以前と同じ割られる数が出てきたらそこまでが循環節
    auto repeat_point = std::find(num_history.cbegin(), num_history.cend(), numerator);
    if (repeat_point != num_history.end()) {
      return num_history.end() - repeat_point;
    }
    // 割られる数を記録しながら割り算を進める
    num_history.push_back(numerator);
    if (numerator % d == 0) { break; }
    numerator -= numerator / d * d;
  }
  return 0;
}


int main(void)
{
  uInt max = 0, max_d = 0;
  for (uInt d = 1; d < 1000; d++) {
    uInt len = length_recurring_cycle(d);
    if(len > max) {
      max = len;
      max_d = d;
    }
  }
  std::cout << max_d << std::endl;
  return 0;
}
