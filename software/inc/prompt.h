#pragma once
#include "header.h"

#ifdef PROMPT_DLL
#define PDLL_API __declspec(dllexport)
#else
#define PDLL_API __declspec(dllimport)
#endif

namespace prompt{
    PDLL_API void prompt();
    PDLL_API void print(const string&);
    PDLL_API void print_enter(const string&);
    // is_line: 默认换行
    PDLL_API void print_result(const string&, bool is_line = true);
    // is_user: 默认用户级别输出
    // is_line: 默认换行
    PDLL_API void print_warn(string, bool is_user = true, bool is_line = true);
    // is_user: 默认用户级别输出
    // is_line: 默认换行
    PDLL_API void print_error(string, bool is_user = true, bool is_line = true);


    // 在 s 前后加上 c 后，同时在前后各加一个空格 
    // 用于标记输出中的特别字符，加以强调
    // flag 用于指定  
    //      print_result = 1 (Green)   
    //      print_warn = 0 (Yellow)
    //      print_error = -1 (Red)
    // ** 默认用于 print_error
    PDLL_API string mark_string(const string& s, const string& c = "\"", int flag = -1);
    PDLL_API string mark_char(char s, char c = '\"', int flag = -1);
}

