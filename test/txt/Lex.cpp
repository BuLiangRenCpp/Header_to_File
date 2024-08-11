#include "D:\code\git\Header_to_File\src\compiler\Lex.h"


namespace htf
{

Lex::Lex(const path::Cfile& tmp_file, const std::set<std::string>& sources)
	: _ts{  }, _eof{  }, _file{  }, _sources{  }, _buffer{  }, _errors{  }, _types{  }, _buffer_tmp{  }
{

}

void Lex::open(const path::Cfile& tmp_file, const std::set<std::string>& sources)
{

}

Lexer Lex::_get()
{

}

Lexer Lex::_peek()
{

}

Lexer Lex::get()
{

}

Lexer Lex::peek()
{

}

void Lex::ignore(const Lexer& lexer)
{

}

void Lex::putback(const Lexer& lexer)
{

}

void Lex::ignore_until(const Lexer& lexer)
{

}

void Lex::ignore_between_bracket()
{

}

void Lex::ignore_brackets(char left_bracket, int count)
{

}

void Lex::ignore_statement()
{

}

void Lex::ignore_current_file()
{

}

void Lex::add_type(const std::string& type)
{

}

void Lex::add_error(const Lexer& where, const std::string& what)
{

}

void Lex::update_type()
{

}

void Lex::deal_template()
{

}

void Lex::deal_class(bool flag)
{

}

void Lex::deal_enum(bool flag)
{

}

void Lex::deal_union(bool flag)
{

}

void Lex::deal_using()
{

}

Lexer Lex::get_basic_type()
{

}

std::string Lex::get_basic_type_un()
{

}

std::string Lex::get_spe_type()
{

}

std::string Lex::get_complex_type()
{

}

void Lex::get_template_type(std::string& res, std::stack<char> stk)
{

}

std::string Lex::get_namespace_type()
{

}

std::string Lex::get_operator_name()
{

}

}   // namespace htf
