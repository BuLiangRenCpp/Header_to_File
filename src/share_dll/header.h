#pragma once

#include <fstream>
#include <iostream>
#include <sstream>

#include <queue>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <climits>
#include <functional>
#include <regex>

namespace output
{
}

namespace htf
{
#define _LINE output::mark("line " + to_string(__LINE__)) + "-> "
using line_t = unsigned long long;   // 记录文件行数

#ifdef _WIN32
const std::string PROGRAM_NAME = "htf.exe";
#else
const std::string PROGRAM_NAME = "htf";
#endif

namespace excep
{
};
namespace path_deal
{
class Hpath;
};
namespace stream
{
namespace Token_kind
{
}
namespace usage
{
}
}   // namespace stream
namespace pre
{
// vector's element: first->whether is source，second->path
using Reliance = std::vector<std::vector<std::pair<bool, path_deal::Hpath>>>;
}   // namespace pre
namespace lex
{
namespace Lexer_kind
{
}
namespace usage
{
}
}   // namespace lex
namespace parse
{
}
// !!!!!!!! don't forget: preprocess --> namespace pre !!!!!!!!!!!!!
// ! should delete ...
namespace core
{
// vector's element: first->whether is source，second->path
using Reliance = std::vector<std::vector<std::pair<bool, path_deal::Hpath>>>;
}   // namespace core
namespace arugment
{
}
}   // namespace htf


// ********************************************************************************
namespace my_std
{
// ***** 由于在程序中使用了noskipws可能有时会读取空格，因此采用此方式读取字符 *****
inline void getchar(std::istream& is, char& c)
{
    while (is >> c) {
        if (!isspace(c)) return;
    }
}

template<typename T>
bool is_in(const T& t, const std::vector<T>& ts)
{
    for (const T& x : ts)
        if (x == t) return true;
    return false;
}

template<typename T>
bool index(const T& t, const std::vector<T>& ts)
{
    int i = ts.size() - 1;
    for (; i >= 0; i--)
        if (ts[i] == t) break;
    return i;
}

template<typename T>
std::vector<T> unite(const std::vector<T>& t1, const std::vector<T>& t2)
{
    auto res = t1;
    for (const auto& t : t2) {
        if (!is_in(t, res)) res.emplace_back(t);
    }
    return res;
}
}   // namespace my_std
// *****************************************************************************
