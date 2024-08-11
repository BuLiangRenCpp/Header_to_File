#pragma once

#include "ExcepPath.h"
#include "using.h"


namespace htf
{
namespace path
{

extern const std::string Htf_Temp_Dir;

// 用于保存头文件目录，只接受已经存在的目录路径
class Dir
{
    friend Dir operator/(const Dir& lhs, const std::string& rhs);

public:
    // 默认 cur
    Dir();
    // if 'dir' not exist:  _path = current dir / 'dir'
    Dir(const std::string& dir);
    ~Dir() = default;

public:
    // 此目录是否存在此文件
    bool have_file(const std::string& file) const
    {
        return FS::is_regular_file(_dir / FS::path(file));
    }

    std::string str() const { return _dir.string(); }
    // 返回 _dir / filename (不修改 _dir)
    // 不检查 path 是否存在
    std::string append_path(const std::string& path) const;
    // 生成指定后缀的 htf 临时文件路径: 并创建
    // 确保序号唯一
    std::string htf_temp_file(const std::string& extension = ".htf_temp") const
    {
        std::string filename = "header_to_file";
        FS::path    res;
        while (true) {
            res = _dir / (filename + std::to_string(_num++) + extension);
            if (!FS::exists(res)) break;
        }
        Ofstream ofs(res);
        if (!ofs.is_open())
            throw ExcepPath{"Dir::htf_temp_file", "not create temp file" + res.string()};
        return res.string();
    }

private:
    FS::path             _dir;
    mutable unsigned int _num;
};

// 操作之后还是目录
// * 如果需要文件应使用 add_file
Dir           operator/(const Dir& lhs, const std::string& rhs);
std::ostream& operator<<(std::ostream& os, const Dir& dir);

}   // namespace path
}   // namespace htf