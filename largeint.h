#ifndef __LARGEINT_H__
#define __LARGEINT_H__

#include <cmath>
#include <initializer_list>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <type_traits>

namespace Euler
{
  template <typename Int_t>
  class LargeInt
  {
  private:
    /** 巨大な整数の内部表現 */
    std::vector<Int_t> num;
    /** 符号 */
    bool is_negative;
    /** 内部値が不正な場合failed == true */
    bool failed;

  public:
    /** 各要素の最大桁数(使用する整数型の最大値の桁数-1; 最大桁でやると計算途中で溢れる可能性がある) */
    static constexpr Int_t MAX_DIGITS = std::log10(std::numeric_limits<Int_t>::max());
    static constexpr Int_t LIMIT_PER_TERMS = std::pow(10, MAX_DIGITS - 1);

  public:  // コンストラクタ
    /**
     * 普通のInt_tから生成
     * @param init 初期値
     */
    LargeInt(Int_t init)
      : is_negative(init < 0),
        failed(false)
    {
      num.push_back(init < 0 ? -init : init);
    }

    /**
     * 値を表現する数値列から生成
     * ex) 12345 -> {1,2,3,4,5}
     * @param  vec 初期値を表す数値列
     */
    LargeInt(const std::initializer_list<Int_t>& vec)
      : is_negative(false),
        failed(false)
    {
      Int_t n = 0;
      const auto size = vec.size();
      using SizeType = typename std::remove_const<decltype(size)>::type;
      if (size > 0) {
        num.reserve(static_cast<SizeType>(std::log10(size) + 1) / (MAX_DIGITS - 1));
      }

      for (SizeType i = 0; i < size; i++) {
        if (*(vec.end() - i - 1) > 9) {
          failed = true;
          return;
        }
        if ((i > 0 && i + 1 < size && i % (MAX_DIGITS - 1) == 0)) {
          num.push_back(n);
          n = 0;
        }
        n += std::pow(10, i % (MAX_DIGITS - 1)) * (*(vec.end() - i - 1));
      }
      num.push_back(n);
    }

    /**
     * コピーコンストラクタ
     */
    LargeInt(const LargeInt<Int_t>& src)
    noexcept
    {
      *this = src;
    }

    /**
     * ムーブコンストラクタ
     */
    LargeInt(LargeInt<Int_t>&& src)
    noexcept
    {
      *this = std::move(src);
    }

  private:
    /**
     * 生成に失敗したオブジェクトを使っていないかチェック
     */
    inline
    void check_availability() const
    {
      if (failed) {
        throw std::runtime_error("cannot use a \'failed\' LargeInt object");
      }
    }

  private:
    /**
     * 足し算の内部実装
     * @param another  足す数(a+bのときのb)
     * @param dest     結果を返す先
     */
    void add_impl(const LargeInt<Int_t>& another, LargeInt<Int_t>* dest)
    const
    {
      check_availability();
      another.check_availability();
      const auto another_size = another.num.size();
      if (dest->num.size() < another_size) {
        dest->num.resize(another_size, 0);
      }
      const auto size = dest->num.size();
      using SizeType = typename std::remove_const<decltype(size)>::type;

      for (SizeType i = 0; i < size; i++) {
        if (i >= another_size) {
          break;
        }
        dest->num.at(i) += another.num.at(i);
        // 要素を跨ぐ繰り上げ
        auto n = i;
        while (dest->num.at(n) >= LIMIT_PER_TERMS) {
          dest->num.at(n) -= LIMIT_PER_TERMS;
          n++;
          if (n < size) {
            dest->num.at(n) += 1;
          }
          else {
            dest->num.push_back(1);
            break;
          }
        }
      }
    }

    /**
     * 引き算の内部実装
     * @param another  引く数(a-bのときのb)
     * @param dest     結果を返す先
     */
    void sub_impl(const LargeInt<Int_t>& another, LargeInt<Int_t>* dest)
    const
    {
      check_availability();
      another.check_availability();
      const auto another_size = another.num.size();
      if (dest->num.size() < another_size) {
        dest->num.resize(another_size, 0);
      }
      const auto size = dest->num.size();
      using SizeType = typename std::remove_const<decltype(size)>::type;

      for (SizeType i = 0; i < size; i++) {
        if (i >= another_size) {
          break;
        }
        if (dest->num.at(i) >= another.num.at(i)) {
          dest->num.at(i) -= another.num.at(i);
          continue;
        }
        // 要素を跨ぐ繰り下げ
        dest->num.at(i) = LIMIT_PER_TERMS - (another.num.at(i) - dest->num.at(i));
        auto n = i + 1;
        while (i < size && dest->num.at(n) >= 0) {
          if (dest->num.at(n) > 0) {
            dest->num.at(n)--;
            break;
          }
          dest->num.at(n) = LIMIT_PER_TERMS - 1;
          n++;
        }
      }

      // 最上位の0要素を消去
      SizeType delete_count = 0;
      for (auto iter = dest->num.rbegin(); iter != dest->num.rend(); iter++) {
        if (*iter > 0) { break; }
        if (*iter == 0 && iter != dest->num.rend() - 1) {
          delete_count++;
        }
      }
      if (delete_count > 0) {
        dest->num.erase(dest->num.cend() - delete_count, dest->num.cend());
      }
    }

    /**
     * 乗算の内部実装
     * @param times       かける数
     * @param dest        出力先
     * @param base_point  出力先の出力開始位置
     */
    void multi_int_impl(const Int_t times, LargeInt<Int_t>* dest, const Int_t base_point = 0)
    const
    {
      check_availability();

      if (times == 1) {
        dest->num = num;
        return;
      }
      if (times == 0) {
        dest->num.clear();
        dest->num.push_back(0);
        return;
      }

      const auto size = num.size();
      using SizeType = typename std::remove_const<decltype(size)>::type;

      const Int_t MAX = std::numeric_limits<Int_t>::max();
      dest->num.resize(base_point + size, 0);
      for (SizeType i = 0; i < size; i++) {
        if (num.at(i) == 0) { continue; }

        Int_t n = times;
        while (n > 0) {
          auto k = base_point + i;

          Int_t times_limit = (MAX - dest->num.at(k)) / num.at(i);
          if (n < times_limit) {
            times_limit = n;
          }

          if (dest->num.size() < i + 1) { dest->num.resize(i + 1, 0); }
          dest->num.at(k) += num.at(i) * times_limit;
          n -= times_limit;
          // 要素を跨ぐ繰り上げ
          while (k < dest->num.size() && dest->num.at(k) >= LIMIT_PER_TERMS) {
            if (dest->num.size() <= k + 1) { dest->num.resize(k + 2, 0); }
            dest->num.at(k + 1) += dest->num.at(k) / LIMIT_PER_TERMS;
            dest->num.at(k) %= LIMIT_PER_TERMS;
            k++;
          }
        }
      }
    }

  public:  // 演算子定義
    /**
     * 代入
     */
    inline
    LargeInt<Int_t>& operator=(const LargeInt<Int_t>& src)
    {
      if (this != &src) {
        num = src.num;
        is_negative = src.is_negative;
        failed = src.failed;
      }
      return *this;
    }

    /**
     * 通常の整数から代入
     */
    inline
    LargeInt<Int_t>& operator=(const Int_t n)
    {
      num.clear();
      num.push_back(n);
      is_negative = n < 0;
      failed = false;
      return *this;
    }

    /**
     * 移譲
     */
    inline
    LargeInt<Int_t>& operator=(LargeInt<Int_t>&& src)
    {
      if (this != &src) {
        num         = std::move(src.num);
        is_negative = std::move(src.is_negative);
        failed      = std::move(src.failed);
      }
      return *this;
    }

  private:
    LargeInt<Int_t> add_control(const LargeInt<Int_t>& another)
    const
    {
      if (is_negative && !another.is_negative) {
        // -a + b = b - a
        LargeInt<Int_t> result = another;
        sub_impl(*this, &result);
        if (*this > another) {
          result.is_negative = true;
        }
        return result;
      }
      else if (!is_negative && another.is_negative) {
        // a + (-b) = a - b
        LargeInt<Int_t> result = *this;
        sub_impl(another, &result);
        if (*this < another) {
          result.is_negative = true;
        }
        return result;
      }

      // (a + b) or (-a + (-b))
      LargeInt<Int_t> result = *this;
      add_impl(another, &result);
      return result;
    }

  public:
    /**
     * 加算
     * @param another 足す数
     * @return 加算結果
     */
    inline
    LargeInt<Int_t> operator+(const Int_t another)
    const
    {
      return (*this + LargeInt<Int_t>(another));
    }

    /**
     * 加算代入
     * @param another  足す数
     * @return 自分自身
     */
    inline
    LargeInt<Int_t>& operator+=(const Int_t another)
    {
      *this += LargeInt<Int_t>(another);
      return *this;
    }

    /**
     * 加算
     * @param another 足す数
     * @return 加算結果
     */
    inline
    LargeInt<Int_t> operator+(const LargeInt<Int_t>& another)
    const
    {
      return add_control(another);
    }

    /**
     * 加算代入
     * @param another  足す数
     * @return 自分自身
     */
    inline
    LargeInt<Int_t>& operator+=(const LargeInt<Int_t>& another)
    {
      auto result = add_control(another);
      this->num = std::move(result.num);
      this->is_negative = std::move(result.is_negative);
      return *this;
    }

  private:
    LargeInt<Int_t> sub_control(const LargeInt<Int_t>& another)
    const
    {
      if (another == 0) { return *this; }
      if (is_negative == another.is_negative) {
        if ((is_negative && *this < another)      // -a - (-b) = b - a  (if b > a then)
         || (!is_negative && *this < another))    // a - b (if b > a then)
        {
          LargeInt<Int_t> result = another;
          sub_impl(*this, &result);
          result.is_negative = !is_negative;
          return result;
        }
        else {                                    // b - a (if a > b then)
          LargeInt<Int_t> result = *this;
          sub_impl(another, &result);
          result.is_negative = is_negative;
          return result;
        }
      }

      LargeInt<Int_t> result = *this;
      if (is_negative && !another.is_negative) {
        // -a - b = -(a + b)
        add_impl(another, &result);
        result.is_negative = true;
      }
      else if (!is_negative && another.is_negative) {
        // a - (-b) = a + b
        add_impl(another, &result);
        result.is_negative = false;
      }
      return result;
    }

  public:
    /**
     * 減算
     * @param another 引く数
     * @return 減算結果
     */
    inline
    LargeInt<Int_t> operator-(const Int_t another)
    const
    {
      return (*this - LargeInt<Int_t>(another));
    }

    /**
     * 減算代入
     * @param another  引く数
     * @return 自分自身
     */
    inline
    LargeInt<Int_t>& operator-=(const Int_t another)
    {
      *this -= LargeInt<Int_t>(another);
      return *this;
    }

    /**
     * 減算
     * @param another 引く数
     * @return 減算結果
     */
    inline
    LargeInt<Int_t> operator-(const LargeInt<Int_t>& another)
    const
    {
      return sub_control(another);
    }

    /**
     * 減算代入
     * @param another  引く数
     * @return 自分自身
     */
    inline
    LargeInt<Int_t>& operator-=(const LargeInt<Int_t>& another)
    {
      auto result = sub_control(another);
      this->num = std::move(result.num);
      this->is_negative = std::move(result.is_negative);
      return *this;
    }

    /**
     * 乗算
     * @param times かける数（現状の実装では非LargeIntでなければならない点に注意）
     * @return 乗算結果
     */
    LargeInt<Int_t> operator*(const Int_t times)
    const
    {
      LargeInt<Int_t> result(0);
      multi_int_impl(times, &result);
      result.is_negative = (times < 0 && !is_negative) || (times >= 0 && is_negative);
      return result;
    }

    /**
     * 乗算代入
     * @param times 何回加算するか
     * @return 自分自身
     */
    LargeInt<Int_t>& operator*=(const Int_t times)
    {
      LargeInt<Int_t> result(0);
      multi_int_impl(times, &result);
      this->num = std::move(result.num);
      this->is_negative = (times < 0 && !is_negative) || (times >= 0 && is_negative);
      return *this;
    }

    /**
     * 乗算
     * @param times かける数（現状の実装では非LargeIntでなければならない点に注意）
     * @return 乗算結果
     */
    LargeInt<Int_t> operator*(const LargeInt<Int_t>& another)
    const
    {
      LargeInt<Int_t> result(0);
      const auto size = another.num.size();
      using SizeType = typename std::remove_const<decltype(size)>::type;
      for (SizeType i = 0; i < size; i++) {
        multi_int_impl(another.num.at(i), &result, i);
      }
      result.is_negative = is_negative != another.is_negative;
      return result;
    }

    /**
     * 乗算代入
     * @param times 何回加算するか
     * @return 自分自身
     */
    LargeInt<Int_t>& operator*=(const LargeInt<Int_t>& another)
    {
      LargeInt<Int_t> result(0);
      const auto size = another.num.size();
      using SizeType = typename std::remove_const<decltype(size)>::type;
      for (SizeType i = 0; i < size; i++) {
        multi_int_impl(another.num.at(i), &result, i);
      }
      this->num = std::move(result.num);
      this->is_negative = is_negative != another.is_negative;
      return *this;
    }

    /**
     * 等価比較演算
     * @param another 比較対象（a==bのうちb）
     * @return 値が一致するかどうか
     */
    bool operator==(const LargeInt<Int_t>& another)
    const
    {
      check_availability();
      another.check_availability();
      if (another.is_negative != this->is_negative) { return false; }
      const auto size = num.size();
      const auto another_size = another.num.size();
      using SizeType = typename std::remove_const<decltype(size)>::type;

      if (size != another_size) { return false; }
      for (SizeType i = 0; i < size; i++) {
        if (*(num.rbegin() + i) != *(another.num.rbegin() + i)) { return false; }
      }
      return true;
    }

    /**
     * 比較演算
     * @param another 比較対象（a<bのうちb）
     * @return 自分の方が小さいかどうか
     */
    bool operator<(const LargeInt<Int_t>& another)
    const
    {
      check_availability();
      another.check_availability();
      if (is_negative != another.is_negative) { return is_negative; }
      const auto size = num.size();
      const auto another_size = another.num.size();
      using SizeType = typename std::remove_const<decltype(size)>::type;

      if (size < another_size) { return !is_negative; }
      if (size > another_size) { return is_negative; }
      for (SizeType i = 0; i < size; i++) {
        if (*(num.rbegin() + i) < *(another.num.rbegin() + i)) { return !is_negative; }
        if (*(num.rbegin() + i) > *(another.num.rbegin() + i)) { return is_negative; }
      }
      return is_negative;
    }

    bool operator<=(const LargeInt<Int_t>& another)
    const
    {
      return this->operator<(another) || this->operator==(another);
    }

    bool operator>(const LargeInt<Int_t>& another)
    const
    {
      return another.operator<(*this);
    }

    bool operator>=(const LargeInt<Int_t>& another)
    const
    {
      return another.operator<(*this) || this->operator==(another);
    }

  public:  // ユーティリティ
    /**
     * 生成に失敗したかを取得
     * @return 失敗時true
     */
    inline
    bool fail() const { return failed; }

    /**
     * 内部の値のストリーム出力
     * @param os 出力ストリーム
     */
    void to_stream(std::ostream& os)
    const
    {
      check_availability();
      if (is_negative) { os << "-"; }
      for (auto it = num.rbegin(); it < num.rend(); it++) {
        const Int_t digits = (*it == 0) ? 1 : std::log10(*it) + 1;
        for (Int_t i = digits + 1; it != num.rbegin() && i < MAX_DIGITS; i++) {
          os << 0;
        }
        os << *it;
      }
    }

    /**
     * 内部の値の文字列化
     */
    inline
    std::string str()
    const
    {
      check_availability();
      std::ostringstream oss;
      to_stream(oss);
      return oss.str();
    }

    /**
     * 桁を反転
     */
    LargeInt<Int_t> reverse()
    const
    {
      check_availability();
      LargeInt<Int_t> res(0);
      for (auto it = num.begin(); it < num.end(); it++) {
        Int_t n = *it;
        while (n) {
          const Int_t d = n % 10;
          n /= 10;
          res *= 10;
          *res.num.rbegin() += d;
        }
      }
      return res;
    }

    /**
     * 各位の数の和を計算
     */
    Int_t digits_sum()
    const
    {
      check_availability();
      Int_t sum = 0;
      for (Int_t x : num) {
        while (x) {
          const Int_t digits = std::pow(10, static_cast<Int_t>(std::log10(x)));  // ex) 123 -> 3digits
          sum += (x - (x % digits)) / digits;        // case: num==123) sum += 1 + 2 + 3
          x -= x / digits * digits;                  // case: num==123) x = 123 -> 23 -> 3 -> 0
        }
      }
      return sum;
    }

    /**
     * 桁数を返す
     * @return  内部数値の桁数
     */
    inline
    std::size_t num_digits()
    const
    {
      check_availability();
      return (num.size() - 1) * (MAX_DIGITS - 1) + std::log10(*num.rbegin()) + 1;
    }
  };

  /**
   * 累乗計算の結果をLargeIntで返す
   */
  template <typename Int_t>
  LargeInt<Int_t> large_pow(const Int_t base, const Int_t exp)
  {
    LargeInt<Int_t> res(base);
    for (Int_t i = 1; i < exp; i++) {
      res *= base;
    }
    return res;
  }

} // namespace Euler


namespace std
{
  /**
   * LargeIntの10進桁数を返す
   * log10()は厳密にはそういうモノじゃないが，今のところ用途はそれだけなのでそう実装している
   * （つまり10底log演算では全然ない）
   */
  template <typename Int_t>
  inline
  double log10(const Euler::LargeInt<Int_t>& li)
  {
    return li.num_digits();
  }

  /**
   * 汎用的なLargeIntの文字列化
   */
  template <typename Int_t>
  inline
  std::string to_string(const Euler::LargeInt<Int_t>& li)
  {
    return li.str();
  }
}

/**
 * LargeIntをストリーム出力するための演算子オーバーロード
 */
template <typename Int_t>
inline
std::ostream& operator<<(std::ostream& os, const Euler::LargeInt<Int_t>& li)
{
  li.to_stream(os);
  return os;
}

#endif
