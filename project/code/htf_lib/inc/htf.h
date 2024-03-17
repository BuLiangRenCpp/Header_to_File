#pragma once

#include "Class_fun.h"


// ----------------------------------------------------------------------------

namespace htf {
	// 从 ifile头文件 读取函数声明，将内容写到 odir，默认 odir 与 ifile 在同一目录
	// ** odir 可以是存在的目录或者文件路径，idir 只能是文件路径 **
	// 返回是否生成成功
    // 注：如果目标文件已经存在，则询问用户是否覆盖
	// 	   如果 is_force == true，则不提醒，默认提醒
	bool header_to_file(std::string ifile, std::string odir = "", bool is_force = false);

	// 从 idir 目录下读取所有头文件，将内容写到 odir下，默认 idir == odir
	// 注：idir、odir可以以 "\" 结尾 也可以不是，但必须是目录 如 "C:\dir\" or "C:\dir"
	// 注：如果目标文件已经存在，则询问用户是否覆盖
	// 	   如果 is_force == true，则不提醒，默认提醒
	void header_to_files(std::string idir, std::string odir = "", bool is_force = false);
}