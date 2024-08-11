#pragma once

// 模拟 std::ifstream, 但采用了 FileBuffer 作为缓冲区
// * if 文件大小小于 MAX_SIZE / 2: 采用 std::ifstream
// * else: 采用 htf::Ifstream

#include "FileBuffer.h"

namespace htf
{
namespace stream
{

class Ifstream
{
public:
    Ifstream() = default;
    Ifstream(const std::string& file);
    ~Ifstream() = default;

public:
    void open(const std::string& file);

private:
    std::string _file;
    FileBuffer  _buffer;

public:
    bool               eof() const { return _buffer.eof(); }
    int                get();
    std::string        get_str();
    int                peek();
    void               putback(char c);
    inline std::string file() const { return _file; }
};

}   // namespace stream
}   // namespace htf
