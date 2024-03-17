#pragma once

#include "ins_judge.h"


// 保存 ins 的参数 (-后的字符)
class Ins_arg_key {
public:
	Ins_arg_key();
	Ins_arg_key(char);

	char ch() const;
	bool empty() const;

	bool is_harg() const;
	bool is_narg() const;

private:
	char _ch;
};


// ---------------------------------- 重载 -------------------------------------------

std::istream& operator>>(std::istream&, Ins_arg_key&);
