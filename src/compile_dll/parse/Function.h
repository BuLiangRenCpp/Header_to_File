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

            // ! 表示的是'函数为空'，__var 为内部变量，不参与判断
            // * 因此当返回 true 时，可能表示是 定义变量语句
            bool empty() const;

            // 是否是定义变量语句
            bool is_define_var() const;

            // * 是否读取失败 (不包括读取 定义变量语句)
            // * 失败：函数定义、A() = delete; 、错误格式
            bool fail() const;  
            
            std::string var() const;

            // 没有则返回空
            // is_class 默认为 false: 表示类外函数
            // * 定义函数语句直接忽略，返回 empty 
            // * 定义变量语句保留变量名
            Function get(lex::Lex&, bool is_class = false);

            // 返回定义语句  -->  如：int add(int) { }
            // count 控制缩进，"\t"的个数
            std::string str(unsigned int count = 0) const;

            // 类的成员函数，参数为类名、成员变量
            // count 控制缩进，"\t"的个数
            std::string str_class(const stream::Identifier& class_name, unsigned int count = 0, const std::vector<std::string>& class_vars = {}) const;
            
        private:
            Type _type;
            Fun_name _name;
            std::vector<Fun_arg> _args;
            bool _is_const;       // 参数列表后是否有 const
            bool _is_destructor;     // 是否是析构函数

            // * 定义语句变量名 (用于类的成员成员变量) 由内部自己管理
            std::string __var;   

            // 用于内部：定义变量语句
            Function(const std::string& var);

            // 是否是构造函数
            bool _is_constructor() const;

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
            // 当遇到 函数定义语句 || A() = delete; 时应做的处理
            void _clear_define_fun(lex::Lex&);
        };
    }
}