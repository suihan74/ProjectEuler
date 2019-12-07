/**
 * Problem 99 「最大のべき乗」
 * 指数の形で表される2つの数, 例えば 211 と 37, の大小を調べることは難しくはない. 電卓を使えば, 211 = 2048 < 37 = 2187 であることが確かめられる.
 *
 * しかし, 632382518061 > 519432525806 を確認することは非常に難しい (両者ともに300万桁以上になる).
 *
 * 各行に1組が書かれている1000個の組を含んだ22Kのテキストファイル base_exp.txt から, 最大の数が書かれている行の番号を求めよ.
 *
 * 注: ファイル中の最初の二行は上の例である.
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdint>
#include <cmath>
#include <vector>

using uInt = std::uint_fast32_t;
using Real = std::double_t;

struct Data
{
    const uInt line;
    const uInt base;
    const uInt exp;

    explicit Data(uInt line, uInt base, uInt exp)
        : line(line), base(base), exp(exp)
        {}

    Real calcLog() const
    {
        return exp * std::log(base);
    }
};


std::vector<Data> load_data_file(const char* filename)
{
    std::vector<Data> result;
    std::ifstream ifs(filename);
    if (ifs.fail()) {
        throw std::invalid_argument("cannot open a datafile.");
    }

    try {
        std::string line;
        while (std::getline(ifs, line))
        {
            std::replace(line.begin(), line.end(), ',', ' ');
            std::istringstream line_stream(line);
            uInt base = 0, exp = 0;
            line_stream >> base >> exp;
            result.emplace_back(result.size() + 1, base, exp);
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


int main()
{
    auto data = load_data_file("euler099.txt");
    auto iter = std::max_element(
        data.begin(),
        data.end(),
        [](const Data& a, const Data& b) { return a.calcLog() < b.calcLog(); });

    std::cout << iter->line << " : " << iter->base << " ^ " << iter->exp << std::endl;
    return 0;
}
