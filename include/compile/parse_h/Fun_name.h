#pragma once
#include "Identifier.h"

namespace htf {
    namespace parse {
        // * 函数名，包括一般函数名、特殊函数名（重载运算符函数名 opretor>> 等)
        class Fun_name{
        public:
            Fun_name();
            // 一般函数名一般使用此初始化
            Fun_name(const stream::Identifier&);
            // 特殊函数名一般使用此初始化
            Fun_name(const std::string&);		

            std::string str() const;
            bool empty() const;

        private:
            std::string _name = "";
        };
    }
}