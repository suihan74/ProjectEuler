/**
 * Problem 2 「偶数のフィボナッチ数」
 * フィボナッチ数列の項は前の2つの項の和である. 最初の2項を 1, 2 とすれば, 最初の10項は以下の通りである.
 * 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, ...
 * 数列の項の値が400万以下の, 偶数値の項の総和を求めよ.
 */

#include <cstdint>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <vector>

using uInt = std::uint_fast32_t;
constexpr uInt UPPER_BOUND = 4000000;

uInt pushNextFibonacci(std::vector<uInt> *vec)
{
  if (vec->size() < 2) {
    throw std::invalid_argument("the fibonacci vector does not have two initial numbers");
  }

  uInt next = (*(vec->end() - 1)) + (*(vec->end() - 2));
  if (next < UPPER_BOUND) {
    vec->push_back(next);
  }
  return next;
}

int main(void)
{
  try {
    std::vector<uInt> fib { 1, 2 };
    while (pushNextFibonacci(&fib) < UPPER_BOUND);
    uInt sum = std::accumulate(fib.begin(), fib.end(), static_cast<uInt>(0),
      [](uInt init, uInt i) {
        return (i % 2 ? init : init + i);
      });

    std::cout << "Euler002: " << sum << std::endl;
  }
  catch (const std::invalid_argument &e) {
    std::cout << e.what() << std::endl;
    return 1;
  }

  return 0;
}
