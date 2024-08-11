#include "ExcepSyntax.h"

namespace htf
{

ExcepSyntax::ExcepSyntax(const path::Cfile& file, line_t line, col_t col, const what_type& what,
                         const SyntaxError& error_type)
    : _file{file}, _line{line}, _col{col}, _error_type{error_type}, ExcepBase{"", what}
{}

std::string ExcepSyntax::str() const
{
    std::ostringstream oss;
    oss << _file << ":" << _line << ":" << _col << ":\n";
    oss << "    ";
    switch (_error_type) {
    case SyntaxError::bad_preprocess: oss << "<bad preprocess>"; break;
    case SyntaxError::bad_token: oss << "<bad token>"; break;
    case SyntaxError::bad_lexer: oss << "<bad lexer>"; break;
    case SyntaxError::bad_syntax: oss << "<bad syntax>"; break;
    default: break;
    }
    oss << " " << _what;
    return oss.str();
}

}   // namespace htf
