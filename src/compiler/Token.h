#pragma once

#include <string>

#include "TokenKind.h"
#include "using.h"

namespace htf
{

// 此类使用合法性由使用者自行决断
class Token
{
    using kind_type  = TokenKind;
    using value_type = std::string;

public:
    Token(const value_type& v = "", const kind_type& k = kind_type::eof, line_t l = 0, col_t c = 0);
    // 专用于单字符
    Token(char ch, line_t l = 0, col_t c = 0);
    ~Token() = default;

public:   // 为了方便使用，采用 public
    kind_type  kind;
    value_type val;
    line_t     line;   // 所在行号
    col_t      col;    // 所在列数 (第几个字符，从 1 开始)

public:
    // eof
    bool empty() const { return kind == kind_type::eof && val.empty(); }

public:
    bool operator==(const Token& other);
    bool operator!=(const Token& other);
};

}   // namespace htf