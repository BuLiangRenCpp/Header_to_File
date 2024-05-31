#pragma once
#ifdef _WIN32
#define LIB_EXPORT
#endif

#include "dll.h"
#include "header.h"
#include "Hpath.h"


namespace htf {
    namespace core {
        // is_force: 是否提示 "是否要覆盖文件"
        DLL_API void compile(const std::vector<std::vector<std::pair<bool, path_deal::Hpath>>>& reliances, 
            const path_deal::Hdir& output_dir, bool is_force = false);
    }
}