#pragma once

#include "prompt.h"
#include "usage.h"


// ------------------------ Identifier类 ----------------------------
// 标识符
class Identifier {
public:
	Identifier();
	Identifier(const std::string&);

	std::string str() const;
	bool empty() const;

private:
	std::string _name;
};


// ------------------------ 重载 -----------------------------

std::istream& operator>>(std::istream& is, Identifier& n);

std::ostream& operator<<(std::ostream& os, const Identifier& n);