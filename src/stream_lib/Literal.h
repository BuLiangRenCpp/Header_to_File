#pragma once

#include "header.h"

namespace htf {
	namespace stream{
		// 字面值(literal)：数字， ' '和" "包含的字符  (包括 ' ") 
		// ! 不能处理其他进制的数字
		class Literal {
		public:
			Literal();
			Literal(const std::string&, line_t line = 0);		// line string中 看不见的换行的个数

			std::string str() const;
			bool empty() const;
			line_t line() const;		// 返回包含的换行数

		private:
			std::string _val;
			line_t _line;
		};

		std::istream& operator>>(std::istream& is, Literal& l);
		std::ostream& operator<<(std::ostream& os, const Literal& l);
	}
}