#include "Function.h"

using namespace std;
using namespace prompt;

static const string FUN_END_STR = "\n{\n\n}\n";


static bool is_construct_fun(const Type& t, const vector<Fun_arg>& ps)
{
	// ~A();
	if (t.empty()) return false;		
	if (!t.empty() && !ps.empty()) return false;		// 空函数
	return true;
}

// ------------------------------------- Function类 ------------------------------------------------

// ------------------ public -------------------------

Function::Function()
	:_have_const{ false }, _is_construct{ false }
{

}

Function::Function(const Type& t, const Fun_name& n, const vector<Fun_arg>& ps, bool have_const, bool is_construct)
	:_type{ t }, _name{ n }, _pars{ ps }, _have_const{ have_const }, _is_construct{ is_construct }
{
	// 1. 空函数
	if (t.empty() && n.empty() && ps.empty()) {
		if (have_const == false && is_construct == false) return;
		if (have_const == true) throw string("Function::Function: 空函数的_bave_const = false");
		if (is_construct) throw string("Function::Function: 空函数不能作为析构函数");
	}
	// 2. 特殊函数
	if (n.empty()) {
		if (is_construct) {		// 2.1 析构函数
			if (!is_construct_fun(t, ps)) 
				throw string("Function::Funtion(for developer->is_construct 参数应该为 false): 析构函数不能有参数");
		}
		else {		// 2.2 构造函数
			if (t.empty()) throw string("Function::Funtion: 构造函数缺少类型");
		}
		if (have_const) throw string("Function::Function: 构造、析构函数不能设为常量函数");
	}
	// 3. 一般函数
	else {
		if (is_construct) throw string("Function::Funtion: 析构函数不能有函数名");
		if (t.empty()) throw string("Function::Function: 缺少函数类型");
	}
}

Type Function::type() const
{
	return _type;
}

string Function::str(unsigned int count) const
{
	if (this->is_sepcial())
		throw string("Function::str: 此函数是特殊函数(类的构造函数或者析构函数)，请使用" + mark_string("str_class()") + "调用");
	if (this->empty()) return "";
	string ps;
	int size = static_cast<int>(_pars.size());
	for (int i = 0; i < size; i++) {		// par1, par2, pa3...
		ps += _pars[i].str();
		if (i < size - 1) ps += ", ";
	}
	string const_str = (_have_const) ? "const" : "";
	string indentation(count, '\t');
	string res = (indentation + _type.str() + " " + _name.str() + "(" + ps + ")" + " " + const_str + "\n");
	res += indentation + "{" + "\n\n" + indentation + "}\n";
	return res;
}

string Function::str_class(const string& class_name, unsigned int count) const
{
	if (this->empty()) return "";
	string ps;
	int size = static_cast<int>(_pars.size());
	for (int i = 0; i < size; i++) {		// par1, par2, pa3...
		ps += _pars[i].str();
		if (i < size - 1) ps += ", ";
	}
	string indentation(count, '\t');
	string res = indentation;
	if (this->is_sepcial()) {
		string t = (_is_construct) ? "~" + _type.str() : _type.str();
		res = class_name + "::" + t + "(" + ps + ")" + " ";
	}
	else {
		string const_str = (_have_const) ? "const" : "";
		res = _type.str() + " " + class_name + "::" + _name.str() + "(" + ps + ")" + " " + const_str;
	}
	res += "\n" + indentation + "{" + "\n\n" + indentation + "}\n";
	return res;
}

bool Function::empty() const
{
	return _type.empty();
}

bool Function::is_sepcial() const
{
	return (!this->empty()) && _name.empty();
}

void Function::print(ostream& os, unsigned int count) const
{
	os << this->str(count);
}

// ------------------------ 重载 -----------------------------

istream& operator>>(istream& is, Function& f)
{
	bool is_construct = false;
	if (TS.peek(is).kind == '~') {		// 1. ~
		TS.get(is);
		is_construct = true;
	}

	Type t;		// 2. Type
	is >> t;
	if (t.empty()) {
		f = Function{};
		return is;
	}

	Fun_name n;
	is >> n;		// 3. Fun_name

	if (TS.peek(is).kind != '(') {		// 3. "("
		f = Function{};
		return is;
	}

	vector<Fun_arg> ps = get_pars(is, n.str());			// 4. Pars      // 5. ")"
	if (is.fail()) {
		is.clear();		// ****** 注意
		f = Function{};
		return is;
	}

	Token token = TS.get(is);		// 6. 是否有const
	bool have_const = false;
	if (token.val == "const") {
		have_const = true;
		token = TS.get(is);		// 7. ";" 或 "{"
	}
	// -----> 7. ";" 或 "{"
	if (token.kind == '{') {		// 忽略函数定义语句
		TS.putback(token);
		TS.ignore_between_bracket(is);
		TS.putback(Token{ ';' });		// --> 用于 htf.cpp文件清除不是定义语句的语句 --> 调用 TS.ignore(ifs, Token{ ';' })
		f = Function{};
		return is;
	}
	if (token.kind != ';') {
		TS.putback(token);
		f = Function{};
		return is;
	}
	f = Function{ t, n, ps, have_const, is_construct };
	return is;
}

ostream& operator<<(ostream& os, const Function& f)
{
	if (f.is_sepcial()) os << f.str_class(f.type().str());
	else os << f.str();
	return os;
}

