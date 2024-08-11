#include "Function.h"

#include "usage.h"

namespace htf
{

const std::string Function_Brackets_Str = "\n{\n\n}\n";

Function::Function() : ret_type{""}, name{""}, args{}, limits{""}, is_destructor{false}
{}

Function::Function(const std::string& ret_type, const std::string& name,
                   const std::vector<Variable>& args, const std::string& limits_str,
                   bool is_destructor)
    : ret_type{ret_type}, name{name}, args{args}, limits{limits_str}, is_destructor{is_destructor}
{
    HTF_DEV_ASSERT_MESSAGE(!ret_type.empty(), "Function::Function(....): ret_type cannot be empty");
}

std::ostream& operator<<(std::ostream& os, const Function& fun)
{
    os << fun.str();
    return os;
}

namespace detail
{

bool get_args(Lex& lex, std::vector<Variable>& res)
{
    lex.get();   // (
    int  flag = 1;
    auto peek = lex.peek();
    while (true) {
        Variable var;
        peek = lex.get();
        if (peek.empty()) return false;
        if (peek.val == ")") {
            --flag;
            if (flag == 0) return true;
        }
        else if (peek.val == "(")
            ++flag;
        else if (peek.kind == LexerKind::type) {
            var.type = peek.val;
            if (lex.peek().kind == LexerKind::identifier ||   // int val;
                (lex.peek().kind == LexerKind::type && is_identifier(lex.peek().val))) {
                // ! int path (path被识别为 type -> std::filesystem::path)
                lex.putback(peek);
                var = get_variable(lex);
                if (var.empty()) return false;
                res.emplace_back(std::move(var));
            }
            else if (lex.peek().val == "," || lex.peek().val == ")") {
                res.emplace_back(std::move(var));   // (int, bool)
            }
            else {
                lex.add_error(peek, "the argument list have some errors");
                lex.ignore_brackets('(', flag);
                return false;
            }
        }
        else if (lex.peek().val == ",")
            lex.get();
    }
    return true;
}

std::string get_function_limits(Lex& lex)
{
    std::string limits;
    auto        peek = lex.peek();
    while (peek.kind == LexerKind::keyword) {
        if (peek.val == "noexcept" || peek.val == "throw") {
            limits += lex.get().val + get_between_brackets(lex) + " ";
        }
        else if (peek.val == "const" || peek.val == "volatile" || peek.val == "override" ||
                 peek.val == "final")
            limits += lex.get().val + " ";
        else
            break;
        peek = lex.peek();
    }
    if (!limits.empty()) limits.pop_back();
    return limits;
}

Function get_function(Lex& lex)
{
    auto peek          = lex.peek();
    bool is_destructor = false;
    if (peek.val == "~") {
        is_destructor = true;
        lex.get();
        peek = lex.peek();
    }
    if (peek.kind != LexerKind::type) return Function{};
    std::string type = lex.get().val;
    peek             = lex.peek();
    if (!(peek.kind == LexerKind::identifier ||
          (peek.kind == LexerKind::type && is_identifier(peek.val))) &&
        peek.val != "(") {   // 构造函数没有名称
        lex.ignore_statement();
        return Function{};
    }
    std::string name;
    if (peek.kind == LexerKind::identifier) name = lex.get().val;
    if (lex.peek().val != "(") {
        lex.ignore_statement();
        return Function{};
    }
    std::vector<Variable> args;
    if (!get_args(lex, args)) {
        lex.ignore_statement();
        return Function{};
    }
    std::string limits = get_function_limits(lex);
    if (lex.peek().val != ";") {
        lex.ignore_statement();
        return Function{};
    }
    lex.get();   // ;
    return Function{type, name, args, limits, is_destructor};
}

}   // namespace detail
}   // namespace htf
