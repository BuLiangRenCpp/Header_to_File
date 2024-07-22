#ifdef _WIN32
#    define LIB_EXPORT
#endif

#include <filesystem>

#include "Excep_path.h"
#include "output.h"
#include "path_deal.h"

using namespace std;
using namespace output;

namespace FS = std::filesystem;

// *************************************************************************************
const unordered_set<string> Extensions{
    ".txt", ".h", ".hpp", ".h++", ".inl", ".cpp", ".cxx", ".c++", ".c", ".cc"};   // 扩展名集合
const unordered_set<string> Header_Extensions{".h", ".hpp", ".h++", ".inl"};      // 头文件扩展名集合
const unordered_set<string> Source_Extensions{".cpp", ".cxx", ".c++", ".c", ".cc"};

// 在特殊字符前加上的字符
const unordered_map<char, char> Reg_Insert_Str{{'*', '.'}, {'.', '\\'}};
// *************************************************************************************

static vector<int> _index(char c, const string& s)
{
    vector<int> res;
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == c) res.emplace_back(i);
    }
    return res;
}

static void _insert(string& s, char c, const vector<int>& index)
{
    if (Reg_Insert_Str.count(c)) {
        for (int i = index.size() - 1; i >= 0; i--) {
            s.insert(s.begin() + index[i], Reg_Insert_Str.at(c));
        }
    }
}

static void legal_regex(string& s)
{
    auto index = _index('.', s);
    _insert(s, '.', index);
    index = _index('*', s);
    _insert(s, '*', index);
}

static bool is_extension(const string& s)
{
    return Extensions.count(s);
}

// 是否从开头开始有连续 . 的个数超过 3 个
// ** 在 win 平台 FS::exists("...") 返回 true
static bool is_more_point(const string& s)
{
    if (s.length() < 3) return false;
    return (s[0] == '.' && s[1] == '.' && s[2] == '.');
}


namespace htf
{
namespace path_deal
{

const string IDEFAULT_EXTENSION = ".h";
const string ODEFAULT_EXTENSION = ".cpp";

bool is_header_extension(const string& s)
{
    return (s == "") ? true : Header_Extensions.count(s);
}

bool is_source_extension(const string& s)
{
    return (s == "") ? true : Source_Extensions.count(s);
}

bool is_exist(const string& path)
{
    if (is_more_point(path)) return false;   // 不处理 ... 连续三个以上的情形
    FS::path f{path};
    return FS::exists(f);
}

bool is_exist_file(const string& path)
{
    if (is_exist_dir(path)) return false;
    return is_exist(path);
}

bool is_exist_dir(const string& path)
{
    FS::path f{path};
    return FS::is_directory(f);
}

bool is_same_parent_dir(const string& file1, const string& file2)
{
    FS::path dir1{file1}, dir2{file2};
    return (dir1.parent_path() == dir2.parent_path());
}

string file_name(const string& s, bool have_extension)
{
    FS::path f(s);
    string   name      = f.filename().string();
    string   extension = f.extension().string();
    return (have_extension) ? name : name.substr(0, name.rfind(extension));
}

string extension(const string& s)
{
    FS::path f(s);
    return f.extension().string();
}

string parent_dir(const string& s)
{
    FS::path f(s);
    return f.parent_path().string();
}

string change_extension(const string& path, const string& extension)
{
    FS::path dir{path};
    string   parent_dir = dir.parent_path().string();
    if (parent_dir != "") parent_dir += "\\";
    string suffix = dir.extension().string();
    if (!is_extension(suffix)) return path + extension;

    string file_name = dir.filename().string();
    file_name        = file_name.substr(0, file_name.rfind(suffix));
    return parent_dir + file_name + extension;
}

vector<string> regex_find_files(const string& dir_path_str, string pattern)
{
    if (!is_exist_dir(dir_path_str))
        throw excep::Excep_path{"path_deal::regex_find_files",
                                "directory" + mark(dir_path_str) + "not exist"};
    legal_regex(pattern);
    FS::path dir_path{dir_path_str};
    if (!FS::is_directory(dir_path)) return {};
    regex          r{pattern};
    vector<string> res;
    for (const auto& entry : FS::directory_iterator(dir_path)) {
        if (regex_match(entry.path().filename().string(), r) && entry.is_regular_file())
            res.emplace_back(entry.path().filename().string());
    }
    return res;
}

vector<string> find_same_extension_files(const string& dir_path_str, const string& extension)
{
    FS::path dir_path{dir_path_str};
    if (!FS::is_directory(dir_path))
        throw excep::Excep_path("path_deal::find_same_extension_files",
                                mark(dir_path.string()) + "不是目录或者不存在");
    vector<string> res;
    for (const auto& entry : FS::directory_iterator(dir_path)) {
        if (entry.path().extension() == extension && entry.is_regular_file())
            res.emplace_back(entry.path().filename().string());
    }
    return res;
}

vector<string> find_cpp_header_files(const string& dir)
{
    FS::path dir_path{dir};
    if (!FS::is_directory(dir_path))
        throw excep::Excep_path("path_deal::find_same_extension_files",
                                mark(dir_path.string()) + "不是目录或者不存在");
    vector<string> res;
    for (const auto& entry : FS::directory_iterator(dir_path)) {
        if (is_header_extension(entry.path().extension().string()) && !entry.is_directory())
            res.emplace_back(entry.path().filename().string());
    }
    return res;
}

string current_dir()
{
    return FS::current_path().string();
}

string normalize(const string& sourse, const string& target)
{
    FS::path p(sourse);
    p.append(target);
    string res = p.lexically_normal().string();
    if (!is_exist_dir(res))
        if (is_exist(res + IDEFAULT_EXTENSION)) return res + IDEFAULT_EXTENSION;
    return res;
}

bool remove_file(const string& s)
{
    FS::path f{s};
    if (!FS::exists(s)) throw excep::Excep_path("path_deal::remove_file", mark(s) + "not exist");
    return FS::remove(f);
}

bool create_dirs(const string& s)
{
    FS::path f{s};
    if (FS::exists(s)) throw excep::Excep_path("path_deal::remove_file", mark(s) + "already exist");
    return FS::create_directories(s);
}

}   // namespace path_deal
}   // namespace htf