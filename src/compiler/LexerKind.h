#pragma once

namespace htf
{

enum class LexerKind
{
    eof,
    type,
    keyword,      // 类型优先: 比如 void 是类型
    identifier,   // * operator-> 也视为 id (函数名)
    literal,      // constant and string_literal (目前不重要，故归为一类)
    dchar,
    schar
};

}   // namespace htf