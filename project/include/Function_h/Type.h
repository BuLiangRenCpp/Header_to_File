#pragma once

#include "Token_stream.h"

// ***********************************************************************************
// 自定义类型判断方式  
//  思路： 
//	Token_stream可以在读取的过程中记录所读取到的可能是类型的关键字，用一块内存来保存，在后面读取的可根据此内存来判断是否是类型


// 类型，包括基本类型和容器类型
// 注：不做完整的语法检查，因为这是编译器做的事，同时语法过于复杂
// 思路：
//	1. cpp已定义的类型应能识别
//	2. 自定义类型可根据其他信息来推断，如
//    (1) A* fic(B, C c = 'c');	先读取 A，如果 TS.peek为函数名或者是 '*' or '&'，则 A 是类型
//	  (2) vector<A> fic();    在 容器后的<>内的是类型
class Type {
public:
	Type();
	// 直接赋值，不做检查，使用 重载函数
	Type(const std::string&);

	std::string str() const;
	bool empty() const;

private:
	std::string _type;
};


// ------------------------- 重载 ------------------------------

// 注：语法正确性是编译器的事
std::istream& operator>>(std::istream& is, Type& t);

std::ostream& operator<<(std::ostream& os, const Type& t);