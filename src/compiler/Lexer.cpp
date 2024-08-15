#include "Lexer.h"

#include "output.h"
#include "assertions.h"

namespace htf
{

Lexer::Lexer(const value_type& value, const kind_type& kind, line_t line, col_t col)
    : kind{kind}, val{value}, line{line}, col{col}
{}

Lexer::Lexer(char value, line_t line, col_t col)
    : kind{kind_type::schar}, val{value}, line{line}, col{col}
{}

Lexer::Lexer(const Token& token)
    : kind{kind_type::eof}, val{token.val}, line{token.line}, col{token.col}
{
    switch (token.kind) {
    case TokenKind::eof: break;
    case TokenKind::constant:
    case TokenKind::string_literal: kind = kind_type::literal; break;
    case TokenKind::keyword: kind = kind_type::keyword; break;
    case TokenKind::identifier: kind = kind_type::identifier; break;
    case TokenKind::dchar: kind = kind_type::dchar; break;
    case TokenKind::schar: kind = kind_type::schar; break;
    default:
        HTF_DEV_ASSERT_MESSAGE(false,
                               "Lexer::Lexer(.): the token.kind don't cast to lexer kind: "
                                   << static_cast<int>(token.kind) << ":"
                                   << mark(token.val));
        break;
    }
}

bool Lexer::operator==(const Lexer& other)
{
    return kind == other.kind && val == other.val;
}

bool Lexer::operator!=(const Lexer& other)
{
    return !(*this == other);
}

}   // namespace htf
