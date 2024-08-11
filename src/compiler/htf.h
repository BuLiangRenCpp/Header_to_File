#pragma once

// 核心功能

#include "Parse.h"

namespace htf
{

// 单文件处理
void header_to_file(path::Cfile source, const std::string& output_path,
                    const std::vector<path::Dir>& include_dirs = {}, bool is_force = false);

// 多文件处理
void header_to_file(const std::set<std::string>& sources, const path::Dir& output_dir,
                    const std::vector<path::Dir>& include_dirs = {}, bool is_force = false);

}   // namespace htf