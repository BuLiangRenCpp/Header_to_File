#include "Class.h"
#include "output.h"
#include "usage.h"

namespace htf
{

Class::Class() : _vars{}, _funs{}
{}

Class::Class(const std::string& name, const std::vector<Variable>& vars,
             const std::vector<Function>& funs)
    : _name{name}, _vars{vars}, _funs{funs}
{
    HTF_DEV_ASSERT_MESSAGE(!_name.empty(), "Class::Class(...): the class name cannot be empty");
}

std::string Class::str() const
{
    if (empty()) return "";
    std::ostringstream oss;
    for (int i = 0; i < _funs.size(); ++i) {
        auto f = _funs[i];
        if (f.is_constructor())
            oss << constructor_str(i);
        else if (f.is_destructor)
            oss << destructor_str(i);
        else {
            oss << f.ret_type << " " << _name << "::" << f.name << f.args_str() << " " << f.limits
                << HTF_Function_Brackets_Str;
        }
        oss << "\n";
    }
    return oss.str();
}


static int find_similar_type(const std::string& type, const std::vector<Variable>& args,
                             std::set<int>& visited)
{
    int  i    = 0;
    auto size = args.size();
    for (; i < size; ++i) {   // 需要从前往后遍历，即默认先出现的先匹配
        if (!visited.count(i) && detail::is_similar_type(type, args[i].type)) {
            visited.emplace(i);
            break;
        }
    }
    return (i == size) ? -1 : i;
}

std::string Class::constructor_str(int index) const
{
    std::ostringstream oss;
    auto               f = _funs[index];
    oss << _name << "::" << f.ret_type << f.args_str();
    bool is_default = f.args.empty();
    auto size       = _vars.size();
    if (size > 0) oss << "\n\t: ";
    std::set<int> visited;   // 已经使用过 用来填充参数 的下标不再使用
    for (int i = 0; i < size; ++i) {
        if (is_default)
            oss << _vars[i].name << "{ " << _vars[i].val << " }";
        else {
            int         tmp = find_similar_type(_vars[i].type, f.args, visited);
            std::string val = "";   // 填充参数列表}
            if (tmp != -1) val = f.args[tmp].name;
            oss << _vars[i].name << "{ " << val << " }";
        }
        if (i < size - 1) oss << ", ";
    }
    oss << HTF_Function_Brackets_Str;
    return oss.str();
}

std::string Class::destructor_str(int i) const
{
    std::ostringstream oss;
    auto               f = _funs[i];
    oss << _name << "::~" << f.ret_type << "()" << HTF_Function_Brackets_Str;
    return oss.str();
}

std::ostream& operator<<(std::ostream& os, const Class& c)
{
    os << c.str();
    return os;
}

namespace detail
{

// 应为 var or fun 都是以 type + name 开头
// lex 只能 putback 一次
void get_fun_or_var(Lex& lex, std::vector<Variable>& vars, std::vector<Function>& funs)
{
    std::string type = lex.get().val;
    auto        peek = lex.peek();
    if ((peek.kind != LexerKind::identifier && peek.kind != LexerKind::type) ||
        peek.kind == LexerKind::type && !is_identifier(peek.val)) {
        lex.add_error(lex.peek(), "after" + mark(type) + "lack of idenfitier");
        lex.ignore_statement();
        return;
    }
    std::string name = lex.get().val;
    peek             = lex.peek();
    // ----------- function --------------
    if (peek.val == "(") {
        std::vector<Variable> args;
        if (!get_args(lex, args)) {
            lex.ignore_statement();
            return;
        }
        auto limits = get_function_limits(lex);
        if (lex.peek().val != ";") {
            lex.ignore_statement();
            return;
        }
        lex.get();
        funs.emplace_back(Function{type, name, args, limits});
    }
    // ------------ variable --------------
    else if (peek.val == "=" || peek.val == "{") {        // string s = string{};
        if (type.find("static") != std::string::npos) {   // static 成员不需要在类内初始化
            lex.ignore();
            return;
        }
        vars.emplace_back(Variable{type, name, get_value(lex)});
        if (lex.peek().val == ";") lex.get();
    }
    else if (peek.val == ";") {                           // bool flag;
        if (type.find("static") != std::string::npos) {   // static 成员不需要在类内初始化
            lex.ignore();
            return;
        }
        lex.get();
        vars.emplace_back(Variable{type, name});
    }
    else
        lex.ignore_statement();
}

Class get_class(Lex& lex)
{
    if (!is_class_key(lex.peek().val)) return Class{};
    lex.get();
    auto peek = lex.get();
    if (peek.val == "__declspec") {   // MSCV
        if (lex.peek().val != "(")
            lex.add_error(peek, "after" + mark(peek.val) + "lack of" + mark('('));
        lex.ignore_between_bracket();
        peek = lex.get();
    }
    if ((peek.kind == LexerKind::type && !is_identifier(peek.val)) ||   // 避免重定义
        (peek.kind != LexerKind::identifier && peek.kind != LexerKind::type)) {
        if (peek.val == "{")
            lex.add_error(peek, "cannot deal no name class");
        else
            lex.add_error(peek, "after" + mark("class") + "lack of identifier");
        lex.ignore_statement();
        return Class{};
    }
    std::string name = peek.val;
    lex.add_type(name);
    if (lex.peek().val == ";") {
        lex.get();
        return Class{};
    }
    lex.ignore_until();   // 跳过 : public X
    if (lex.peek().val != "{") {
        lex.ignore_statement();
        return Class{};
    }
    lex.get();
    int                   flag = 1;   // { 的个数，模拟 stack 匹配大括号
    std::vector<Variable> vars;
    std::vector<Function> funs;
    while (flag != 0) {
        peek = lex.get();
        if (peek.empty()) {
            return Class{};
        }
        if (peek.val == "}") {
            --flag;
            if (flag == 0) break;
        }
        if (peek.val == "{")
            ++flag;
        else if (is_class_limit_key(peek.val)) {
            if (lex.peek().val != ":") {   // 继续解析
                lex.add_error(peek, "after" + mark(peek.val) + "lack of" + mark(':'));
                continue;
            }
            lex.get();
        }
        // --------- construcotr ------------
        else if (peek.val == name && lex.peek().val == "(") {
            lex.putback(peek);
            auto f = get_function(lex);
            if (!f.empty()) funs.emplace_back(std::move(f));
        }
        // ---------- destructor ----------
        else if (peek.val == "~" && lex.peek().val == name) {
            lex.putback(peek);
            auto f = get_function(lex);
            if (!f.empty()) funs.emplace_back(std::move(f));
        }
        else if (peek.kind == LexerKind::type) {
            lex.putback(peek);
            get_fun_or_var(lex, vars, funs);
        }
        else if (peek.val == "virtual") {
            auto f = get_function(lex);
            if (!f.empty()) funs.emplace_back(std::move(f));
        }
        else
            lex.ignore_statement();
    }
    // class A{} | id1, id2, id3;
    while (true) {
        peek = lex.get();
        if (peek.val == ";") break;
        if (peek.kind != LexerKind::identifier) {
            lex.add_error(peek, "after" + mark("class " + name + "{..}") + "lack of" + mark(';'));
            lex.ignore();
            break;   // 只是末尾没有 ; 而已
        }
        if (lex.peek().val == ",")
            lex.get();
        else if (lex.peek().val != ";") {
            lex.add_error(peek,
                          "after" + mark("class " + name + "{..}.. " + mark(peek.val) + "lack of" +
                                         mark(';')));
            lex.ignore();
            break;
        }
    }
    return Class{name, vars, funs};
}

}   // namespace detail
}   // namespace htf
