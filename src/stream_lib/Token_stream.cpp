#include "Token_stream.h"
#include "Excep_dev.h"
#include "usage.h"
#include "Literal.h"
#include "Identifier.h"
#include "output.h"

using namespace std;
using namespace output;

namespace htf {
	using namespace excep;
	namespace stream {
		Token_stream::Token_stream(istream& is)
			:_is{ is }, _buffer{ }, _line{ 1 }
		{
			
		}

		bool Token_stream::eof()
		{
			return (this->peek().kind == Token_kind::NULL_KIND);
		}

		void Token_stream::sync()
		{
			_buffer.sync();
		}

		void Token_stream::putback(const Token& t)
		{
			if (t.kind == Token_kind::NULL_KIND) 
				throw Excep_dev("Token_stream::putback", _LINE + "don't putback a empty Token");
			_buffer.putback(t);
		}

		void Token_stream::ignore(const Token& target)
		{
			this->ignore_until(target);
			this->get();
		}

		void Token_stream::ignore_until(const Token& token)
		{
			if (this->eof())  
				return;
			if (_buffer.empty()) this->peek();	// * 会将下一个token提到buffer中，因此只需要判断 buffer 即可 
			if (_buffer.peek() == token) {	 // * get -> 将 eof 提到 buffer 中的 token 删除
				return;
			}
			_buffer.get();
			this->ignore_until(token);
		}

		void Token_stream::ignore_line()
		{
			string line;
			getline(_is, line);
			_line++;
			_buffer.clear();
		}

		void Token_stream::ignore_between_bracket()
		{
			if (usage::is_lbracket(this->peek().kind)) {
				stack<Token> stk;
				Token token = this->get();
				stk.push(token);
				Token rbra = Token{ usage::ret_rbracket(token.kind) };
				while (!this->eof() && !stk.empty()) {
					token = this->get();
					if (token == rbra) stk.pop();
					else if (token == stk.top()) stk.push(token);
				}
			}
		}

		Token Token_stream::get()
		{
			if (!_buffer.empty()) 
				return _buffer.get();
			_space_update_line();

			// 1. 标识符
			Identifier i;
			_is >> i;
			// 2. 不是标识符，则可能是特殊字符、Literal、Dchar、注释中其中一个
			if (i.empty()) {		
				char c = Token_kind::NULL_KIND;	
				my_std::getchar(_is, c);

				// * i. 忽略注释
				if (c == '/') {
					if (_is.peek() == '/') {
						_ignore_note();
						return this->get();
					}
					if (_is.peek() == '*') {
						_ignore_note(true);
						return this->get();
					}
				}

				// * ii. 处理 Literal
				if ((c == '-' && isdigit(_is.peek())) || isdigit(c) || c == '\'' || c == '\"') {	// ! 为了处理 #include "file_path"
					_is.putback(c);
					Literal t;
					_is >> t;
					if (t.empty()) return this->get();
					_line += t.line();		// Literal中可能出现换行
					return {Token_kind::LITERAL_KIND, t.str()};
				}

				// * iii. Dchar
				auto dchar = _get_dchar(c);
				if (!dchar.empty()) return dchar;
				
				// * iiii. 特殊字符
				return Token{ c };	
			}
			return Token{ i.str() };
		}

		Token Token_stream::peek()
		{
			if (_buffer.empty_peek()) _buffer.push_peek(this->get());
			return _buffer.peek();
		}

		line_t Token_stream::line() const
		{
			return _line;
		}


		void Token_stream::_ignore_note(bool flag)
		{
			if (flag == false) {		// 行注释
				string s;
				getline(_is, s);
				_line++;
			}
			else {		 // 块注释
				char c;
				while (true) {
					_is >> noskipws >> c;
					if (c == '\n') _line++;
					if (_is.eof() || (c == '*' && _is.peek() == '/')) {
						_is >> c;
						return;
					}
				}
			}
		}

		void Token_stream::_space_update_line()
		{
			while (true) {	
				char c;
				_is >> noskipws >> c;		// 避免跳过空行
				if (_is.eof()) return;
				if (c == '\n') _line++;
				if (!isspace(c)) {
					_is.putback(c);
					break;
				}
			}
		}

		Token Token_stream::_get_dchar(char c)
		{
			if (c == ':' && _is.peek() == ':') {
				string s = "::";
				_is.get();
				return Token{ s };
			}
			if (c == '-' && _is.peek() == '>') {
				string s = "->";
				_is.get();
				return Token{ s };
			}
			return Token{};
		}
	}
}