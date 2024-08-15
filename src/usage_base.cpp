#include "usage_base.h"

namespace htf
{

bool is_lbracket(char c)
{
    return c == '(' || c == '[' || c == '<' || c == '{';
}

bool is_rbracket(char c)
{
    return c == ')' || c == ']' || c == '>' || c == '}';
}

bool is_bracket(char c)
{
    return is_lbracket(c) || is_rbracket(c);
}

char ret_lbracket(char c)
{
    switch (c) {
    case ')': return '(';
    case ']': return '[';
    case '}': return '{';
    case '>': return '<';
    default: break;
    }
    return c;
}

char ret_rbracket(char c)
{
    switch (c) {
    case '(': return ')';
    case '[': return ']';
    case '{': return '}';
    case '<': return '>';
    default: break;
    }
    return c;
}

}   // namespace htf
