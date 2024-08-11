#pragma once

// Parse: 读取 头文件 中声明语句 (function and class)
// * 作用: 读取单个 PreProcess 文件，通过 sources 确定哪些文件需要输出

#include "Lex.h"

namespace htf
{

class Parse
{
public:
    // 单文件处理
    // tmp_file 必须是 PreProcess 处理后的文件
    Parse(const path::Cfile& tmp_file, const path::Cfile& source);
    // 多文件处理
    // * 注意 tmp_files 顺序，否则可能处理错误 (type 被识别为 id)
    Parse(const std::vector<path::Cfile>& tmp_files, const std::set<std::string>& sources);
    ~Parse() = default;

public:
    void run(const std::string& output_path, bool is_force = false);
    // is_force: 当文件存在时是否覆盖，默认不覆盖
    void run(const path::Dir& output_dir, bool is_force = false);

    std::string errors() const
    {
        std::ostringstream oss;
        for (int i = 0; i < _errors.size(); ++i) {
            oss << "[" << i << "] " << _errors[i].str() << "\n";
        }
        auto tmp = _lex.errors();
        return (_errors.empty()) ? tmp : tmp + "[parse error]:\n" + oss.str();
    }

private:
    void write_single_file(const std::string& ofile);
    void deal_class(Ofstream& ofs);
    void deal_function(Ofstream& ofs);

private:
    std::vector<path::Cfile> _tmp_files;
    Lex                      _lex;
    std::set<std::string>    _sources;
    std::vector<ExcepSyntax> _errors;
};

}   // namespace htf