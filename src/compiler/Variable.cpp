#include "Variable.h"
#include "assertions.h"
#include "usage.h"

using namespace std;

namespace htf
{

// public
Variable::Variable(const std::string& t, const std::string& n, const std::string& v)
    : type{t}, name{n}, val{v}
{
    HTF_DEV_ASSERT_MESSAGE(!type.empty(),
                           "Variable::Variable(...): variable's type cannot be empty");
    if (name.empty()) name = t[0];
}

namespace detail
{

std::string get_between_brackets(Lex& lex)
{
    if (!is_lbracket(lex.peek().val)) return "";
    std::string res;
    std::string lbra = lex.get().val;
    std::string rbra{ret_rbracket(lbra)};
    int         flag = 1;
    while (flag > 0) {
        auto lexer = lex.get();
        if (lexer.empty()) {
            lex.add_error(lexer, "lack of" + mark(rbra));
            return "";
        }
        if (lexer.val == lbra)
            ++flag;
        else if (lexer.val == rbra)
            --flag;
        if (is_id_ch(res.back()) && is_id_ch(lexer.val[0])) {
            res = res + " " + lexer.val;
        }
        else
            res += lexer.val;
    }
    return res;
}

std::string get_value(Lex& lex)
{
    auto        lexer = lex.get();
    std::string res;
    if (lexer.val == "=") {
        lexer = lex.get();
        if (lexer.val == ";") {
            lex.add_error(lexer, "before" + mark(lexer.val) + "lack of identifier");
            lex.ignore_statement();
            return "";
        }
        if (lexer.val == "{")
            res += get_between_brackets(lex);
        else if (lexer.kind == LexerKind::identifier || lexer.kind == LexerKind::type) {
            res += lexer.val;
            if (lexer.val == "{") res += get_between_brackets(lex);
        }
    }
    else if (lexer.val == "{") {
        res += get_between_brackets(lex);
    }
    else
        HTF_DEV_ASSERT_MESSAGE(false, "htf::detail::get_value(.): lex.peek() != '=' or '{'");
    return res;
}


Variable get_variable(Lex& lex)
{
    Variable res{};
    auto     peek = lex.peek();
    if (peek.kind != LexerKind::type) return res;
    res.type = lex.get().val;
    peek     = lex.peek();
    if ((peek.kind != LexerKind::identifier && peek.kind != LexerKind::type) ||
        (peek.kind == LexerKind::type && !is_identifier(peek.val))) {
        lex.add_error(peek, mark(res.type) + "lack of identifier");
        return res;
    }
    res.name = lex.get().val;
    peek     = lex.peek();
    if (peek.val == "=" || peek.val == "{") {
        res.val = get_value(lex);
    }
    return res;
}

}   // namespace detail
}   // namespace htf