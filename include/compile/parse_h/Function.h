#pragma once
#include "Fun_arg.h"
#include "Fun_name.h"
#include "Lex.h"

namespace htf {
    namespace parse {
        // * 格式满足：[修饰词] + Type + Identifier + "(" + [Args] + ")" + [const] + ";" 
        // * 包括 一般函数、重载运算符函数、成员常量函数、特殊函数(构造函数、析构函数等)
        class Function {
        public:
            // void();
            Function();
            Function(const Type&, const Fun_name&, const std::vector<Fun_arg>&, bool is_const = false, bool is_construct = false);

            bool empty() const;

            // * 是否读取失败 (不包括读取定义语句而返回空)
            bool fail() const;  

            // 没有则返回空
            // is_class 默认为 false: 表示类外函数
            // * 定义语句，定义变量语句 直接忽略，返回 empty
            Function get(lex::Lex&, bool is_class = false);

            // 返回定义语句  -->  如：int add(int) { }
            // count 控制缩进，"\t"的个数
            std::string str(unsigned int count = 0) const;

            // 类的成员函数，参数为类名
            // count 控制缩进，"\t"的个数
            std::string str_class(const stream::Identifier& class_name, unsigned int count = 0) const;
            
        private:
            Type _type;
            Fun_name _name;
            std::vector<Fun_arg> _args;
            bool _is_const;       // 参数列表后是否有 const
            bool _is_construct;     // 是否是析构函数

            // ! 由于get 会忽略定义语句，返回空，导致无法判断是否是因为读取了错误的函数而导致的空，因此加入 _is_define
            bool _is_define;        

            // 类内函数
            Function _get_class_fun(lex::Lex&);
            // 类外函数
            // 应为类内函数与类外函数有相似之处，避免重复代码，加入 is_class 参数 
            // * is_class = true 说明可以为常量函数
            Function _get_fun(lex::Lex&, bool is_class = false);
            // 无名函数 (比如 构造函数)
            // 返回是否成功修改
            bool _get_noname_fun(lex::Lex&);
            // 读取参数列表，应以 '(' 开头, ')' 结尾
            bool _get_args(lex::Lex&);
            // 返回 _type + _name，用于报错
            std::string _type_name() const;
            // 返回 '( _args )'，用于报错
            std::string _ret_args() const;
            // 返回声明语句 (不包含 ';')
            std::string _str() const;       
            void _clear();
        };
    }
}