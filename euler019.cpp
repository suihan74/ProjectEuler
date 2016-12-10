/**
 * Problem 19 「日曜日の数え上げ」
 * 次の情報が与えられている.
 *
 * 1900年1月1日は月曜日である.
 * 9月, 4月, 6月, 11月は30日まであり, 2月を除く他の月は31日まである.
 * 2月は28日まであるが, うるう年のときは29日である.
 * うるう年は西暦が4で割り切れる年に起こる. しかし, 西暦が400で割り切れず100で割り切れる年はうるう年でない.
 * 20世紀（1901年1月1日から2000年12月31日）中に月の初めが日曜日になるのは何回あるか?
 */

#include <cstdint>
#include <iostream>
#include <stdexcept>

using uInt = std::uint_fast32_t;

uInt num_of_days_in_month(uInt year, uInt month)
{
  if (year < 1900 || month > 12) {
    throw std::out_of_range("num_of_days_in_month(): invalid date");
  }

  if (month == 2) {
    return (year % 4 || (year % 100 == 0 && year % 400)) ? 28 : 29;
  }
  return (month == 4 || month == 6 || month == 9 || month == 11) ? 30 : 31;
}

uInt get_day(uInt year, uInt month, uInt day)
{
  if (year < 1900 || month > 12) {
    throw std::out_of_range("get_day(): invalid date");
  }

  uInt days = 0;
  for (uInt y = 1900; y <= year; y++) {
    for (uInt m = 1; y < year ? m <= 12 : m < month ; m++) {
      days += num_of_days_in_month(y, m);
    }
  }
  return (days + day) % 7;
}

int main(void)
{
  try {
    uInt count = 0;
    for (uInt y = 1901; y <= 2000; y++) {
      for (uInt m = 1; m <= 12; m++) {
        if (get_day(y, m, 1) == 0) {
          count++;
        }
      }
    }
    std::cout << "Euler019: " << count << std::endl;
  }
  catch (std::out_of_range &e) {
    std::cout << e.what() << std::endl;
    return 1;
  }
  return 0;
}
