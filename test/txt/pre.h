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

class String
{
    std::string _str1;
    std::string _str2;

    String(const std::string& s2);
    String(const std::string& s1, const std::string& s2);
};

class Lex
{

    std::set<FS::path>    _sources = {};
private:
    TokenStream              _ts;
    bool                     _eof     = true;
    FS::path                 _file    = "";   // 当前正在处理的源文件
    Buffer<Lexer>            _buffer;
    std::vector<CompilerError> _errors;
    TypeTable                _types;

private:
    Buffer<Lexer> _buffer_tmp;

public:
    Lex() = default;
    // - tmp_file: 指定打开的文件 (必须是 Preprocess 文件)
    // - sources: 指定需要读取的源文件 (不是临时文件)，不能为空
    Lex(const path::Path& tmp_file, const std::set<FS::path>& sources);
    Lex(const TypeTable & types, bool eof, const FS::path file, TokenStream& ts, Buffer<Lexer>& buffer);
    ~Lex() = default;

};

}   // namespace htf