#pragma once

/**
 * 并非处理 c++ 中的所有词法单元，仅处理有用的
 */

namespace htf
{

enum class TokenKind
{
    eof = 0,          // end of file
    string_literal,   // "" or ''
    constant,         // integer or float
    identifier,
    keyword,
    dchar,   // double char:   ::  ->
    schar    // single char
};

}   // namespace htf