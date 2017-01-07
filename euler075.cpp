/**
 * Problem 75 「1通りの整数直角三角形」
 * ある長さの鉄線を折り曲げて3辺の長さが整数の直角三角形を作るとき, その方法が1通りしかないような最短の鉄線の長さは12cmである. 他にも沢山の例が挙げられる.
 *
 * 12 cm: (3,4,5)
 * 24 cm: (6,8,10)
 * 30 cm: (5,12,13)
 * 36 cm: (9,12,15)
 * 40 cm: (8,15,17)
 * 48 cm: (12,16,20)
 *
 * それとは対照的に, ある長さの鉄線 (例えば20cm) は3辺の長さが整数の直角三角形に折り曲げることができない. また2つ以上の折り曲げ方があるものもある. 2つ以上ある例としては, 120cmの長さの鉄線を用いた場合で, 3通りの折り曲げ方がある.
 *
 * 120 cm: (30,40,50), (20,48,52), (24,45,51)
 *
 * Lを鉄線の長さとする. 直角三角形を作るときに1通りの折り曲げ方しか存在しないような L ≤ 1,500,000 の総数を答えよ.
 *
 * 注: この問題は最近変更されました. あなたが正しいパラメータを使っているか確認してください.
 */

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

using uInt = std::uint_fast32_t;

/*
m > n >= 1 のとき，三辺が整数になる直角三角形の各辺
・m^2 - n^2
・2mn
・m^2 + n^2

さらに以下の条件に絞ることで，m,nの組から作られる三角形の相似重複を排除できる
・m と n について、m＞n≧1が成り立つ奇数であること。
・nが1より大きい場合は、mとnの両者に共通する素因数がないこと。（互いに素であること。1（または-1）以外の公約数がないこと。）
このとき，三辺は
・(m^2 - n^2) / 2
・mn
・(m^2 + n^2) / 2

L = m * m + m * n = m * (m + n)
Lmin = m * (m + 1)
*/

/**
 * aとbの最大公約数をユークリッドの互除法で計算する
 * @param a  自然数a
 * @param b  自然数b
 * @return   aとbの最大公約数
 */
template<typename uInt_t>
inline
uInt_t gcd(uInt_t a, uInt_t b)
{
  while (b) {
    auto tmp = a % b;
    a = b;
    b = tmp;
  }
  return a;
}

int main(void)
{
  constexpr uInt BOUND = 1500000;
  std::vector<uInt> counts_vec(BOUND + 1, 0);
  for (uInt m = 3; m * (m + 1) <= BOUND; m += 2) {
    for (uInt n = 1; n < m; n += 2) {
      if (n > 1 && gcd(m, n) != 1) { continue; }
      const uInt a = (m * m - n * n) / 2;
      const uInt b = m * n;
      const uInt c = (m * m + n * n) / 2;
      const uInt L = a + b + c;
      if (L > BOUND) { continue; }
      // 相似なものを全て探す
      for (uInt i = 1; L * i <= BOUND; i++) {
        counts_vec.at(L * i)++;
      }
    }
  }
  const uInt result = std::count(counts_vec.begin(), counts_vec.end(), 1);
  std::cout << "Euler075: " << result << std::endl;
  return 0;
}
