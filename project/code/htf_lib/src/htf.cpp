#include "htf.h"

using namespace std;
using namespace prompt;
using namespace path_deal;


static void clear_mess(istream& is)
{
    TS.ignore(is, Token{ ';' });
}

// 询问用户是否决定做某个操作，输入 y or n
static bool input_y_n()
{
    while (true) {
        print_enter(mark_char('y') + "or" + mark_char('n'));
        string line;
        getline(cin, line);
        usage::delete_space_pre_suf(line);
        if (line.length() == 1) {
            if (line[0] == 'y' || line[0] == 'Y') return true;
            if (line[0] == 'n' || line[0] == 'N') return false;
        }
    }
    return false;
}

// *******************************************************************************************
// ************ 特殊用途，针对 rw_file *************
struct Bras {
    stack<char> brackets;       // {} 嵌套判断，确定缩进
    bool write_success;         // 用于判断文件是否写入成功
    Bras() :brackets{ }, write_success{ false } { }
};
// ********************************************************************************************

// 读写函数、类
static bool rw_function(istream& ifs, ostream& ofs, int count)
{
    if (count < 0) count = 0;

    Function f;
    Class_fun cf;

    bool write_success = false;     // 是否写入成功

    while (TS.peek(ifs).val != "namespace" && TS.peek(ifs) != '}' && !TS.eof(ifs))
    try{
        ifs >> cf;          // 先是类再是类外函数，注意顺序
        if (cf.empty()) {
            ifs >> f;
            if (f.empty()) clear_mess(ifs);        // 都不是函数
            else {
                f.print(ofs, count);
                ofs << endl;
                write_success = true;
            }
        }
        else {
            cf.print(ofs, count);
            ofs << endl;
            write_success = true;
        }
    }
    catch (string& e)
    {
#ifdef USER
        print_warn(mark_string("line " + to_string(TS.line())) + " " + e);
#else
        print_warn(mark_string("line " + to_string(TS.line())) + " " + e, false);           // 开发者模式
#endif
        clear_mess(ifs);
    }
    
    return write_success;
}

// 读写文件，增加识别 namespace 的功能
static void rw_file(Bras& t, istream& ifs, ostream& ofs)
{
    if (TS.eof(ifs)) return;
    Identifier name;
    if (TS.peek(ifs).val == "namespace") {          // --- 区分 namespace FS = ...
        TS.get(ifs);
        name = Identifier{ (TS.get(ifs).val) };
        if (TS.peek(ifs).kind != '{')
            clear_mess(ifs);
        else {
            t.brackets.push('{');
            print_indentation(ofs, t.brackets.size() - 1);
            ofs << "namespace " << name << " " << TS.get(ifs).kind << endl;
        }
    } 
    bool flag = rw_function(ifs, ofs, t.brackets.size());
    if (t.write_success == false && flag == true) t.write_success = true;       // 只要依次写入成功就行
    if (TS.peek(ifs).kind == '}') {
        print_indentation(ofs, t.brackets.size() - 1);
        ofs << TS.get(ifs).kind << endl << endl;
        if (t.brackets.empty())
            throw string("namespace " + name.str() + " 缺少" + mark_char('}'));
        t.brackets.pop();
    }
    rw_file(t, ifs, ofs);
}

static bool is_file_path(const string& path)
{
    return extension(path) != "";
}

// 将 header_to_file 的 odir 转为文件路径
// odir = "" 直接返回 
static void legal_odir(string& odir, const string& name)
{
    if (odir == "") return;
    if (!is_file_path(odir)) {       // 1. 目录加上文件名
        odir = odir + "/" + name;
        odir = replace_extension(odir);
    }
    else {      // 2. 路径加上扩展名
        if (!is_source_extension(extension(odir))) odir += ODEFAULT_EXTENSION;
    }
}

// 对 path 进行判断，如果父目录不存在则提示创建 （is_force == true 不提示直接创建)
// 返回的是用户的选择 y or n
static bool create_parent_dir(const string& path, bool is_force)
{
    string dir = parent_dir(path);
    if (!is_exist(dir)) {
        if (is_force) create_dirs(dir);
        else {
            print_warn("目录" + mark_string(dir) + "不存在，是否创建此文件" + "(y - 创建, n - 取消)");
            if (input_y_n()) create_dirs(dir);
            else return false;
        }
    }
    return true;
}


namespace htf {
    bool header_to_file(string ifile, string odir, bool is_force)
    {
        // ****** 每个文件使用刚初始化的 TS ******
        TS.clear();

        // ************************ read file **********************************
        if (extension(ifile) == "") ifile += IDEFAULT_EXTENSION;
        else if (!is_header_extension(extension(ifile)))
            throw string("htf::header_to_file:" + mark_string(ifile) + "不是C++头文件");
        ifstream ifs(ifile);
        if (!ifs) throw string("htf::hearer_to_file: 文件" + mark_string(ifile) + "打开失败");

        // ************************ legal odir *********************************** 
        legal_odir(odir, file_name(ifile));
        if (odir == "") odir = replace_extension(ifile);        // 默认路径

        // 1. 提示创建不存在目录
        if (!create_parent_dir(odir, is_force)) return false;

        // 2. 避免覆盖文件，提示用户
        if (is_force == false && is_exist_file(odir)) {
            print_warn(mark_string(odir) + "已经存在，是否覆盖源文件" + "(y-覆盖, n-不覆盖)");
            if (!input_y_n()) {
                print_result("跳过头文件" + mark_string(ifile) + "\n");
                return true;
            }
        }

        // ************************* write file *******************************
        ofstream ofs(odir);
        if (!ofs) throw string("htf::header_to_file: 文件" + mark_string(odir) + "打开失败");

        ofs << "#include";
        ofs << " \"" << file_name(ifile) << "\"" << "\n\n" << endl;

        Bras t;
        try {
            rw_file(t, ifs, ofs);
        }
        catch (string& e) {
#ifdef USER
            print_warn(mark_string("line " + to_string(TS.line())) + " " + e);
#else
            print_warn(mark_string("line " + to_string(TS.line())) + " " + e, false);           // 开发者模式
#endif
        }
        bool res = t.write_success;

        // ************************ print result ********************************
        if (res) print_result("生成成功 -> " + odir + "\n");
        else {
            print_error("生成失败 -> " + ifile + "，请查看头文件是否存在函数声明语句或者有语法错误" + "\n");
            ofs.close();
            path_deal::remove_file(odir);       // 删除失败文件
        }
        return res;
    }

    void header_to_files(string idir, string odir, bool is_force)
    {
        if (!is_exist_dir(idir)) 
            throw string("htf::header_to_files:" + mark_string(idir) + "目录不存在");
        if (is_exist_file(odir))
            throw string("htf::header_to_files: 输出路径应该是目录，" + mark_string(odir) + "是文件路径");

        vector<string> files = find_same_extension_files(idir);
        if (files.empty())
            throw string("htf::header_to_files:" + mark_string(idir) + "目录中没有C++头文件");
        int count = 0;
        if (odir == "") odir = idir;
        // 1. 提示创建不存在目录
        if (!is_exist_dir(odir)) {
            print_warn("目录" + mark_string(odir) + "不存在，是否创建此文件" + "(y - 创建, n - 取消)");
            if (input_y_n()) create_dirs(odir);
            else return;
        }

        for (const auto& name : files) {
            print_result(mark_string("file " + to_string(++count)) + "->" + mark_string(name) + ": ");
            string ifile = normalize(idir, name);
            string ofile = normalize(odir, replace_extension(name));
            header_to_file(ifile, ofile, is_force);
        }
    }
}