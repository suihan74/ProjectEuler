/**
 * Problem 32 「パンデジタル積」
 * すべての桁に 1 から n が一度だけ使われている数をn桁の数がパンデジタル (pandigital) であるということにしよう: 例えば5桁の数 15234 は1から5のパンデジタルである.
 *
 * 7254 は面白い性質を持っている. 39 × 186 = 7254 と書け, 掛けられる数, 掛ける数, 積が1から9のパンデジタルとなる.
 *
 * 掛けられる数/掛ける数/積が1から9のパンデジタルとなるような積の総和を求めよ.
 *
 * HINT: いくつかの積は, 1通り以上の掛けられる数/掛ける数/積の組み合わせを持つが1回だけ数え上げよ.
 */

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <vector>
#include "pandigital.h"

using uInt = std::uint_fast32_t;
using namespace Euler::Pandigital;

/**
 * ベクタに重複する値が無いことを確認
 * そのまま書くと長くなって嫌だったのでラップしただけ
 * @param vec    対象のベクタ
 * @param value  確認したい値
 * @return  vecに同じ値が無いときtrue
 */
inline
bool nfind(const std::vector<uInt>& vec, uInt value)
{
  return std::find(vec.cbegin(), vec.cend(), value) ==  vec.cend();
}

int main(void)
{
  std::vector<uInt> pandigitals;

  // 4987 * 2 = 9974 ... a,bは計5桁，cは4桁になるものだけ考える（これ以外の組では成立しない）
  // a,bの重複部分を省く．aは1or2桁，bは3or4桁で考える
  for (uInt a = 1; a <= 98; a++) {
    const uInt ba = used_num_bits(a);
    if (!ba) { continue; }
    for (uInt b = 123; b <= 9876; b++) {
      const uInt bb = used_num_bits(b);
      if (!bb || is_overlap(ba, bb)) { continue; }

      const uInt c = a * b;
      const uInt bc = used_num_bits(c);
      if (!bc || is_overlap(ba, bc) || is_overlap(bb, bc)) { continue; }

      if (pandigital_test(ba | bb | bc) && nfind(pandigitals, c)) {
        pandigitals.push_back(c);
      }
    }
  }
  std::cout << "Euler032: "
            << std::accumulate(pandigitals.begin(), pandigitals.end(), (uInt)0)
            << std::endl;
  return 0;
}
