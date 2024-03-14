#pragma once

#include "Class_fun.h"


// ----------------------------------------------------------------------------

namespace htf {
	// 从 ifile头文件 读取函数声明，将内容写到 ofile，默认 ofile 与 ifile 在同一目录
	// 返回是否生成成功
    // 注：如果目标文件已经存在，则询问用户是否覆盖
	bool header_to_file(const string& ifile, string ofile = "");

	// 从 idir 目录下读取所有头文件，将内容写到 odir下，默认 idir == odir
	// 注：idir、odir可以以 "\" 结尾 也可以不是，但必须是目录 如 "C:\dir\" or "C:\dir"
	void header_to_files(string idir, string odir = "");
}