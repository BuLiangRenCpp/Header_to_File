#include "Cfile.h"
#include <unordered_set>

#include "assertions.h"

const std::unordered_set<std::string> Header_Extensions{".h", ".hpp", ".h++", ".inl"};
const std::unordered_set<std::string> Source_Extensions{".cpp", ".cxx", ".c++", ".c", ".cc"};

namespace htf
{
namespace path
{

const std::string Pre_Extension = ".i_htf";

// static bool is_cextension(const std::string& extension)
// {
//     return Header_Extensions.count(extension) || Source_Extensions.count(extension);
// }

// static bool is_cfile(const FS::path& path)
// {
//     auto extension = path.extension().string();
//     return is_cextension(extension);
// }

Cfile::Cfile(const std::string& path) : _path{FS::current_path() / path}, _num{0}
{
    if (!FS::is_regular_file(_path))
        throw ExcepPath{
            "Cfile::Cfile(const std::string&)", _path.string(), PathErrors::not_exist_file};
    // if (!is_cfile(_path))
    //     throw ExcepPath{"Cfile::Cfile(const std::string&)", _path.string(),
    //     PathErrors::not_cfile};
}

bool Cfile::is_header() const
{
    return Header_Extensions.count(_path.extension().string());
}

bool Cfile::is_source() const
{
    return Source_Extensions.count(_path.extension().string());
}

std::string Cfile::change_extension(const std::string& extension) const
{
    HTF_DEV_ASSERT_MESSAGE(extension.length() > 1,
                           "Cfile::change_extension(const std::string&): "
                           "extension.length must be more than 1");
    HTF_DEV_ASSERT_MESSAGE(extension[0] == '.',
                           "Cfile::change_extension(const std::string&): extension[0] must be '.'");
    // HTF_DEV_ASSERT_MESSAGE(is_cextension(extension), HTF_DEV_MARK(extension) << "isn't
    // a valid cpp extension"); if (!is_cextension(extension))
    //     throw ExcepPath{"Cfile::change_extension", mark(extension) + "isn't a valid cpp
    //     extension"};
    auto name = filename(false);
    return (_path.parent_path() / (name + extension)).string();
}

std::string Cfile::change_extension_filename(const std::string& extension) const
{
    return FS::path{change_extension(extension)}.filename().string();
}

std::ostream& operator<<(std::ostream& os, const Cfile& cfile)
{
    os << cfile.str();
    return os;
}

}   // namespace path
}   // namespace htf
