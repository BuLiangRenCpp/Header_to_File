#pragma once

// 语法错误

#include <sstream>

#include "Path.h"

namespace htf
{

class CompilerError
{
public:
    enum class ErrorCode
    {
        bad_preprocess,
        bad_token,
        bad_lexer,
        bad_syntax   // 语法错误
    };

public:
    CompilerError(const path::Path& file, line_t line, col_t col, const std::string& msg,
                  const ErrorCode& code = ErrorCode::bad_syntax);
    ~CompilerError() = default;

public:
    std::string str() const;

private:
    path::Path  _file;   // 错误发生的文件
    line_t      _line;   // 错误发生的行号
    col_t       _col;    // 错误发生的列数
    ErrorCode   _code;   // 错误类型
    std::string _msg;
};

}   // namespace htf