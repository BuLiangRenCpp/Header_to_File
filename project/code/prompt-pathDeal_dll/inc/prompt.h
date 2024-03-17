#pragma once
#include "header.h"

#ifdef PROMPT_DLL
#define PDLL_API __declspec(dllexport)
#else
#define PDLL_API __declspec(dllimport)
#endif

namespace prompt{
    PDLL_API void prompt();
    PDLL_API void print(const std::string&);
    PDLL_API void print_enter(const std::string&);
    // is_line: 默认换行
    PDLL_API void print_result(const std::string&, bool is_line = true);
    // is_user: 默认用户级别输出
    // is_line: 默认换行
    PDLL_API void print_warn(std::string, bool is_user = true, bool is_line = true);
    // is_user: 默认用户级别输出
    // is_line: 默认换行
    PDLL_API void print_error(std::string, bool is_user = true, bool is_line = true);

    // 输出 count 个 "\n"，用于缩进
    PDLL_API void print_indentation(std::ostream& os, int count = 1);

    // 在 s 前后加上 c 后，同时在前后各加一个空格 
    // 用于标记输出中的特别字符，加以强调
    PDLL_API std::string mark_string(const std::string& s, const std::string& c = "\"");
    PDLL_API std::string mark_char(char s, char c = '\"');
}

