/**
 * Problem 81 「経路の和:2方向」
 * 下記の5次の正方行列で, 左上のセルから開始し右下のセルで終わるパスを探索する. ただし下方向と右方向にのみ移動できるものとする.
 * 通過したセルの和が最小となるパスは赤の太字で示されたもので, その値は2427である.
 *
 * 131 673 234 103 18
 * 201 96  342 965 150
 * 630 803 746 422 111
 * 537 699 497 121 956
 * 805 732 524 37  331
 * 今, 31Kのテキストファイルmatrix.txt (右クリックして, 『名前をつけてリンク先を保存』)には80×80の行列が書かれている.
 * 同様に左上のセルから開始し右下のセルで終わり, かつ右方向と下方向にのみ移動するときの最小のパスの和を求めよ.
 */

// e018の応用で解ける

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
  std::vector<uInt> memo(WIDTH * HEIGHT, 0);
  memo.at(0) = *mat.begin()->begin();
  // (0,0)から真横・真下のセルの最小値はそのまま足していけば良い
  for (uInt x = 1; x < WIDTH; x++) {
    memo.at(x) = memo.at(x - 1) + mat.at(0).at(x);
  }
  for (uInt y = 1; y < HEIGHT; y++) {
    memo.at(y * WIDTH) = memo.at((y - 1) * WIDTH) + mat.at(y).at(0);
  }
  // 他のセルは,そのセルの一つ前の状態2候補から小さい方を選んで計算
  for (uInt y = 1; y < HEIGHT; y++) {
    for (uInt x = 1; x < WIDTH; x++) {
      const uInt top = memo.at(x + (y - 1) * WIDTH);
      const uInt left = memo.at(x - 1 + y * WIDTH);
      memo.at(x + y * WIDTH) = std::min(top, left) + mat.at(y).at(x);
    }
  }
  return *memo.rbegin();
}

int main(void)
{
  try {
    const auto mat = load_data_file("euler081.txt");
    std::cout << "Euler081: " << trace_min_path(mat) << std::endl;
  }
  catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}
