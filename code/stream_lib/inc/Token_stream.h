#pragma once
#ifdef _WIN32
#define LIB_EXPORT
#endif

#include "dll.h"
#include "Token.h"
#include "Buffer.h"

// -------------------------- 模拟输入流 ------------------------------------
// 自动忽略注释，同时判断注释格式是否正确
namespace htf {
	namespace stream {
		class DLL_API Token_stream {
		public:
			Token_stream() = delete;
			Token_stream(std::istream&);
			bool eof();
			void sync();
			void putback(const Token&);
			// 一直忽略到 token，token 也忽略
			// 默认忽略一条语句
			void ignore(const Token& token = Token{ ';' });	
			// 一直忽略直到 token，(token不忽略)
			void ignore_until(const Token& token = Token{ '{'} );	
			// 忽略一行 (注：会把buffer、peek清空)
			// * 此函数的目的是给 lex 忽略 预处理指令，因此没有考虑太多问题
			// ! 感觉没必要考虑：可能存在 _buffer 中是上一行的内容，那么此函数应该只需要忽略 _buffer，而现在的函数是
			// ! 	 既忽略_buffer，也忽略下一行 (如果要改动的话代价会比较大，因为此类只会读取非空白字符)
			void ignore_line();
			// 忽略 括号之间的部分 (注：按匹配规则，如 '{1 { 2 } 3} 4' 会一直忽略到 '4' 之前)
			// * 忽略的括号 (应为左括号) 是看 _is.peek，如果是 左括号 则执行，不是则不执行
			// ! 如果到文件结束了但是括号没有匹配完，不报错，全忽略
			void ignore_between_bracket();
			// 返回 empty Token 说明没有内容了
			Token get();
			Token peek();
			line_t line() const;	

		private:
			std::istream& _is;
			Buffer<Token> _buffer;	
			line_t _line = 1;

			// 忽略注释，注释格式不对的报错 
			// flag = false：行注释
			// 在使用此函数时已经判断了接下来的字符时 /*，现在下一个字符时 *
			void _ignore_note(bool flag = false);
			void _space_update_line();
			Token _get_dchar(char c);
		};
	}
}