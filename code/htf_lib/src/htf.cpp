#include "htf.h"

using namespace prompt;
using namespace path_deal;


static void clear_mess(istream& is)
{
    TS.ignore(is, Token{ ';' });
}

// 询问用户是否覆盖已有文件，输入 y or n
static bool is_write_file()
{
    while (true) {
        print_enter("\"" + mark_char('y') + "or" + mark_char('n') + "\"");
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
// *************************************************


// *********************************************************************************************

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


namespace htf {
    bool header_to_file(const string& ifile, string ofile)
    {
        // ****** 每个文件使用刚初始化的 TS ******
        TS.clear();
        // **************************************
        ifstream ifs(ifile);
        if (!ifs) throw string("htf::hearer_to_file:" + mark_string(ifile) + "打开失败");

        if (ofile == "") ofile = replace_extension(ifile);

        // 避免覆盖文件
        if (is_exist(ofile)) {
            print_warn(ofile + "已经存在，是否覆盖源文件" + mark_string("y-覆盖, n-不覆盖"));
            if (!is_write_file()) {
                print_result("跳过头文件" + mark_string(ifile) + "\n");
                return true;
            }
        }
        ofstream ofs(ofile);
        if (!ofs) throw string("htf::header_to_file:" + mark_string(ofile) + "打开失败");

        ofs << "#include";
        ofs << " \"" << file_name(ifile) << "\"" << "\n\n" << endl;

        Bras t;
        rw_file(t, ifs, ofs);
        bool res = t.write_success;
        if (res) print_result("生成成功 -> " + ofile + "\n");
        else {
            print_error("生成失败 -> " + ifile + "，请查看头文件是否存在函数声明语句或者有语法错误" + "\n");
            ofs.close();
            path_deal::remove_file(ofile);       // 删除失败文件
        }
        return res;
    }

    void header_to_files(string idir, string odir)
    {
        if (!is_exist_dir(idir)) 
            throw string("htf::header_to_files:" + mark_string(idir) + "目录不存在");
        if (odir != "" && !is_exist_dir(idir)) 
            throw string("htf::header_to_files:" + mark_string(odir) + "目录不存在");
        if (idir.back() != '\\') idir.push_back('\\');

        if (odir == "") odir = idir;
        else if (odir.back() != '\\') odir.push_back('\\');

        vector<string> files = find_same_extension_files(idir);
        if (files.empty())
            throw string("htf::header_to_files: " + mark_string(idir) + "目录中没有C++头文件");
        int count = 0;
        for (const auto& name : files) {
            print_result("file " + to_string(++count) + "->" + mark_string(name) + ": ");
            string ifile = idir + name;
            string ofile = odir + replace_extension(name);
            header_to_file(ifile, ofile);
        }
    }
}