#include "FileBuffer.h"

#include "assertions.h"

namespace htf
{
namespace stream
{
FileBuffer::FileBuffer(const std::string& file)
    : _ifs{file}, _buffer{buffer()}, _full{false}, _putback{'\0'}, _cur{0}, _eof{false}
{
    HTF_DEV_ASSERT_MESSAGE(_ifs.good(), "FileBuffer::FileBuffer: ifs.good == false");
    _read();   ////  初始化 _cur_use = two，使得 _read() 先使用 _buffer
}

bool FileBuffer::open(const std::string& file)
{
    _ifs.open(file);
    _ifs.clear();
    _full    = false;
    _putback = '\0';
    _size    = 0;
    _cur     = 0;
    _eof     = _ifs.eof();
    _read();
    return _ifs.good();
}

int FileBuffer::get()
{
    if (_full) {   // 先查看 _putback 是否有数据
        _full = false;
        return _putback;
    }
    if (_cur == _size) {
        if (_size < MAX_SIZE) {
            _eof = true;
            return EOF;
        }
        _cur = 0;
        _read();
    }
    return _buffer[_cur++];
}

std::string FileBuffer::get_str()
{
    std::string res;
    while (true) {
        if (isspace(peek())) {
            if (res.empty()) {
                get();
                continue;
            }
            break;
        }
        char c = get();
        if (eof()) break;
        res.push_back(c);
    }
    return res;
}

int FileBuffer::peek()
{
    if (_cur == _size) {
        if (_size < MAX_SIZE) {
            _eof = true;
            return EOF;
        }
        _read();
        _cur = 0;
    }
    return _buffer[_cur];
}

void FileBuffer::putback(char c)
{
    HTF_DEV_ASSERT_MESSAGE(!_full, "FileBuffer::putback: _putback is full");
    _full    = true;
    _putback = c;
}

bool FileBuffer::_read()
{
    auto& tmp = _ifs.read(_buffer.data(), MAX_SIZE);
    _size     = tmp.gcount();
    return tmp.good();
}

}   // namespace stream
}   // namespace htf
