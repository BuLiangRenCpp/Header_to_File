#include "path_regex.h"

#include "ExcepPath.h"

namespace htf
{
namespace path
{

bool is_match_path(const FS::path& path, std::string p)
{
    std::string s         = path.string();
    auto        all_stars = [](const std::string& str, int left, int right) {
        for (int i = left; i < right; ++i) {
            if (str[i] != Match_Any_Char) return false;
        }
        return true;
    };
    auto char_match = [](char u, char v) {
        return u == v || (v == Match_One_Char && u != '\\' && u != '/');
    };

    while (s.size() && p.size() && p.back() != Match_Any_Char) {
        if (char_match(s.back(), p.back())) {
            s.pop_back();
            p.pop_back();
        }
        else
            return false;
    }
    if (p.empty()) return s.empty();

    int s_index = 0, p_index = 0;
    int s_record = -1, p_record = -1;
    while (s_index < s.size() && p_index < p.size()) {
        if (p[p_index] == Match_Any_Char && s[s_index] != '\\' && s[s_index] != '/') {
            ++p_index;
            s_record = s_index;
            p_record = p_index;
        }
        else if (char_match(s[s_index], p[p_index])) {
            ++s_index;
            ++p_index;
        }
        else if (s_record != -1 && s_record + 1 < s.size()) {
            ++s_record;
            s_index = s_record;
            p_index = p_record;
        }
        else
            return false;
    }
    return all_stars(p, p_index, p.size()) && s.find('\\', s_index) == std::string::npos &&
           s.find('/', s_index) == std::string::npos;
}

// 返回第一次出现通配符的路径所在的目录 (绝对路径)
// first: parent
// seconde: 通配符出现的部分
static std::pair<FS::path, std::string> pattern_split(const std::string& pattern)
{
    auto pos1 = pattern.find(Match_Any_Char);
    auto pos2 = pattern.find(Match_One_Char);
    if (pos1 == std::string::npos && pos2 == std::string::npos)
        return std::make_pair(FS::path{""}, pattern);

    auto pos = pos1;
    if (pos1 == std::string::npos)
        pos = pos2;
    else if (pos1 > pos2)
        pos = pos2;

    char        separator = FS::path::preferred_separator;
    FS::path    p{pattern};
    std::string p_str           = p.string();
    auto        separator_index = p_str.rfind(separator, pos);
    std::string parent_str =
        (separator_index == std::string::npos) ? "" : pattern.substr(0, separator_index);
    FS::path    parent{FS::current_path() / parent_str};
    std::string sub = (separator_index == std::string::npos)   // 丢弃分隔符
                          ? pattern.substr(parent_str.size())
                          : pattern.substr(parent_str.size() + 1);
    THROW_EXCEP_PATH_IF(
        !FS::is_directory(parent), parent, ExcepPath::ErrorCode::not_exist_directory);
    return std::make_pair(FS::canonical(parent), sub);
}

// 支持路径通配符
// ../*.h
std::set<std::string> path_regex(std::string pattern, bool is_dir)
{
    pattern = FS::path{pattern}.lexically_normal().string();
    std::set<std::string> res;
    auto                  tmp         = pattern_split(pattern);
    FS::path              parent      = tmp.first;
    std::string           sub_pattern = tmp.second;
    if (parent.empty()) {
        FS::path tmp{FS::current_path() / pattern};
        if (FS::exists(tmp)) res.emplace(std::move(tmp.string()));
        return res;
    }
    for (const auto& entry : FS::recursive_directory_iterator(parent)) {
        auto p        = entry.path();
        auto relative = FS::relative(p, parent);
        if (FS::is_directory(p)) relative = relative / "";
        if (is_match_path(relative, sub_pattern)) {
            if (is_dir) {
                if (!FS::is_directory(p)) {
                    cout_warn("it's not a directory" + mark_path(p));
                    continue;
                }
            }
            else {
                if (FS::is_directory(p)) {
                    cout_warn("it's a directory" + mark_path(p));
                    continue;
                }
            }
            res.emplace(std::move(p.string()));
        }
    }
    // for (auto it : res) std::cerr << "--------------------------- path_regex: " << it <<
    // std::endl;
    return res;
}

}   // namespace path
}   // namespace htf