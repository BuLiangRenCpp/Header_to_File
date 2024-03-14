#include "Fun_arg.h"

using namespace prompt;


// ----------------------------------- Fun_arg --------------------------------------

// ------------------ pubilc ---------------------

Fun_arg::Fun_arg()
	: _type{}, _name{}
{

}

Fun_arg::Fun_arg(const Type& t, const Identifier& i)
	:_type{ t }, _name{ i }
{
	if (_type.empty() && !_name.empty()) throw string("Fun_arg::Fun_arg:" + mark_string(_name.str()) + "没有类型");
}

string Fun_arg::str() const
{
	if (this->empty()) return "";
	return (_type.str() + " " + _name.str());
}

bool Fun_arg::empty() const
{
	return _type.empty() && _name.empty();
}

// -------------------------------------------------------------------

vector<Fun_arg> get_pars(istream& is, const string& name)
{
	vector<Fun_arg> ps;
	Token token = TS.get(is);
	if (token.kind != '(') return ps;
	while (true) {
		Fun_arg p;
		is >> p;
		if (p.empty()) {		// 说明参数为空，或参数格式不对，或者此时程序无法处理的情况
			if (TS.peek(is).kind == ')') {
				TS.get(is);
				break;		// --> 正确结束
			}
			is.setstate(ios_base::failbit);			// ***** 错误结束，格式有问题
			return ps;
		}
		else ps.emplace_back(p);
		token = TS.get(is);
		if (token.kind == ',') continue;
		if (token.kind == ')') break;		// 正确结束
		throw string("Function.cpp::operator>>: 语法错误 -> " + mark_string(name) + "的参数列表缺少" + mark_char(','));
	}
	return ps;
}


// ------------------------ 重载 -----------------------------

istream& operator>>(istream& is, Fun_arg& p)
{
	// Type (Identifier) ( = val)
	Type t;
	is >> t;
	if (t.empty()) {
		p = Fun_arg{};
		return is;
	}

	Identifier i;
	Token token = TS.get(is);
	if (token.kind == Token_kind::IDENTIFIER_KIND) {
		i = Identifier{ token.val };
		token = TS.get(is);
	}
	// Fun_arg 应以 "," 或 ")" 结束
	if (token.kind == '=') {		// 忽略默认参数
		TS.ignore_default(is);
		if (TS.peek(is).kind != ',' && TS.peek(is).kind != ')')
			throw string("Fun_arg.cpp::operator>>: 语法错误 -> 默认参数" + mark_string(t.str() + " " + i.str()) + "默认值格式有误");
	}
	else if (token.kind != ',' && token.kind != ')') {
		TS.putback(token);
		p = Fun_arg{};
		return is;
	}
	else if (token.kind == ',' || token.kind == ')') TS.putback(token);

	p = Fun_arg{ t, i };
	return is;
}

ostream& operator<<(ostream& os, const Fun_arg& p)
{
	os << p.str();
	return os;
}