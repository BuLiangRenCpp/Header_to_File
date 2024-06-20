#pragma once
#include "Arg.h"


// * 指令格式： htf [-f] [-i] file1.h file2.h ... [-o dir1 [-I include1 inlcude2 ...]] (类似 g++) 
// * -i 多文件 在 -o 目录下输出  -I 指明 include目录
// * 注： -o 只能指明一个输出目录 
// *      -i 可以省略 (除了初始化外，NULL_ARG 看做 -i)
namespace htf {
    namespace argument{
        class Htf_args {
        public:
            // * NULL_ARG (不看做 -i)
            Htf_args();
            // * 此函数初始化对于 NULL_ARG 视为 -i
            Htf_args(const std::vector<Arg>&);
            std::vector<Arg> args() const;
        private:
            std::vector<Arg> _args;

            void _legalize();
        };

        std::istream& operator>>(std::istream&, Htf_args&);
    }
}