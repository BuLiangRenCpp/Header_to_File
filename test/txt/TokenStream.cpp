#include "D:\code\git\Header_to_File\src\compiler\TokenStream.h"


namespace htf
{

TokenStream::TokenStream(const path::Cfile& tmp_file)
	: _ifs{  }, _eof{  }, _buffer{  }, _file{  }, _line{  }, _col{  }, _errors{  }
{

}

void TokenStream::open(const path::Cfile& tmp_file)
{

}

Token TokenStream::get()
{

}

Token TokenStream::peek()
{

}

void TokenStream::deal_pre()
{

}

void TokenStream::putback(const Token& token)
{

}

void TokenStream::ignore(const Token& token)
{

}

void TokenStream::ignore_until(const Token& token)
{

}

void TokenStream::ignore_between_bracket()
{

}

void TokenStream::ignore_statement()
{

}

std::string TokenStream::get_float()
{

}

std::string TokenStream::get_decimal()
{

}

std::string TokenStream::get_binary()
{

}

std::string TokenStream::get_octal()
{

}

std::string TokenStream::get_hex()
{

}

std::string TokenStream::get_interger_suffix()
{

}

Token TokenStream::get_constant()
{

}

Token TokenStream::get_string_literal()
{

}

Token TokenStream::get_identifier()
{

}

Token TokenStream::get_dchar()
{

}

}   // namespace htf
