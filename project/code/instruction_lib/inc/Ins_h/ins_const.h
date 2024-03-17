#pragma once
#include "header.h"

namespace ins_const {
    namespace arg {
        constexpr char ARG_STA_CH = '-';		// 参数开始符号
        
        constexpr char NULL_ARG = '`';          // 空参数
        // ------------------------- 有参参数hpar(have par) ---------------------------------
        constexpr char INPUT_ARG = 'i';		// 指定读取文件目录或路径参数
        constexpr char OUTPUT_ARG = 'o';		// 指定输出文件目录或路径参数
        // ------------------------- 无参参数npar(no par) ---------------------------------
        constexpr char FORCE_ARG = 'f';        

        const std::set<char> Hargs{ INPUT_ARG, OUTPUT_ARG };
        const std::set<char> Nargs{ FORCE_ARG };
        const std::set<char> Args{ NULL_ARG, INPUT_ARG, OUTPUT_ARG, FORCE_ARG };   
    } 

    namespace ins {
        const std::string LIST_DEFAULT_INS = "ld";
        const std::string HTF_INS = "htf";
        const std::string RUN_END_INS = "q";
        
        const std::set<std::string> Single_Ins{ LIST_DEFAULT_INS, RUN_END_INS };
        const std::set<std::string> Inss{ LIST_DEFAULT_INS, HTF_INS, RUN_END_INS };

        // {关键字, {参数限制个数, 字符参数}}
        const std::unordered_map<std::string, std::pair<int, std::set<char>>> Ins_to_Args {
            {LIST_DEFAULT_INS, {0, {}}},
            {HTF_INS, {3, {arg::NULL_ARG, arg::INPUT_ARG, arg::OUTPUT_ARG, arg::FORCE_ARG}}},
            {RUN_END_INS, {0, {}}}
        };
    }
}