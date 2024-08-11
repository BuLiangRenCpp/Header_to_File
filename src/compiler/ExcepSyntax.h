#pragma once

// 语法错误

#include <sstream>

#include "Cfile.h"
#include "ExcepBase.h"

namespace htf
{

enum class SyntaxError
{
    bad_preprocess,
    bad_token,
    bad_lexer,
    bad_syntax   // 语法错误
};

class ExcepSyntax : public ExcepBase
{
public:
    // - source: 该行的内容
    // - what: 错误描述
    // 输出格式：
    // file:line:col:
    //      source
    //          ^ what
    ExcepSyntax(const path::Cfile& file, line_t line, col_t col, const what_type& what,
                const SyntaxError& error_type = SyntaxError::bad_syntax);
    ~ExcepSyntax() = default;

public:
    std::string where() const override
    {
        std::ostringstream oss;
        oss << _file << ":" << _line << ":" << _col;
        return oss.str();
    }

    std::string str() const override;

private:
    path::Cfile _file;         // 错误发生的文件
    line_t      _line;         // 错误发生的行号
    col_t       _col;          // 错误发生的列数
    SyntaxError _error_type;   // 错误类型
};

}   // namespace htf