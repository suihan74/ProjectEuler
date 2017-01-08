#ifndef __PRIME_H__
#define __PRIME_H__

#include <stdexcept>
#include <vector>

namespace Euler
{
namespace Prime
{
  // 合成数か素数か
  enum class NumberType
  {
    PRIME,
    COMPOSITE,
  };

  /**
   * 与えられた数numが素数であるかを確認
   * @param num  確認する数
   * @return     numが素数であるときtrue
   */
  template <typename T>
  bool is_prime(T num)
  {
    if (num < 4) { return num > 1; }
    if (num % 2 == 0 || num % 3 == 0) { return false; }
    for (T i = 5; i * i <= num; i += 6) {
      if (num % i == 0 || num % (i + 2) == 0) { return false; }
    }
    return true;
  }

  /**
   * MAX以下の素数リストを生成する
   * @param MAX  生成する素数の最大値
   * @return     2~MAXの間に存在する全ての素数をもつベクタ
   */
  template <typename Int_t>
  std::vector<Int_t> make_primes_vector_impl(const Int_t MAX, std::vector<NumberType>* tbl)
  {
    std::vector<Int_t> primes;
    tbl->resize(MAX + 1, NumberType::PRIME);
    tbl->at(0) = NumberType::COMPOSITE;
    tbl->at(1) = NumberType::COMPOSITE;
    if (MAX < 3) {
      if (MAX == 2) { primes.push_back(2); return primes; }
      throw std::out_of_range("the argument of make_primes_vector() is must be 2 or over");
    }
    // エラトステネスの篩やりながら素数リストを生成
    for (Int_t i = 2; i <= MAX; i++) {
      if (tbl->at(i) == NumberType::COMPOSITE) { continue; }
      primes.push_back(i);
      if (i * i > MAX) { continue; }
      for (Int_t j = i * 2; j <= MAX; j += i) {
        tbl->at(j)= NumberType::COMPOSITE;
      }
    }
    return primes;
  }

  template <typename Int_t>
  inline
  std::vector<Int_t> make_primes_vector(const Int_t MAX, std::vector<NumberType>* tbl = nullptr)
  {
    if (tbl == nullptr) {
      std::vector<NumberType> p_tbl;
      return make_primes_vector_impl(MAX, &p_tbl);
    }
    else {
      return make_primes_vector_impl(MAX, tbl);
    }
  }

  /**
   * num番目の素数を取得する
   * @param num 何番目の素数か
   * @return    num番目の素数
   */
  template <typename T>
  T get_prime_at(const T num)
  {
    if (num <= 0) { throw std::out_of_range("the target index must be greater than 0"); }
    std::vector<T> primes {2};
    primes.reserve(num);

    for (T i = 3; primes.size() < num; i++) {
      for (auto p : primes) {
        if (i % p == 0) {
          break;
        }
        else if (p * p > i) { // sqrt(i)未満の全素数pで割り切れなければiは素数
          primes.push_back(i);
          break;
        }
      }
    }
    return *primes.rbegin();
  }

}
}

#endif
