#pragma once

#include "Type.h"

// ------------------------ Par类 -----------------------------------
// 用于存储参数列表中的一个参数，如果参数有默认值则忽略  -->  会使得putback中有信息却是
// Fun_arg = Type + Identifier (如果 Type 为空，则 Identifier 必须也为空)
class Fun_arg {
public:
	Fun_arg();
	Fun_arg(const Type&, const Identifier&);

	// 返回  _type + " " + _name
	string str() const;
	bool empty() const;

private:
	Type _type;
	Identifier _name;
};

// ---------------------------------------------------------

// 读取函数的 参数列表
// *** 读取失败将 is 设置为 fail 状态	   -->    为区分空参数列表  ***
// name 函数名，用于报错
// ** 参数列表错误结束，格式有问题，is.fail = true
vector<Fun_arg> get_pars(istream& is, const string& name = "");

// ------------------------ 重载 -----------------------------

istream& operator>>(istream& is, Fun_arg& p);

ostream& operator<<(ostream& os, const Fun_arg& p);