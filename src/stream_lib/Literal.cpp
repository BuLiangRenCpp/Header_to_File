#include "Literal.h"

using namespace std;

// 读取直到遇见 target，不跳过空格,，用于读取 '' 与 "" 之间的 Literal
static string get_literal(istream& is, char target, htf::line_t& line)
{
    string s;
    char   c;
    while (is >> noskipws >> c) {
        if (c == '\n') line++;                                         // 文本看不见的换行 (看得见的换行 \n 不需要处理)
        if (c == '\\' && (is.peek() == '\'' || is.peek() == '\"')) {   // ** 处理转义  (注: 文本格式 \" 是两个字符)
            s += c;
            is >> c;
            s += c;
            continue;
        }
        if (c == target) {   // * 保留 ' or "
            s += c;
            break;
        }
        s += c;
    }
    return s;
}

namespace htf
{
namespace stream
{
Literal::Literal()
    : _val{""}
    , _line{0}
{
}

Literal::Literal(const string& s, line_t line)
    : _val{s}
    , _line{line}
{
}

string Literal::str() const
{
    return _val;
}

bool Literal::empty() const
{
    return _val.empty();
}

line_t Literal::line() const
{
    return _line;
}

istream& operator>>(istream& is, Literal& l)
{
    // 1. num  (为了处理其他进制，按 string 处理)
    if (is.peek() == '-' || isdigit(is.peek())) {
        double d = 0.0;
        is >> d;
        if (is.fail()) {
            l = Literal{};
            is.clear();
            return is;
        }
        l = Literal{to_string(d)};
        return is;
    }
    // 2. '' or ""
    line_t line = 0;
    string s;
    char   c;
    my_std::getchar(is, c);
    if (c != '\"' && c != '\'') {
        is.putback(c);
        l = Literal{};
        return is;
    }
    else
        s = get_literal(is, c, line);
    s = s.back() + s;   // * 保留 ' or "
    l = Literal{s, line};
    return is;
}

ostream& operator<<(ostream& os, const Literal& l)
{
    os << l.str();
    return os;
}
}   // namespace stream
}   // namespace htf