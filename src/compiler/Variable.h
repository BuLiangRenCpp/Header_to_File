#pragma once

// 采用此结构的原因：
// - 参数列表中的单个参数 与 类的长远变量 满足下面
// - 由于函数也是 (type + name) 开头，但是 Lex 只能 putback 一次，
// 故 get 方法仅在确定是变量时使用，否则需使用者自行实现

#include "Lex.h"

namespace htf
{

struct Variable {
    std::string type = "";   // 记录此可用于 类的构造函数 补齐初始化列表
    std::string name = "";   // 如果为空，则 name = type[0]
    std::string val  = "";   // 可为空

    bool empty() const { return type == ""; }

    Variable() = default;   // 空: 用于判断 get 是否成功
    Variable(const std::string& type, const std::string& name = "", const std::string& val = "");
};


namespace detail
{

// 判断两个类型是否相似
bool is_similar_type(const std::string& type1, const std::string& type2);

// 根据下一字符进行判断
// 读取括号之间的部分，按 匹配规则
std::string get_between_brackets(Lex& lex);

// 读取变量的值
// type id = value;
// type id { value };
// * 调用者确定第一个 token == "=" or "{"
std::string get_value(Lex& lex);

// * 当结尾是 ; 时会被读取掉
Variable get_variable(Lex& lex);

}   // namespace detail
}   // namespace htf