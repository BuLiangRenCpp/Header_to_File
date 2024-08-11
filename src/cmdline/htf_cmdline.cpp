#include "htf_cmdline.h"

#include "Cfile.h"
#include "base_info.h"
#include "htf.h"
#include "path_regex.h"

using namespace htf;
using namespace htf::path;

static void preprocess(const std::string& source, std::string output_path,
                       const std::vector<std::string>& include_dirs)
{
    std::vector<Dir> inl;
    for (auto it : include_dirs) {
        inl.emplace_back(Dir{it});
    }
    PreProcess pre(source, inl);
    if (output_path.empty()) {
        output_path = (FS::current_path() / Cfile{source}.change_extension_filename()).string();
    }
    if (!pre.run(output_path)) {
        cout_error("preprocess errors");
        std::cout << pre.errors() << std::endl;
    }
}

static std::set<std::string> legal_input_paths(const std::vector<std::string>& paths)
{
    std::set<std::string> res;
    for (auto it : paths) {
        auto tmp = path::path_regex(it);
        res.insert(tmp.begin(), tmp.end());
    }
    return res;
}

static std::set<std::string> legal_include_paths(const std::vector<std::string>& paths)
{
    std::set<std::string> res;
    for (auto it : paths) {
        auto tmp = path::path_regex(it, true);
        res.insert(tmp.begin(), tmp.end());
    }
    return res;
}

static void compiler1(const std::string& source, std::string output_path,
                      const std::vector<Dir>& include_dirs, bool is_force)
{
    if (output_path.empty())
        output_path =
            (FS::current_path() / Cfile{source}.change_extension_filename(".cpp")).string();
    header_to_file(source, output_path, include_dirs, is_force);
}

static void compiler(const std::vector<std::string>& sources, const std::string& output_path,
                     const std::vector<std::string>& include_dirs, bool is_force)
{
    auto             inl = legal_include_paths(include_dirs);
    std::vector<Dir> dirs;
    for (auto it : inl) dirs.emplace_back(it);
    if (sources.size() == 1 && !is_regex_path_str(sources[0]))
        compiler1(sources[0], output_path, dirs, is_force);
    else {
        auto input = legal_input_paths(sources);
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
            if (input.size() > 1)
                throw ExcepCmdline{"htf_cmdline_parse(..)",
                                   "option" + mark("input") + "have too much values"};
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