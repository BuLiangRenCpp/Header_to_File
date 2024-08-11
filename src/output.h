#pragma once

// 输出提示信息：对信息添加颜色、格式等 (用于用户)

#include <iostream>

namespace htf
{

// 在 s 前后加上 c 后，同时在前后各加一个空格
// 用于标记输出中的特别字符，加以强调
std::string mark(const std::string& s, char c = '\'');
std::string mark(char s, char c = '\'');

// - 带 "" 之前的内容为 reset，其余有颜色
// - 各自具有一定格式
// - 都换行
void cout_log(const std::string& what);
void cout_warn(const std::string& what);
void cout_error(const std::string& what);

// ------------ dev ------------------
void cout_error_dev(const std::string& what);

}   // namespace htf
