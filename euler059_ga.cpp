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

#include <algorithm>
#include <cstddef>
#include <fstream>
#include <numeric>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <vector>

using std::size_t;

class GADecrypter
final
{
public:
  using Solution   = std::string;
  using Population = std::vector<std::pair<Solution, int>>;  // 解とその評価値

  static constexpr size_t MAX_GENERATIONS = 300;  /**< 世代数上限 */
  static constexpr size_t POPULATION_SIZE = 20;   /**< 解集団サイズ */
  static constexpr size_t VARIANT_SIZE    = 17;   /**< 鍵の長さ */
  static constexpr double MUTATION_RATIO  = 0.1;  /**< 突然変異発生率 */

private:
  /** 暗号文 */
  std::string cipher;
  /** 得られた最良解 */
  std::pair<Solution, int> best_key;

public:
  /** コンストラクタ */
  GADecrypter(void)
    : best_key(Solution(), 0)
  {
    load_cipher("euler059_difficult.txt");
  }

private:
  /**
   * 暗号文をロード
   * @param filename 暗号文のファイル名
   */
  void load_cipher(const char* filename)
  {
    std::ifstream ifs(filename);
    std::string buf;
    if (ifs.fail()) {
      std::ostringstream oss;
      oss << "Euler059: cannot open the cipher file: " << filename;
      throw std::runtime_error(oss.str());
    }
    std::getline(ifs, buf);
    std::istringstream iss(buf);
    while (!iss.eof()) {
      int code;
      char c;
      iss >> code >> c;
      cipher.push_back(char(code));
    }
  }

  /**
   * 初期解集団の生成
   * @param solutions 解集団生成先
   * @param rng       乱数生成器
   */
  void initialize(Population* solutions, std::mt19937& rng)
  const
  {
    solutions->reserve(POPULATION_SIZE * 2);
    std::uniform_int_distribution<char> dist('a', 'z');
    for (size_t i = 0; i < POPULATION_SIZE; i++) {
      solutions->emplace_back(Solution(VARIANT_SIZE, '\0'), 0);
      for (size_t j = 0; j < VARIANT_SIZE; j++) {
        solutions->rbegin()->first.at(j) = dist(rng);
      }
      solutions->rbegin()->second = evaluate_solution(solutions->rbegin()->first);
    }
  }

  /**
   * 復号鍵を評価
   * @param  s 解（復号鍵）
   * @return   評価値
   */
  int evaluate_solution(const Solution& s)
  const
  {
    auto size = cipher.size();
    char cur, prev = ' ';
    int result = 1;  // ゼロ除算除けのため最小値は1
    std::string dest;
    // アルファベット・英文の基本的な記号が含まれている個数
    for (decltype(size) i = 0; i < size; i++) {
      cur = cipher.at(i) ^ s.at(i % VARIANT_SIZE);
      dest.push_back(cur);
      if ((cur >= 'a' && cur <= 'z')) {
        result++;
      }
      else if ((cur >= 'A' && cur <= 'Z') && prev == ' ') {
        result += 10;
      }
      else if ((cur == ' ' || cur == ',') && prev >= 'a' && prev <= 'z') {
        result += 10;
      }
      else if ((cur == '.' || cur == '?' || cur == '!') && prev >= 'a' && prev <= 'z') {
        if (i + 1 == size) {
          result += 100;
        }
        else {
          result++;
        }
      }
      prev = cur;
    }

    return result;
  }

  /**
   * 子生成のための両親を選択する
   * @param solutions  解集団
   * @param rng        乱数生成器
   */
  std::pair<Solution, Solution> select(Population& solutions, std::mt19937& rng)
  const
  {
    int sum = std::accumulate(solutions.begin(), solutions.begin() + POPULATION_SIZE, 0,
      [](int init, std::pair<Solution, int>& x)
      {
        return init + x.second;
      });
    std::uniform_int_distribution<int> dist(0, sum);
    int first = 0;
    int second = 0;

    Solution* parent_a;
    Solution* parent_b;

    do {
      first = dist(rng);
      second = first;
      while (first == second) { second = dist(rng); }

      parent_a = parent_b = nullptr;
      int cur = 0;
      for (auto& s : solutions) {
        cur += s.second;
        if (parent_a == nullptr && first <= cur) {
          parent_a = &s.first;
        }
        if (parent_b == nullptr && second <= cur) {
          parent_b = &s.first;
        }
        if (parent_a && parent_b) { break; }
      }
    } while (parent_a == parent_b);

    return std::make_pair(*parent_a, *parent_b);
  }

  /**
   * 交叉
   * @param p   両親から次の2個体を作るためのペア
   * @param rng 乱数生成器
   */
  void crossover(std::pair<Solution, Solution>& p, std::mt19937& rng)
  const
  {
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    for (size_t i = 0; i < VARIANT_SIZE; i++) {
      if (dist(rng) < 0.5) {
        std::swap(p.first.at(i), p.second.at(i));
      }
    }
  }

  /**
   * 突然変異
   * @param p     両親から次の子2個体を作るためのペア
   * @param rng   乱数生成器
   */
  void mutate(std::pair<Solution, Solution>& p, std::mt19937& rng)
  const
  {
    std::uniform_real_distribution<double> dist_r(0.0, 1.0);
    std::uniform_int_distribution<char> dist_i('a', 'z');
    for (size_t i = 0; i < VARIANT_SIZE; i++) {
      if (dist_r(rng) < MUTATION_RATIO) {
        p.first.at(i) = dist_i(rng);
      }
      if (dist_r(rng) < MUTATION_RATIO) {
        p.second.at(i) = dist_i(rng);
      }
    }
  }

public:
  /**
   * 乱数シードを指定して実行
   * @param seed 乱数シード値
   */
  void run_seed(const size_t seed = 0)
  {
    Population solutions;
    std::mt19937 rng(seed);
    initialize(&solutions, rng);
    for (size_t gen = 0; gen < MAX_GENERATIONS; gen++) {
      while (solutions.size() < POPULATION_SIZE * 2) {
        auto p = select(solutions, rng);
        crossover(p, rng);
        mutate(p, rng);
        solutions.emplace_back(p.first, evaluate_solution(p.first));
        solutions.emplace_back(p.second, evaluate_solution(p.second));
      }
      std::sort(solutions.begin(), solutions.end(), [](std::pair<Solution, int>& a, std::pair<Solution, int>& b)
        {
          return a.second > b.second;
        });
      solutions.erase(solutions.begin() + POPULATION_SIZE, solutions.end());
      std::cout << "[" << gen << "] " << solutions.begin()->first << " : " << solutions.begin()->second << std::endl;
    }

    if (best_key.second < solutions.begin()->second) {
      best_key = std::move(*solutions.begin());
    }
  }

  /**
   * 探索実行
   * @param times 試行数
   */
  void run(const size_t times = 1)
  {
    for (size_t i = 0; i < times; i++) {
      run_seed(i);
    }
  }

  /**
   * 最終的に得られた解を使用して暗号文を復号して表示
   */
  std::string decrypt(void)
  const
  {
    std::ostringstream oss;
    auto size = cipher.size();
    for (decltype(size) i = 0; i < size; i++) {
      oss << char(cipher.at(i) ^ best_key.first.at(i % VARIANT_SIZE));
    }
    return oss.str();
  }

  /**
   * 別の鍵で再度暗号化（より難しい問題を作るために作ったため，解法には無関係）
   * @param filename 出力ファイル名
   * @param key      新しい鍵
   */
  void encrypt_to_file(const char* filename, const std::string& key)
  const
  {
    std::ofstream ofs(filename);
    if (ofs.fail()) {
      std::ostringstream oss;
      oss << "Euler059: cannot open the output file: " << filename;
      throw std::runtime_error(oss.str());
    }
    auto size = cipher.size();
    const auto key_length = key.length();
    for (decltype(size) i = 0; i < size; i++) {
      ofs << int(key.at(i % key_length) ^ (char(cipher.at(i) ^ best_key.first.at(i % VARIANT_SIZE))));
      if (i + 1 < size) { ofs << ","; }
    }
  }
};

int main(void)
{
  GADecrypter ga;
  ga.run();
  std::cout << ga.decrypt() << std::endl;
//  ga.encrypt_to_file("euler059_difficult.txt", "moremoredifficult");
  return 0;
}
