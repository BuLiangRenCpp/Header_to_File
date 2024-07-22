#pragma once
#ifdef _WIN32
#    define LIB_EXPORT
#endif

#include "Hpath.h"
#include "dll.h"

/**
 * * [方案]：根据预处理指令，采用 图结构 (拓扑排序) 处理文件依赖顺序，返回此顺序
 */

namespace htf
{
// * 返回自定义类型
/**
 * htf -i x 中的 x 为 sourse，而在sourse中被include的头文件，但是不存在x中的为include
 */
namespace core
{
// * 处理预处理指令：
// 多文件预处理 (避免不必要的处理，如可能有的文件被处理多次)
// * 1. #include: 返回每个独立依赖关系 (按顺序)，同时独立关系中的每个元素 first->是否是source，second->路径
DLL_API Reliance preprocess(const std::vector<path_deal::Hpath>& source, std::vector<path_deal::Hdir> include = {});
}   // namespace core
}   // namespace htf