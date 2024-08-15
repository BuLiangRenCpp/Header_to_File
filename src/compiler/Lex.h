#pragma once

// * 词法分析:
// * - 处理 PreProcess 产生的文件:
// *    · 对于 源文件 (-i 参数指定的) 输出 Lexer 给 Parse 处理
// *    · 对于 导入文件 只在内部记录文件定义的 type，不输出 Lexer
// * - 提取出 type:   (namespace:: 后的 id 视为 type -- std::cout 不会出现在头文件中 ->
// 因为函数定义部分会被忽略)
// * - lexer error

#include "Lexer.h"
#include "TokenStream.h"
#include "TypeTable.h"

namespace htf
{

class Lex
{
public:
    Lex() = default;
    // - tmp_file: 指定打开的文件 (必须是 Preprocess 文件)
    // - sources: 指定需要读取的源文件 (不是临时文件)，不能为空
    Lex(const path::Path& tmp_file, const std::set<FS::path>& sources);
    ~Lex() = default;

public:
    // 打开 PreProcess 的临时文件
    // - _types 不会丢失
    void open(const path::Path& tmp_file, const std::set<FS::path>& sources);

    bool        eof() const { return _eof; }
    FS::path    file() const { return _file; }
    line_t      line() const { return _ts.line(); }
    col_t       col() const { return _ts.col(); }
    std::string errors() const
    {
        std::ostringstream oss;
        for (int i = 0; i < _errors.size(); ++i) {
            oss << "[" << i << "] " << _errors[i].str() << "\n";
        }
        auto tmp = _ts.errors();
        return (_errors.empty()) ? tmp : tmp + "[lexer error]:\n" + oss.str();
    }

private:
    // 辅助 get: 主要实现识别 const type
    Lexer _get();
    Lexer _peek();

public:
    // 仅输出对 声明有用的 lexer
    Lexer get();
    Lexer peek();
    void  ignore(const Lexer& lexer = Lexer{';'});
    void  putback(const Lexer& lexer);
    void  ignore_until(const Lexer& lexer = Lexer{'{'});
    void  ignore_between_bracket();
    // 忽略 count 个 left brecket 对应的 right
    void ignore_brackets(char left_bracket, int count = 1);
    void ignore_statement();
    void ignore_current_file();
    void add_type(const std::string& type);
    void add_error(const Lexer& where, const std::string& what);

private:
    TokenStream              _ts;
    bool                     _eof     = true;
    FS::path                 _file    = "";   // 当前正在处理的源文件
    std::set<FS::path>    _sources = {};
    Buffer<Lexer>            _buffer;
    std::vector<CompilerError> _errors;
    TypeTable                _types;

private:
    Buffer<Lexer> _buffer_tmp;

private:
    // 处理 PreProcess 文件中非源文件部分
    // - 记录文件中的 type
    void update_type();
    void deal_template();
    // class and struct
    // * 目前不处理 内部类
    // * 仅将 该类名 加入 type
    // flag: 是否忽略 {} 之后的 ;
    void deal_class(bool flag = true);

private:
    // flag: 是否忽略 {} 之后的 ;
    void deal_enum(bool flag = true);
    // flag: 是否忽略 {} 之后的 ;
    void deal_union(bool flag = true);
    // 处理别名:
    // - using id = [typename] type;
    // - typedef type..  id;
    void deal_using();

private:
    // 处理基本类型，并且处理 type 之后的 *, &
    Lexer get_basic_type();
    // 同 get_basic_type，但不处理 unsigned signed 开头的 type
    std::string get_basic_type_un();
    // 处理 *, &, *const, &const
    std::string get_spe_type();

    // 处理 type 后的  <>, *, &，以及 内部类 ::type
    // * 由于语法过于复杂，因此将 <> 中的全部视为类型
    // * 对于类，还需要处理 内部类 (A<int>::value_type)
    std::string get_complex_type();
    // 读取 <> 之间
    void get_template_type(std::string& res, std::stack<char> stk);
    // 判断有它内部自己判断，调用者只需要判断 第一个是否是 id
    // 函数内部处理 ::type
    std::string get_namespace_type();
    // 处理 operator->  (视为 id -> 函数名)
    std::string get_operator_name();
};

}   // namespace htf