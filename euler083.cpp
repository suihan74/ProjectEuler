/**
 * Problem 83 「経路の和:3方向」
 * 注: この問題はProblem 81よりも非常に挑戦しがいがあるだろう.
 *
 * 下記の5次の正方行列で, 上下左右に移動し左上のセルから開始し右下のセルで終了する道を探索する. 一番小さな道は下で赤で示されており, このときの合計は2297になる.
 *
 * 131	673	234	103	18
 * 201	96	342	965	150
 * 630	803	746	422	111
 * 537	699	497	121	956
 * 805	732	524	37	331
 * 今, 31Kのテキストファイルmatrix.txtには80×80の行列が書かれている. 上下左右に移動し左上のセルから開始し右下のセルで終了する道に沿った和の最小を求めよ.
 */

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

using uInt = std::uint_fast32_t;

/**
 * マスの値とA*探索に必要な情報を持たせた探索用ノード
 */
class Node
{
public:
  uInt x;
  uInt y;

  /** cost of the node */
  uInt node_value;
  /** f(n) = g(n) + h(n) */
  uInt path_cost;
  /** h(n) */
  uInt estimation_cost_to_goal;

  Node* parent;

public:
  explicit Node(uInt x, uInt y, uInt node_value)
    : x(x),
      y(y),
      node_value(node_value),
      path_cost(0),
      estimation_cost_to_goal(0),  // 推定値を使わない場合A*的に微妙だが最短経路を辿ること自体はできる
      parent(nullptr)
    {}

public:
  /** calculate f'(m), n->m */
  uInt calc_cost(Node& fromNode) const
  {
    return fromNode.calc_cost_from_start() + node_value + estimation_cost_to_goal;
  }

  /** calculate g(n) */
  uInt calc_cost_from_start() const
  {
    return path_cost - estimation_cost_to_goal;
  }
};

using Matrix = std::vector<std::vector<Node>>;

Matrix load_data_file(const char* filename)
{
  Matrix result;
  std::ifstream ifs(filename);
  if (ifs.fail()) {
    throw std::invalid_argument("cannot open a datafile.");
  }

  try {
    std::string line_buffer, num_buffer;
    uInt y = 0;
    while (!ifs.eof()) {
      std::getline(ifs, line_buffer);
      std::istringstream line_stream(line_buffer);
      result.resize(result.size() + 1);
      uInt x = 0;
      while (!line_stream.eof()) {
        std::getline(line_stream, num_buffer, ',');
        result.rbegin()->emplace_back(x, y, std::stoul(num_buffer));
        x++;
      }
      y++;
    }
  }
  catch (std::invalid_argument& e) {
    throw std::runtime_error("the datafile contains illegal data. (not a number)");
  }
  catch (std::out_of_range& e) {
    throw std::runtime_error("the datafile contains illegal data. (out of range)");
  }

  return std::move(result);
}

/**
 * fromノードからtoノードに移動する際のコストを評価する
 * 評価値がより良好になる場合はノードのコストと探索リストを更新する
 */
void calc_node(Node* from, Node* to, std::vector<Node*>& open_list, std::vector<Node*>& close_list)
{
  const auto new_cost = to->calc_cost(*from);

  const auto iter_in_open = std::find(open_list.begin(), open_list.end(), to);
  const auto iter_in_close = std::find(close_list.begin(), close_list.end(), to);
  const auto existed_in_open = iter_in_open != open_list.end();
  const auto existed_in_close = iter_in_close != close_list.end();

  if (!existed_in_open && !existed_in_close)
  {
    to->path_cost = new_cost;
    to->parent = from;
    open_list.push_back(to);
  }
  else if (existed_in_open && new_cost < to->path_cost)
  {
    to->path_cost = new_cost;
    to->parent = from;
    open_list.erase(iter_in_open);
    open_list.push_back(to);
  }
  else if (existed_in_close && new_cost < to->path_cost)
  {
    to->path_cost = new_cost;
    to->parent = from;
    close_list.erase(iter_in_open);
    open_list.push_back(to);
  }
}

uInt trace_min_path(Matrix& mat)
{
  const uInt WIDTH = mat.begin()->size();
  const uInt HEIGHT = mat.size();

  std::vector<Node*> open_list;
  std::vector<Node*> close_list;
  auto* start_node = &mat[0][0];
  auto* goal_node = &(*(mat.rbegin()->rbegin()));
  start_node->path_cost = start_node->estimation_cost_to_goal;

  open_list.push_back(start_node);

  while (!open_list.empty()) {
    // 最小コストのノードを取得
    auto target_iter = std::min_element(open_list.begin(), open_list.end(), [](const Node* a, const Node* b)
      {
        return a->path_cost < b->path_cost;
      });
    auto* target_node = *target_iter;
    open_list.erase(target_iter);

    if (target_node == goal_node) break;
    close_list.push_back(target_node);

    // 移動可能なノードを評価する
    if (target_node->x > 0)
    {
      auto* left = &mat[target_node->y][target_node->x - 1];
      calc_node(target_node, left, open_list, close_list);
    }

    if (target_node->y > 0)
    {
      auto* top = &mat[target_node-> y - 1][target_node->x];
      calc_node(target_node, top, open_list, close_list);
    }

    if (target_node->x < WIDTH - 1)
    {
      auto* right = &mat[target_node->y][target_node->x + 1];
      calc_node(target_node, right, open_list, close_list);
    }

    if (target_node->y < HEIGHT - 1)
    {
      auto* bottom = &mat[target_node->y + 1][target_node->x];
      calc_node(target_node, bottom, open_list, close_list);
    }
  }

  uInt result = 0;
  Node* n = goal_node;
  for (;;) {
    result += n->node_value;
    if (n->parent == nullptr)
    {
      if (n == start_node) break;
      else throw std::runtime_error("failed to trace min path");
    }
    n = n->parent;
  }

  return result;
}

int main(void)
{
  try {
    auto mat = load_data_file("euler081.txt");
    std::cout << "Euler083: " << trace_min_path(mat) << std::endl;
  }
  catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}
