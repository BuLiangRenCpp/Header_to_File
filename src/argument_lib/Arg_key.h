#pragma once
#include "header.h"

namespace htf
{
namespace argument
{
// 保存 ins 的参数 (-后的字符: f i o I)
class Arg_key
{
public:
    Arg_key();
    Arg_key(const char);

    char ch() const;
    bool empty() const;

    bool is_harg() const;
    bool is_narg() const;

private:
    char _ch;
};

std::istream& operator>>(std::istream&, Arg_key&);
}   // namespace argument
}   // namespace htf
