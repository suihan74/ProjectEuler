/**
 * Problem 31 「硬貨の和」
 * イギリスでは硬貨はポンド£とペンスpがあり，一般的に流通している硬貨は以下の8種類である.
 *
 * 1p, 2p, 5p, 10p, 20p, 50p, £1 (100p) and £2 (200p).
 *
 * 以下の方法で£2を作ることが可能である．
 *
 * 1×£1 + 1×50p + 2×20p + 1×5p + 1×2p + 3×1p
 *
 * これらの硬貨を使って£2を作る方法は何通りあるか?
 */

#include <array>
#include <cstdint>
#include <iostream>

using uInt = std::uint_fast32_t;

int main(void)
{
  constexpr uInt TARGET = 200;
  std::array<uInt, 8> coins = {1, 2, 5, 10, 20, 50, 100, 200};
  std::array<uInt, TARGET + 1> memo = {1};
  for (const uInt coin : coins) {
    for (uInt i = coin; i <= TARGET; i++) {
      memo.at(i) += memo.at(i - coin);
    }
  }
  std::cout << "Euler031: " << memo.at(TARGET) << std::endl;
  return 0;
}
