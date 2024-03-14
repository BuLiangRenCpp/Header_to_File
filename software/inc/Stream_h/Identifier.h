#pragma once

#include "prompt.h"
#include "usage.h"


// ------------------------ Identifier类 ----------------------------
// 标识符
class Identifier {
public:
	Identifier();
	Identifier(const string&);

	string str() const;
	bool empty() const;

private:
	string _name;
};


// ------------------------ 重载 -----------------------------

istream& operator>>(istream& is, Identifier& n);

ostream& operator<<(ostream& os, const Identifier& n);