#include "htf.h"
#include "ExcepPath.h"
#include "output.h"

void htf::header_to_file(FS::path source, const FS::path& output_path,
                         const std::vector<path::Path>& include_dirs, bool is_force)
{
    path::Path p{source};
    if (!p.is_cpp_header()) {
        cout_warn("it's not c++ header file [.h, .hpp, .h++, .inl]:" + mark_path(source));
        return;
    }
    // ----------------- preprocess ---------------------
    auto       temp_dir = path::create_htf_temp_directory().path();
    PreProcess pre(p, include_dirs);
    FS::path   preprocess_file =
        temp_dir / (p.filename(false) + path::HTF_Temp_PreProcess_File_Extension);
    if (!pre.run(preprocess_file)) {
        cout_error("preprocess errors");
        std::cout << pre.errors() << std::endl;
        pre.clear();
        return;
    }
    pre.clear(false);
    // ------------------- compiler -----------------------
    {
        Parse parse(preprocess_file, source);
        parse.run1(output_path, is_force);
        auto errors = parse.errors();
        if (!errors.empty()) {
            cout_error("compiler errors");
            std::cout << errors << std::endl;
        }
    }
    FS::remove_all(temp_dir);
}

/************************************************************************************ */
/**
 * * [方案]:
 * *    - PreProcess 依次处理文件，记录每一次已经展开了的文件路径，使得每个文件只展开一次
 * *    - 可能对应的 tmp_file 有多个，按处理顺序加入 tmp_files
 * *    - 最后交给 Parse 处理即可
 */

void htf::header_to_file(const std::set<FS::path>& sources, const FS::path& output_dir,
                         const std::vector<path::Path>& include_dirs, bool is_force)
{
    std::set<FS::path>      visited;
    std::vector<path::Path> tmp_files;
    auto                    temp_dir = path::create_htf_temp_directory().path();
    for (auto it : sources) {
        if (!path::Path{it}.is_cpp_header()) {
            cout_warn("it's not c++ header file [.h, .hpp, .h++, .inl]:" + mark_path(it));
            continue;
        }
        PreProcess pre(it, include_dirs, visited);
        FS::path   preprocess_file =
            temp_dir / (path::Path{it}.filename(false) + path::HTF_Temp_PreProcess_File_Extension);
        if (!pre.run(preprocess_file)) {
            cout_error("preprocess errors");
            std::cout << pre.errors() << std::endl;
            pre.clear();
            return;
        }
        auto v = pre.visited_files();
        visited.insert(v.begin(), v.end());
        if (FS::exists(preprocess_file)) tmp_files.emplace_back(std::move(preprocess_file));
        pre.clear(false);
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
    FS::remove_all(temp_dir);
}
