#include "Token.h"

using namespace Token_kind;
using namespace prompt;

static bool is_kind(char c)
{
	if (!isalpha(c) && !isdigit(c)) return true;		// 非字母按特殊字符处理
	return Kinds.count(c);
}

// 是否是特殊字符
static bool is_spe_ch(char c)
{
	return !(isalnum(c) || c == '_');
}

// ----------------------------------- Token --------------------------------------------

// -------------------- pubilc -------------------------

Token::Token()
	:kind{ NULL_KIND }, val{}
{

}

Token::Token(char c)
	:kind{ c }, val{ "" }
{
	if (!is_kind(kind)) throw string("Token::Token:" + mark_char(c) + "illegal Token's kind");
}

Token::Token(const string& s)
	:kind{ ret_kind(s) }, val{ s }
{
	if (s.empty()) throw string("Token::Token: value cannot be empty except literal");
	if (s.length() == 1 && is_spe_ch(s[0])) 
        throw string("Token::Token: value.length == 1," + mark_string("Token{ char }") + "should be used");
}

Token::Token(char c, const string& s)
    :kind{ c }, val{ s }
{
    if (!is_kind(kind)) throw string("Token::Token:" + mark_char(c) + "illegal Token's kind");
}


// ------------------------------ 重载 -------------------------------------------

bool operator==(const Token& t1, const Token& t2)
{
	return (t1.kind == t2.kind && t1.val == t2.val);
}

bool operator!=(const Token& t1, const Token& t2)
{
	return !(t1 == t2);
}