#include "Dir.h"

namespace htf
{
namespace path
{

const std::string Htf_Temp_Dir = "htf_temp_dir-BLR-header_to_file-htf-jiangmeng-0123";

Dir::Dir() : _dir{FS::current_path()}, _num{0}
{}


Dir::Dir(const std::string& dir) : _dir{FS::current_path() / dir}, _num{0}
{
    if (!FS::is_directory(_dir)) throw ExcepPath{"Dir::Dir", dir, PathErrors::not_exist_dir};
}

std::string Dir::append_path(const std::string& path) const
{
    return (_dir / path).string();
}

Dir operator/(const Dir& lhs, const std::string& rhs)
{
    auto tmp = lhs._dir / FS::path(rhs);
    if (!FS::exists(tmp))
        throw ExcepPath{
            "Dir::operator/(const std::string&)", tmp.string(), PathErrors::not_exist_path};
    return Dir{tmp.string()};
}

std::ostream& operator<<(std::ostream& os, const Dir& dir)
{
    os << dir.str();
    return os;
}

}   // namespace path
}   // namespace htf
