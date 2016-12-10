/**
 * Problem 22 「名前のスコア」
 * 5000個以上の名前が書かれている46Kのテキストファイル names.txt を用いる. まずアルファベット順にソートせよ.
 *
 * のち, 各名前についてアルファベットに値を割り振り, リスト中の出現順の数と掛け合わせることで, 名前のスコアを計算する.
 *
 * たとえば, リストがアルファベット順にソートされているとすると, COLINはリストの938番目にある. またCOLINは 3 + 15 + 12 + 9 + 14 = 53 という値を持つ. よってCOLINは 938 × 53 = 49714 というスコアを持つ.
 *
 * ファイル中の全名前のスコアの合計を求めよ.
 */

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

using uInt = std::uint_fast64_t;

/**
 * 名前を構成するアルファベットのスコアを計算
 * @param  name 名前
 * @return      スコア（アルファベットを数値化して和をとる）
 */
inline
uInt name_score(const std::string &name)
{
  return std::accumulate(name.cbegin(), name.cend(), uInt(0), [](uInt init, char c)
    {
      return init + c - 'A' + 1;
    });
}

int main(void)
{
  std::vector<std::string> names;
  names.reserve(5200);  // 実際にはファイル中に5163個の名前がある

  std::ifstream ifs("euler022.txt");
  if (ifs.fail()) {
    std::cout << "cannot open a datafile" << std::endl;
    return 1;
  }
  // データ読み込みとソート（記号"と改行文字が入る可能性があるが，単純に無視する）
  std::string name_buffer;
  while (std::getline(ifs, name_buffer, ',')) {
    for (std::size_t i = 0; i < name_buffer.length(); i++) {
      if (name_buffer.at(i) < 'A' || name_buffer.at(i) > 'Z') {
        name_buffer.erase(i--, 1);
      }
    }
    names.push_back(name_buffer);
  }
  std::sort(names.begin(), names.end());
  // スコアの計算
  uInt score = 0;
  const uInt size = names.size();
  for (uInt i = 0; i < size; i++) {
    score += (i + 1) * name_score(names.at(i));
  }

  std::cout << "Euler022: " << score << std::endl;
  return 0;
}
