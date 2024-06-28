#pragma once
#include "Hpath.h"

/**
 * * 增加预处理器：
 *      1. #include: 处理依赖关系
 * *    2. #define A xx: 目前选择在 lex 中将 A 直接忽略，因为正确处理过于复杂 (有许多预定义宏无法处理)
 */

namespace htf{
    namespace pre {
        // return Reliance and 
        Reliance preprocess(const std::vector<path_deal::Hpath>& source, std::vector<path_deal::Hdir> include = {});
    }
}