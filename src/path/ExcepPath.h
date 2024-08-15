#pragma once

#include "ExcepBase.h"
#include "using.h"

namespace htf
{
namespace path
{

#define THROW_EXCEP_PATH1_IF(expr, msg)            \
    do {                                           \
        if ((expr)) {                              \
            std::ostringstream oss;                \
            oss << HTF_ERROR_WHERE << msg;         \
            throw htf::path::ExcepPath{oss.str()}; \
        }                                          \
    } while (0)

#ifdef DEBUG
#define THROW_EXCEP_PATH_IF(expr, _path, _error_code)                                    \
    do {                                                                                 \
        if ((expr)) {                                                                    \
            throw htf::path::ExcepPath{_error_code, _path / mark_path(HTF_ERROR_WHERE)}; \
        }                                                                                \
    } while (0)
#else
#define THROW_EXCEP_PATH_IF(expr, _path, _error_code)       \
    do {                                                    \
        if ((expr)) {                                       \
            throw htf::path::ExcepPath{_error_code, _path}; \
        }                                                   \
    } while (0)
#endif

class ExcepPath : public ExcepBase
{
public:
    enum class ErrorCode   // 错误号
    {
        not_exist_file,
        not_exist_directory,
        not_exist,   // file or dir
        not_open_file,
        not_create_file,
        not_create_directory,
        not_create,
        not_remove_file,
        not_remove_directory,
        not_remove
    };

public:
    ExcepPath(const std::string& what);
    ExcepPath(const ErrorCode& error_num, const FS::path& path);
    ~ExcepPath() = default;
};

}   // namespace path
}   // namespace htf
