#pragma once

#include "usage_base.h"
#include <string>

namespace htf
{

bool is_lbracket(const std::string& ch);
bool is_rbracket(const std::string& ch);
bool is_bracket(const std::string& ch);
char ret_lbracket(const std::string& ch);
char ret_rbracket(const std::string& ch);

bool is_dchar(const std::string& dchar);

// 是否是 dchar 的首字符
bool is_dchar_fir(char c);

// 是否是标识符
bool is_identifier(const std::string& str);

// ! 目前仅记录有用的
bool is_keyword(const std::string& str);

// class or struct
inline bool is_class_key(const std::string& str)
{
    return str == "class" || str == "struct";
}

// using or typedef
inline bool is_using_key(const std::string& str)
{
    return str == "using" || str == "typedef";
}

inline bool is_class_limit_key(const std::string& str)
{
    return str == "public" || str == "protected" || str == "private";
}

inline bool is_type_limit_key(const std::string& str)
{
    return str == "const" || str == "static" || str == "volatile";
}

// 是否是可以出现在 id 中的字符: alnum and '_'
inline bool is_id_ch(char c)
{
    return isalnum(c) || c == '_';
}

}   // namespace htf