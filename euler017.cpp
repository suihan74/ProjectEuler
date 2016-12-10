/**
 * Problem 17 「数字の文字数」
 * 1 から 5 までの数字を英単語で書けば one, two, three, four, five であり, 全部で 3 + 3 + 5 + 4 + 4 = 19 の文字が使われている.
 *
 * では 1 から 1000 (one thousand) までの数字をすべて英単語で書けば, 全部で何文字になるか.
 *
 * 注: 空白文字やハイフンを数えないこと. 例えば, 342 (three hundred and forty-two) は 23 文字, 115 (one hundred and fifteen) は20文字と数える. なお, "and" を使用するのは英国の慣習.
 */

#include <iostream>
#include <sstream>

std::string NUMSTR[] = {
  "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine",
  "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"
};
std::string DIGITSTR[] = {
  "", "", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"
};

std::string num2str(unsigned int num)
{
  if (num < 20) {
    return NUMSTR[num];
  }
  else if (num == 1000) {
    return "onethousand";
  }

  std::ostringstream oss;
  if (num < 100) {
    oss << DIGITSTR[num / 10];
    if (num % 10) {
      oss << num2str(num % 10);
    }
    return oss.str();
  }
  else if (num < 1000) {
    oss << num2str(num / 100) << "hundred";
    if (num % 100) {
      oss << "and" << num2str(num % 100);
    }
    return oss.str();
  }

  oss << "num2str cannot handle given number: " << num << " (>1000)";
  throw std::out_of_range(oss.str());
}


int main(void)
{
  try {
    std::size_t length = 0;
    for (unsigned int i = 1; i <= 1000; i++) {
      length += num2str(i).length();
    }
    std::cout << "Euler017: " << length << std::endl;
  }
  catch (std::out_of_range &e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}

