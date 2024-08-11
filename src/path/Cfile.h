#pragma once

#include "Dir.h"

namespace htf
{
namespace path
{

// !可能扩展：
// ! 支持所有后缀 (目前不支持的原因是无法通过其他手段实现判断文件是否是 cpp
// 文件，因此将判断方法交给用户)
// * C++ 文件路径 (通过后缀判断)
// - header extensions: ".h", ".hpp", ".h++", ".inl"
// - source extensions: ".cpp", ".cxx", ".c++", ".c", ".cc"
extern const std::string Pre_Extension;

class Cfile
{
public:
    // if 'path' not exist:  _path = current dir / 'path'
    Cfile(const std::string& path);
    ~Cfile() = default;

public:
    bool   is_header() const;
    bool   is_source() const;
    bool   empty() const { return FS::file_size(_path) == 0; }
    size_t size() const { return FS::file_size(_path); }
    // 不修改 _path，只返回结果
    std::string change_extension(const std::string& extension = ".cpp") const;
    std::string change_extension_filename(const std::string& extension = Pre_Extension) const;
    std::string filename(bool have_extension = true) const
    {
        std::string name      = _path.filename().string();
        std::string extension = _path.extension().string();
        return (have_extension) ? name : name.substr(0, name.rfind(extension));
    }
    Dir         parent_dir() const { return Dir{_path.parent_path().string()}; }
    std::string str() const { return _path.string(); }

    // 生成的 htf 临时文件路径
    // demanded:
    // - false: 仅生成后缀名改变的文件，不管是否已经存在，都将文件清空
    // - true: 增加序号，确保文件不存在
    std::string htf_temp_file(const std::string& extension = ".htf_temp",
                              bool               demanded  = false) const
    {
        std::string name = filename(false);
        FS::path    res;
        while (true) {
            res = parent_dir().append_path(name + std::to_string(_num++) + extension);
            if (!FS::exists(res)) break;
        }
        Ofstream ofs(res);
        if (!ofs.is_open())
            throw ExcepPath{"Dir::htf_temp_file", "not create temp file" + res.string()};
        return res.string();
    }

private:
    FS::path             _path;
    mutable unsigned int _num;
};

std::ostream& operator<<(std::ostream& os, const Cfile& hfile);

}   // namespace path
}   // namespace htf