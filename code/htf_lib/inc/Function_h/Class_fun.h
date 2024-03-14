#pragma once

#include "Function.h"

// 自定义类中的成员函数
class Class_fun {
public:
	Class_fun();
	// pub：pubilc   pri：private 
	// (&pub, &pro, &pri)
	// 若 n 为空，则 pub、pro、pri 都必须为空
	Class_fun(const string& n, const vector<vector<Function>>& t);

	// 返回成员函数的定义语句
	string str() const;
	bool empty() const;

private:
	string _name = "";		// 类名
	vector<vector<Function>> _funs;		// first->pub   second->pri
};


// ------------------------ 重载 -----------------------------

istream& operator>>(istream& is, Class_fun& c);

ostream& operator<<(ostream& os, const Class_fun& c);