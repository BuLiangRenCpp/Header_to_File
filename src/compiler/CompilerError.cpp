#include "CompilerError.h"


namespace htf
{

CompilerError::CompilerError(const path::Path& file, line_t line, col_t col, 
    const std::string& msg, const ErrorCode& code)
	: _file{ file  }, _line{ line  }, _col{ col  }, _code{ code }, _msg{ msg }
{}

std::string CompilerError::str() const
{
    std::ostringstream oss;
    oss << _file << ":" << _line << ":" << _col << ":\n   ";
    switch (_code)
    {
    case ErrorCode::bad_preprocess:
        oss << "<bad preprocess>";
        break;
    case ErrorCode::bad_token:
        oss << "<bad token>";
        break;
    case ErrorCode::bad_lexer:
        oss << "<bad lexer>";
        break;
    case ErrorCode::bad_syntax:
        oss << "<bad syntax>";
        break;
    default:
        break;
    }
    oss << ": " << _msg;
    return oss.str();
}

}   // namespace htf
