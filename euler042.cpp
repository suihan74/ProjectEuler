/**
 * Problem 42 「符号化三角数」
 * 三角数のn項は tn = ½n(n+1)で与えられる. 最初の10項は
 *
 * 1, 3, 6, 10, 15, 21, 28, 36, 45, 55, ...
 * である.
 *
 * 単語中のアルファベットを数値に変換した後に和をとる. この和を「単語の値」と呼ぶことにする. 例えば SKY は 19 + 11 + 25 = 55 = t10である. 単語の値が三角数であるとき, その単語を三角語と呼ぶ.
 *
 * 16Kのテキストファイル words.txt 中に約2000語の英単語が記されている. 三角語はいくつあるか?
 */

#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <string>

using uInt = std::uint_fast64_t;
using Real = double;

constexpr auto REAL_EPS = std::numeric_limits<Real>::epsilon();

/**
 * 浮動小数の等価比較
 */
inline
bool real_equals(Real a, Real b)
{
  return std::abs(a - b) < REAL_EPS;
}

/**
 * 単語を構成するアルファベットのスコアを計算
 * @param  name 名前
 * @return      スコア（アルファベットを数値化して和をとる）
 */
inline
uInt word_score(const std::string& name)
{
  return std::accumulate(name.cbegin(), name.cend(), uInt(0), [](uInt init, char c)
    {
      return (c >='A' && c <= 'Z') ? init + c - 'A' + 1 : init;
    });
}

/**
 * 与えられた数が三角数かを確認
 * @param  num 対象の値
 * @return     三角数ならtrue
 */
inline
bool is_triangle_number(const uInt num)
{
  // 三角数
  // n * (n + 1) / 2 = Tn
  // n ^ 2 + n - 2 * Tn = 0
  // n = (-1 + sqrt(1 + 8 * Tn)) / 2
  const Real nf = (std::sqrt(1 + 8 * num) - 1) / 2;
  return real_equals(nf, Real(uInt(nf)));
}

int main(void)
{
  uInt triangle_count = 0;
  std::ifstream ifs("euler042.txt");
  if (ifs.fail()) {
    std::cout << "cannot open a datafile" << std::endl;
    return 1;
  }

  std::string word_buffer;
  while (std::getline(ifs, word_buffer, ',')) {
    if (is_triangle_number(word_score(word_buffer))) {
      triangle_count++;
    }
  }

  std::cout << "Euler042: " << triangle_count << std::endl;
  return 0;
}
