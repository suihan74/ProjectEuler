/**
 * Problem 40 「チャンパーノウン定数」
 * 正の整数を順に連結して得られる以下の10進の無理数を考える:
 *
 * 0.123456789101112131415161718192021...
 * 小数第12位は1である.
 *
 * dnで小数第n位の数を表す.
 * d1 × d10 × d100 × d1000 × d10000 × d100000 を求めよ.
 */

#include <cmath>
#include <cstdint>
#include <iostream>

using uInt = std::uint_fast32_t;

/**
 * d桁の数すべてを連結した場合の長さを計算する
 * ただし，今回0を考えるとややこしくなるため1桁の数に0は含めない
 * ex) count_digits(1) = 9;  count_digits(2) = 180 (= 2桁 * 90個);
 * @param d  対象の桁数
 * @return   d桁の数すべてを連結した場合の長さ
 */
inline
uInt digits_length(uInt d)
{
  return d > 0 ? ((std::pow(10, d) - std::pow(10, d - 1)) * d) : 0;
}

/**
 * チャンパーノウン定数の小数第n位の値を取得する
 * @param  n チャンパーノウン定数から取り出す小数点以下の任意の位置
 * @return   小数第n位にある値
 */
uInt champer_digit_at(uInt n)
{
  if (n < 10) { return n; }
  // 第n位は何桁の整数に含まれるかを調査
  uInt d, i;
  for (i = 1, d = 0; ; i++) {
    const uInt dcl = digits_length(i);
    if (d + dcl > n) { break; }
    d += dcl;
  }
  // d桁の整数のうち，第n位を含むもの
  const uInt target = std::pow(10, i - 1) + (n - d - 1) / i;
  // 整数targetのどの位(※「10の位」とかの「位」)が目的の位置nであるか
  const uInt relative_pos = std::log10(target) - (n - d - 1) % i;
  return (target / (uInt)std::pow(10, relative_pos)) % 10;
}

int main(void)
{
  uInt p = 1;
  for (uInt i = 1; i <= 100000; i *= 10) {
    p *= champer_digit_at(i);
  }
  std::cout << "Euler040: " << p << std::endl;
  return 0;
}
