/**
 * Problem 18 「最大経路の和 その1」
 * 以下の三角形の頂点から下まで移動するとき, その数値の和の最大値は23になる.
 *
 * 3
 * 7 4
 * 2 4 6
 * 8 5 9 3
 * この例では 3 + 7 + 4 + 9 = 23.
 *
 * 以下の三角形を頂点から下まで移動するとき, その最大の和を求めよ.
 *
 * 75
 * 95 64
 * 17 47 82
 * 18 35 87 10
 * 20 04 82 47 65
 * 19 01 23 75 03 34
 * 88 02 77 73 07 63 67
 * 99 65 04 28 06 16 70 92
 * 41 41 26 56 83 40 80 70 33
 * 41 48 72 33 47 32 37 16 94 29
 * 53 71 44 65 25 43 91 52 97 51 14
 * 70 11 33 28 77 73 17 78 39 68 17 57
 * 91 71 52 38 17 14 91 43 58 50 27 29 48
 * 63 66 04 68 89 53 67 30 73 16 69 87 40 31
 * 04 62 98 27 23 09 70 98 73 93 38 53 60 04 23
 * 注: ここではたかだか 16384 通りのルートしかないので, すべてのパターンを試すこともできる. Problem 67 は同じ問題だが100行あるので, 総当りでは解けない. もっと賢い方法が必要である.
 */

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

using uInt = std::uint_fast64_t;

std::vector<std::vector<uInt>> load_triangle(const char *filename)
{
  std::vector<std::vector<uInt>> triangle;
  std::string line_buffer, num_buffer;
  std::ifstream ifs(filename);
  try {
    while (!ifs.eof()) {
      std::getline(ifs, line_buffer);
      if (line_buffer.size() == 0) { continue; }
      std::istringstream iss(line_buffer);
      triangle.resize(triangle.size() + 1);
      triangle.rbegin()->reserve(triangle.size());
      while (iss >> num_buffer) {
        uInt num = std::stoul(num_buffer);
        triangle.rbegin()->push_back(num);
      }
    }
  }
  catch (std::invalid_argument &e) {
    std::cout << "triangle file contains not a number..." << std::endl;
    std::exit(1);
  }
  return triangle;
}


int main(int argc, const char *argv[])
{
  const auto tri = load_triangle(argc > 1 ? argv[1] : "euler018.txt");
  std::vector<uInt> prev_row;
  prev_row.push_back(tri[0][0]);

  const std::size_t row_size = tri.size();
  for (std::size_t r = 1; r < row_size; r++) {
    std::vector<uInt> next_row;
    next_row.reserve(r + 1);
    auto &row = tri.at(r);
    const std::size_t col_size = r + 1;
    for (std::size_t c = 0; c < col_size; c++) {
      const uInt a = c == 0 ? 0 : prev_row.at(c - 1);
      const uInt b = c + 1 == col_size ? 0 : prev_row.at(c);
      next_row.push_back(row.at(c) + std::max(a, b));
    }

    if (r + 1 == row_size) {
      std::cout << "Euler018: " << *std::max_element(next_row.begin(), next_row.end()) << std::endl;
    }
    else {
      prev_row = std::move(next_row);
    }
  }

  return 0;
}
