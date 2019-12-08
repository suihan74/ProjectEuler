/**
 * Problem 97 「大きな非メルセンヌ素数」
 * 100万桁を超える初めての素数は1999年に発見された. これはメルセンヌ素数であり, 2^6972593-1 である. 実際, 2,098,960桁ある.
 * それ以降も, より多くの桁になるメルセンヌ素数 (2p-1の形の数) が他にも発見されている.
 *
 * しかし, 2004年に, 非常に大きな非メルセンヌ素数が発見された. これは2,357,207桁の数であり, 28433*2^7830457+1である.
 *
 * この素数の末尾10桁を答えよ.
 */

#include <cstdint>
#include <cmath>
#include <iostream>

using uInt = std::uint_fast64_t;

int main()
{
    // 備考: C++14以降なら以下の処理すべてconstexpr化できるのでは
    constexpr uInt EXP = 7830457;
    constexpr uInt DIGITS_LIMIT = std::pow(10, 10);  // 最後の10桁より大きい部分は常に捨ててよい
    uInt result = 28433;
    for (uInt i = 0; i < EXP; i++)
    {
        result = (result * 2) % DIGITS_LIMIT;
    }
    result += 1;

    std::cout << result << std::endl;
    return 0;
}
