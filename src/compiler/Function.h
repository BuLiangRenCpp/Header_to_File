#pragma once

// 从 Lex 中读取 函数声明

#include "Variable.h"

namespace htf
{

extern const std::string Function_Brackets_Str;

class Function
{
public:
    Function();
    Function(const std::string& ret_type, const std::string& name = "",
             const std::vector<Variable>& args = {}, const std::string& limits = "",
             bool is_destructor = false);
    ~Function() = default;

public:
    std::string str() const
    {
        if (empty()) return "";
        std::ostringstream oss;
        oss << ret_type;
        if (!name.empty()) oss << " " << name;
        oss << args_str() << " " << limits << Function_Brackets_Str;
        return oss.str();
    }

    std::string args_str() const
    {
        std::ostringstream oss;
        oss << "(";
        auto size = args.size();
        for (int i = 0; i < size; ++i) {
            oss << args[i].type << " " << args[i].name;
            if (i < size - 1) oss << ", ";
        }
        oss << ")";
        return oss.str();
    }

    bool is_constructor() const { return !empty() && !is_destructor && name.empty(); }

    bool empty() const { return ret_type.empty(); }

public:
    std::string           ret_type;
    std::string           name;
    std::vector<Variable> args;
    std::string           limits;          // 限定符
    bool                  is_destructor;   // ~
};

std::ostream& operator<<(std::ostream& os, const Function& fun);

namespace detail
{

bool get_args(Lex& lex, std::vector<Variable>& res);

std::string get_function_limits(Lex& lex);

// 内部如果出现语法错误，自动忽略
// * 只读取 函数声明
Function get_function(Lex& lex);

}   // namespace detail
}   // namespace htf