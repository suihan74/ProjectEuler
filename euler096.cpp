/**
 * Problem 96 「数独」
 * "数独"(日本語で数字を配置するという意味)とは人気があるパズルの名前である. 起源は不明だが, その評判はラテン語で"Squares"と呼ばれる同様な, そしてはるかに難しいパズルを考案した レオンハルト・オイラーの貢献によるものに違いない. しかしながら, "数独"パズルの目的は それぞれの行, 列が3×3の枠を含む9×9の格子の空白（もしくは0）をそれぞれ1から9の数字で置き換えることである. 下に, 一般的なパズルの開始状態とその解答の例がある.
 *
 * うまく作られている"数独"パズルは, 選択肢を消去するために"仮定とテスト"方式を用いる必要があるかもしれないが, ただ一つの解を持ち, 論理によって解くことができる(これについては様々な意見がある).
 *
 * 探索の複雑さがパズルの難易度を決定する; 上に挙げた例は, 単純で直接的な推論によって解く事ができるため, 簡単であると考えられる.
 *
 * 6kバイトのテキストファイルfilesudoku.txt(右クリックで,"名前をつけてリンク先を保存") にはただ一つの解を持つ, 様々な難易度の50の"数独"パズルが含まれている(上の例題はこのファイルにおける最初のパズルである).
 *
 * 50すべてのパズルを解き, それぞれの解答の左上隅にある3桁の数の合計を求めよ; 例えば483は上の解答例の左上隅の3桁の数である.
 */

#include <array>
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

using uInt = std::uint_fast32_t;

struct Cell;
class Group   : public std::array<Cell*, 9> {};
class Section : public Group {};
class Line    : public Group {};

/** 数独の各セル */
struct Cell
{
  /** セルの値 : 未解決の場合0が割り当てられる */
  uInt num;
  /** セルが属する区画 */
  Section* section;
  /** セルが属する行 */
  Line* row;
  /** セルが属する列 */
  Line* column;
  /** 割り当て候補を記録するメモ */
  std::vector<uInt> memo;
};

class Sudoku
{
private:
  /** ゲーム板 */
  std::array<Cell, 9*9> matrix;
  /** 区画 */
  std::array<Section, 9> sections;
  /** 行 */
  std::array<Line, 9> rows;
  /** 列 */
  std::array<Line, 9> columns;
  /** 未解決セルの個数 */
  uInt num_unsolved_cells;

private:
  void initialize()
  {
    for (uInt row = 0; row < 9; row++) {
      for (uInt col = 0; col < 9; col++) {
        const uInt pos = col + row * 9;
        const uInt sec = row / 3 * 3 + pos / 3 % 3;
        const uInt sec_idx = row % 3 * 3 + col % 3;
        Cell& cell = matrix.at(pos);
        // 各セルへのポインタを各種グループに割り当てる
        sections.at(sec).at(sec_idx) = &cell;
        rows.at(row).at(col) = &cell;
        columns.at(col).at(row) = &cell;
        // セルの初期化
        cell.num = 0;
        cell.section = &sections.at(sec);
        cell.row = &rows.at(row);
        cell.column = &columns.at(col);
        cell.memo.clear();
      }
    }
    num_unsolved_cells = 0;
  }

public:
  /**
   * ゲームデータをストリームから読み込む
   * @param  in 入力ストリーム
   * @return    未解決なセルの個数
   */
  uInt load_data(std::istream& in)
  {
    initialize();
    try {
      std::string line_buffer, num_buffer;
      std::getline(in, line_buffer);  // 最初の行は要らない情報
      for (uInt row = 0; row < 9; row++) {
        std::getline(in, line_buffer);
        uInt col = 0;
        for (const char c : line_buffer) {
          if (c < '0' || c > '9') { break; }
          matrix.at(col + row * 9).num = uInt(c - '0');
          if (c == '0') {
            num_unsolved_cells++;
          }
          col++;
        }
      }
    }
    catch (std::invalid_argument& e) {
      throw std::runtime_error("the datafile contains illegal data. (not a number)");
    }
    catch (std::out_of_range& e) {
      throw std::runtime_error("the datafile contains illegal data. (out of range)");
    }
    return num_unsolved_cells;
  }

  /**
   * クリアするか手詰まりまでゲームを進行
   * @return 未解決なセルの個数
   */
  uInt run()
  {
    for (;;) {
      // まずは正攻法での解決を図る
      uInt prev_num_unsolved_cells;
      do {
        prev_num_unsolved_cells = num_unsolved_cells;
        step();
      }
      while(prev_num_unsolved_cells != num_unsolved_cells);

      // それでも駄目なら,"仮定とテスト"で突破口を探る
      if (num_unsolved_cells > 0) {
        solve_with_assumption();
      }
      else {
        break;
      }
    }

    // 破綻が生じていないか（一応）確認
    if (!test()) {
      print();
      throw std::runtime_error("trial has been failed. something is wrong");
    }

    return num_unsolved_cells;
  }

  /**
   * e096の答えを得る
   * @return 左上端の3桁の数
   */
  inline
  uInt get_result_e096()
  const
  {
    return matrix.at(0).num * 100 + matrix.at(1).num * 10 + matrix.at(2).num;
  }

private:
  /**
   * ゲーム進行の1ステップ
   */
  void step()
  {
    for (Cell& cell : matrix) {
      if (cell.num) { continue; }
      // 未解決のセルの解決を試行
      if (cell.memo.empty()) { make_memo(cell); }
      if (cell.num) {
        // 解決した場合,グループ（区画・行・列）の他のセルのメモを更新する
        update_related_memo(cell);
      }
    }
    // グループを対象にしたメモの解決
    for (const auto& section : sections) {
      solve_unique_memo(section);
      solve_subline(section);
    }
    for (const auto& lines : {rows, columns}) {
      for (const auto& line : lines) { solve_unique_memo(line); }
    }
  }

  /**
   * セル値を仮定し,破綻が起きないかテストする方法で解決を図る
   */
  void solve_with_assumption()
  {
    // 現状態を保存
    std::array<Cell, 9*9> matrix_copy = matrix;
    uInt num_unsolved_cells_copy = num_unsolved_cells;

    // 未解決セルを収集
    std::vector<Cell*> candidates;
    for (auto& cell : matrix) {
      if (!cell.num) { candidates.push_back(&cell); }
    }

    for (auto* const target_cell : candidates) {
      if (target_cell->num) { continue; }  // 処理過程で解決する可能性があるため,除けておく
      const auto target_memo = target_cell->memo;

      // 候補をセル値として扱い,破綻が起きるかを確認
      for (uInt num : target_memo) {
        target_cell->num = num;
        num_unsolved_cells--;
        update_related_memo(*target_cell);
        // 推論を使わない通常試行を進められるだけ進める
        uInt prev_num_unsolved_cells;
        do {
          prev_num_unsolved_cells = num_unsolved_cells;
          step();
        }
        while(num_unsolved_cells && prev_num_unsolved_cells != num_unsolved_cells);

        if (!test()) {
          // 破綻する場合,その候補はメモから除外
          matrix = matrix_copy;
          num_unsolved_cells = num_unsolved_cells_copy;
          target_cell->num = 0;
          target_cell->memo.erase(std::remove(target_cell->memo.begin(), target_cell->memo.end(), num), target_cell->memo.end());
          if (target_cell->memo.size() == 1) {
            target_cell->num = target_cell->memo.at(0);
            num_unsolved_cells--;
            update_related_memo(*target_cell);
            break;
          }
          update_related_memo(*target_cell);
        }
        else if (num_unsolved_cells > 0) {
          // 破綻が起こらないが最後まで解決できない場合
          // 状態を復元して保留
          matrix = matrix_copy;
          num_unsolved_cells = num_unsolved_cells_copy;
          target_cell->num = 0;
        }
        else {
          // 破綻せず最後まで到達した場合,そのまま終了
          return;
        }
      }
    }
  }

  /**
   * 仮定の結果導かれた状態が破綻していないかを確認
   * @return 破綻していない: true
   */
  bool test()
  const
  {
    for (const Section& section : sections) {
      if (!test_group(section)) { return false; }
    }

    for (const auto& lines : {rows, columns}) {
      for (const Line& line : lines) {
        if (!test_group(line)) { return false; }
      }
    }

    return true;
  }

  /**
   * グループ内のセルが破綻なく並んでいるかを確認
   * @param  group 対象グループ
   * @return       破綻していない: true
   */
  bool test_group(const Group& group)
  const
  {
    std::array<uInt, 10> arr;
    std::fill(arr.begin(), arr.end(), 0);
    for (const Cell* cell : group) {
      if (!cell->num && cell->memo.size() == 0) { return false; }
      if (cell->num && arr.at(cell->num)) { return false; }
      arr.at(cell->num) = 1;
    }
    return true;
  }

private:
  /**
   * セルの割り当て候補メモをはじめから作成
   * @param cell 対象のセル
   */
  void make_memo(Cell& cell)
  {
    cell.memo.clear();
    cell.memo.resize(10);
    // 区画・行・列に存在しない数字のベクタを作成する
    std::iota(cell.memo.begin(), cell.memo.end(), 0);
    for (const Group& t : {(Group&)*cell.section, (Group&)*cell.row, (Group&)*cell.column}) {
      for (const Cell* c : t) {
        cell.memo.at(c->num) = 0;
      }
    }
    cell.memo.erase(std::remove(cell.memo.begin(), cell.memo.end(), 0), cell.memo.end());
    // ベクタに残った値が1個だけならセルの値を一意に決定できる
    if (cell.memo.size() == 1) {
      cell.num = cell.memo.at(0);
      num_unsolved_cells--;
      update_related_memo(cell);
    }
  }

  /**
   * 解決セルの関連セルのメモを更新
   * @param cell 解決したセル
   */
  void update_related_memo(const Cell& cell)
  {
    if (!cell.num) { return; }
    for (const Group& t : {(Group&)*cell.section, (Group&)*cell.row, (Group&)*cell.column}) {
      for (Cell* c : t) {
        if (c->num) { continue; }
        if (c->memo.empty()) {
          make_memo(*c);
        }
        else {
          // 該当セルの値を関連セルのメモから消去
          c->memo.erase(std::remove(c->memo.begin(), c->memo.end(), cell.num), c->memo.end());
          if (c->memo.size() == 1) {
            c->num = c->memo.at(0);
            num_unsolved_cells--;
          }
        }
        // 更新を伝播
        if (c->num) {
          update_related_memo(*c);
        }
      }
    }
  }

  /**
   * グループ内でユニークなメモ値を解決
   * @param group 対象のグループ
   */
  void solve_unique_memo(const Group& group)
  {
    for (auto* cell : group) {
      if (cell->num) { continue; }
      for (uInt num : cell->memo) {
        const auto iter = std::find_if(group.cbegin(), group.cend(),
          [cell, num](const Cell* other_cell)
          {
            if (other_cell == cell) { return false; }
            for (uInt other_num : other_cell->memo) {
              if (num == other_num) {
                return true;
              }
            }
            return false;
          });

        // グループ中でユニークなメモ値を発見した場合,セルをその値で解決する
        if (iter == group.end()) {
          cell->num = num;
          num_unsolved_cells--;
          update_related_memo(*cell);
          break;
        }
      }
    }
  }

  /**
   * 区画内の行・列のメモ内容から他の区画の行・列のメモを削減する
   * @param section 対象区画
   */
  void solve_subline(const Section& section)
  {
    for (uInt i = 0; i < 9; i += 3) {  // 行からの削減
      solve_subline_impl(section, section.at(i)->row, i);
    }
    for (uInt i = 0; i < 3; i++) {     // 列からの削減
      solve_subline_impl(section, section.at(i)->column, i);
    }
  }

  /**
   * 区画内の行・列のメモ内容から他の区画の行・列のメモを削減する（実装）
   * @param section 対象区画
   * @param line    対象ライン（行か列か）
   * @param offset  起点セルの区画内のインデックス
   */
  void solve_subline_impl(const Section& section, Line* line, uInt offset)
  {
    const bool mode_row = line == section.at(offset)->row;
    // 区画内の行・列中にある各セルメモの和集合になるメモを生成
    std::vector<uInt> line_memo(10, 0);
    for (uInt j = 0; j < 3; j++) {
      const uInt pos = offset + (mode_row ? j : j * 3);
      if (section.at(pos)->num) { continue; }
      for (const uInt num : section.at(pos)->memo) {
        line_memo.at(num) = num;
      }
    }
    line_memo.erase(std::remove(line_memo.begin(), line_memo.end(), 0), line_memo.end());
    // 区画内の他の部分にメモ値が無ければ,全体の行・列からそのメモ値を除外する
    for (const uInt num : line_memo) {
      bool failed = false;
      for (uInt k = 0; k < 9; k++) {
        if (section.at(k)->num
         || (mode_row && section.at(k)->row == line)
         || (!mode_row && section.at(k)->column == line))
        {
          continue;
        }
        const auto& sec_memo = section.at(k)->memo;
        if (std::find(sec_memo.begin(), sec_memo.end(), num) != sec_memo.end()) {
          failed = true;
          break;
        }
      }
      if (failed) { continue; }
      // 他の区画の同一行から該当メモ値を除外
      for (Cell* cell : *line) {
        if (cell->num || cell->section == &section) { continue; }
        cell->memo.erase(std::remove(cell->memo.begin(), cell->memo.end(), num), cell->memo.end());
        if (cell->memo.size() == 1) {
          // 解決
          cell->num = cell->memo.at(0);
          num_unsolved_cells--;
          update_related_memo(*cell);
          // 伝搬した解決をline_memoに反映するのが大変なので,いっそのこと終了することにする
          // ここで対策無くループを続行してしまうと
          // 伝搬した解決によってメモ値として使用不可能になった値がline_memoに残る可能性がある
          // そのゾンビメモ値を使って他区画のメモを更新すると破綻が生じる可能性がある
          return;
        }
      }
    }
  }

public:
  /**
   * 板を表示
   */
  void print()
  {
    for (uInt row = 0; row < 9; row++) {
      if (row > 0 && row % 3 == 0) { std::cout << "---+---+---" << std::endl; }
      for (uInt col = 0; col < 9; col++) {
        if (col > 0 && col % 3 == 0) { std::cout << "|"; }
        if (matrix.at(col + row * 9).num) {
          std::cout << matrix.at(col + row * 9).num;
        }
        else {
          std::cout << "o";
        }
      }
      std::cout << std::endl;
    }
  }
};

int main(void)
{
  try {
    std::ifstream ifs("euler096.txt");
    if (ifs.fail()) {
      throw std::invalid_argument("cannot open a datafile.");
    }

    Sudoku s;
    uInt count = 1;
    uInt init_unsolves = 0;
    uInt fin_unsolves = 0;
    uInt result = 0;
    while (!ifs.eof()) {
      std::cout << "Game " << (count++) << std::endl;
      init_unsolves += s.load_data(ifs);
      fin_unsolves += s.run();
      result += s.get_result_e096();
      s.print();
    }
    std::cout << "achievement: " << (init_unsolves - fin_unsolves) << " / " << init_unsolves
              << "  (" << (100.0 * (init_unsolves - fin_unsolves) / init_unsolves) << " %)"
              << std::endl;
    std::cout << "Euler096: " << result << std::endl;
  }
  catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    return 1;
  }
  return 0;
}
