#pragma once

// 路径通配符支持

#include <set>
#include <string>

#include "using.h"

namespace htf
{
namespace path
{

// 都不能匹配 路径分隔符: / 与 \ 都不能匹配，不区分平台 (因为路径中不存在 / or \)
constexpr char Match_Any_Char = '*';
// 都不能匹配 路径分隔符: / 与 \ 都不能匹配，不区分平台 (因为路径中不存在 / or \)
constexpr char Match_One_Char = '?';

inline bool is_regex_path_str(const std::string& s)
{
    return s.find(Match_Any_Char) != std::string::npos ||
           s.find(Match_One_Char) != std::string::npos;
}

bool is_match_path(const FS::path& path, std::string pattern);
// 返回通配符匹配的所有路径  (绝对路径)
// 路径必须存在
// * 默认不匹配目录
// 只匹配 目录 或者 文件路径
std::set<std::string> path_regex(std::string pattern, bool is_dir = false);

}   // namespace path
}   // namespace htf
