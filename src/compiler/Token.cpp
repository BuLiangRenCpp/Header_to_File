#include "Token.h"

namespace htf
{

Token::Token(const value_type& v, const kind_type& k, line_t l, col_t c)
    : kind{k}, val{v}, line{l}, col{c}
{}

Token::Token(char ch, line_t l, col_t c) : kind{kind_type::schar}, val{ch}, line{l}, col{c}
{}

bool Token::operator==(const Token& other)
{
    return (kind == other.kind) && (val == other.val);
}

bool Token::operator!=(const Token& other)
{
    return !(*this == other);
}

}   // namespace htf
