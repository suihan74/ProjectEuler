
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "prime.h"
#include "totient.h"

using uInt = std::uint_fast32_t;
using namespace Euler::Prime;
using namespace Euler::Totient;

int main(void)
{
  constexpr uInt MAX = 10000000;
  double min = std::numeric_limits<double>::max();
  uInt min_n = 0;

  // 素数の場合真面目に計算するとトーシェント関数で時間がかかるので
  // 予め素数テーブルを作成しておき，時間短縮する
  std::vector<NumberType> nt_vec;
  make_primes_vector(MAX, &nt_vec);

  for (uInt n = 2; n < MAX; n++) {
    const uInt phi = totient(n, nt_vec);
    const auto n_div_phi = double(n) / phi;
    if (min <= n_div_phi) { continue; }

    // φ(n)がnの各桁を置換したものかを確認
    // 簡単に，文字列化してソートして一致するか見る
    auto n_str = std::to_string(n);
    auto phi_str = std::to_string(phi);
    std::sort(n_str.begin(), n_str.end());
    std::sort(phi_str.begin(), phi_str.end());

    if (n_str == phi_str) {
      min = n_div_phi;
      min_n = n;
    }
  }
  std::cout << "Euler070: " << min_n << std::endl;
  return 0;
}
