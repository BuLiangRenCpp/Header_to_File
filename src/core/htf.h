#pragma once

// 核心功能

#include "PreProcess.h"
#include "compiler/Parse.h"

namespace htf
{

// 单文件处理
void header_to_file(FS::path source, const FS::path& output_path,
                    const std::vector<path::Path>& include_dirs = {}, bool is_force = false);

// 多文件处理
void header_to_file(const std::set<FS::path>& sources, const FS::path& output_dir,
                    const std::vector<path::Path>& include_dirs = {}, bool is_force = false);

}   // namespace htf