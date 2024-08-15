#pragma once

/**
 * * htf 指令:
 * * - 多文件: -i file1 file2.. | -o dir | -I dir1 dir2.. |  -f
 * * - 单文件: -i file | -o file | -I dir1 dir2.. | -f
 * * - PreProcess:  -i file | -o file | -I dir1 dir2 | -E
 * * - --help (-h)
 * * - --version (-v)
 */

#include "cmdline/Parse.h"

namespace htf
{
namespace cmdline
{

void htf_cmdline_parse(int argc, char* argv[]);

}   // namespace cmdline
}   // namespace htf