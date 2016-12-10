/**
 * Problem 16 「各位の数字の和」
 * 215 = 32768 であり, 各位の数字の和は 3 + 2 + 7 + 6 + 8 = 26 となる.
 *
 * 同様にして, 21000 の各位の数字の和を求めよ.
 *
 * 注: Problem 20 も各位の数字の和に関する問題です。解いていない方は解いてみてください。
 */

#include <iostream>
#include <numeric>
#include <vector>


int main(void)
{
  // euler13同様の考え方でpow(2,N)を表現するベクタを作成
  std::vector<int> bits {2};
  for (std::size_t i = 2; i <= 1000; i++) {
    std::size_t size = bits.size();
    for (std::size_t j = 0; j < size; j++) {
      bits.at(j) *= 2;
      // 繰り上げ処理
      for (std::size_t k = j; bits.at(k) > 9; k++) {
        bits.at(k) -= 10;
        if (k < size - 1) {
          if ((bits.at(k + 1) = bits.at(k + 1) * 2 + 1) < 10) {
            j = k + 1;
            break;
          }
        }
        else {
          // 最上桁の繰り上げ
          bits.push_back(1);
          j = k;
          break;
        }
      }
    }
  }

  std::cout << "2^1000 = ";
  for (auto x = bits.rbegin(); x < bits.rend(); x++) {
    std::cout << *x;
  }
  std::cout << std::endl;

  std::cout << "Euler016: " << std::accumulate(bits.begin(), bits.end(), 0) << std::endl;
  return 0;
}
