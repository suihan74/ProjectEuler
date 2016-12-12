/**
 * Problem 63 「べき乗の桁の個数」
 * 5桁の数 16807 = 75は自然数を5乗した数である. 同様に9桁の数 134217728 = 89も自然数を9乗した数である.
 *
 * 自然数を n 乗して得られる n 桁の正整数は何個あるか?
 */

#include <cmath>
#include <cstdint>
#include <iostream>

using uInt = std::uint_fast64_t;

// コメント中において，xの桁数をd(x)と表記する．また，log(x)は底が10の対数を表す

int main(void)
{
  uInt count = 0;
  for (uInt n = 1; ; n++) {
    // 初期値ibはd(i^n)==nとなる可能性がある最小の値
    // log(ib^n) + 1 == n
    // n * log(ib) + 1 == n
    // log(ib) == (n - 1) / n
    // ib == 10^((n - 1) / n)
    //
    // 全体の終了条件は，ibから始めてd(i^n)==nを見つけることなくd(i^n)>nになったとき
    // これ以降の試行では常にd(i^n)>nとなるためここで打ち切る
    // d(ib^n)<n && d(i(b+1)^n)>n みたいになることがあるみたいなのでこのようにする
    bool finish = true;
    for (uInt i = std::pow(10, (n - 1.0) / n); ; i++) {
      const uInt digits = n * std::log10(i) + 1;
      if (digits > n) {
        break;
      }
      if (digits == n) {
        count++;
        finish = false;
      }
    }
    if (finish) { break; }
  }
  std::cout << "Euler063: " << count << std::endl;
  return 0;
}
