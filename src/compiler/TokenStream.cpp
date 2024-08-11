#include "TokenStream.h"

#include "ExcepSyntax.h"
#include "assertions.h"
#include "usage.h"

using namespace std;

namespace htf
{

TokenStream::TokenStream(const path::Cfile& tmp_file)
    : _ifs{tmp_file.str()}, _eof{false}, _file{}, _line{1}, _col{0}, _errors{}, _stk{}
{
    HTF_DEV_ASSERT_MESSAGE(_ifs.good(), "TokenStream::TokenStream(.): bad ifstream");
    string str;
    getline(_ifs, str);
    HTF_DEV_ASSERT_MESSAGE(str == HTF_PRE_ID,
                           "TokenStream::TokenStream(.): not htf temp file" << HTF_DEV_MARK(_file));
    getline(_ifs, str);
    HTF_DEV_ASSERT_MESSAGE(str.length() > 2 && str.substr(0, 2) == "##",
                           "TokenStream::TokenStream(.): htf temp file: line 2 must be"
                               << HTF_DEV_MARK("##path"));
    _file = str.substr(2);
    HTF_DEV_ASSERT_MESSAGE(FS::exists(_file), "TokenStream::TokenStream(..): source must be exist");
}

void TokenStream::open(const path::Cfile& tmp_file)
{
    _ifs.close();
    _ifs.open(tmp_file.str());
    if (!_ifs.is_open())
        throw path::ExcepPath{"TokenStream::open(.)", _file, path::PathErrors::not_open_file};
    string str;
    getline(_ifs, str);
    HTF_DEV_ASSERT_MESSAGE(str == HTF_PRE_ID,
                           "TokenStream::open(.): not htf temp file" << HTF_DEV_MARK(_file));
    _line = 1;
    _col  = 0;
    _eof  = false;
    _buffer.clear();
    while (!_stk.empty()) _stk.pop();
    getline(_ifs, str);
    HTF_DEV_ASSERT_MESSAGE(str.length() > 2 && str.substr(0, 2) == "##",
                           "TokenStream::open(.): htf temp file: line 2 must be"
                               << HTF_DEV_MARK("##path"));
    _file = str.substr(2);
    HTF_DEV_ASSERT_MESSAGE(FS::exists(_file), "TokenStream::TokenStream(..): source must be exist");
}

Token TokenStream::get()
{
    if (!_buffer.empty()) {
        return _buffer.get();
    }
    if (_eof) return Token{};
    char c = ' ';
    while (true) {
        c = _ifs.get();
        ++_col;
        if (_ifs.eof()) {
            _eof = true;
            return Token{};
        }
        while (isspace(c)) {
            if (c == '\n') {
                ++_line;
                _col = 0;
            }
            c = _ifs.get();
            if (_ifs.eof()) {
                _eof = true;
                return Token{};
            }
            if (c != '\n') ++_col;
        }
        _ifs.putback(c);
        --_col;
        // ------------------------------
        if (c == '#') {
            deal_pre();
            return get();
        }
        // ---------- constant ----------
        if (isdigit(c)) {
            return get_constant();
        }
        // ----------- string_literal ---------
        if (c == '\'' || c == '\"') {
            return get_string_literal();
        }
        //  -- encoding-prefix string_literal --
        if ((c == 'u' || c == 'U' || c == 'L' || c == 'R')) {
            char c = _ifs.get();
            ++_col;
            if (_ifs.peek() == '\"') {   // [u|U|L] " "
                return get_string_literal();
            }
            if ((c == 'u' && _ifs.peek() == '8')) {
                _ifs.get();
                ++_col;
                if (_ifs.peek() == '\"') {   // u8 ""
                    return get_string_literal();
                }
                else {   // u8 开头的 identifier
                    auto res = get_identifier();
                    res.val  = "u8" + res.val;
                    if (is_keyword(res.val)) res.kind = TokenKind::keyword;
                    return res;
                }
            }
            auto res = get_identifier();   // [u|U|L] 开头的 identifier
            res.val  = string{c} + res.val;
            if (is_keyword(res.val)) res.kind = TokenKind::keyword;
            return res;
        }
        // ------- identifier or keyword --------
        if (isalpha(c) || c == '_') {   // 必须以 非数字字符开头
            return get_identifier();
        }
        // -------------- schar -----------------
        _ifs.get();
        // * 特殊 constant
        if ((c == '.' || c == '+' || c == '-') && isdigit(_ifs.peek())) {
            _ifs.putback(c);
            return get_constant();
        }
        // -------------- dchar ----------------
        if (is_dchar_fir(c)) {   // * ->
            _ifs.putback(c);
            return get_dchar();
        }
        ++_col;
        return Token{c, _line, _col};
    }
}

Token TokenStream::peek()
{
    if (_buffer.empty()) _buffer.push_peek(get());
    return _buffer.peek();
}

void TokenStream::deal_pre()
{
    _ifs.get();
    string s;
    _ifs >> s;
    if (_ifs.peek() == '\n') {
        _line++;
        _col = 0;
        _ifs.get();
    }
    if (s == "end") {
        HTF_DEV_ASSERT_MESSAGE(
            !_stk.empty(), "TokenStream::deal_pre(): there are some errors in the PreProcess file");
        auto top = _stk.top();
        _file    = top.file;
        _line    = top.line;
        _col     = top.col;
        _stk.pop();
    }
    else {   // #path
        _stk.emplace(Info{_file, _line, _col});
        _file = s;
        _line = 1;
        _col  = 0;
    }
}

void TokenStream::putback(const Token& token)
{
    HTF_DEV_ASSERT_MESSAGE(!token.empty(), "TokenStream::putback(): don't putback a empty token");
    _buffer.putback(token);
}

void TokenStream::ignore(const Token& token)
{
    ignore_until(token);
    get();
}

void TokenStream::ignore_until(const Token& token)
{
    if (eof()) return;
    if (_buffer.empty())
        peek();   // * 会将下一个 token load to buffer中，因此只需要判断 buffer 即可
    if (_buffer.peek() == token)   // * get -> 将 eof() load to buffer 中
        return;
    _buffer.get();
    ignore_until(token);
}

void TokenStream::ignore_between_bracket()
{
    if (is_lbracket(peek().val)) {
        std::stack<Token> stk;
        Token             token = get();
        stk.push(token);
        Token rbra = Token{ret_rbracket(token.val)};
        while (!this->eof() && !stk.empty()) {
            token = get();
            if (token == rbra)
                stk.pop();
            else if (token == stk.top())
                stk.push(token);
        }
    }
}

void TokenStream::ignore_statement()
{
    while (true) {
        char c = _ifs.get();
        if (_ifs.eof()) return;
        if (c == ';') return;
        if (c == '{') {
            _ifs.putback(c);
            ignore_between_bracket();
            return;
        }
    }
}

// void TokenStream::ignore_current_file()
// {
//     string old = _file;
//     while (true) {
//         auto token = peek();
//         if (token.empty() || old != _file) return;
//         get();
//     }
// }

// 读取 . 之后的值
string TokenStream::get_float()
{
    string res = get_decimal();
    if (_ifs.peek() == 'e' || _ifs.peek() == 'E') {
        res += _ifs.get();
        if (_ifs.peek() == '-' || _ifs.peek() == '+') {
            res += _ifs.get();
        }
        if (isdigit(_ifs.peek())) {
            res += get_decimal();
        }
        else {
            _errors.emplace_back(ExcepSyntax{
                _file, _line, _col, "bad float number" + mark(res), SyntaxError::bad_token});
        }
    }
    // float-suffix
    if (_ifs.peek() == 'f' || _ifs.peek() == 'F' || _ifs.peek() == 'l' || _ifs.peek() == 'L') {
        res.push_back(_ifs.get());
    }
    return res;
}

std::string TokenStream::get_decimal()
{
    string num;
    while (true) {
        char c = _ifs.get();
        if (_ifs.eof()) {
            _eof = true;
            break;
        }
        if (!isdigit(c)) {
            _ifs.putback(c);
            break;
        }
        num.push_back(c);
    }
    return num;
}

std::string TokenStream::get_binary()
{
    string num;
    bool   res = true;
    while (true) {
        char c = _ifs.get();
        if (_ifs.eof()) {
            _eof = true;
            break;
        }
        if (!isdigit(c)) {
            _ifs.putback(c);
            break;
        }
        if (c != '0' && c != '1') res = false;
        num.push_back(c);
    }
    if (res == false)
        _errors.emplace_back(ExcepSyntax{
            _file, _line, _col, "bad binary number" + mark(num), SyntaxError::bad_token});
    return num;
}

std::string TokenStream::get_octal()
{
    string num;
    bool   res = true;
    while (true) {
        char c = _ifs.get();
        if (_ifs.eof()) {
            _eof = true;
            break;
        }
        if (!isdigit(c)) {
            if (res == false && c != '.') {
                _errors.emplace_back(ExcepSyntax{
                    _file, _line, _col, "bad octal number" + mark(num), SyntaxError::bad_token});
            }
            _ifs.putback(c);
            break;
        }
        if (!(c >= '0' && c <= '7')) res = false;
        num.push_back(c);
    }
    return num;
}

std::string TokenStream::get_hex()
{
    string num;
    while (true) {
        char c = _ifs.get();
        if (_ifs.eof()) {
            _eof = true;
            break;
        }
        if (!(isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))) {
            _ifs.putback(c);
            break;
        }
        num.push_back(c);
    }
    return num;
}

std::string TokenStream::get_interger_suffix()
{
    if (_eof) return "";
    string suffix;
    switch (_ifs.peek()) {
    case 'u':
    case 'U':   // u  ul  ull
    {
        suffix.push_back(_ifs.get());
        if (_ifs.peek() == 'L' || _ifs.peek() == 'l') {
            suffix.push_back(_ifs.get());
            if (_ifs.peek() == 'L' || _ifs.peek() == 'l') {
                suffix.push_back(_ifs.get());
            }
        }
        break;
    }
    case 'l':
    case 'L':   // lu  llu
    {
        suffix.push_back(_ifs.get());
        if (_ifs.peek() == 'u' || _ifs.peek() == 'U') {
            suffix.push_back(_ifs.get());
        }
        else if (_ifs.peek() == 'L' || _ifs.peek() == 'l') {
            suffix.push_back(_ifs.get());
            if (_ifs.peek() == 'u' || _ifs.peek() == 'U') {
                suffix.push_back(_ifs.get());
            }
        }
        break;
    }
    default: break;
    }
    return suffix;
}

Token TokenStream::get_constant()
{
    char  c = _ifs.get();
    Token res{string{c}, TokenKind::constant, _line, _col};

    ostringstream oss;
    if (c == '.') {
        res.val += get_float();
        return res;
    }
    if (c == '+' || c == '-') {
        c = _ifs.get();
    }
    // ----------- 其他进制数 -----------
    if (c == '0') {
        // -------- octal -----------
        if (isdigit(_ifs.peek())) {
            res.val += get_octal();
            if (_ifs.peek() == '.') {
                res.val = res.val + "." + get_float();
                return res;
            }
        }
        // -------- other -----------
        else {
            switch (_ifs.peek()) {
            case 'b':
            case 'B':
            {
                c       = _ifs.get();
                res.val = res.val + string{c} + get_binary();
                break;
            }
            case 'x':
            case 'X':
            {
                c       = _ifs.get();
                res.val = res.val + string{c} + get_hex();
                break;
            }
            default: break;
            }
        }
    }   // c == '0'
    // ------------ 十进制 --------------
    else {
        res.val += get_decimal();
        if (_ifs.peek() == '.') {
            res.val = res.val + "." + get_float();
            return res;
        }
    }
    res.val += get_interger_suffix();
    _col += res.val.length();
    return res;
}

Token TokenStream::get_string_literal()
{
    Token res{"", TokenKind::string_literal, _line, _col};
    char  end = _ifs.get();
    res.val.push_back(end);
    while (true) {
        char c = _ifs.get();
        if (_ifs.eof()) {
            _eof = true;
            break;
        }
        // 转义字符
        if (c == '\\' && (_ifs.peek() == '\'' || _ifs.peek() == '"')) {
            res.val.push_back(_ifs.get());
            continue;
        }
        if (c == end) {
            res.val.push_back(c);
            break;
        }
        res.val.push_back(c);
    }
    _col += res.val.length();
    return res;
}

Token TokenStream::get_identifier()
{
    Token res{"", TokenKind::identifier, _line, _col};
    if (!is_id_ch(_ifs.peek())) return res;
    while (true) {
        char c = _ifs.get();
        if (_ifs.eof()) {
            _eof = true;
            break;
        }
        if (!is_id_ch(c)) {
            _ifs.putback(c);
            break;
        }
        res.val.push_back(c);
    }
    if (is_keyword(res.val)) res.kind = TokenKind::keyword;
    _col += res.val.length();
    return res;
}

Token TokenStream::get_dchar()
{
    Token res{"", TokenKind::schar, _line, _col};
    char  c = _ifs.get();
    res.val.push_back(c);
    if (c == ':' && _ifs.peek() == ':') {
        res.val  = "::";
        res.kind = TokenKind::dchar;
        _ifs.get();
    }
    else if (c == '-' && _ifs.peek() == '>') {
        res.val  = "->";
        res.kind = TokenKind::dchar;
        _ifs.get();
    }
    else if (c == '<' && _ifs.peek() == '<') {
        res.val  = "<<";
        res.kind = TokenKind::dchar;
        _ifs.get();
    }
    _col += res.val.length();
    return res;
}

}   // namespace htf
