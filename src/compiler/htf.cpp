#include "htf.h"

#include "PreProcess.h"

void htf::header_to_file(path::Cfile source, const std::string& output_path,
                         const std::vector<path::Dir>& include_dirs, bool is_force)
{
    if (!source.is_header()) {
        cout_warn("it's not c++ header file [.h, .hpp, .h++, .inl]:" + mark(source.str()));
        return;
    }
    PreProcess  pre(source, include_dirs);
    std::string tmp = path::Dir{}.append_path(source.change_extension_filename());
    if (!pre.run(tmp)) {
        cout_error("preprocess errors");
        std::cout << pre.errors() << std::endl;
        return;
    }
    {
        Parse parse(tmp, source);
        parse.run(output_path, is_force);
        auto errors = parse.errors();
        if (!errors.empty()) {
            cout_error("syntax errors");
            std::cout << errors << std::endl;
        }
    }
    auto p = FS::path{tmp}.parent_path();
    if (p.filename() == path::Htf_Temp_Dir)
        FS::remove_all(p);
    else
        FS::remove(tmp);
}

/************************************************************************************ */
/**
 * * [方案]:
 * *    - PreProcess 依次处理文件，记录每一次已经展开了的文件路径，使得每个文件只展开一次
 * *    - 可能对应的 tmp_file 有多个，按处理顺序加入 tmp_files
 * *    - 最后交给 Parse 处理即可
 */

void htf::header_to_file(const std::set<std::string>& sources, const path::Dir& output_dir,
                         const std::vector<path::Dir>& include_dirs, bool is_force)
{
    std::set<std::string>    visited;
    std::vector<path::Cfile> tmp_files;
    for (auto it : sources) {
        if (!path::Cfile{it}.is_header()) {
            cout_warn("it's not c++ header file [.h, .hpp, .h++, .inl]:" + it);
            continue;
        }
        PreProcess  pre(it, include_dirs, visited);
        std::string tmp = path::Dir{}.append_path(path::Cfile{it}.change_extension_filename());
        if (!pre.run(tmp)) {
            cout_error("preprocess errors");
            std::cout << pre.errors() << std::endl;
            return;
        }
        auto v = pre.visited_files();
        visited.insert(v.begin(), v.end());
        if (FS::exists(tmp)) tmp_files.emplace_back(std::move(tmp));
    }
    if (tmp_files.empty()) return;
    {
        Parse parse(tmp_files, sources);
        parse.run(output_dir, is_force);
        auto errors = parse.errors();
        if (!errors.empty()) {
            cout_error("syntax errors");
            std::cout << errors << std::endl;
        }
    }
    auto p = FS::path{tmp_files[0].str()}.parent_path();
    if (p.filename() == path::Htf_Temp_Dir)
        FS::remove_all(p);
    else
        for (auto it : tmp_files) FS::remove(it.str());
}
