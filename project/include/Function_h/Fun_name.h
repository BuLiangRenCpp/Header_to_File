#pragma once

#include "Token_stream.h"


// 函数名，包括一般函数名、特殊函数名（重载运算符函数名 opretor>> 等)
class Fun_name {
public:
	Fun_name();
	// 一般函数名一般使用此初始化
	Fun_name(const Identifier&);
	// 特殊函数名一般使用此初始化
	Fun_name(const string&);

	string str() const;
	bool empty() const;

private:
	string _name = "";
};



// ------------------------ 重载 -----------------------------

istream& operator>>(istream& is, Fun_name& i);

ostream& operator<<(ostream& os, const Fun_name& i);