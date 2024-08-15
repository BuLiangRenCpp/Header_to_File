#include "PreProcess.h"
#include "ExcepPath.h"
#include "assertions.h"
#include "base_info.h"

using namespace std;

namespace htf
{

const std::string HTF_PreProcess_File_Id = "BLR::Header_to_File:htf:" + std::string{HTF_VERSION};

PreProcess::PreProcess(const path::Path& cfile, const std::vector<path::Path>& include_dirs,
                       const std::set<FS::path>& not_include)
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

bool PreProcess::run(FS::path output_path)
{
    path::Path temp_dir = path::create_htf_temp_directory();
    if (_not_include.count(_source.path())) return true;
    if (output_path.empty())
        output_path =
            temp_dir.path() / (_source.filename(false) + path::HTF_Temp_PreProcess_File_Extension);
    // --------- 覆盖源文件 -------------
    Ofstream o(output_path);
    THROW_EXCEP_PATH_IF(!o, output_path, path::ExcepPath::ErrorCode::not_create_file);
    o.close();
    // ------------ app -----------------
    Ofstream ofs(output_path, Ofstream::app);
    THROW_EXCEP_PATH_IF(!ofs, output_path, path::ExcepPath::ErrorCode::not_open_file);
    ofs << HTF_PreProcess_File_Id << endl;
    ofs << "##" << _source.path().string() << endl;
    // -------------------------------------
    _tmp_files[_cur.path()] = path::create_htf_temp_file(temp_dir).path();
    _tmp_tasks.push(_cur.path());
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
    merge_temp_file(_source.path(), ofs);
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

void PreProcess::clear(bool clear_dir)
{
    if (clear_dir) {
        for (auto it : _tmp_files) {
            auto parent = it.second.parent_path();
            HTF_DEV_ASSERT_MESSAGE(
                parent.filename() == path::HTF_Temp_Directory_Name,
                "htf temp directory filename !=" + mark(path::HTF_Temp_Directory_Name));
            FS::remove_all(it.second.parent_path());
            return;
        }
    }
    else {
        for (auto it : _tmp_files) {
            auto path = it.second;
            HTF_DEV_ASSERT_MESSAGE(
                FS::path(path).extension().string() == path::HTF_Temp_File_Extension,
                "not htf temp file" << mark_path(path));
            try {
                FS::remove(path);
            }
            catch (const std::exception& e) {
                HTF_DEV_ASSERT_MESSAGE(
                    false, "not remove htf temp file" << mark_path(path) << ": " << e.what());
            }
        }
    }
}

void PreProcess::deal_single_file(const FS::path& path)
{
    Ifstream ifs(path);
    FS::path ofile = _tmp_files[path];
    Ofstream ofs(ofile);
    THROW_EXCEP_PATH_IF(!ifs, path, path::ExcepPath::ErrorCode::not_open_file);
    THROW_EXCEP_PATH1_IF(!ofs, "cannot create temp file" << ofile);
    ofs << HTF_PreProcess_File_Id << endl;
    while (true) {
        auto   old_line = _line;   // 用于计算处理过 续行符 之后的换行符的个数
        string str      = deal_continuation_char(ifs);
        if (str.empty()) break;
        // -------------- comment and raw string ---------------
        string old_comment_str  = str;   // 用于报错
        auto   old_comment_line = _line;
        auto   flag             = deal_new_line(str);
        while (flag != DealLineFuncRetType::is_success) {
            ofs << str;
            str = deal_continuation_char(ifs);
            // -------------- error ----------------
            if (str.empty()) {
                switch (flag) {
                case DealLineFuncRetType::unclose_block_comment:
                {
                    col_t i = static_cast<col_t>(old_comment_str.find("/*"));
                    _errors.emplace_back(
                        CompilerError{_cur, old_comment_line, i, "unclosed black comment"});
                    break;
                }
                case DealLineFuncRetType::unclose_raw_string:
                {
                    col_t i = static_cast<col_t>(old_comment_str.find("R\""));
                    _errors.emplace_back(
                        CompilerError{_cur,
                                      old_comment_line,
                                      i,
                                      "lack of" + mark('"', '[') + "after raw string delimiter"});
                    break;
                }
                default: HTF_SWITCH_DEFAULT_THROW;
                }
                break;
            }
            // ------------- deal old line -------------
            switch (flag) {
            case DealLineFuncRetType::unclose_block_comment:
                flag = deal_old_line_block_comment(str);
                break;
            case DealLineFuncRetType::unclose_raw_string:
                flag = deal_old_line_block_comment(str);
                break;
            default: HTF_SWITCH_DEFAULT_THROW;
            }
        }
        // --------------------- 预处理指令 -----------------------
        int index = is_pre_ins(str);
        if (index != -1) {
            // ------------ include ------------
            std::string new_path = is_include(str, index + 1);
            if (new_path != "") {
                deal_include(new_path, ofile.parent_path());
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

void PreProcess::merge_temp_file(const FS::path& ifile, Ofstream& ofs)
{
    if (_tmp_visited.count(ifile)) return;
    auto path           = _tmp_files[ifile];
    _tmp_visited[ifile] = path;
    Ifstream ifs(path);
    THROW_EXCEP_PATH_IF(!ifs, path, path::ExcepPath::ErrorCode::not_open_file);
    string line;
    getline(ifs, line);
    HTF_DEV_ASSERT_MESSAGE(line == HTF_PreProcess_File_Id,
                           mark_path(path) << "lack of preprocess-file-id");
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

std::string PreProcess::deal_continuation_char(Ifstream& ifs)
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

PreProcess::DealLineFuncRetType PreProcess::deal_new_line(std::string& str)
{
    auto len                 = str.size();
    bool block_comment_close = true;
    bool raw_string_close    = true;   // raw string 的 "" 是否闭合
    for (int i = 0; i < len; ++i) {
        switch (str[i]) {
        case '"':
        case '\'':   // "string literal" ''
        {
            char tar      = str[i];
            bool is_close = false;
            for (int j = i + 1; j < len; ++j) {
                if (j == '\\' && str[j + 1] == tar) {   // deal_line 处理后 str.back() != '\'
                    ++j;
                    continue;
                }
                if (str[j] == tar) {
                    i        = j + 1;
                    is_close = true;
                    break;
                }
            }
            if (is_close == false) {
                _errors.emplace_back(
                    CompilerError{_cur,
                                  _line,
                                  static_cast<col_t>(i),
                                  "missing terminating character" + mark(str[i], '[')});
            }
            break;
        }
        case 'R':   // R"()"
        {
            if (i + 1 == len) continue;
            if (str[i + 1] == '"') {
                bool have_lbra   = false;
                raw_string_close = false;
                for (int j = i + 2; j < len; ++j) {
                    if (str[j] == '"') {
                        raw_string_close = true;
                        i                = j + 1;
                        break;
                    }
                    if (str[j] == '(') {
                        have_lbra = true;
                    }
                }
                if (have_lbra == false)   // * 则忽略下一个 "
                    _errors.emplace_back(
                        CompilerError{_cur,
                                      _line,
                                      static_cast<col_t>(i),
                                      "lack of" + mark('(') + "in raw string delimiter"});
                if (raw_string_close == false) return DealLineFuncRetType::unclose_raw_string;
            }
            break;
        }
        case '/':   // comment
        {
            if (i + 1 == len) continue;
            // ----------- line comment -----------
            if (str[i + 1] == '/') {
                str = str.substr(0, i) + "\n";
                return DealLineFuncRetType::is_success;
            }
            // ----------- block comment ------------
            else if (str[i + 1] == '*') {
                block_comment_close = false;
                for (int j = i + 2; j < len - 1; ++j) {
                    if (str[j] == '*' && str[j + 1] == '/') {
                        block_comment_close = true;
                        str                 = str.substr(0, i) + str.substr(j + 1);
                        str[i++]            = ' ';   // * 将注释转为一个空格
                        len                 = str.size();
                        break;
                    }
                }
                if (block_comment_close == false) {
                    str = str.substr(0, i);
                    return DealLineFuncRetType::unclose_block_comment;
                }
            }
            break;
        }
        default: break;
        }
    }
    return DealLineFuncRetType::is_success;
}

PreProcess::DealLineFuncRetType PreProcess::deal_old_line_block_comment(std::string& str)
{
    auto len = str.size();
    for (int i = 0; i < len; ++i) {
        switch (str[i]) {
        case '*':
        {
            if (i + 1 == len) continue;
            if (str[i + 1] == '/') {
                str    = str.substr(i + 1);
                str[0] = ' ';
                return deal_new_line(str);
            }
            break;
        }
        default: break;
        }
    }
    str = "";
    return DealLineFuncRetType::unclose_block_comment;
}

PreProcess::DealLineFuncRetType PreProcess::deal_old_line_raw_string(std::string& str)
{
    auto len = str.size();
    for (int i = 0; i < len; ++i) {
        switch (str[i]) {
        case '\\':
        {
            if (i + 1 == len) continue;
            if (str[i + 1] == '"') ++i;
            break;
        }
        case '"':
        {
            if (i + 1 == len) return DealLineFuncRetType::is_success;
            string rear = str.substr(i + 1);
            auto   res  = deal_new_line(rear);
            str         = str.substr(0, i + 1) + rear;
            return res;
        }
        default: break;
        }
    }
    return DealLineFuncRetType::unclose_raw_string;
}

std::string PreProcess::is_include(string str, int pos)
{
    if (str.back() == '\n') str.pop_back();
    auto index = str.find("include", pos);
    if (index == string::npos) return "";
    index    = index + 7;   // include|
    auto len = str.length();
    if (index >= len) {
        _errors.emplace_back(CompilerError{_cur,
                                           _line,
                                           static_cast<col_t>(index),
                                           "after" + mark("#include") + "lack of path",
                                           CompilerError::ErrorCode::bad_preprocess});
        return "";
    }
    else if (index < len && str[index] != ' ') {
        _errors.emplace_back(CompilerError(_cur,
                                           _line,
                                           pos,
                                           "after" + mark("#include") + "lack of space",
                                           CompilerError::ErrorCode::bad_preprocess));
        return "";
    }
    for (int i = index + 1; i < len; i++) {
        if (str[i] == ' ') continue;
        if (str[i] == '<') return "";
        if (str[i] == '\"') {
            auto right = str.rfind('\"');
            if (right <= i) {
                _errors.emplace_back(CompilerError(_cur,
                                                   _line,
                                                   len - 1,
                                                   "after" + mark(str) + "lack of" + mark('"'),
                                                   CompilerError::ErrorCode::bad_preprocess));
                return "";
            }
            return str.substr(i + 1, right - i - 1);
        }
        _errors.emplace_back(CompilerError{
            _cur,
            _line,
            static_cast<col_t>(i),
            "after" + mark(str.substr(0, i)) + "lack of" + mark('\"') + "or" + mark('<'),
            CompilerError::ErrorCode::bad_preprocess});
        break;
    }
    return "";
}


// 如果为找到: return ""
// 否则: return path
// - first: find  _cur / path
// - second: find include_dir
FS::path PreProcess::find_path(const FS::path& path)
{
    auto tmp = _cur.parent_directory().path() / path;
    if (FS::is_regular_file(tmp)) return FS::canonical(tmp).string();
    for (auto dir : _include_dirs) {
        path::Path p(dir);
        if (p.have_file(path.string())) return FS::canonical(p.path() / path);
    }
    return "";
}

void PreProcess::deal_include(std::string& path, const path::Path& temp_dir)
{
    auto tmp = find_path(path);
    if (tmp == "") {
        _errors.emplace_back(CompilerError{_cur,
                                           _line,
                                           0,
                                           mark("#include") + ": not find" + mark_path(path),
                                           CompilerError::ErrorCode::bad_preprocess});
        path = "\n";
        return;
    }
    path = "\n";
    if (_not_include.count(tmp)) return;
    if (!_tmp_files.count(tmp)) {
        _tmp_files[tmp] = path::create_htf_temp_file(temp_dir).path();
        _tmp_tasks.push(tmp);
        path = "#" + std::move(tmp.string()) + path;
    }
}

}   // namespace htf
