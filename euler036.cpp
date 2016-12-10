/**
 * Problem 36 「二種類の基数による回文数」
 * 585 = 10010010012 (2進) は10進でも2進でも回文数である.
 *
 * 100万未満で10進でも2進でも回文数になるような数の総和を求めよ.
 *
 * (注: 先頭に0を含めて回文にすることは許されない.)
 */

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>

using uInt = std::uint_fast32_t;

/**
 * 与えられた数値の2進数表現を"反転した"文字列を返す
 * （用途的にどうせ後でひっくり返すので，それなら最初からひっくり返ったものを返せばいいやと）
 * @param  n 対象の数値
 * @return   頭と尻がひっくり返った2進表現の文字列
 */
std::string to_binary_rstring(uInt n)
{
  if (n == 0) { return "0"; }
  std::ostringstream oss;
  while (n) {
    oss << n % 2;
    n /= 2;
  }
  return oss.str();
}

int main(void)
{
  uInt sum = 0;
  for (uInt i = 1; i < 1000000; i++) {
    auto str_base10 = std::to_string(i);
    std::reverse(str_base10.begin(), str_base10.end());
    if (i != std::stoul(str_base10, nullptr, 10)) { continue; }

    auto str_base2 = to_binary_rstring(i);
    if (i != std::stoul(str_base2, nullptr, 2)) { continue; }
    sum += i;
  }
  std::cout << "Euler036: " << sum << std::endl;
  return 0;
}
