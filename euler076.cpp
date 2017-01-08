/**
 * Problem 76 「和の数え上げ」
 * 5は数の和として6通りに書くことができる:
 *
 * 4 + 1
 * 3 + 2
 * 3 + 1 + 1
 * 2 + 2 + 1
 * 2 + 1 + 1 + 1
 * 1 + 1 + 1 + 1 + 1
 *
 * 2つ以上の正整数の和としての100の表し方は何通りか.
 */

#include <array>
#include <cstdint>
#include <iostream>

using uInt = std::uint_fast64_t;

// 類題: Problem 31 「硬貨の和」
// -> e076では，硬貨が 1,2,...,99 で,これを足し合わせて100を作る問題と捉えることができる

/**
 * https://ja.wikipedia.org/wiki/分割数
 * k > n のとき: p(k, n) = 0
 * k = n のとき: p(k, n) = 1
 * それ以外: p(k, n) = p(k+1, n) + p(k, n − k)
 * p(n) = Σp(k,n); k=1~n
 * 求めるものはp(n)-1  (n+0のケースを含めない)
 */

int main(void)
{
  constexpr uInt TARGET = 100;
  std::array<uInt, TARGET + 1> memo = {1};
  for (uInt n = 1; n < TARGET; n++) {
    for (uInt i = n; i <= TARGET; i++) {
      memo.at(i) += memo.at(i - n);
    }
  }
  std::cout << "Euler076: " << memo.at(TARGET) << std::endl;
  return 0;
}

