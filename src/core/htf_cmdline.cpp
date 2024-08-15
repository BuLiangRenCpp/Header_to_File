#include "htf_cmdline.h"

#include "base_info.h"
#include "htf.h"
#include "path_regex.h"

using namespace htf;
using namespace htf::path;

static void preprocess(const std::string& source, std::string output_path,
                       const std::vector<std::string>& include_dirs)
{
    std::vector<Path> inl;
    for (auto it : include_dirs) {
        inl.emplace_back(it);
    }
    PreProcess pre(FS::path{source}, inl);
    FS::path out = output_path;
    if (out.empty()) out = FS::current_path() /  (Path{source}.filename(false) + HTF_Temp_PreProcess_File_Extension);
    auto temp_dir = path::create_htf_temp_directory().path();
    if (!pre.run(out, temp_dir)) {
        cout_error("preprocess errors");
        std::cout << pre.errors() << std::endl;
    }
    else cout_log("output:" + mark_path(out));
    FS::remove_all(temp_dir);
}

static std::set<FS::path> legal_input_paths(const std::vector<std::string>& paths)
{
    std::set<FS::path> res;
    for (auto it : paths) {
        auto tmp = path::path_regex(it);
        for (auto t : tmp) {
            res.emplace(FS::canonical(t));      // ! 无语：必须处理成绝对路径
        }
    }
    return res;
}

static std::set<FS::path> legal_include_paths(const std::vector<std::string>& paths)
{
    std::set<FS::path> res;
    for (auto it : paths) {
        auto tmp = path::path_regex(it, true);
        res.insert(tmp.begin(), tmp.end());
    }
    return res;
}

static void compiler1(FS::path source, FS::path output_path,
                      const std::vector<Path>& include_dirs, bool is_force)
{
    if (output_path.empty()) output_path = FS::current_path();
    source = FS::canonical(FS::current_path() / source);
    header_to_file(source, output_path, include_dirs, is_force);
}

static void compiler(const std::vector<std::string>& sources, FS::path output_path,
                     const std::vector<std::string>& include_dirs, bool is_force)
{
    auto             inl = legal_include_paths(include_dirs);
    std::vector<Path> dirs;
    for (auto it : inl) dirs.emplace_back(it);
    if (sources.size() == 1 && !is_regex_path_str(sources[0]))
        compiler1(sources[0], output_path, dirs, is_force);
    else {
        auto input = legal_input_paths(sources);
        if (output_path.empty()) output_path = FS::current_path();
        THROW_EXCEP_CMDLINE_IF(!FS::is_directory(output_path), 
            "Inputting much files requires to use output directory: it's not directory" << mark_path(output_path));
        header_to_file(input, output_path, dirs, is_force);
    }
}

void htf::cmdline::htf_cmdline_parse(int argc, char* argv[])
{
    // compiler and preprocess
    Parse parse;
    parse.set_program_name(HTF_PROGRAM);
    parse.add("force", 'f', "If output file exists, overwrite the file");
    parse.add("pre", 'E', "preprocess the input file");
    parse.add_more<std::string>("input", 'i', "input files", true, true);
    parse.add_more<std::string>("include", 'I', "include directories", false, false, {});
    parse.add<std::string>("output", 'o', "output path", false, false, "");
    parse.add("help", 'h', "the htf-options prompt");
    parse.add("version", 'v', "the htf version");
    parse.add("show", 's', "show the values of input options");
    if (!parse.parse(argc, argv)) {
        if (parse.exist('h')) {
            std::cout << parse.usage() << std::endl;
        }
        else if (parse.exist('v')) {
            cout_log(HTF_VERSION);
        }
        else {
            cout_error("htf-options errors");
            std::cout << parse.errors() << std::endl;
        }
    }
    else {
        if (parse.exist('h')) {
            std::cout << parse.usage() << std::endl;
        }
        else if (parse.exist('v')) {
            cout_log(HTF_VERSION);
        }
        // --------------- show -------------------
        else if (parse.exist('s')) {
            std::cout << parse.show() << std::endl;
        }
        // ------------ preprocess ---------------
        else if (parse.exist('E')) {
            auto input = parse.get_values('i');
            THROW_EXCEP_CMDLINE_IF(input.size() > 1, "-E:" << "option" << mark("--input", '[') << "have too much values");
            preprocess(input[0], parse.get_value('o'), parse.get_values('I'));
        }
        // ----------- compilier ----------------
        else {
            compiler(parse.get_values('i'),
                     parse.get_value('o'),
                     parse.get_values('I'),
                     parse.exist('f'));
        }
    }
}