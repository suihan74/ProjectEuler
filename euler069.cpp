
#include <algorithm>
#include <cstdint>
#include <iostream>

#include "prime.h"
#include "totient.h"

using uInt = std::uint_fast32_t;
using namespace Euler::Prime;
using namespace Euler::Totient;

int main(void)
{
  constexpr uInt MAX = 1000000;
  std::vector<NumberType> nt_vec;
  make_primes_vector(MAX, &nt_vec);

  double best = 0.0;
  uInt best_n = 0;
  for (uInt n = 2; n <= MAX; n += 2) {
    const auto x = double(n) / totient(n, nt_vec);
    if (x > best) {
      best = x;
      best_n = n;
    }
  }
  std::cout << "Euler069: " << best_n << std::endl;
  return 0;
}
