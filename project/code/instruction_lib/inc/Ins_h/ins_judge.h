#pragma once

#include "ins_const.h"
#include "prompt.h"

namespace ins_judge {
	bool is_ins(const std::string&);
	// 是否是单指令，如 q、ld
	bool is_single_ins(const std::string&);

	bool is_arg(char);
	bool is_harg(char);
	bool is_narg(char);
	// *** 判断 args是否是 key 的 arg，对于不合法的情况直接 throw ***
    bool is_ins_args_throw(const std::string&, const std::vector<char>&);
    // 返回指令允许的参数最大个数
    int arg_count_max(const std::string&);
}