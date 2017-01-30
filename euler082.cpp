/**
 * Problem 82 「経路の和:3方向」
 * 注: この問題はProblem 81よりも挑戦しがいがあるだろう.
 *
 * 下記の5次の正方行列で, 一番左の行の任意のセルから開始し一番右の行の任意のセルで終わる道を探索する. ただし上下右にのみ移動できるものとする. 一番小さなパスは下で赤の太字で示されたものである. このときの合計は994になる.
 *
 * 131 673 234 103 18
 * 201 96  342 965 150
 * 630 803 746 422 111
 * 537 699 497 121 956
 * 805 732 524 37  331
 * 今, 31Kのテキストファイルmatrix.txtには80×80の行列が書かれている. 一番左の行から一番右の行へ移動する際の一番小さなパスの和を求めよ.
 */

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

using uInt = std::uint_fast32_t;
using Matrix = std::vector<std::vector<uInt>>;

Matrix load_data_file(const char* filename)
{
  Matrix result;
  std::ifstream ifs(filename);
  if (ifs.fail()) {
    throw std::invalid_argument("cannot open a datafile.");
  }

  try {
    std::string line_buffer, num_buffer;
    while (!ifs.eof()) {
      std::getline(ifs, line_buffer);
      std::istringstream line_stream(line_buffer);
      result.resize(result.size() + 1);
      while (!line_stream.eof()) {
        std::getline(line_stream, num_buffer, ',');
        result.rbegin()->push_back(std::stoul(num_buffer));
      }
    }
  }
  catch (std::invalid_argument& e) {
    throw std::runtime_error("the datafile contains illegal data. (not a number)");
  }
  catch (std::out_of_range& e) {
    throw std::runtime_error("the datafile contains illegal data. (out of range)");
  }

  return result;
}

uInt trace_min_path(const Matrix& mat)
{
  const uInt WIDTH = mat.begin()->size();
  const uInt HEIGHT = mat.size();
  // あるセルの最小値のメモ
  std::vector<std::vector<uInt>> memo(WIDTH);  // .at(x).at(y)で各セルにアクセス可能なように格納
  // 左端のセルは全てスタート地点になりうるので,そのまま代入
  for (uInt y = 0; y < HEIGHT; y++) {
    memo.at(0).push_back(mat.at(y).at(0));
  }
  // 他のセルは,左隣の行にあるセルが全てわかっているものとして最小値を探す
  std::vector<uInt> costs; costs.reserve(HEIGHT);
  for (uInt x = 1; x < WIDTH; x++) {
    for (uInt y = 0; y < HEIGHT; y++) {
      costs.clear();
      // ひとつ左隣の行にある全てのセルから現在のセルまでの移動コストを列挙する
      for (uInt p_y = 0; p_y < HEIGHT; p_y++) {
        uInt cost = memo.at(x - 1).at(p_y);
        if (y < p_y) {
          for (uInt i = y; i <= p_y; i++) { cost += mat.at(i).at(x); }
        }
        else {
          for (uInt i = p_y; i <= y; i++) { cost += mat.at(i).at(x); }
        }
        costs.push_back(cost);
      }
      // コストが最も小さいものを採用
      memo.at(x).push_back(*std::min_element(costs.begin(), costs.end()));
    }
  }
  // 右端行内全てのセルがゴール候補であることに注意
  return *std::min_element(memo.rbegin()->begin(), memo.rbegin()->end());
}

int main(void)
{
  try {
    const auto mat = load_data_file("euler082.txt");
    std::cout << "Euler082: " << trace_min_path(mat) << std::endl;
  }
  catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}
