#pragma once

// 与 std::filesystem::path 不同，此类只处理存在的路径
// * 使用 Path 意味着路径必须存在
// 以下操作只是在语义上存在 文件 与 目录 的区别

#include "using.h"

namespace htf
{
namespace path
{

extern const std::string HTF_Temp_Directory_Name;
extern const std::string HTF_Temp_File_Extension;
// 此后缀只是默认使用的后缀，并不是文件的标识
extern const std::string HTF_Temp_PreProcess_File_Extension;       

class Path
{    
public:
    Path();     // 默认当前目录
    Path(const FS::path& path);
    ~Path() = default;

public:     /** 通用方法 **/
    FS::path path() const { return _path; }
    bool is_regular_file() const { return FS::is_regular_file(_path); }
    bool is_directory() const { return FS::is_directory(_path); }

public:
    Path parent_directory() const { return _path.parent_path(); }

public:     /** 文件操作 **/
    // 根据后缀判断
    bool is_cpp_header() const;
    bool is_htf_temp_file() const;

public:
    std::string filename(bool have_extension = true) const;
    std::string extension() const { return _path.extension().string(); }
    // 后缀应加上 '.' (但是内部不检查)
    FS::path ret_change_extension_path(const std::string& extension = ".cpp") const;

public:     /** 目录操作 **/
    bool is_htf_temp_diretory() const;
    bool have_file(const std::string& filename) const;

private:
    FS::path _path;
};

std::ostream& operator<<(std::ostream& os, const Path& path);

// ****************************************************

// 在指定目录下创建
Path create_htf_temp_directory(const Path& dir = Path{ FS::current_path() });
Path create_htf_temp_file(const Path& dir = Path{ FS::current_path() });


}   //
}//