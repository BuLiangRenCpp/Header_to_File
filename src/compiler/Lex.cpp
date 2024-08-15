#include "Lex.h"

#include "assertions.h"
#include "output.h"
#include "usage.h"

using namespace std;

const set<string> Basic_Type_Pre{"short", "int", "long", "signed", "unsigned"};
// 基本类型关键字 (只是可以出现在有效组合中的关键字)
const set<string> Basic_Type_Key{"short", "int", "long", "double"};
const set<string> Basic_Types{// 基本类型有效组合 (不带 unsigned signed)
                              "short",
                              "short int",
                              "int",
                              "long",
                              "long int",
                              "long long",
                              "long long int",
                              "long double"};

namespace htf
{

Lex::Lex(const path::Path& tmp_file, const std::set<FS::path>& sources)
    : _ts{tmp_file}
    , _eof{false}
    , _file{_ts.file()}
    , _sources{sources}
    , _buffer{}
    , _errors{}
    , _types{}
{
    HTF_DEV_ASSERT_MESSAGE(!_sources.empty(), "Lex::Lex(..): sources cannot be empty");
}

// 是否是 基础type 前缀
// 处理:
// short, short int
// int
// long, long int, long long, long long int, long double
// signed, siged char, signed + 上述
// unsigned, unsigned char, unsigned + 上述
static bool is_basic_type_pre(const Token& token)
{
    return token.kind == TokenKind::keyword && Basic_Type_Pre.count(token.val);
}

void Lex::open(const path::Path& tmp_file, const std::set<FS::path>& sources)
{
    _eof = false;
    _ts.open(tmp_file);
    _file    = _ts.file();
    _sources = sources;
    HTF_DEV_ASSERT_MESSAGE(!_sources.empty(), "Lex::open(..): sources cannot be empty");
    _buffer.clear();
}

Lexer Lex::_get()
{
    if (!_buffer_tmp.empty()) {
       return _buffer_tmp.get();
    }
    if (_eof) return Lexer{};
    while (true) {
        auto token = _ts.get();
        _file      = _ts.file();
        if (token.empty()) {
            _eof = true;
            return Lexer{};
        }
        // -------------- 非源文件 ---------------
        if (!_sources.count(_file)) {
            _ts.putback(token);
            update_type();
        }
        // ------------- 当前源文件 -------------
        else {
            // ---- ignore and update type ----
            if (is_using_key(token.val)) {
                _ts.putback(token);
                deal_using();
                return get();
            }
            if (token.val == "enum") {
                _ts.putback(token);
                deal_enum();
                return get();
            }
            if (token.val == "union") {
                _ts.putback(token);
                deal_union();
                return get();
            }
            if (token.val == "template") {
                _ts.putback(token);
                deal_template();
                return get();
            }
            // ------------ basic type ---------
            if (is_basic_type_pre(token)) {
                _ts.putback(token);
                return get_basic_type();
            }
            // ---------- other type ---------
            if (_types.exist(token.val)) {
                if (_ts.peek().val == "const")
                    token.val = _ts.get().val + " " + token.val;   // A const == const A
                Lexer res{token.val + get_complex_type(), LexerKind::type, token.line, token.col};
                return res;
            }
            // ---------- operator -----------
            if (token.val == "operator") {
                auto tmp = get_operator_name();
                if (tmp.empty()) {
                    _errors.emplace_back(
                        CompilerError{_ts.file(),
                                    token.line,
                                    token.col,
                                    "after" + mark("operator") + "lack of operation char",
                                    CompilerError::ErrorCode::bad_lexer});
                }
                return Lexer{"operator" + tmp, LexerKind::identifier, token.line, token.col};
            }
            // ------ namespace::type ------
            if (token.kind == TokenKind::identifier) {
                auto tmp = get_namespace_type();
                if (!tmp.empty()) {
                    token.val += tmp;
                    Lexer res{token};
                    res.kind = LexerKind::type;
                    return res;
                }
            }
            // ------- global :: ----------
            if (token.val == "::") {
                _ts.putback(token);
                auto  tmp = get_namespace_type();
                Lexer res{tmp, LexerKind::type, token.line, token.col};
                return res;
            }
            return Lexer{token};
        }
    }
}

// ! _peek 与 peek 共用一个 _buffer，存在 bug

Lexer Lex::get()
{
    if (!_buffer.empty()) {
        return _buffer.get();
    }
    if (_eof) return Lexer{};
    if (_peek().val == "const") {
        Lexer lexer = _get();
        if (_peek().val == "static") {   // const static
            lexer.val = lexer.val + " " + _get().val;
            if (_peek().kind == LexerKind::type) {
                lexer     = _get();
                lexer.val = "const static " + lexer.val;
            }
            return lexer;
        }
        if (_peek().kind == LexerKind::type) {
            lexer = _get();
            if (lexer.val.substr(0, 5) != "const") lexer.val = "const " + lexer.val;
        }
        return lexer;
    }
    if (_peek().val == "static") {
        Lexer lexer = _get();
        if (_peek().val == "const") {   // static const int
            lexer.val = lexer.val + " " + _get().val;
            if (_peek().kind == LexerKind::type) {
                lexer = _get();
                if (lexer.val.substr(0, 5) != "const") lexer.val = "static const " + lexer.val;
            }
            return lexer;
        }
        if (_peek().kind == LexerKind::type) {
            lexer     = _get();
            lexer.val = "static " + lexer.val;
        }
        return lexer;
    }
    return _get();
}

Lexer Lex::_peek()
{
    if (_buffer_tmp.empty()) _buffer_tmp.push_peek(_get());
    return _buffer_tmp.peek();
}

Lexer Lex::peek()
{
    if (_buffer.empty()) _buffer.push_peek(get());
    return _buffer.peek();
}

void Lex::putback(const Lexer& lexer)
{
    HTF_DEV_ASSERT_MESSAGE(!lexer.empty(), "Lex::putback(.): don't putback a empty lexer");
    _buffer.putback(lexer);
}

void Lex::ignore(const Lexer& lexer)
{
    ignore_until(lexer);
    get();
}

void Lex::ignore_until(const Lexer& lexer)
{
    if (eof()) return;
    if (_buffer.empty())
        peek();   // * 会将下一个 lexer load to buffer中，因此只需要判断 buffer 即可
    if (_buffer.peek() == lexer)   // * get -> 将 eof() load to buffer 中
        return;
    _buffer.get();
    ignore_until(lexer);
}

void Lex::ignore_between_bracket()
{
    if (is_lbracket(peek().val)) {
        std::stack<Lexer> stk;
        Lexer             lexer = get();
        stk.push(lexer);
        Lexer rbra = Lexer{ret_rbracket(lexer.val)};
        while (!this->eof() && !stk.empty()) {
            lexer = get();
            if (lexer == rbra)
                stk.pop();
            else if (lexer == stk.top())
                stk.push(lexer);
        }
    }
}

void Lex::ignore_brackets(char bracket, int count)
{
    HTF_DEV_ASSERT_MESSAGE(is_lbracket(bracket),
        "Lex::ignore_brackets(..):" << mark(bracket) + "isn't a left bracket");
    char rbra = ret_rbracket(bracket);
    while (count > 0) {
        auto lexer = get();
        if (lexer.empty()) break;
        if (lexer.kind == LexerKind::schar) {
            if (lexer.val[0] == rbra) --count;
        }
    }
}

void Lex::ignore_statement()
{
    while (true) {
        auto lexer = get();
        if (lexer.empty()) {
            _eof = true;
            return;
        }
        if (lexer.val == "{") {
            putback(lexer);
            ignore_between_bracket();
            return;
        }
        if (lexer.val == ";") return;
    }
}

void Lex::ignore_current_file()
{
    if (_sources.count(_file)) {
        _sources.erase(_file);   // update type
        _buffer.clear();
        peek();
    }
}

void Lex::add_type(const std::string& type)
{
    _types.add(type);
}

void Lex::add_error(const Lexer& where, const string& error)
{
    _errors.emplace_back(CompilerError{_file, where.line, where.col, error});
}

void Lex::update_type()
{
    while (true) {
        auto token = _ts.get();
        if (token.empty()) {
            _eof = true;
            return;
        }
        if (_sources.count(_ts.file())) {
            _ts.putback(token);
            break;
        }
        if (token.kind == TokenKind::keyword) {
            _ts.putback(token);
            if (is_using_key(token.val)) {
                deal_using();
            }
            else if (is_class_key(token.val)) {
                deal_class();
            }
            else if (token.val == "template") {
                deal_template();
            }
            else if (token.val == "enum") {
                deal_enum();
            }
            else if (token.val == "union") {
                deal_union();
            }
            else
                _ts.get();
        }
    }
}

void Lex::deal_template()
{
    auto peek = _ts.get();
    peek      = _ts.peek();
    if (peek.val != "<") {
        _errors.emplace_back(CompilerError{
            _ts.file(), peek.line, peek.col, "after" + mark("template") + "lack of" + mark('<')});
        return;
    }
    _ts.ignore_between_bracket();
    peek = _ts.peek();
    if (peek.val == "class" || peek.val == "struct") deal_class();
    // ------ template function ----
    else {
        _ts.ignore_until('(');
        _ts.ignore_between_bracket();
        _ts.ignore_statement();
    }
}

void Lex::deal_class(bool flag)
{
    auto token = _ts.get();   // class struct
    token      = _ts.peek();
    if (token.val == "__declspec") {   // MSCV
        _ts.get();
        ignore_between_bracket();   //
        token = _ts.get();
    }
    if (token.kind == TokenKind::identifier) {
        _types.add(token.val);
        _ts.get();
    }
    if (_ts.peek().val == ";") {
        if (flag) _ts.get();
        return;
    }
    _ts.ignore_until();
    _ts.ignore_between_bracket();
    if (flag) _ts.ignore();
}

void Lex::deal_enum(bool flag)
{
    auto token = _ts.get();
    token      = _ts.peek();
    if (is_class_key(token.val)) {   // enum class A
        token = _ts.get();
        if (_ts.peek().kind == TokenKind::identifier) {
            token = _ts.get();
            _types.add(token.val);
        }
        else {
            _errors.emplace_back(
                CompilerError{_ts.file(),
                            token.line,
                            token.col,
                            "after" + mark("enum" + token.val) + "lack of identifier"});
        }
    }
    _ts.ignore_until();
    _ts.ignore_between_bracket();
    if (flag) _ts.ignore();
}

void Lex::deal_union(bool flag)
{
    auto token = _ts.get();
    token      = _ts.peek();
    if (token.kind == TokenKind::identifier) {
        _types.add(token.val);
        _ts.get();
    }
    _ts.ignore_between_bracket();
    if (flag) _ts.ignore();
}

void Lex::deal_using()
{
    auto token = _ts.get();
    // using ID = [typename] type;
    if (token.val == "using") {
        token = _ts.get();
        if (token.kind == TokenKind::identifier) {
            if (_ts.peek().val == "=") {
                _types.add(token.val);
                _ts.get();
                token = _ts.peek();
                if (is_class_key(token.val)) {
                    deal_class();
                    return;
                }
                if (token.val == "enum") {
                    deal_enum();
                    return;
                }
                if (token.val == "union") {
                    deal_union();
                    return;
                }
            }
        }
        _ts.ignore();
    }
    // typedef type.. ID;
    else {
        token = _ts.peek();
        if (is_class_key(token.val)) {
            deal_class(false);
            if (_ts.peek().kind == TokenKind::identifier) {
                auto tmp = _ts.get();
                _types.add(tmp.val);
                if (_ts.peek().val != ";") {
                    _errors.emplace_back(
                        CompilerError{_ts.file(),
                                    tmp.line,
                                    tmp.col,
                                    "after" + mark(tmp.val) + "lack of" + mark(';')});
                }
                _ts.ignore();
            }
            return;
        }
        if (token.val == "enum") {
            deal_enum(false);
            if (_ts.peek().kind == TokenKind::identifier) {
                auto tmp = _ts.get();
                _types.add(tmp.val);
                if (_ts.peek().val != ";") {
                    _errors.emplace_back(
                        CompilerError{_ts.file(),
                                    tmp.line,
                                    tmp.col,
                                    "after" + mark(tmp.val) + "lack of" + mark(';')});
                }
                _ts.ignore();
            }
            return;
        }
        if (token.val == "union") {
            deal_union(false);
            if (_ts.peek().kind == TokenKind::identifier) {
                auto tmp = _ts.get();
                _types.add(tmp.val);
                if (_ts.peek().val != ";") {
                    _errors.emplace_back(
                        CompilerError{_ts.file(),
                                    tmp.line,
                                    tmp.col,
                                    "after" + mark(tmp.val) + "lack of" + mark(';')});
                }
                _ts.ignore();
            }
            return;
        }
        while (true) {
            token = _ts.get();
            if (token.empty()) {
                _eof = true;
                return;
            }
            if (_ts.peek().val == ";") {
                _types.add(token.val);
                _ts.get();
                return;
            }
        }
    }
}

// short, short int
// int
// long, long int, long long, long long int, long double
// signed, siged char, signed + 上述
// unsigned, unsigned char, unsigned + 上述
Lexer Lex::get_basic_type()
{
    auto  token = _ts.get();
    Lexer res{token.val, LexerKind::type, token.line, token.col};

    if (!(token.val == "unsigned" || token.val == "signed")) {
        _ts.putback(token);
        res.val = "";
    }
    auto type = get_basic_type_un();
    if (!res.val.empty() && !type.empty() && isalpha(type[0]))
        res.val = res.val + " " + type;
    else
        res.val = res.val + type;
    return res;
}

std::string Lex::get_basic_type_un()
{
    string res;
    auto   peek = _ts.peek();
    if (peek.val == "unsigned" || peek.val == "signed") {
        _errors.emplace_back(CompilerError{_ts.file(),
                                         peek.line,
                                         peek.col,
                                         "bad type: redefine" + mark(peek.val),
                                         CompilerError::ErrorCode::bad_lexer});
        return "";
    }
    if (peek.empty()) {
        _eof = true;
        return "";
    }
    // * 中间的 多个 const 转为一个，提到类型前面
    bool have_const = false;
    while (true) {
        peek = _ts.get();
        if (Basic_Type_Key.count(peek.val)) {
            res = res + " " + peek.val;
        }
        else if (peek.val == "const") {
            have_const = true;
        }
        else {
            _ts.putback(peek);
            break;
        }
    }
    if (!res.empty()) {
        res.erase(0, 1);
        if (!Basic_Types.count(res)) {
            _errors.emplace_back(CompilerError{_ts.file(),
                                             peek.line,
                                             peek.col,
                                             "invalid type" + mark(res),
                                             CompilerError::ErrorCode::bad_lexer});
            return res;
        }
    }
    if (have_const) res = "const " + res;
    return res += get_spe_type();
}

std::string Lex::get_spe_type()
{
    string res;
    auto   peek = _ts.peek();
    // *  const 放在其左边最近的 * 之后，如果没有则不保留 const
    bool have_const = false;
    while (true) {
        peek = _ts.get();
        if (peek.empty()) break;
        if (peek.val == "*" || peek.val == "&")
            res += peek.val;
        else if (peek.val == "const") {
            auto pos = res.rfind('*');
            if (pos != string::npos) {
                res.insert(pos + 1, " const");
            }
        }
        else {
            _ts.putback(peek);
            break;
        }
    }
    return res;
}

std::string Lex::get_complex_type()
{
    auto tmp = get_spe_type();
    if (tmp != "") return tmp;
    // ------- template class ------
    string res;
    if (_ts.peek().val == "<") {
        res += _ts.get().val;
        stack<char> stk;
        stk.push('<');
        get_template_type(res, stk);
    }
    res = res + get_namespace_type() + get_spe_type();
    return res;
}

void Lex::get_template_type(string& res, std::stack<char> stk)
{
    auto token = _ts.get();
    if (token.val == "<") {
        stk.push('<');
        res += "<";
        if (_ts.peek().val == "<") {   // <<
            _errors.emplace_back(CompilerError{_ts.file(),
                                             token.line,
                                             token.col,
                                             "after" + mark(token.val) + "lack of identifier",
                                             CompilerError::ErrorCode::bad_lexer});
        }
        return get_template_type(res, stk);
    }
    else if (token.val == ">") {
        res += ">";
        if (stk.empty()) {
            _errors.emplace_back(
                CompilerError{_ts.file(),
                            token.line,
                            token.col,
                            "bad template type: before" + mark(res) + "lack of" + mark('<'),
                            CompilerError::ErrorCode::bad_lexer});
            return;
        }
        stk.pop();
        if (stk.empty()) return;
        return get_template_type(res, stk);
    }
    else if (token.val == ",") {
        if (!is_id_ch(res.back())) {
            _errors.emplace_back(
                CompilerError{_ts.file(),
                            token.line,
                            token.col,
                            "bad template type: after" + mark(res) + "lack of valid type",
                            CompilerError::ErrorCode::bad_lexer});
            return;
        }
        res += ", ";
    }
    else
        res += token.val;
    if (token.empty()) return;
    return get_template_type(res, stk);
}

std::string Lex::get_namespace_type()
{
    if (_ts.peek().val != "::") return "";
    string res;
    res += _ts.get().val;
    bool flag = false;
    while (true) {
        if (_ts.peek().kind == TokenKind::identifier) {
            auto tmp = _ts.get();
            res += tmp.val;
            flag = true;
            if (_types.exist(tmp.val) && _ts.peek().val == "<") {   // std::vector<int>::value_type
                res += _ts.get().val;
                stack<char> stk;
                stk.push('<');
                get_template_type(res, stk);
            }
        }
        if (_ts.peek().val == "::") {
            auto tmp = _ts.get();
            if (flag == false) {
                _errors.emplace_back(CompilerError{_ts.file(),
                                                 tmp.line,
                                                 tmp.col,
                                                 "after" + mark(res) + "lack of indentifier",
                                                 CompilerError::ErrorCode::bad_lexer});
                return res;
            }
            res += tmp.val;
            if (_types.exist(tmp.val) && _ts.peek().val == "<") {
                res += "<";
                stack<char> stk;
                stk.push('<');
                get_template_type(res, stk);
            }
        }
        else
            break;
    }
    return res + get_spe_type();
}

std::string Lex::get_operator_name()
{
    auto token = _ts.get();
    if (token.empty()) return "";
    string res;
    if (token.val == "new" || token.val == "delete") {
        res   = " " + token.val;
        token = _ts.get();
        if (token.val == "[") {
            if (_ts.peek().val != "]") {
                _errors.emplace_back(
                    CompilerError{_ts.file(),
                                token.line,
                                token.col,
                                "after" + mark(token.val + "[") + "lack of" + mark(']'),
                                CompilerError::ErrorCode::bad_lexer});
                return res;
            }
            res += "[]";
        }
        else
            _ts.putback(token);
    }
    else if (token.kind == TokenKind::dchar) {
        res += token.val;
    }
    else if (token.kind == TokenKind::schar && !is_id_ch(token.val[0])) {
        res += token.val;
        while (true) {
            token = _ts.get();
            if (token.empty()) break;
            if (is_id_ch(token.val[0]) || token.val == "(" || token.val == ";") {
                _ts.putback(token);
                break;
            }
            res += token.val;
        }
    }
    return res;
}

}   // namespace htf
