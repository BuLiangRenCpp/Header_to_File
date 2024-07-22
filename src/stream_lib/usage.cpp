#include "usage.h"
#include "Excep_dev.h"
#include "Token_kind.h"
#include "output.h"

using namespace std;
using namespace output;

const unordered_set<char> Lbrackets{'(', '[', '{', '<'};   // 左括号集
const unordered_set<char> Rbrackets{')', ']', '}', '>'};   // 右括号集

/**
 * * << 与 >> 不能当做 Dchars 处理，否则造成 vector < vector < int '>>' 报错
 */
const unordered_set<string> Dchars{"::", "->"};


namespace htf
{
namespace stream
{
using namespace Token_kind;
namespace usage
{
bool is_lbracket(const char& c)
{
    return Lbrackets.count(c);
}

bool is_rbracket(const char& c)
{
    return Rbrackets.count(c);
}

bool is_bracket(char c)
{
    return is_lbracket(c) || is_rbracket(c);
}

char ret_lbracket(char c)
{
    switch (c) {
    case ')':
        return '(';
    case ']':
        return '[';
    case '}':
        return '{';
    case '>':
        return '<';
    default:
        throw htf::excep::Excep_dev("usage.cpp::ret_rbracket", _LINE + mark(c) + "不是右括号");
    }
    return ' ';
}

char ret_rbracket(char c)
{
    switch (c) {
    case '(':
        return ')';
    case '[':
        return ']';
    case '{':
        return '}';
    case '<':
        return '>';
    default:
        throw htf::excep::Excep_dev("usage.cpp::ret_rbracket", _LINE + mark(c) + "不是左括号");
    }
    return ' ';
}

void delete_space_pre_suf(string& s)
{
    // suffix
    int len = static_cast<int>(s.length());
    int i   = len - 1;
    for (; i >= 0; i--) {
        if (s[i] != ' ') break;
        s.pop_back();
    }
    // prefix
    int count = 0;
    for (int j = 0; j < i; j++) {
        if (s[j] != ' ') break;
        count++;
    }
    s.erase(0, count);
}

bool is_dchar(const string& s)
{
    return Dchars.count(s);
}

bool is_identifier(const string& temp)
{
    string pattern = "^[a-zA-Z_](?:[a-zA-Z0-9_]+)*";
    regex  reg(pattern);
    return (regex_match(temp, reg));
}

char ret_kind(const string& s)
{
    if (is_identifier(s)) return IDENTIFIER_KIND;
    if (is_dchar(s)) return DCHAR_KIND;
    return LITERAL_KIND;
}
}   // namespace usage
}   // namespace stream
}   // namespace htf
