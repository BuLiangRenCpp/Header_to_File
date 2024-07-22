#ifdef _WIN32
#    define LIB_EXPORT
#endif

#include "compile.h"
#include "Excep_path.h"
#include "Excep_syntax.h"
#include "output.h"
#include "usage.h"

#include "Class.h"
#include "Lex.h"

using namespace std;
using namespace output;

namespace htf
{
using namespace path_deal;
using namespace lex;
using namespace parse;

namespace core
{
static bool input_y_n()
{
    while (true) {
        print_enter(mark('y') + "or" + mark('n'));
        string line;
        getline(cin, line);
        stream::usage::delete_space_pre_suf(line);
        if (line.length() == 1) {
            if (line[0] == 'y' || line[0] == 'Y') return true;
            if (line[0] == 'n' || line[0] == 'N') return false;
        }
    }
    return false;
}

// 忽略一条语句
static void clear_mess(Lex& lex)
{
    lex.ignore();
}

// *********************************************************
// ************ 特殊用途，针对 rw_file *************
struct Bras {
    stack<char> brackets;        // namespace 的 {} 嵌套判断，确定缩进
    bool        write_success;   // 用于判断文件是否写入成功
    Bras()
        : brackets{}
        , write_success{false}
    {
    }
};
// **********************************************************


// 只是查询 type
static pair<vector<string>, vector<string>> _pre(Lex& lex)
{
    lex.ignore(Lexer{});
    return make_pair(lex.basic_types(), lex.containers());
}

// 读写类内、类外函数
static bool rw_function(Lex& lex, ostream& ofs, int count)
{
    if (count < 0) count = 0;

    Function f;
    Class    cf;
    bool     write_success = false;   // 是否写入成功
    bool     is_first      = true;    // 格式化
    while (true)
        try {
            if (lex.peek().kind == Lexer_kind::NAMESPACE_KIND || lex.peek().kind == '}' || lex.eof()) break;
            cf.get(lex);   // 先是类再是类外函数，注意顺序
            if (cf.empty()) {
                f.get(lex);
                if (!f.empty()) {
                    if (!is_first) ofs << "\n";
                    ofs << f.str(count);
                    write_success = true;
                    is_first      = false;
                } else
                    clear_mess(lex);   // f.fail() = true
            } else {
                if (!is_first) ofs << "\n";
                ofs << cf.str(count);
                write_success = true;
                is_first      = false;
            }
        } catch (excep::Excep_syntax& e) {
            print_warn(e.str());
            clear_mess(lex);
        }
    return write_success;
}

// * 读写文件，增加识别 namespace 的功能
static void rw_file(Bras& t, Lex& lex, ostream& ofs)
{
    if (lex.eof()) return;
    string name;
    // namespace A { }
    if (lex.peek().kind == Lexer_kind::NAMESPACE_KIND) {
        name = lex.get().val;
        if (lex.peek().kind != '{')
            clear_mess(lex);
        else {
            t.brackets.push('{');
            print_indentation(ofs, t.brackets.size() - 1);
            ofs << "namespace " << name << " " << lex.get().kind << endl;
        }
    }
    bool flag = rw_function(lex, ofs, t.brackets.size());
    // **************************************************************
    if (lex.peek().kind == Lexer_kind::NAMESPACE_KIND) rw_file(t, lex, ofs);
    if (lex.peek().kind == '}') {   // * namespace 结束
        print_indentation(ofs, t.brackets.size() - 1);
        ofs << lex.get().kind << endl;
        if (t.brackets.empty())
            throw excep::Excep_syntax(lex.hpath().str(), lex.line(), "lack of" + mark('{'));
        t.brackets.pop();
        rw_file(t, lex, ofs);
    }
    // ***************************************************************
    if (flag == false) clear_mess(lex);
    if (t.write_success == false && flag == true) t.write_success = true;   // 只要依次写入成功就行
    rw_file(t, lex, ofs);
}

// 既找 type，也 输出定义语句
static pair<vector<string>, vector<string>> _compile(Lex& lex, const Hdir& output_dir, bool is_force)
{
    string output_path    = normalize(output_dir.str(), lex.hpath().filename());
    output_path           = change_extension(output_path);
    bool is_already_exist = false;      // 避免删除已经存在的源文件
    if (is_exist_file(output_path)) {   // 是否覆盖源文件
        is_already_exist = true;
        if (!is_force) {
            print_warn(mark(output_path) + "already exist, do you want to overwrite the source file?" +
                       "(y / n)");
            if (!input_y_n()) return _pre(lex);
        }
    }

    ofstream ofs(output_path);
    if (!ofs) throw excep::Excep_path("compile.cpp::_compile", "not open file" + mark(output_path));

    // ********************** write file ***************************
    ofs << "#include";
    ofs << mark(file_name(lex.hpath().str())) << "\n\n";
    Bras t;
    try {
        rw_file(t, lex, ofs);
    } catch (excep::Excep_syntax& e) {
        print_warn(e.str());
    }

    // ************************ print result ********************************
    if (t.write_success)
        print_result("success -> " + output_path);
    else {
        print_error("failure ->" + mark(lex.hpath().str()));
        ofs.close();
        if (!is_already_exist) path_deal::remove_file(output_path);   // * 删除失败文件
    }
    return make_pair(lex.basic_types(), lex.containers());
}

void compile(const Reliance& reliances, const Hdir& output_dir, bool is_force)
{
    for (const auto& lex : reliances) {
        // 一个依赖关系
        vector<string> bt, co;
        for (const auto& t : lex) {
            ifstream ifs{t.second.str()};
            Lex      lex{ifs, t.second, bt, co};
            if (t.first == false) {
                auto temp = _pre(lex);
                bt        = my_std::unite(bt, temp.first);
                co        = my_std::unite(co, temp.second);
            } else {
                auto temp = _compile(lex, output_dir, is_force);
                bt        = my_std::unite(bt, temp.first);
                co        = my_std::unite(co, temp.second);
            }
        }
    }
}
}   // namespace core
}   // namespace htf
