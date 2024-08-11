#pragma once

#include "LexerKind.h"
#include "Token.h"
#include "using.h"

namespace htf
{

class Lexer
{
public:
    using kind_type  = LexerKind;
    using value_type = std::string;

public:
    kind_type  kind;
    value_type val;
    line_t     line;
    col_t      col;

public:
    Lexer(const value_type& value = "", const kind_type& kind = kind_type::eof, line_t line = 0,
          col_t col = 0);
    // 专用于 schar
    Lexer(char value, line_t line = 0, col_t col = 0);
    // 仅用于 kind = [eof, keyward, identifier, schar]
    Lexer(const Token& token);
    ~Lexer() = default;

public:
    bool empty() const { return kind == kind_type::eof && val == ""; }

    bool operator==(const Lexer& other);
    bool operator!=(const Lexer& other);
};

}   // namespace htf