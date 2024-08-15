#pragma once

// TokenStream 将基本单元提取出来

#include <stack>

#include "Buffer.hpp"
#include "PreProcess.h"
#include "Token.h"

namespace htf
{

/**
 * ! 此类由针对 PreProcess 生成的文件
 * ! 此类的 eof 方法与 cin.eof 方法作用是一致的，但是实现有区别，会导致：
 * - 如果文件一开始就是空：那么 eof = true  -- cin.eof = false (在执行一次 cin >> c，那么
 * cin.eof = true)
 * - 如果文件中只有一个有效的 token：
 *      - cin.eof: after "cin >> c;" -> cin.eof = false  (在读取一次才 cin.eof = true)
 *      - eof:     after "ts  >> c;" ->     eof = true
 * * 此类的 line 函数并不是特别精确：对于处理边界情况(在一行的末尾时)会与实际相差 1，
 * * 这基本无影响
 */

// 对于错误的词法单元直接报错
class TokenStream
{
public:
    TokenStream() = default;
    // - tmp_file: 临时文件路径
    // - source: 临时文件对应的源文件路径 ( = "" 默认为 tmp_file 更换后缀名为 .h)
    // ! 原本想直接在 PreProcess 处理时在 tmp_file 中加上路径，但是这存在 bug:
    // !    .i 文件开始就是连续的 #file.h，那么调用 file() 返回的时最后一个
    TokenStream(const path::Path& tmp_file);
    ~TokenStream() = default;

public:
    // 使用此接口的目的是:
    // * 后期优化程序时使用，读取过的文件不在读取，保留识别出的 type
    void        open(const path::Path& tmp_file);
    bool        eof() const { return _eof; }
    Token       get();
    Token       peek();
    FS::path    file() const { return _file; }
    line_t      line() const { return _line; }
    col_t       col() const { return _col; }
    std::string errors() const
    {
        if (_errors.empty()) return "";
        std::ostringstream oss;
        for (int i = 0; i < _errors.size(); ++i) {
            oss << "[" << i << "] " << _errors[i].str() << "\n";
        }
        return "[token error]:\n" + oss.str();
    }

private:
    std::ifstream            _ifs;
    bool                     _eof = true;
    Buffer<Token>            _buffer;
    FS::path                 _file;       // 当前文件
    line_t                   _line = 1;   // 当前行号
    col_t                    _col  = 0;   // 当前列号
    std::vector<CompilerError> _errors;

private:
    struct Info {
        FS::path file;
        line_t      line;
        col_t       col;

        Info(const FS::path& f, line_t l, col_t c) : file{f}, line{l}, col{c} {}
    };
    // ---------------
    std::stack<Info> _stk;

private:
    // 处理 #file.h ... #end
    void deal_pre();

public:
    void putback(const Token& token);
    // 一直忽略到 token，token 也忽略
    // 默认忽略一条语句
    void ignore(const Token& token = Token{';'});
    // 一直忽略直到 token，(token不忽略)
    // 默认忽略到 '{' (主要用于类 class 忽略掉 class A ": public xx" {})
    void ignore_until(const Token& token = Token{'{'});
    // 忽略 括号之间的部分 (注：按匹配规则，如 '{1 { 2 } 3} 4' 会一直忽略到 '4' 之前)
    // - 忽略的括号 (应为左括号) 是看 _ifs.peek，如果是 左括号 则执行，不是则不执行
    // - 如果到文件结束了但是括号没有匹配完，不报错，全忽略
    void ignore_between_bracket();
    // 先遇见 '{' 就使用 ignore_between_bracket
    // 先遇到 ';' 就停止
    void ignore_statement();
    // // 跳过当前文件
    // void ignore_current_file();

private:
    // 处理 R"xx()xx"
    // 调用这必须判断字符以 R" 开头，并且将 R 读取掉
    std::string get_raw_string();
    // 读取 . 之后的值以及 flaot-suffix
    std::string get_float();
    // 以下只读取前缀到后缀之间的数字
    std::string get_decimal();
    std::string get_binary();
    // * 读取 0-9 之间的数字:
    // * - 如果遇到 . 则将其看做 十进制
    // * - 否则是 八进制
    std::string get_octal();
    std::string get_hex();
    // 读取 interger-suffix
    std::string get_interger_suffix();

    Token get_constant();
    // ''  or  ""
    // * 目前对转义字符的处理仅处理:  \" \'
    Token get_string_literal();
    // 包括 keyword
    // ! 第一个字符由调用者判断: 内部仅判断第一个字符是否是 非 id 字符
    // * 即不判断是否以 非数字字符 开头
    std::string get_identifier();
    // 调用者判断第一个字符是 dchar first，
    // 但可能返回的是 schar
    Token get_dchar();
};

}   // namespace htf