#pragma once

#include "header.h"

namespace htf {
	namespace stream{
		namespace usage {
			// 是否是左括号
			bool is_lbracket(const char&);

			// 是否是右括号
			bool is_rbracket(const char&);

			bool is_bracket(char);

			char ret_lbracket(char);

			char ret_rbracket(char);

			// 删除前后导空格
			void delete_space_pre_suf(std::string&);

			bool is_dchar(const std::string&);

			// 是否是标识符
			bool is_identifier(const std::string&);

			// 是否是特殊字符 
			inline bool is_spe_ch(char c)
			{
				return !(isalnum(c) || c == '_');	// 下划线可以是标识符
			}

			// 返回字符串对应的kind
            // ** 注：特殊字符的kind就是它本身，不用写此函数
            // ** 如果不是其他优先级高的类型，则返回 NULL_KIND
            char ret_kind(const std::string&);
		}
	}
}