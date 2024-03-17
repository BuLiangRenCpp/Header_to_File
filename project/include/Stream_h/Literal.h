#pragma once

#include "header.h"

// ---------------------------------- Liter类 ----------------------------------------
// 字面值(literal)：数字， ' '和" "之间的字符 
// ****************** 注：不是 Literal，则设置 is.fail = true *************************
// ** 主要用于函数参数忽略默认值
class Literal {
public:
	Literal();
	Literal(const std::string&);

	std::string str() const;
	bool empty() const;

private:
	std::string _val;
};


// ------------------------ 重载 -----------------------------

std::istream& operator>>(std::istream& is, Literal& l);

std::ostream& operator<<(std::ostream& os, const Literal& l);