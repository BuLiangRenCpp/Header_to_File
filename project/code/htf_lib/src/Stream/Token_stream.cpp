#include "Token_stream.h"

using namespace std;

// ***************************************
// -------------- 公用流 -----------------
Token_stream TS;

using namespace Token_kind;
using namespace prompt;


// 跳过与处理指令  
// --->  注: 对于 #include "A.h"，会将 A 作为自定义类型，开头字母大写
static void ignore_pretreat(istream& is)
{
	string s;
	Identifier i;
	is >> i;
	if (i.str() == "include") {
		Literal l;
		is >> l;
		if (!is.fail() && !l.empty() && isupper(l.str()[0])) {
			string name = path_deal::file_name(l.str(), false);
			if (usage::is_identifier(name)) TS.push_type(name);
		}
		is.clear();
	}
	if (is.peek() == '\n') return;
	getline(is, s);
}


// ------------------------------- Token_stream ----------------------------------------

// ------------------- public ---------------------

Token_stream::Token_stream()
	:_full{ false }, _buffer{ }, _types{ }, _line{ 1 }
{
	
}

bool Token_stream::eof(istream& is)
{
	return (peek(is).kind == NULL_KIND);
}

void Token_stream::sync()
{
	_full = false;
}

Token Token_stream::get(istream& is)
{
	if (_full) {
		_full = false;
		// ** 处理bug: peek会将标识符读到buffer，使得下一次读取标识符仍然是标识符，不能转为Type
		if (_buffer.kind == IDENTIFIER_KIND && _types.count(_buffer.val)) 
			_buffer.kind = TYPE_KIND;	
		return _buffer;
	}

	//if (is.eof()) return Token{ NULL_KIND };
	//if (is.peek() == '\n') _line++;		// 记录行数

	while (true) {		// 避免跳过空行，记录行数
		char c;
		is >> noskipws >> c;
		if (is.eof()) return Token{};
		if (c == '\n') _line++;
		if (!isspace(c)) {
			is.putback(c);
			break;
		}
	}

	Identifier i;
	is >> i;
	if (i.empty()) {		// 不是标识符，则看做特殊字符、Literal类
		char c = NULL_KIND;	
		my_std::getchar(is, c);

		// ** 忽略注释
		if (c == '/') {
			if (is.peek() == '/') {
				_ignore_note(is);
				return this->get(is);
			}
			if (is.peek() == '*') {
				_ignore_note(is, true);
				return this->get(is);
			}
		}
		// 处理与处理指令
		if (c == '#') {
			ignore_pretreat(is);
			return this->get(is);
		}
		// 处理 Literal
		if (c == '-' || isdigit(c) || c == '\'' || c == '\"') {
			is.putback(c);
			Literal t;
			is >> t;
			if (is.fail()) {
				is.clear();
				return Token{ c };
			}
			return this->get(is);
		}
		return Token{ c };		// 特殊字符
	}
    // 先查询是否是存在 _types 中
	if (_types.count(i.str())) return Token{ TYPE_KIND, i.str() };	
	return Token{ i.str() };
}

void Token_stream::putback(Token t)
{
	if (t.kind == NULL_KIND) throw string("Token_stream::putback: 参数不能为 空Token");
	if (_full) throw string("Token_stream::putback: buffer is full");
	_buffer = t;
	_full = true;
}

void Token_stream::ignore(istream& is, Token target)
{
	if (_full && (_buffer == target)) {		// 先查看 buffer
		_full = false;
		return;
	}
	_full = false;

	while (TS.peek(is).kind != NULL_KIND && TS.peek(is) != target)
		this->get(is);
}

void Token_stream::ignore_between_bracket(istream& is)		// 主要用于跳过参数默认值中的用括号初始化的值或函数定义语句
{
	if (_full) {
		if (!usage::is_lbracket(_buffer.kind)) return;
		is.putback(_buffer.kind);
		_full = false;
	}
	usage::ignore_between_bracket(is);
}

void Token_stream::ignore_default(istream& is)
{
	Token peek = this->peek(is);
	if (peek.kind == NULL_KIND) return;
    // 1. 默认值为 Literal 情况已经被 this->get(is) 忽略了
    // 2. 针对  (A a = A{}) 情况
	else if (usage::is_identifier(peek.val)) {	
		while (this->peek(is).kind != ',' && this->peek(is).kind != ')') {		// 参数默认值后应该是 , 或者 )
			this->get(is);
			if (this->peek(is) == NULL_KIND) return;
		}
	}
	else if (usage::is_lbracket(peek.kind)) this->ignore_between_bracket(is);
	else {
		string s = "Token{" + peek.kind;
		s += ", \"" + peek.val + "\"}";
		throw string("Token_stream::ignore_default: 语法错误 -> " + mark_string(s) + "不能作为参数默认值");
	}
}

Token Token_stream::peek(istream& is)
{
	if (_full) return _buffer;

	_buffer = this->get(is);
	_full = (_buffer.kind == NULL_KIND) ? false : true;
	return _buffer;
}

void Token_stream::push_type(const string& s)
{
	if (!usage::is_identifier(s))
		throw string("Token_stream::push_type: 语法错误 -> " + mark_string(s) + "非法标识符不能作为类型");
	_types.emplace(s);
}

int Token_stream::line() const
{
	return _line;
}

void Token_stream::clear()
{
	TS = Token_stream();
}

// ------------------- private -----------------------

void Token_stream::_ignore_note(istream& is, bool flag)
{
	if (flag == false) {		// 行注释
		string s;
		getline(is, s);
		TS._line++;
	}
	else {		 // 块注释
		char c;
		while (true) {
			if (is.peek() == '\n') TS._line++;
			is >> noskipws >> c;
			if (is.eof() || (c == '*' && is.peek() == '/')) {
				is >> c;
				return;
			}
		}
	}
}