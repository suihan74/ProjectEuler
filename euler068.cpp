/**
 * Problem 68 「Magic 5-gon ring」
 * 下に示す図のようなものを"magic" 3-gon ringという. これは1～6の数字を当てはめて, 各列の数字の和が9となっている. これを例として説明する.
 *
 * 外側のノードのうち一番小さいものの付いた列(例では4,3,2)から時計回りに回ってそれぞれ列の数字を3つ連ねて説明する. 例えば例のものは4,3,2; 6,2,1; 5,1,3という組で説明することができる.
 *
 * 1～6の数字を当てはめて, 各列の数字の和が等しくなるものは次の8通りある.
 *
 * 合計  組
 * 9 4,2,3; 5,3,1; 6,1,2
 * 9 4,3,2; 6,2,1; 5,1,3
 * 10  2,3,5; 4,5,1; 6,1,3
 * 10  2,5,3; 6,3,1; 4,1,5
 * 11  1,4,6; 3,6,2; 5,2,4
 * 11  1,6,4; 5,4,2; 3,2,6
 * 12  1,5,6; 2,6,4; 3,4,5
 * 12  1,6,5; 3,5,4; 2,4,6
 * この組の各数字を連結して, 9桁の数字で表すことができる. 例えば, 上の図のものは4,3,2; 6,2,1; 5,1,3であるので432621513である.
 *
 * さて, 下の図に1～10の数字を当てはめ, 各列の数字の和が等しくなる"magic" 5-gon ringを作って, それを表す16桁または17桁の数字のうち, 16桁のものの最大の数字を答えよ.
 *
 * (注, 3つの場合の例を見ても分かる通り, 列の始まりの数字を比べた時一番小さい数字で始まる列から時計回りに繋げるという条件のもとで文字列を生成する必要があります. この条件下で最大となる数字を答えてください. )
 */

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

using uInt = std::uint_fast64_t;

// 3-gon
// ade
// bef
// cfd
// ==> (数列)=adebefcfd;  a+d+e == b+e+f == c+f+d

// 5-gon
// afg
// bgh
// chi
// dij
// ejf
// ==> (数列)=afgbghchidijejf;  a+f+g == b+g+h == c+h+i == d+i+j == e+j+f

// 説明のため，リングの各ノードを以上のように名付ける
// 外側のノードをa,b,...(N個)とし，プログラム中ではベクタの先頭N個に配置する
// その内aが必ず最小値になるようにする
//
// 残りのノードは，aに繋がっているものから順番に名付ける
// ベクタ中の位置もそのアルファベット順に対応する

/**
 * 題意に沿うmagic n-gon ringの場合その各ノードを連結した数値を返す
 * @param  vec リングを生成するためのベクタ
 * @param  N   何角形か
 * @return     題意に沿うリングの場合ノードを連結した数値，そうでない場合0
 */
uInt make_gon_string_num(std::vector<uInt>& vec, const uInt N)
{
  if (vec.size() < N * 2) { return 0; }
  // 和が一致するか確認しながら数列を生成
  uInt result = 0;
  uInt sum_prev = 0;
  for (uInt i = 0; i < N; i++) {
    const uInt a = vec.at(i);
    const uInt b = vec.at(N + i);
    const uInt c = vec.at(N + ((i + 1) % N));
    const uInt sum = a + b + c;
    if (i == 0) {
      sum_prev = sum;
    }
    else if (sum_prev != sum) {
      return 0;
    }
    // 3つずつの部分数列を作って，結果を生成
    uInt x = c;
    x += std::pow(10, uInt(std::log10(x) + 1)) * b;
    x += std::pow(10, uInt(std::log10(x) + 1)) * a;
    result = result * std::pow(10, uInt(std::log10(x) + 1)) + x;
  }
  // 問題文には，（求める5-gonのとき）16桁と指定があるのでここで絞っておく
  return (N != 5 || uInt(std::log10(result) + 1) == 16) ? result : 0;
}

/**
 * 1~N*2の数字からなる数列を総当りで生成しながら題意に沿うものの中から最大値を返す
 * @param  vec       数列生成用のベクタ
 * @param  N         magic N-gon ringの角数N
 * @param  depth     現在の探索の深さ
 * @param  used_nums 数列中で既に使用している数字を表すビット列
 * @return           magic N-gon ringから生成できる数列の最大値
 */
uInt best_n_gon_ring_recursive(
  std::vector<uInt>& vec,
  const uInt N,
  const uInt depth = 0,
  const uInt used_nums = 0)
{
  const uInt DEPTH_BOUND = N * 2;
  if (depth == 0) {
    vec.clear();
    vec.resize(DEPTH_BOUND);
  }

  uInt best = 0;
  for (uInt i = 1; i <= DEPTH_BOUND; i++) {
    uInt b = (1 << i);
    if ((used_nums & b) || (depth > 0 && depth < N && vec.at(0) > i)) {
      continue;
    }
    vec.at(depth) = i;
    if (depth + 1 == DEPTH_BOUND) {
      return make_gon_string_num(vec, N);
    }
    else {
      uInt res = best_n_gon_ring_recursive(vec, N, depth + 1, (used_nums | b));
      if (res > best) {
        best = res;
      }
    }
  }
  return best;
}

int main()
{
  std::vector<uInt> vec;
  uInt best = best_n_gon_ring_recursive(vec, 5);
  std::cout << "Euler068: " << best << std::endl;
  return 0;
}
