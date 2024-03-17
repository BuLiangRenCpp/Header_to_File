#pragma once

#include "Function.h"

// 自定义类中的成员函数
class Class_fun {
public:
	Class_fun();
	// pub：pubilc   pri：private 
	// (&pub, &pro, &pri)
	// 若 n 为空，则 pub、pro、pri 都必须为空
	Class_fun(const std::string& n, const std::vector<std::vector<Function>>& t);

	// 返回成员函数的定义语句
	std::string str(unsigned int count = 0) const;
	bool empty() const;
	// 格式化输出，按缩进输出
	void print(std::ostream& os, unsigned int count = 1) const;

private:
	std::string _name = "";		// 类名
	std::vector<std::vector<Function>> _funs;		// first->pub   second->pri
};


// ------------------------ 重载 -----------------------------

std::istream& operator>>(std::istream& is, Class_fun& c);

std::ostream& operator<<(std::ostream& os, const Class_fun& c);