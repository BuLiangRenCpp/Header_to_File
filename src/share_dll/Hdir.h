#pragma once
#ifdef _WIN32
#    define LIB_EXPORT
#endif

#include "dll.h"
#include "header.h"

namespace htf
{
namespace path_deal
{
// 用于保存头文件目录，只接受已经存在的目录
class DLL_API Hdir
{
public:
    Hdir() = delete;
    Hdir(const std::string&);

    std::string str() const;
    // 此目录是否存在此文件
    bool have_file(const std::string& file) const;

private:
    std::string _dir;
};

DLL_API bool operator==(const Hdir&, const Hdir&);
}   // namespace path_deal
}   // namespace htf