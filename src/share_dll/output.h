#pragma once

#ifdef _WIN32
#    define LIB_EXPORT
#endif

#include "dll.h"
#include "header.h"

namespace output
{

DLL_API void prompt();
DLL_API void print(const std::string&);
DLL_API void print_enter(const std::string&);
// is_line: 默认换行
DLL_API void print_result(const std::string&, bool is_line = true);
// is_line: 默认换行
DLL_API void print_warn(std::string, bool is_line = true);
// is_line: 默认换行
DLL_API void print_error(std::string, bool is_line = true);

// 输出 count 个 "\n"，用于缩进
DLL_API void print_indentation(std::ostream& os, int count = 1);

// 在 s 前后加上 c 后，同时在前后各加一个空格
// 用于标记输出中的特别字符，加以强调
DLL_API std::string mark(const std::string& s, const std::string& c = "\"");
DLL_API std::string mark(char s, char c = '\"');

}   // namespace output
