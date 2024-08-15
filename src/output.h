#pragma once

// 输出提示信息：对信息添加颜色、格式等 (用于用户)

#include <iostream>
#include "usage_base.h"
#include "using.h"

namespace htf
{

std::string mark(char str, char c = '\'');

std::string mark(const std::string& str, char c = '\'');

inline std::string mark_path(const FS::path& path)
{
    std::ostringstream oss;
    oss << " [ " << path.string() <<" ] ";
    return oss.str();
}


// - 带 "" 之前的内容为 reset，其余有颜色
// - 各自具有一定格式
// - 都换行
void cout_log(const std::string& what);
void cout_warn(const std::string& what);
void cout_error(const std::string& what);

// ------------ dev ------------------
void cout_error_dev(const std::string& what);

}   // namespace htf
