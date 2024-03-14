#include "Literal.h"


// ----------------------------------- Literal类 ------------------------------------------------

// ----------------- public ---------------------

Literal::Literal()
	:_val{ "" }
{

}

Literal::Literal(const string& s)
	:_val{ s }
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


// ------------------------ 重载 -----------------------------
	
// 读取直到遇见 target，不跳过空格,，用于读取 '' 与 "" 之间的 Literal
static string get_literal(istream& is, char target)
{
	string s;
	char c;
	while (is >> noskipws >> c) {
		if (c == '\\' && (is.peek() == '\'' || is.peek() == '\"')) {		// ** 处理转义  (注: 文本格式 \" 是两个字符)
			s += c;
			is >> c;
			s += c;
			continue;
		}
		if (c == target) break;		
		s += c;
	}
	return s;
}


istream& operator>>(istream& is, Literal& l)
{
	// 1. num
	double d = 0.0;
	is >> d;
	if (!is.fail()) {
		l = Literal{ to_string(d) };
		return is;
	}
	// 2. '' or ""
	is.clear();
	string s;
	char c;
	my_std::getchar(is, c);
	if (c != '\"' && c != '\'') {
		is.putback(c);
		is.setstate(ios_base::failbit);			// **** 不是 Literal，则设置 is.fail = true
		l = Literal{};
		return is;
	}
	else s = get_literal(is, c);
	l = Literal{ s };
	return is;
}

ostream& operator<<(ostream& os, const Literal& l)
{
	os << l.str();
	return os;
}