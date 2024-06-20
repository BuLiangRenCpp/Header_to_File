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
            // 是否有函数，不管 __vars (成员变量)
            bool empty() const;

        private:
            stream::Identifier _name;		// 类名
            std::vector<std::vector<Function>> _funs;		// first->pub   second->pri

            std::vector<std::string> __vars;     // 成员变量

            std::vector<Function> _get_function(lex::Lex& lex);
            // is_class: 是否是 'class' (false -> struct)
            void _get_functions(lex::Lex& lex, bool is_class);
            void _clear();
        };
    }
}