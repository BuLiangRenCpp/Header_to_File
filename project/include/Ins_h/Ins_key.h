#pragma once
#include "ins_judge.h"

// 指令关键字
class Ins_key {
public:
    // 默认为 ""，对应没有操作
    Ins_key();
	Ins_key(const std::string&);

	std::string str() const;
    bool empty() const;

private:
	std::string _ins;
};


// --------------------------------- 重载 ----------------------------------------

// 未读取成功则返回空
std::istream& operator>>(std::istream& is, Ins_key&);