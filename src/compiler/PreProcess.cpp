#include "PreProcess.h"
#include "base_info.h"

using namespace std;

namespace htf
{

const std::string HTF_PRE_ID = "BLR::Header_to_File:htf:" + std::string{HTF_VERSION};

PreProcess::PreProcess(const path::Cfile& cfile, const std::vector<path::Dir>& include_dirs,
                       const std::set<std::string>& not_include)
    : _source{cfile}
    , _cur{cfile}
    , _include_dirs{include_dirs}
    , _not_include{not_include}
    , _line{0}
    , _errors{}
    , _tmp_files{}
    , _tmp_tasks{}
    , _tmp_visited{}
{}

PreProcess::~PreProcess()
{
    clear();
}

// 检查 str 的第一个 非空字符 是否是 '#'
// true: return pos
// false: return -1
static int is_pre_ins(const string& str)
{
    auto pos = str.find('#');
    if (pos == string::npos) return -1;
    for (int i = pos - 1; i >= 0; --i)
        if (str[i] != ' ') return -1;
    return pos;
}

bool PreProcess::run(string output_path, const std::string& temp_dir)
{
    if (_not_include.count(_source.str())) return true;
    if (!FS::is_directory(temp_dir)) FS::create_directory(temp_dir);
    if (output_path.empty())
        output_path = path::Dir{temp_dir}.append_path(_source.change_extension_filename());
    // --------- 覆盖源文件 -------------
    Ofstream o(output_path);
    if (!o)
        throw path::ExcepPath{
            "PreProcess::run(..out)", output_path, path::PathErrors::not_create_file};
    o.close();
    // ------------ app -----------------
    Ofstream ofs(output_path, Ofstream::app);
    if (!ofs)
        throw path::ExcepPath{
            "PreProcess::run(..add)", output_path, path::PathErrors::not_open_file};
    ofs << HTF_PRE_ID << endl;
    ofs << "##" << _source << endl;
    // -------------------------------------
    _tmp_files[_cur.str()] = path::Dir{temp_dir}.htf_temp_file();
    _tmp_tasks.push(_cur.str());
    // ------------ 处理单文件 -------------
    while (!_tmp_tasks.empty()) {
        auto path = _tmp_tasks.front();
        _tmp_tasks.pop();
        if (_tmp_visited.count(path)) continue;   // 跳过已处理
        _cur = path;
        deal_single_file(path);
        _tmp_visited[path] = _tmp_files[path];
        _line              = 0;
    }
    if (!_errors.empty()) {
        ofs.close();
        FS::remove(output_path);
        return false;
    }
    // ---------- 将临时文件合并 ------------
    _tmp_visited.clear();
    merge_temp_file(_source.str(), ofs);
    return _errors.empty();
}

std::string PreProcess::errors() const
{
    if (_errors.empty()) return "";
    ostringstream oss;
    for (int i = 0; i < _errors.size(); ++i) {
        oss << "[" << i << "] " << _errors[i].str() << std::endl;
    }
    return oss.str();
}

void PreProcess::clear()
{
    auto tmp = _tmp_files.begin();
    if (tmp != _tmp_files.end()) {
        auto dir = FS::path(tmp->second).parent_path();
        if (dir.filename() == path::Htf_Temp_Dir) {
            FS::remove_all(dir);
            try {
                FS::remove_all(dir);
            }
            catch (const std::exception& e) {
                cout_error_dev("PreProcess::clear(): cannot remove dir" + mark(dir.string()) +
                               ": " + e.what());
            }
        }
    }
    for (auto it : _tmp_files) {
        auto path = it.second;
        if (FS::path(path).extension().string() != ".htf_temp") {
            cout_error_dev("PreProcess::clear(): not htf temp file" + mark(path));
            return;
        }
        try {
            FS::remove(path);
        }
        catch (const std::exception& e) {
            cout_error_dev("PreProcess::clear(): cannot remove file" + mark(path) + ": " +
                           e.what());
        }
    }
}

void PreProcess::deal_single_file(const std::string& path)
{
    Ifstream    ifs(path);
    std::string ofile = _tmp_files[path];
    Ofstream    ofs(ofile);
    if (!ifs)
        throw path::ExcepPath{"PreProcess::run(....)", _cur.str(), path::PathErrors::not_open_file};
    if (!ofs) throw path::ExcepPath{"PreProcess::run(....)", "cannot create temp file" + ofile};
    ofs << HTF_PRE_ID << endl;
    while (true) {
        auto   old_line = _line;
        string str      = deal_line(ifs);
        if (str.empty()) break;
        // deal comment
        auto pos = str.find('/');
        // ------------------ 注释 -------------------------------
        if ((pos != string::npos) && (pos < str.length())) {
            if (str[pos + 1] == '/') {   // 行注释
                deal_line_comment(str, pos + 1);
            }
            else if (str[pos + 1] == '*') {   // 块注释
                ++pos;
                line_t old_line = _line;   // 用于计算注释中的 \n 个数
                int    index    = deal_block_comment(str, pos);
                while (-1 != index) {
                    string tmp = deal_line(ifs);
                    if (tmp.empty()) {
                        _errors.emplace_back(ExcepSyntax{_cur,
                                                         _line,
                                                         0,
                                                         "block comment: lack of" + mark("*/"),
                                                         SyntaxError::bad_preprocess});
                        return;
                    }
                    str += std::move(tmp);
                    index = deal_block_comment(str, index);
                }
                str += string(_line - old_line, '\n');
            }
        }
        // --------------------- 预处理指令 -----------------------
        int index = is_pre_ins(str);
        if (index != -1) {
            // ------------ include ------------
            string new_path = is_include(str, index + 1);
            if (new_path != "") {
                deal_include(new_path, path::Cfile(ofile).parent_dir());
                str = new_path;
            }
            // ------ other only ignore -------
            else {
                str = string(_line - old_line, '\n');
            }
        }
        ofs << str;
    }
}

void PreProcess::merge_temp_file(const std::string& ifile, Ofstream& ofs)
{
    if (_tmp_visited.count(ifile)) return;
    auto path           = _tmp_files[ifile];
    _tmp_visited[ifile] = path;
    Ifstream ifs(path);
    if (!ifs)
        throw path::ExcepPath{"PreProcess::merge_temp_file", path, path::PathErrors::not_open_file};
    string line;
    getline(ifs, line);
    if (line != HTF_PRE_ID)
        throw path::ExcepPath{"PreProcess::merge_temp_file", mark(path) + "isn't htf temp file"};
    bool is_eof = false;
    while (!is_eof) {
        getline(ifs, line);
        // ! 避免 getline 后 line 有读取的字符，但是 eof = true，下一次在读取 state = fail
        // ! 但是 line == 最后一次读取的字符，而不是空字符
        if (ifs.eof()) {
            is_eof = true;
            if (line.empty()) continue;   // == break
        }
        if (!line.empty() && line[0] == '#') {
            ofs << line << "\n";
            merge_temp_file(line.substr(1), ofs);
            ofs << "#end\n";
            continue;
        }
        ofs << line << "\n";
    }
}



// 删除前导空格 (保留一个)
static void delete_space_pre(string& str)
{
    int  i   = 0;
    auto len = str.length();
    for (; i < len; ++i) {
        if (str[i] != ' ') break;
    }
    if (i > 0) str = str.substr(i - 1);
}

// 删除后导空格 (保留一个)
static void delete_space_suf(string& str)
{
    auto len = str.length();
    int  i   = len - 1;
    for (; i >= 0; --i) {
        if (str[i] != ' ') break;
    }
    if (i == -1)
        str = " ";
    else
        str = str.substr(0, i + 1);
}

std::string PreProcess::deal_line(Ifstream& ifs)
{
    string str;
    // ! 与 138 行不同，由于初始化为空字符，所以 'ifs.eof() && str.empty()' 是有效的
    getline(ifs, str);
    if (ifs.eof() && str.empty()) {
        // _col = 0;
        return "";
    }
    ++_line;
    if (!str.empty() && str.back() == '\\') {
        str.pop_back();
        delete_space_suf(str);
        std::string new_line = str;
        getline(ifs, new_line);
        int line_num = 1;   // 记录 '\' 的个数
        while (!new_line.empty() && new_line.back() == '\\') {
            ++line_num;
            delete_space_pre(new_line);
            new_line.pop_back();
            str += new_line;
            delete_space_suf(str);
            getline(ifs, new_line);
        }
        str += new_line + string(line_num, '\n');
        _line += line_num;
    }
    str += "\n";
    return str;
}

void PreProcess::deal_line_comment(std::string& str, int pos)
{
    str = str.substr(0, pos - 1);
    str += "\n";
}

// pos: the '*' pos
// return: -1 正确
// return: /* 中的 '*' 的下标
int PreProcess::deal_block_comment(string& str, int pos)
{
    auto tar_pos = str.find("*/", pos + 1);
    if (tar_pos == string::npos) return pos;
    string suf = (tar_pos + 2 < str.length()) ? str.substr(tar_pos + 2) : "";
    str        = str.substr(0, pos - 1) + suf;
    pos        = str.find("/*");
    if (pos == string::npos) {
        pos = str.find("//");
        if (pos != string::npos) deal_line_comment(str, pos + 1);
        return -1;
    }
    return deal_block_comment(str, pos + 1);
}

string PreProcess::is_include(string str, int pos)
{
    if (str.back() == '\n') str.pop_back();
    auto index = str.find("include", pos);
    if (index == string::npos) return "";
    index    = index + 7;   // include|
    auto len = str.length();
    if (index >= len) {
        _errors.emplace_back(ExcepSyntax{_cur,
                                         _line,
                                         static_cast<col_t>(index),
                                         "after" + mark("#include") + "lack of path",
                                         SyntaxError::bad_preprocess});
        return "";
    }
    else if (index < len && str[index] != ' ') {
        _errors.emplace_back(ExcepSyntax(_cur,
                                         _line,
                                         pos,
                                         "after" + mark("#include") + "lack of space",
                                         SyntaxError::bad_preprocess));
        return "";
    }
    for (int i = index + 1; i < len; i++) {
        if (str[i] == ' ') continue;
        if (str[i] == '<') return "";
        if (str[i] == '\"') {
            auto right = str.rfind('\"');
            if (right <= i) {
                _errors.emplace_back(ExcepSyntax(_cur,
                                                 _line,
                                                 len - 1,
                                                 "after" + mark(str) + "lack of" + mark('"'),
                                                 SyntaxError::bad_preprocess));
                return "";
            }
            return str.substr(i + 1, right - i - 1);
        }
        _errors.emplace_back(ExcepSyntax{
            _cur,
            _line,
            static_cast<col_t>(i),
            "after" + mark(str.substr(0, i)) + "lack of" + mark('\"') + "or" + mark('<'),
            SyntaxError::bad_preprocess});
        break;
    }
    return "";
}


// 如果为找到: return ""
// 否则: return path
// - first: find  _cur / path
// - second: find include_dir
string PreProcess::find_path(const string& path)
{
    auto tmp = _cur.parent_dir().append_path(path);
    if (FS::is_regular_file(tmp)) return FS::canonical(tmp).string();
    for (auto dir : _include_dirs) {
        if (dir.have_file(path)) return FS::canonical(dir.append_path(path)).string();
    }
    return "";
}

void PreProcess::deal_include(string& path, const path::Dir& temp_dir)
{
    auto tmp = find_path(path);
    if (tmp == "") {
        _errors.emplace_back(ExcepSyntax{_cur,
                                         _line,
                                         0,
                                         mark("#include") + ": not find" + mark(path),
                                         SyntaxError::bad_preprocess});
        path = "\n";
        return;
    }
    path = "\n";
    if (_not_include.count(tmp)) return;
    if (!_tmp_files.count(tmp)) {
        _tmp_files[tmp] = temp_dir.htf_temp_file();
        _tmp_tasks.push(tmp);
        path = "#" + std::move(tmp) + path;
    }
}

}   // namespace htf
