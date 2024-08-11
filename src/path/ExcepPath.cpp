#include "ExcepPath.h"
#include "output.h"

namespace htf
{
namespace path
{

ExcepPath::ExcepPath(const std::string& where, const what_type& what) : ExcepBase{where, what}
{}

ExcepPath::ExcepPath(const std::string& where, const std::string& path, const PathErrors& error_num)
    : ExcepPath{where, "x"}
{
    switch (error_num) {
    case PathErrors::not_exist_path: _what = "No such file or directory:" + mark(path); break;
    case PathErrors::not_exist_dir: _what = "No such directory:" + mark(path); break;
    case PathErrors::not_exist_file: _what = "No such file:" + mark(path); break;
    case PathErrors::not_open_file: _what = "Not open file:" + mark(path); break;
    case PathErrors::not_remove_file: _what = "Not remove file:" + mark(path); break;
    case PathErrors::not_create_file: _what = "Not create file:" + mark(path); break;
    case PathErrors::not_cfile: _what = "No cpp file:" + mark(path); break;
    default: break;
    }
}
}   // namespace path
}   // namespace htf
