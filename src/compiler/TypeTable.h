#pragma once

// 类型表：类似符号表，用于存储类型相关信息：名称、namespace...
// ! 目前仅存储 类型的名称
// * 预计可能扩展: 增加 namespace 识别
// * 目前将模板也视为类型比如 vector

#include <set>
#include <string>

namespace htf
{

class TypeTable
{
public:
    // 将 c++ 标准库常用类型默认创建出来
    // * 都不带空格，只有 id : 比如 vector (不保存为 vector<..>)、string，不保存 long long
    TypeTable();
    ~TypeTable() = default;

public:
    bool exist(const std::string& type) const;
    void add(const std::string& type);

private:
    std::set<std::string> _types;
};

}   // namespace htf