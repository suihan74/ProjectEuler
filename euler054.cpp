/**
 * Problem 54 「ポーカーハンド」
 * カードゲームのポーカーでは, 手札は5枚のカードからなりランク付けされている. 役を低い方から高い方へ順に並べると以下である.
 *
 * 役無し(ハイカード): 一番値が大きいカード
 * ワン・ペア: 同じ値のカードが2枚
 * ツー・ペア: 2つの異なる値のペア
 * スリーカード: 同じ値のカードが3枚
 * ストレート: 5枚の連続する値のカード
 * フラッシュ: 全てのカードが同じスート (注: スートとはダイヤ・ハート・クラブ/スペードというカードの絵柄のこと)
 * フルハウス: スリーカードとペア
 * フォーカード: 同じ値のカードが4枚
 * ストレートフラッシュ: ストレートかつフラッシュ
 * ロイヤルフラッシュ: 同じスートの10, J, Q, K, A
 * ここでカードの値は小さい方から2, 3, 4, 5, 6, 7, 8, 9, 10, J, Q, K, Aである. (訳注：データ中で10は'T'と表される)
 *
 * もし2人のプレイヤーが同じ役の場合には, 役を構成する中で値が最も大きいカードによってランクが決まる: 例えば, 8のペアは5のペアより強い (下の例1を見よ). それでも同じランクの場合には (例えば, 両者ともQのペアの場合), 一番値が大きいカードによってランクが決まる (下の例4を見よ). 一番値が大きいカードが同じ場合には, 次に値が大きいカードが比べれられ, 以下同様にランクを決定する.
 *
 * poker.txtには1000個のランダムな手札の組が含まれている. 各行は10枚のカードからなる (スペースで区切られている): 最初の5枚がプレイヤー1の手札であり, 残りの5枚がプレイヤー2の手札である. 以下のことを仮定してよい
 *
 * 全ての手札は正しい (使われない文字が出現しない. 同じカードは繰り返されない)
 * 各プレイヤーの手札は特に決まった順に並んでいるわけではない
 * 各勝負で勝敗は必ず決まる
 * 1000回中プレイヤー1が勝つのは何回か? (訳注 : この問題に置いてA 2 3 4 5というストレートは考えなくてもよい)
 */

#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <vector>
#include <stdexcept>

using std::size_t;

/** トランプのスート */
enum class Suit { CLOVER, DIA, HEART, SPADE };

/** ポーカーの役種（弱い順）*/
enum class PokerHandRank
{
  HIGH_CARD,
  ONE_PAIR,
  TWO_PAIR,
  THREE_OF_A_KIND,
  STRAIGHT,
  FLUSH,
  FULL_HOUSE,
  FOUR_OF_A_KIND,
  STRAIGHT_FLUSH,
  ROYAL_FLUSH,
};

/** 役 */
struct PokerHand
{
  PokerHandRank rank;  /**< 役種 */
  int value;           /**< 役を構成するカードの値 */
};

/** カード */
struct Card
{
  int value;  /**< カードの値（2が最弱でAが最強） */
  Suit suit;  /**< スート */

  explicit Card(void) = default;
  /** ソート用ムーブコンストラクタ定義 */
  Card(Card&& src) : value(std::move(src.value)), suit(std::move(src.suit)) {}

  /** ソート用比較演算子定義 */
  bool operator<(const Card& other) { return value < other.value; }

  /** ソート用ムーブ代入定義 */
  Card& operator=(Card&& other)
  {
    value = std::move(other.value);
    suit = std::move(other.suit);
    return *this;
  }
};

using Hand = std::vector<Card>;

/*****************************************************************************/

/**
 * ストレート(and/or)フラッシュの確認
 * @param  hand   ソート済みの手札
 * @param  result 役を返す先への参照
 * @return        ストレート(and/or)フラッシュである場合true
 */
bool check_straight_flush(const Hand& hand, PokerHand& result)
{
  bool flush_flag = true;
  bool straight_flag = true;
  const size_t size = hand.size();
  for (size_t i = 1; (flush_flag || straight_flag) && i < size; i++) {
    if (hand.at(i).suit != hand.at(i - 1).suit) {
      flush_flag = false;
    }
    if (hand.at(i).value != hand.at(i - 1).value + 1) {
      straight_flag = false;
    }
  }

  if (straight_flag || flush_flag) {
    if (straight_flag && flush_flag) {
      // A(=14)で終わるストレートフラッシュは最強
      result.rank = hand.rbegin()->value == 14 ? PokerHandRank::ROYAL_FLUSH : PokerHandRank::STRAIGHT_FLUSH;
    }
    else if (straight_flag) {
      result.rank = PokerHandRank::STRAIGHT;
    }
    else {
      result.rank = PokerHandRank::FLUSH;
    }
    result.value = hand.rbegin()->value;
    return true;
  }
  return false;
}

/**
 * 同一値のカードによる役の確認（1,2ペア，スリーカード，フォーカード，フルハウス）
 * @param  hand   ソート済みの手札
 * @param  result 役を返す先への参照
 * @return        役を発見した場合true
 */
bool check_same_cards(const Hand& hand, PokerHand& result)
{
  size_t size = hand.size();
  for (size_t i = 0; i < size; i++) {  // カードを順番に注目
    // 注目カードと同じ値のカードを数える
    size_t count = 1;
    for (size_t j = i + 1; j < size; j++) {
      if (hand.at(j).value == hand.at(i).value) {
        count++;
      }
    }
    switch (count) {
      case 2:
        if (result.rank == PokerHandRank::THREE_OF_A_KIND) {
          // フルハウスではスリーカードの方の数値を記録するため，
          // 役の値はペアの値では書き換えない
          result.rank = PokerHandRank::FULL_HOUSE;
        }
        else if (result.rank == PokerHandRank::ONE_PAIR) {
          result.rank = PokerHandRank::TWO_PAIR;
          if (hand.at(i).value > result.value) {
            result.value = hand.at(i).value;
          }
        }
        else {
          result.rank = PokerHandRank::ONE_PAIR;
          result.value = hand.at(i).value;
        }
        break;

      case 3:
        if (result.rank == PokerHandRank::ONE_PAIR) {
          result.rank = PokerHandRank::FULL_HOUSE;
        }
        else {
          result.rank = PokerHandRank::THREE_OF_A_KIND;
        }
        result.value = hand.at(i).value;
        break;

      case 4:
        result.rank = PokerHandRank::FOUR_OF_A_KIND;
        result.value = hand.at(i).value;
        break;

      default: break;
    }
    // 注目カードと同じ値のカードを再度注目しないようにスキップする
    i += count - 1;
  }
  return result.rank == PokerHandRank::HIGH_CARD;
}

/**
 * 手札から役を作る
 * @param  hand 手札
 * @return      役
 */
PokerHand calc_rank(Hand& hand)
{
  std::sort(hand.begin(), hand.end());

  // 役が見つからない場合返される初期値
  PokerHand result {
    PokerHandRank::HIGH_CARD,
    hand.rbegin()->value     // 手札で最も大きい値
  };

  // ストレート(and/or)フラッシュ であるか確認
  if (check_straight_flush(hand, result)) {
    return result;
  }

  // 同一値のカード確認
  check_same_cards(hand, result);

  return result;
}

/**
 * ふたつの手札の役を比較，勝敗を判定する
 * @param  player1_hand プレイヤー1
 * @param  player2_hand プレイヤー2
 * @return              プレイヤー1が勝った場合true
 */
bool play_poker(Hand& player1_hand, Hand& player2_hand)
{
  auto p1_rank = calc_rank(player1_hand);
  auto p2_rank = calc_rank(player2_hand);

  // 役の強さで勝敗が決するパターン
  if (p1_rank.rank != p2_rank.rank) {
    return p1_rank.rank > p2_rank.rank;
  }
  // 同じ役だが役の値で勝敗が決するパターン
  if (p1_rank.value != p2_rank.value) {
    return p1_rank.value > p2_rank.value;
  }
  // ここまで同等なら手札の値で勝敗を決する
  int p1_best = 0;
  int p2_best = 0;
  for (size_t i = 0; i < player1_hand.size(); i++) {
    const auto& p1_value = player1_hand.at(i).value;
    const auto& p2_value = player2_hand.at(i).value;
    if (p1_value != p1_rank.value && p1_value > p1_best) {
      p1_best = p1_value;
    }
    if (p2_value != p2_rank.value && p2_value > p2_best) {
      p2_best = p2_value;
    }
  }
  return p1_best > p2_best;
}

/*****************************************************************************/

/**
 * カードの値を文字から数値に変換
 */
inline
int char2value(char c)
{
  switch (c) {
    case 'T': return 10;
    case 'J': return 11;
    case 'Q': return 12;
    case 'K': return 13;
    case 'A': return 14;
    default:
      if (c >= '0' && c <= '9') {
        return c - '0';
      }
      else {
        throw std::invalid_argument("invalid value");
      }
  }
}

/**
 * カードのスートを文字からSuit型に変換（要るか？という感じではある）
 */
inline
Suit char2suit(char c)
{
  switch (c) {
    case 'C': return Suit::CLOVER;
    case 'D': return Suit::DIA;
    case 'H': return Suit::HEART;
    case 'S': return Suit::SPADE;
    default: throw std::invalid_argument("invalid suit");
  }
}

int main(void)
{
  // それぞれ手札は5枚
  Hand player1(5);
  Hand player2(5);

  std::ifstream ifs("euler054.txt");
  if (ifs.fail()) {
    std::cout << "Euler054: cannot open the hands data: euler054.txt" << std::endl;
    return 1;
  }

  try {
    std::string buf;
    size_t count = 0;
    while (!ifs.eof()) {
      // 一行が1ゲームにおけるそれぞれの手札セット
      std::getline(ifs, buf);
      if (buf.size() == 0) { continue; }
      // 手札を作成
      for (size_t i = 0; i < 5; i++) {
        player1.at(i).value = char2value(buf.at(i * 3));
        player1.at(i).suit  = char2suit(buf.at(i * 3 + 1));

        player2.at(i).value = char2value(buf.at(15 + i * 3));
        player2.at(i).suit  = char2suit(buf.at(15 + i * 3 + 1));
      }
      // プレイヤー1の勝数が知りたい
      if (play_poker(player1, player2)) {
        count++;
      }
    }
    std::cout << "Euler054: " << count << std::endl;
    return 0;
  }
  catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    return 1;
  }
}
