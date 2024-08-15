#pragma once

// * 预处理器：htf -i file -E [-o outputfile]    // 都是单值
// * #include:
//      - <xx>: 不处理
//      - "xx": 展开此文件
// * #define A B:
//      直接删除所有的 A (此语法难以处理，并且在头文件中一般用于 动态库导出 API)
// * #define A:
//      目前：同 #define A
// * 其余：均直接忽略 (可能下一步会处理：条件编译指令)

/**
 * * [输出约定]:
 * * - 第一行为标识字符: Htf_PreProcess_File_Id，表明是 PreProcess 生成的文件 (如果没有则不能处理)
 * * - 第二行为： ##path  (path 为源文件的路径，不是 tempfile 的路径)
 * * - #include 只处理使用 ""
 * *    - 将 #include "file.h" 换为  '#file.h完整路径'
 * *    - #file.h 以下都是 file.h 的内容
 * *    - file.h 内容结束标记为:  '#end'
 * ! #path 以下， #end 以上是 path 的内容:
 * ! -  path 的第一行为 #path 的下一行
 * ! -  path 的最后一行是 #end 的上一行
 */

#include <map>
#include <queue>
#include <set>
#include <vector>

#include "Path.h"
#include "CompilerError.h"

namespace htf
{

extern const std::string HTF_PreProcess_File_Id;

class PreProcess
{
public:
    // - source: 源文件 (一般都是头文件路径)
    // - include_dirs: 指出 source 中 include 路径所在的目录
    // - not_include: 指出不需要展开的路径
    // - - 默认将所有  #include "" 都展开
    // - - 指出的话应该是绝对路径，在 deal_include 时比较
    PreProcess(const path::Path& source, const std::vector<path::Path>& include_dirs = {},
               const std::set<FS::path>& not_include = {});
    ~PreProcess();

public:
    // - output_path: 指定输出路径 (if output_path == "": use default path
    // 注意可能会覆盖源文件)
    // - temp_dir: 指定临时文件输出路径 (默认 current)
    bool run(FS::path output_path = "",
             const FS::path& temp_dir     = (FS::current_path() / path::HTF_Temp_Directory_Name));
    // 返回已经处理过的文件
    std::set<FS::path> visited_files() const
    {
        std::set<FS::path> res;
        for (auto it : _tmp_visited) {
            res.insert(it.first);
        }
        return res;
    }

    std::string errors() const;
    // 仅移除临时文件
    void        clear();

private:
    path::Path            _source;   // 第一次初始化的文件
    path::Path            _cur;      // 当前正在处理的文件
    std::vector<path::Path> _include_dirs;
    std::set<FS::path>  _not_include;
    line_t                 _line;   // 当前文件所在行号
    std::vector<CompilerError> _errors;   // * 后续想法: 当错误达到指定量时才输出 (或者 run 结束)

private:
    std::map<FS::path, FS::path> _tmp_files;   // file.h 对应的临时文件 file.i
    std::queue<FS::path>
        _tmp_tasks;   // * 从 tasks 取任务，通过 map 找到对应的 输出路径
    std::map<FS::path, FS::path>
        _tmp_visited;   // * 已经处理的文件, 用于避免重复处理文件，以及删除临时文件

private:
    // 处理单个文件:
    // path: input file path, 调用则保证 path 已经存在 _tmp_files
    void deal_single_file(const FS::path& path);
    // 合并临时文件
    // ofs: 调用前已经确定 ofs.good
    // * ifile must be '_source'
    void merge_temp_file(const FS::path& ifile, Ofstream& ofs);

private:
    /**
     * [目前方案]:
     * - 对于单个文件 先 deal_line，返回处理后的 string，在 deal_comment，由 run
     * 将处理后的结果写入文件
     * - 将 #include "file.h" file.h 路径加入 _files，处理完，处理完的文件从 _files 中删除
     * - #include "file.h"  ->  #file.h
     * - 最后处理所有临时文件中的： #file.h 指令 -- 加入文件内容插入
     */

    // 处理 '\'
    // return: 每一行处理后的结果
    std::string deal_line(Ifstream& ifs);
    // pos: the second '/' index
    void deal_line_comment(std::string& str, int pos);
    // pos: the '*' pos
    // return: 是否找到了 */
    int deal_block_comment(std::string& str, int pos);

private:
    // 判断是否是  "  or  <     (#include <..>     #include "..")
    // 目前只处理  #include ""
    // - str: #include ...
    // - pos: the "#" back index
    // - return:
    // - - "file.h":   #include "file.h"
    // - -    ""   :   #include <..>  and  other(_errors)
    std::string is_include(std::string str, int pos);
    // - str: "path.h"  ->  #path.h
    // 将 path.h 对应的临时文件路径 加入 _tmp_files
    void deal_include(std::string& path, const path::Path& output_dir);
    // 如果为找到: return ""
    // 否则: return path
    // - first: find  _cur / path
    // - second: find include_dir
    FS::path find_path(const FS::path& path);
};

}   // namespace htf