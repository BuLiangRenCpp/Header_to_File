#pragma once
#ifdef _WIN32
#    define LIB_EXPORT
#endif

#include "Hdir.h"
#include "dll.h"
#include "path_deal.h"

namespace htf
{
namespace path_deal
{

// * C++头文件路径(针对单文件)，只支持已经存在的C++头文件
class DLL_API Hpath
{
public:
    Hpath() = delete;
    // 只能是 file-path
    Hpath(const std::string&);
    std::string change_extension(const std::string& extension = ODEFAULT_EXTENSION);
    std::string str() const;
    Hdir        parent() const;
    std::string filename(bool have_extension = true) const;

private:
    std::string _hpath;
};

DLL_API std::ostream& operator<<(std::ostream&, const Hpath&);
DLL_API bool          operator==(const Hpath&, const Hpath&);
DLL_API bool          operator!=(const Hpath&, const Hpath&);

}   // namespace path_deal
}   // namespace htf