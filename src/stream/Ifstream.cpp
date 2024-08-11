#include "Ifstream.h"
#include "ExcepPath.h"

namespace htf
{
namespace stream
{

Ifstream::Ifstream(const std::string& file) : _file{file}, _buffer{}
{
    if (!_buffer.open(file))
        throw path::ExcepPath{"Ifsream::Ifstream(.)", file, path::PathErrors::not_open_file};
}

void Ifstream::open(const std::string& file)
{
    if (!_buffer.open(file))
        throw path::ExcepPath{"Ifsream::Ifstream(.)", file, path::PathErrors::not_open_file};
}

int Ifstream::get()
{
    return _buffer.get();
}

std::string Ifstream::get_str()
{
    return _buffer.get_str();
}

int Ifstream::peek()
{
    return _buffer.peek();
}

void Ifstream::putback(char c)
{
    _buffer.putback(c);
}

}   // namespace stream
}   // namespace htf
