#pragma once

#include "header.h"

namespace htf {
	namespace argument{
		namespace args_judge {
			bool is_arg(char);
			bool is_harg(char);
			bool is_narg(char);
			// -o 只能出现一次，-i 必须出现至少一次，其余可多次出现
			bool is_htf_args_throw(const std::vector<char>&);
			int arg_count_max(char);
			int htf_arg_count_max();
		}
	}
}