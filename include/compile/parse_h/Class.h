#pragma once

#include "Function.h"

namespace htf {
    namespace parse {
        // 自定义类中的成员函数 (class or struct)
        class Class {
        public:
            Class();
            // (&pub, &pro, &pri)
            // 若 n 为空，则 pub、pro、pri 都必须为空
            Class(const stream::Identifier&, const std::vector<std::vector<Function>>&);

            Class get(lex::Lex&);

            // 返回成员函数的定义语句
            std::string str(unsigned int count = 0) const;
            bool empty() const;

        private:
            stream::Identifier _name;		// 类名
            std::vector<std::vector<Function>> _funs;		// first->pub   second->pri

            void _clear();
        };
    }
}