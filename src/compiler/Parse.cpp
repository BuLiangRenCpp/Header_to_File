#include "Parse.h"
#include "Class.h"
#include "usage.h"
#include "ExcepPath.h"

using namespace std;

namespace htf
{
Parse::Parse(const path::Path& tmp_file, const FS::path& source)
    : _tmp_files{tmp_file}, _lex{}, _sources{source}, _errors{}
{}

Parse::Parse(const std::vector<path::Path>& tmp_files, const std::set<FS::path>& sources)
    : _tmp_files{tmp_files}, _lex{}, _sources{sources}, _errors{}
{
    HTF_DEV_ASSERT_MESSAGE(!_tmp_files.empty(), "Parse::Parse(..): tmp_files cannot be empty()");
}

void Parse::run1(FS::path output_path, bool is_force)
{
    _lex.open(_tmp_files[0], _sources);
    _lex.peek();
    path::Path cur = _lex.file();
    if (FS::is_directory(output_path)) output_path /= (cur.filename(false) + ".cpp");
    if (FS::exists(output_path) && is_force == false) {
        _lex.ignore_current_file();
        cout_warn("file already exist" + mark_path(output_path));
        return;
    }
    write_single_file(output_path);
    cout_log("output:" + mark_path(output_path));
}

void Parse::run(const path::Path& output_dir, bool is_force)
{
    THROW_EXCEP_PATH_IF(!output_dir.is_directory(), output_dir.path(), path::ExcepPath::ErrorCode::not_exist_directory);
    for (auto it : _tmp_files) {
        _lex.open(it, _sources);
        _lex.peek();   // 避免没有读取 lexer 时，当前文件是初始化的文件
        while (!_lex.eof()) {
            path::Path cur{_lex.file()};
            FS::path ofile = output_dir.path() / (cur.filename(false) + ".cpp");
            if (FS::exists(ofile) && is_force == false) {
                _lex.ignore_current_file();
                cout_warn("file already exist" + mark_path(ofile));
                continue;
            }
            write_single_file(ofile);
            cout_log("output:" + mark_path(ofile));
        }
    }
}

// 记录上一个写入文件的类别，用于格式化
enum class Kind
{
    is_null,
    is_namespace,
    is_variable,
    is_class,
    is_function
};

void Parse::write_single_file(const FS::path& ofile)
{
    Ofstream ofs(ofile);
    THROW_EXCEP_PATH_IF(!ofs, ofile, path::ExcepPath::ErrorCode::not_open_file);
    ofs << "#include "
        << "\"" << FS::path(_lex.file()).filename().string() << "\"\n\n";
    stack<string> stk;   // 处理 namespace 嵌套
    Kind          kind = Kind::is_null;
    auto          cur  = _lex.file();
    while (true) {
        auto lexer = _lex.get();
        if (lexer.empty()) break;
        if (cur != _lex.file()) {
            _lex.putback(lexer);
            break;
        }
        // ------- namespace -------
        if (lexer.val == "namespace") {
            string name = _lex.get().val;
            if (_lex.peek().val != "{") {   // namespace FS = std::filesystem;
                _lex.ignore_statement();
                continue;
            }
            if (kind != Kind::is_namespace) ofs << "\n";
            ofs << "namespace " << name << "\n{\n";
            kind = Kind::is_namespace;
            stk.emplace(std::move(name));
            _lex.get();
        }
        else if (lexer.val == "}") {
            if (stk.empty()) {
                _errors.emplace_back(CompilerError{_lex.file(),
                                                 lexer.line,
                                                 lexer.col,
                                                 "before" + mark('}') + "lack of" + mark('{')});
                continue;
            }
            string name = stk.top();
            stk.pop();
            if (kind != Kind::is_namespace && kind != Kind::is_function && kind != Kind::is_class)
                ofs << "\n";
            ofs << "}   // "
                << "namespace " << name << "\n";
            kind = Kind::is_namespace;
        }
        // ------- variable --------
        else if (lexer.val == "extern") {
            auto v = detail::get_variable(_lex);
            if (!v.empty() && v.val.empty()) {
                if (kind != Kind::is_variable) ofs << "\n";
                ofs << v.type << " " << v.name << ";\n";
                kind = Kind::is_variable;
            }
        }
        else if (lexer.kind == LexerKind::type) {
            _lex.putback(lexer);
            auto f = detail::get_function(_lex);
            if (!f.empty()) {
                if (kind != Kind::is_function) ofs << "\n";
                ofs << f << "\n";
                kind = Kind::is_function;
            }
        }
        else if (is_class_key(lexer.val)) {
            _lex.putback(lexer);
            auto c = detail::get_class(_lex);
            if (!c.empty()) {
                if (kind != Kind::is_class) ofs << "\n";
                ofs << c;
                kind == Kind::is_class;
            }
        }
        else
            _lex.ignore_statement();
    }
}

void Parse::deal_class(Ofstream& ofs)
{
    ofs << detail::get_class(_lex).str() << "\n";
}

void Parse::deal_function(Ofstream& ofs)
{
    ofs << detail::get_function(_lex).str() << "\n";
}

}   // namespace htf
