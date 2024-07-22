#include "Token.h"
#include "Excep_dev.h"
#include "Token_kind.h"
#include "output.h"
#include "usage.h"

using namespace std;
using namespace output;

static bool iskind(char c)
{
    if (!isalnum(c)) return true;   // 非字母数字按特殊字符处理
    return htf::stream::Token_kind::Kinds.count(c);
}


namespace htf
{
namespace stream
{
using namespace Token_kind;
using namespace usage;
using namespace excep;

Token::Token()
    : kind{NULL_KIND}
    , val{}
{
}

Token::Token(char c)
    : kind{c}
    , val{}
{
    if (!iskind(kind)) throw Excep_dev("Token::Token(char)", _LINE + mark(c) + "illegal Token's kind");
    if (c != NULL_KIND) val += c;
}

Token::Token(const string& s)
    : kind{ret_kind(s)}
    , val{s}
{
    if (s.empty()) throw Excep_dev("Token::Token(const string&)", _LINE + "value cannot be empty");
    if (s.length() == 1 && is_spe_ch(s[0]))
        throw Excep_dev("Token::Token(const string&)", _LINE + "value.length == 1," + mark("Token{ char }") + "should be used");
}

Token::Token(char c, const string& s)
    : kind{c}
    , val{s}
{
    if (!iskind(kind)) throw Excep_dev("Token::Token(char, const string&)", _LINE + mark(c) + "illegal Token's kind");
}

bool Token::empty() const
{
    return kind == NULL_KIND && val.empty();
}

bool operator==(const Token& t1, const Token& t2)
{
    return (t1.kind == t2.kind && t1.val == t2.val);
}

bool operator!=(const Token& t1, const Token& t2)
{
    return !(t1 == t2);
}
}   // namespace stream
}   // namespace htf