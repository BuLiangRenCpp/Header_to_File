#pragma once

// 此文件包含一些重命名的 type、namespace，以及一些基本的辅助函数

#include <filesystem>
#include <fstream>
#include <iostream>

namespace htf
{

using size_t = unsigned long long;
using line_t = unsigned long long;
using col_t  = unsigned int;   // column

// 后期可能会扩展
using Ifstream = std::ifstream;
using Ofstream = std::ofstream;

namespace FS = std::filesystem;

}   // namespace htf