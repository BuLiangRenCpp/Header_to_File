#include "Path.h"
#include "ExcepPath.h"

#include <unordered_set>

const std::unordered_set<std::string> Header_Extensions{ ".h", ".hpp", ".h++", ".h++" };

namespace htf
{
namespace path
{

const std::string HTF_Temp_Directory_Name = "header_to_file-temp-BLR";;
const std::string HTF_Temp_File_Extension = ".htf_temp";
const std::string HTF_Temp_PreProcess_File_Extension = ".i_htf";

Path::Path()
	: _path{ FS::current_path() }
{

}

Path::Path(const FS::path& path)
	: _path{ FS::current_path() / path  }
{
    THROW_EXCEP_PATH_IF(!FS::exists(_path), _path, ExcepPath::ErrorCode::not_exist);
    _path = _path.lexically_normal();
}

bool Path::is_cpp_header() const
{
    return Header_Extensions.count(extension());
}

bool Path::is_htf_temp_file() const
{
    return extension() == HTF_Temp_File_Extension; 
}

std::string Path::filename(bool have_extension) const
{
    std::string name      = _path.filename().string();
    std::string extension = _path.extension().string();
    return (have_extension) ? name : name.substr(0, name.rfind(extension));
}

FS::path Path::ret_change_extension_path(const std::string& extension) const
{
    auto name = filename(false);
    return _path.parent_path() / (name + extension);
}

bool Path::is_htf_temp_diretory() const
{
    return filename() == HTF_Temp_Directory_Name;
}

bool Path::have_file(const std::string& filename) const
{
    return FS::is_regular_file(_path / FS::path(filename));
}

std::ostream& operator<<(std::ostream& os, const Path& path)
{
    os << path.path();
    return os;
}

Path create_htf_temp_directory(const Path& dir)
{
    auto tmp = dir.path() / HTF_Temp_Directory_Name;
    if (!FS::exists(tmp)) 
        THROW_EXCEP_PATH_IF(!FS::create_directory(tmp), tmp, ExcepPath::ErrorCode::not_create_directory);
    return tmp;
}

Path create_htf_temp_file(const Path& dir)
{
    static size_t num = 0;
    std::string filename = "header_to_file";
    auto res = dir.path() / (filename + std::to_string(num) + HTF_Temp_File_Extension);
    while (FS::exists(res)) {
        ++num;
        res = dir.path() / (filename + std::to_string(num) + HTF_Temp_File_Extension);
    }
    Ofstream ofs(res);
    THROW_EXCEP_PATH_IF(!ofs, res, ExcepPath::ErrorCode::not_create_file);
    return res;
}

}   // namespace path
}   // namespace htf
