#include "Fun_name.h"

using namespace prompt;
using namespace Token_kind;

// --------------------------------- Name类 -----------------------------------------

// ---------------- pubilc ----------------------

Fun_name::Fun_name()
{

}

Fun_name::Fun_name(const Identifier& i)
	:_name{ i.str() }
{

}

Fun_name::Fun_name(const string& s)
	:_name{ s }
{
	if (s.find("operator") != string::npos) {		// 1. 重载运算符函数名
		string temp = s;
		if (temp == "operator") throw string("Fun_name::Fun_name: 语法错误 -> " + mark_string(temp) + "缺少运算符");
	}
	else {		// 2. 一般函数名
		if (!usage::is_identifier(s)) 
			throw string("Fun_name::Fun_name: 语法错误 -> " + mark_string(s) + "不是合法的标识符，不能作为函数名");
	}
}

string Fun_name::str() const
{
	return _name;
}


bool Fun_name::empty() const
{
	return _name.empty();
}


// ------------------------ 重载 -----------------------------

istream& operator>>(istream& is, Fun_name& i)
{
	Token token;
	token = TS.get(is);
	if (token.val == "operator") {		// 1. 重载运算符函数名
		string name = token.val;
		while (TS.peek(is).kind != NULL_KIND && TS.peek(is).kind != '(') {
			token = TS.get(is);
			if (token.val != "") 
				throw string("Fun_name.cpp:operator>>: 语法错误 -> " + mark_string(name) + "缺少运算符，" 
							+ mark_string(token.val) +"不能作为运算符");
			name.push_back(token.kind);
		}
		i = Fun_name{ name };
		return is;
	}
	if (token.kind == NULL_KIND || token.kind == IDENTIFIER_KIND) {		// 2. 一般函数名
		Identifier temp{ token.val };
		i = Fun_name{ temp };
		return is;
	}
	// 3. 不是函数名
	i = Fun_name{};		// 注意：将其转为 default
	TS.putback(token);
	return is;
}

ostream& operator<<(ostream& os, const Fun_name& i)
{
	os << i.str();
	return os;
}