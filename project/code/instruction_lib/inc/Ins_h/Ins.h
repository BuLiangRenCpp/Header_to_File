#pragma once

#include "Ins_key.h"
#include "Ins_arg.h"


// 指令集
// 初步判断指令格式: 单指令与多指令、有参参数与无参参数、指令参数个数限制、指令参数对应
// 空指令集对应没有操作
class Ins {
public:
    Ins();
	Ins(const Ins_key&, const std::vector<Ins_arg>& args = {});

	Ins_key key() const;
	std::vector<Ins_arg> args() const;
	bool empty() const;

private:
	Ins_key _key;
	std::vector<Ins_arg> _args;		// 按顺序存储

	// 规范化 Ins  (将省略的参数自动推导后加上)
	// 保证 htf 参数顺序为 -f -i -o
    void _legalize();      
};


// --------------------------------------- 重载 -----------------------------------------------

std::istream& operator>>(std::istream&, Ins&);