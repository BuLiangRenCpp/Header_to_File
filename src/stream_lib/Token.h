#pragma once
#include "Token_kind.h"

namespace htf
{
namespace stream
{
// ***************************** 注意 ************************************
// 使用 struct 是出于对 Token 赋值灵活性与代码简洁性的要求，但是必须注意不能违背以下要求
//		token.val == "" --> token 只能是 特殊单字符 或者 为空
// **********************************************************************
// 将 cpp 每个字作为一个 Token (有 string、char）
// 对于特殊的 cpp中单字符Token（如 ";"、"{"、"("等）其 kind、val 都是它本身
struct Token
{
    char        kind;   // 类型
    std::string val;    // 字面值

    // 空Token
    Token();
    // 用于特殊字符的初始化
    Token(char);
    // 用于字符串的初始化，自动推导类型
    Token(const std::string&);
    // 用于强制类型转换的，类型合法性由调用者自行判断
    Token(char, const std::string&);

    bool empty() const;
};

bool operator==(const Token&, const Token&);
bool operator!=(const Token&, const Token&);

}   // namespace stream
}   // namespace htf