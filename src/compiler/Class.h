#pragma once

#include "Function.h"

namespace htf
{

class Class
{
public:
    Class();
    Class(const std::string& name, const std::vector<Variable>& vars,
          const std::vector<Function>& funs);
    ~Class() = default;

public:
    std::string str() const;
    bool empty() const { return _funs.empty(); }   // 只有成员函数不为空时，才需要写定义
    std::string name() const { return _name; }

private:
    std::string constructor_str(int i) const;
    std::string destructor_str(int i) const;

private:
    std::string           _name;
    std::vector<Variable> _vars;   // 成员变量
    std::vector<Function> _funs;   // 成员函数
};

std::ostream& operator<<(std::ostream& os, const Class& c);


namespace detail
{

// 匿名 class 直接跳过
// ! 目前无法处理 内部类
Class get_class(Lex& lex);

}   // namespace detail
}   // namespace htf