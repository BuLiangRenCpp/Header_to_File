#pragma once

#include "header.h"

// ---------------------------------- Liter类 ----------------------------------------
// 字面值(literal)：数字， ' '和" "之间的字符 
// ****************** 注：不是 Literal，则设置 is.fail = true *************************
// ** 主要用于函数参数忽略默认值
class Literal {
public:
	Literal();
	Literal(const string&);

	string str() const;
	bool empty() const;

private:
	string _val;
};


// ------------------------ 重载 -----------------------------

istream& operator>>(istream& is, Literal& l);

ostream& operator<<(ostream& os, const Literal& l);