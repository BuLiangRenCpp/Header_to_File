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
        print_enter(mark_char('y', '"', 1) + "or" + mark_char('n', '"', 1));
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

        Function f;
        Class_fun cf;
        bool write_success = false;       // 记录是否成功写入文件
        bool warn_flag = true;      // 用于只执行一次 print_warn
        int warn_count = 0;
        while (!ifs.eof()) 
        try{
            ifs >> cf;          // 先是类再是类外函数，注意顺序
            if (cf.empty()) {
                ifs >> f;
                if (f.empty()) clear_mess(ifs);        // 都不是函数
                else {
                    ofs << f << endl;
                    write_success = true;
                }
            }
            else {
                ofs << cf << endl;
                write_success = true;
            }
        }
        catch (string& e) {
            if (warn_flag) {
                print_warn(ifile + "存在异常：");
                warn_flag = false;
            }
            print_warn("异常 " + to_string(++warn_count) + ": ", true, false);
    #ifdef USER
            print_warn(mark_string("line " + to_string(TS.line()), "\"", 0) + " " + e);
    #else
            print_warn(mark_string("line " + to_string(TS.line()), "\"", 0) + " " + e, false);           // 开发者模式
    #endif
            clear_mess(ifs);
        }
        if (write_success) print_result("生成成功 -> " + ofile + "\n");
        else print_error("生成失败 -> " + ifile + "，请查看头文件是否存在函数声明语句或者有语法错误" + "\n");
        return write_success;
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
            print_result("file " + to_string(++count) + ": ");
            string ifile = idir + name;
            string ofile = odir + replace_extension(name);
            if (!header_to_file(ifile, ofile)) path_deal::remove_file(ofile);       // 删除失败文件
        }
    }
}