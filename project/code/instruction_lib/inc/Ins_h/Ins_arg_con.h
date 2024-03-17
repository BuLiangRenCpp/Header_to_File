#pragma once

#include "path_deal.h"
#include "ins_judge.h"


// 指令中的content，目前只是路径，按照路径处理
// ** 采取 string 存储，避免扩展功能
// 如果路径的格式跟特殊字符或者参数格式一致，不强制要求情况下是不允许相同的
// ** 自动将特殊字符转为对应的路径，无扩展名的文件自动加上 
class Ins_arg_con {
public:
	Ins_arg_con();
	// is_force: 强迫特殊字符(程序已经规定的有特殊用处的字符)转为普通字符
	Ins_arg_con(const std::string&, bool is_force = false);

	std::string str() const;
	bool empty() const;

private:
	std::string _con;		// 如果 _con 时目录，则在末尾自动加上 "\"，如果有的则不加 
};


// ---------------------------------------- 重载 -----------------------------------------------------

std::istream& operator>>(std::istream&, Ins_arg_con&);