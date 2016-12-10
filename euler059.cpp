/**
 * Problem 59 「XOR暗号解読」
 * (訳者注: 文字コードの説明は適当です) 各文字はそれぞれ一意のコードに割り当てられている.
 * よく使われる標準としてASCII (American Standard Code for Information Interchange) がある.
 * ASCIIでは, 大文字A = 65, アスタリスク (*) = 42, 小文字k = 107というふうに割り当てられている.
 *
 * モダンな暗号化の方法として, テキストファイルの各バイトをASCIIに変換し, 秘密鍵から計算された値とXORを取るという手法がある.
 * XOR関数の良い点は, 暗号化に用いたのと同じ暗号化鍵でXORを取ると平文を復号できる点である.
 * 65 XOR 42 = 107であり, 107 XOR 42 = 65である.
 *
 * 破られない暗号化のためには, 鍵は平文と同じ長さのランダムなバイト列でなければならない.
 * ユーザーは暗号文と暗号化鍵を別々の場所に保存する必要がある. また, もし一方が失われると, 暗号文を復号することは不可能になる.
 *
 * 悲しいかな, この手法はほとんどのユーザーにとって非現実的である.
 * そこで, 鍵の変わりにパスワードを用いる手法が用いられる.
 * パスワードが平文より短ければ (よくあることだが), パスワードは鍵として繰り返し用いられる.
 * この手法では, 安全性を保つために十分長いパスワードを用いる必要があるが, 記憶するためにはある程度短くないといけない.
 *
 * この問題での課題は簡単になっている. 暗号化鍵は3文字の小文字である.
 * cipher1.txtは暗号化されたASCIIのコードを含んでいる. また, 平文はよく用いられる英単語を含んでいる.
 * この暗号文を復号し, 平文のASCIIでの値の和を求めよ.
 */

#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>

using uInt = std::uint_fast32_t;

int main(void)
{
  std::ifstream ifs("./euler059.txt");
  if (ifs.fail()) {
    std::cout << "Euler059: cannot open the cipher file: euler059.txt" << std::endl;
    return 1;
  }

  std::string line_buffer;
  std::getline(ifs, line_buffer);
  std::istringstream iss(line_buffer);
  uInt i = 0, code;
  char c;
  // key[0] : 最後の文字が丁度3で割り切れる位置にあり，コードが73で，恐らく英文の締めなので'.'になるんだろうと予想
  // key[1] : key[0]を用いて部分的に復号した結果，76文字目も'.'なのでその次の文字は普通の文章なら空白' 'だと予想
  // key[2] : 以上の部分復号の結果だいぶ文章が見えてきたので，あとはぱっと見で分かりそうな単語の部分を探して0,1同様にやるだけ．
  //          ここではとりあえず，最初の4文字が "(TXe" (Xは不明文字)なので，まぁ普通に考えてTheでしょうということでやってみたら解決
  const char KEY[3] = { (73^'.'), (79^' '), (12^'h') };
  uInt result = 0;
  while (!iss.eof()) {
    iss >> code >> c;
    uInt idx = (i++) % 3;
    uInt plain = code ^ KEY[idx];
    result += plain;
    std::cout << char(plain);
  }
  std::cout << std::endl;
  std::cout << "Euler059: " << result << "   : KEY is \"" << KEY << "\"" << std::endl;
  return 0;
}
