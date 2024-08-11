#pragma once

// 文件缓冲区：采用缓冲区，提高 IO 性能
// * 待改进：是否使用双缓冲区

#include <array>
#include <fstream>

namespace htf
{
namespace stream
{

// 此类由 dev 处理
class FileBuffer
{
public:
    static const int MAX_SIZE = 1024;

public:
    using buffer       = std::array<char, MAX_SIZE>;
    using buffer_index = std::size_t;

public:
    FileBuffer() = default;
    // * dev: 使用此类前最好先检查 ifs.good == true
    FileBuffer(const std::string& file);
    ~FileBuffer() = default;

public:
    bool open(const std::string& file);

public:
    bool eof() const { return _eof; }
    int  get();
    // 以空格分隔
    std::string get_str();
    int         peek();
    // 不能连续使用两次即以上 (目前没必要)
    void putback(char c);
    // inline std::string file() const { return _file; }

private:
    // std::string _file;
    std::ifstream _ifs;

    buffer          _buffer;
    bool            _full    = false;   // _putback is full
    char            _putback = '\0';    // store putback char:
    std::streamsize _size    = 0;       // read 加载的字符数

    buffer_index _cur = 0;
    bool         _eof = true;

private:
    // 同 std::ifstream::read(...)
    // * 如果加载的字节数不满 MAX_SIZE 会返回 false
    bool _read();
};

}   // namespace stream
}   // namespace htf