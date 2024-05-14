#pragma once

#include <iostream>
#include <sstream>
#include <fstream>

#include <string>
#include <vector>
#include <utility>
#include <stack>
#include <queue>
#include <unordered_set>
#include <unordered_map>

#include <regex>
#include <functional>
#include <climits>

namespace output { }

namespace htf {
    #define _LINE output::mark_string("line " + to_string(__LINE__)) + "-> "

	using line_t = unsigned long long;      // 记录文件行数

    namespace stream { }
    namespace exception { }
    namespace path_deal { }
    namespace argument { 
        namespace args_judge { }
        namespace args_const { }
    }
    namespace lex { }
    namespace parse { }
    namespace core { }
}


// ***** 由于在程序中使用了noskipws可能有时会读取空格，因此采用此方式读取字符 *****
namespace my_std{
    inline void getchar(std::istream& is, char& c)
    {
        while (is >> c) {
            if (!isspace(c)) return; 
        }
    }

    template <typename T>
    bool is_in(const T& t, const std::vector<T>& ts)
    {
        for (const T& x : ts)
            if (x == t) return true;
        return false;
    }

    template <typename T>
    bool index(const T& t, const std::vector<T>& ts)
    {
        int i = ts.size() - 1;
        for (; i >= 0; i--) 
            if (ts[i] == t) break;
        return i;
    }

    template <typename T>
    std::vector<T> unite(const std::vector<T>& t1, const std::vector<T>& t2)
    {
        auto res = t1;
        for (const auto& t : t2) {
            if (!is_in(t, res)) res.emplace_back(t);
        }
        return res;
    }
}
// *****************************************************************************
