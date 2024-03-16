#include "Class_fun.h"

using namespace prompt;
using namespace Token_kind;


static const string Class_Str_Sta = "// --------------------------------------- ";
static const string Class_Str_End = " ------------------------------------------";
static const string Print_Str_Sta = "// -------------------- ";
static const string Print_Str_End = " -----------------------";


// ------------------------------------- Class_fun类 --------------------------------------------------

// -------------------- public -------------------------

Class_fun::Class_fun()
{

}

Class_fun::Class_fun(const string& n, const vector<vector<Function>>& t)
	:_name{ n }, _funs{ t }
{
	if (t.size() != 3) 
		throw string("Class_fun::Class_fun: 参数 vector<vector<Function>*> size 必须为 3");
	if (n.empty() && (!t[0].empty() || !t[1].empty() || !t[2].empty())) 
		throw string("Class_fun::Class_fun: 类名不能为空");
	if (!usage::is_identifier(n)) throw string("Class_fun::Class_fun:" + mark_string(n) + "不是合法的类名");
}

string Class_fun::str(unsigned int count) const
{
	if (this->empty()) return "";
	string res = Class_Str_Sta + _name + Class_Str_End + "\n\n";
	if (!_funs[0].empty()) {
		res += Print_Str_Sta + "public" + Print_Str_End + "\n\n";
		for (auto i = _funs[0].begin(); i != _funs[0].end(); i++)
			res += i->str_class(_name, count) + "\n";
	}
	if (!_funs[1].empty()) {
		res += Print_Str_Sta + "protected" + Print_Str_End + "\n\n";
		for (auto i = _funs[1].begin(); i != _funs[1].end(); i++)
			res += i->str_class(_name, count) + "\n";
	}
	if (!_funs[2].empty()) {
		res += Print_Str_Sta + "private" + Print_Str_End + "\n\n";
		for (auto i = _funs[2].begin(); i != _funs[2].end(); i++)
			res += i->str_class(_name, count) + "\n";
	}
	return res;
}

bool Class_fun::empty() const
{
	return _name.empty();
}

void Class_fun::print(ostream& os, unsigned int count) const
{
	os << this->str(count);
}



// ------------------------ 重载 -----------------------------

// 将两个vector拼接
template <typename T>

static void vector_insert(vector<T>& t1, const vector<T>& t2)
{
	for (auto t : t2) t1.emplace_back(t);
}

// 读取类的 public 或者 private 中的函数
static vector<Function> get_function(istream& is)
{
	vector<Function> res;
	Function f;
	while (true) {
		is >> f;
		if (f.empty()) {
			if ((TS.peek(is).val == "private" || TS.peek(is).val == "public" ||
				TS.peek(is).val == "protected" || TS.peek(is).kind == '}')) break;
			TS.ignore(is, Token{ ';' });
			continue;
		}
		else res.emplace_back(f);
	}
	return res;
}

// (pub, pro, pri)
// is_class = true: class关键字定义的类
// 读取 {} 中的函数，留下类结尾的 ';'   --->   因为有的可能有变量名
static vector<vector<Function>> get_functions(istream& is, bool is_class)
{
	vector<Function> pub, pro, pri;
	vector<vector<Function>> res;
	Token token = TS.get(is);
	while (token.kind != NULL_KIND && token.kind != '}') {
		if (token.val == "public") {	// 1. public
			if (TS.peek(is).kind != ':')
				throw string("Class_fun.cpp::get_functions:" + mark_string(token.val) + "后缺少" + mark_char(':'));
			token = TS.get(is);
			vector_insert(pub, get_function(is));
		}
		else if (token.val == "protected") {		// 2. protected
			if (TS.peek(is).kind != ':')
				throw string("Class_fun.cpp::get_functions:" + mark_string(token.val) + "后缺少" + mark_char(':'));
			token = TS.get(is);
			vector_insert(pro, get_function(is));
		}
		else if (token.val == "private") {	  // 3. private
			if (TS.peek(is).kind != ':')
				throw string("Class_fun.cpp::get_functions_struct:" + mark_string(token.val) + "后缺少" + mark_char(':'));
			token = TS.get(is);
			vector_insert(pri, get_function(is));
		}
		else {		// 3. 默认
			TS.putback(token);
			if (is_class) vector_insert(pri, get_function(is));
			else vector_insert(pub, get_function(is));
		}
		token = TS.get(is);
	}
	res.emplace_back(pub);
	res.emplace_back(pro);
	res.emplace_back(pri);
	return res;
}


istream& operator>>(istream& is, Class_fun& c)
{
	Token token = TS.get(is);		// 1. class、struct
	if (token.kind == NULL_KIND) {
		c = Class_fun{};
		return is;
	}
	if (!usage::is_class_key(token.val)) {
		TS.putback(token);
		c = Class_fun{};
		return is;
	}
	string class_key = token.val;

	token = TS.get(is);
	string name;		// 2. 类名  -->  可以是 标识符，也可以是自定义类型  ** (处理一个类分开写成两个的情况）
	if (token.kind == IDENTIFIER_KIND || (token.kind == TYPE_KIND && !usage::is_type_key(token.val))) {			
		name = token.val;
		TS.push_type(name);		// ** 自定义类型
	}
	else {		// 语法错误
		TS.putback(token);
		c = Class_fun{};
		return is;
	}

	//if (TS.peek(is).kind != '{')	  // 3. '{'
	//	throw string("Class_fun.cpp:operator>>: 语法错误 -> " + mark_string(name) + "类后缺少" + mark_char('{'));
	// 3. 继承形式   class A :pubilc B {...};
	TS.ignore(is, Token{ '{' });

	vector<vector<Function>> funs;		// 4. { } 中
	if (class_key == "struct") funs = get_functions(is, false);
	else if (class_key == "class") funs = get_functions(is, true);

	// 5. 剩下 ';'，可能会有变量名，直接忽略到 ';'
	if (TS.peek(is).kind != IDENTIFIER_KIND && TS.peek(is).kind != ';')
		throw string("Class_fun.cpp::operator>>: 语法错误 -> " + mark_string(name) + "类语句结尾语法错误，缺少" + mark_char(';'));
	TS.ignore(is, Token{ ';' });
	c = Class_fun{ name, funs };
	return is;
}

ostream& operator<<(ostream& os, const Class_fun& c)
{
	os << c.str();
	return os;
}