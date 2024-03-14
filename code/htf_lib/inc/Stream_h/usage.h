#pragma once

#include "prompt.h"

// 功能
namespace usage {
	// 是否是左括号
	bool is_lbracket(const char&);

	// 是否是右括号
	bool is_rbracket(const char&);

	// 删除前后导空格
	void delete_space_pre_suf(string&);

	// 是否是标识符
	bool is_identifier(const string&);

	// 是否是自定义类的关键字 class or struct
	bool is_class_key(const string&);

	// 是否是类型关键字 (类型修饰词关键字、容器类型关键字、基本类型关键字）
	bool is_type_key(const string&);

	// ** 忽略流的第一个字符对应的括号（必须是左括号，下面叫做参数括号）之间的所有字符，包括对应的右括号
	// 注：
	// 1. 流的第一个字符不是左括号，则不进行忽略操作
	// 2. 参数括号对应的右括号是按照括号正确格式匹配的，不是最近原则
	//    如："(())"，第一个参数括号对应的右括号是做后一个括号，所以如果流内没有参数括号对应的右括号，则全部字符都会被忽略
	void ignore_between_bracket(istream&);
}
