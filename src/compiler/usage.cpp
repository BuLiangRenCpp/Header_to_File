#include "usage.h"
#include "assertions.h"
#include "output.h"

#include <regex>
#include <unordered_set>


using namespace std;

const unordered_set<string> KeyWords{
    "auto",     "bool",      "char",      "char8_t",  "char16_t", "char32_t", "class",
    "const",    "constexpr", "decltype",  "default",  "delete",   "double",   "enum",
    "explicit", "export",    "extern",    "float",    "friend",   "inline",   "int",
    "long",     "mutable",   "namespace", "new",      "noexcept", "nullptr",  "operator",
    "private",  "protected", "public",    "short",    "signed",   "static",   "struct",
    "template", "throw",     "try",       "typedef",  "typename", "union",    "unsigned",
    "using",    "virtual",   "void",      "volatile", "wchar_t",  "finally"};

/**
 * *  >> 不能当做 Dchars 处理，否则造成 vector<vector<int'>>' 报错
 * * 其他一些不纳入双字符(比如 ++、-- ...)是因为目前看来没有必要
 */
const unordered_set<string> Dchars{"::", "->", "<<"};


namespace htf
{

bool is_lbracket(const std::string& ch)
{
    return ch.length() == 1 && is_lbracket(ch[0]);
}

bool is_rbracket(const std::string& ch)
{
    return ch.length() == 1 && is_rbracket(ch[0]);
}

bool is_bracket(const std::string& ch)
{
    return ch.length() == 1 && is_bracket(ch[0]);
}

char ret_lbracket(const std::string& ch)
{
    HTF_DEV_ASSERT_MESSAGE(is_rbracket(ch), mark(ch) << " isn't a right bracket");
    return ret_lbracket(ch[0]);
}

char ret_rbracket(const std::string& ch)
{
    HTF_DEV_ASSERT_MESSAGE(is_lbracket(ch), mark(ch) << " isn't a left bracket");
    return ret_rbracket(ch[0]);
}

bool is_dchar(const string& s)
{
    return Dchars.count(s);
}

bool is_dchar_fir(char c)
{
    for (auto x : Dchars)
        if (c == x[0]) return true;
    return false;
}

bool is_identifier(const string& temp)
{
    string pattern = "^[a-zA-Z_](?:[a-zA-Z0-9_]+)*";
    regex  reg(pattern);
    return (regex_match(temp, reg));
}

bool is_keyword(const std::string& str)
{
    return KeyWords.count(str);
}

}   // namespace htf
