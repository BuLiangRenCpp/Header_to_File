#pragma once

#include "ExcepBase.h"

namespace htf
{
namespace path
{

enum class PathErrors   // 错误号
{
    not_exist_path = 0,
    not_exist_dir,
    not_exist_file,
    not_open_file,
    not_remove_file,
    not_create_file,
    not_cfile   // no cpp file path
};

class ExcepPath : public ExcepBase
{
public:
    ExcepPath(const std::string& where, const what_type& what);
    ExcepPath(const std::string& where, const std::string& path, const PathErrors& error_num);
    ~ExcepPath() = default;

public:
    std::string str() const override
    {
#ifdef USER
        return "path-error: " + what();
#else
        return "path-error: " + ExcepBase::str();
#endif
    }
};

}   // namespace path
}   // namespace htf
