#pragma once

// 此文件包含一些重命名的 type、namespace，以及一些基本的辅助函数

#include <filesystem>
#include <fstream>
#include <iostream>

// #include "Ifstream.h"

namespace htf
{

using size_t = unsigned long long;
using line_t = unsigned long long;
using col_t  = unsigned int;   // column

// using Ifstream = htf::stream::Ifstream;      // ! 存在诸多 bug 暂不使用
using Ifstream = std::ifstream;
using Ofstream = std::ofstream;

namespace FS = std::filesystem;

}   // namespace htf