#pragma once
#include "header.h"

// tool's name + args --> -i xx -o xx -f

/**
 * * 预计增加 'htf file.htf' 的指令，在 file.htf 中写 htf语句，htf取解释运行
*/

namespace htf {
    namespace argument{
        namespace args_const {
            constexpr int MAX_ARG_COUNT = 4; 

            constexpr char ARG_STA_CH = '-';		// 参数开始符号
            
            constexpr char NULL_ARG = '`';          // * 空参数 
            // ------------------------- 有参参数hpar(have par) ---------------------------------
            constexpr char INPUT_ARG = 'i';		// 指定读取文件路径
            constexpr char OUTPUT_ARG = 'o';		// 指定输出文件目录 
            constexpr char INCLUDE_ARG = 'I';       // 指定 inlcude 目录
            // ------------------------- 无参参数npar(no par) ---------------------------------
            constexpr char FORCE_ARG = 'f';        

            const std::unordered_set<char> Hargs{ INPUT_ARG, OUTPUT_ARG, INCLUDE_ARG };
            const std::unordered_set<char> Nargs{ FORCE_ARG };
            // * 包括了 空参数
            const std::unordered_set<char> Args{ NULL_ARG, INPUT_ARG, OUTPUT_ARG, INCLUDE_ARG, FORCE_ARG };   
 
            // harg 所能跟参数的最大个数
            const std::unordered_map<char, int> Hargs_Cnt{
                {INPUT_ARG, INT_MAX},
                {OUTPUT_ARG, 1},
                {INCLUDE_ARG, INT_MAX},
            };
        }
    }
}