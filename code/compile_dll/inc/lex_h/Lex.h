#pragma once
#ifdef _WIN32
#define LIB_EXPORT
#endif

#include "dll.h"
#include "Lexer.h"
#include "Hpath.h"
#include "Token_stream.h"


/**
 * * Lex 对 只是 include 的文件进行 读取 自定义类型，而对于 source 则是利用之前的 
 * *     include 的信息输出 Lexer 供 Parse 使用
 * ! 1. 只返回对 parse 由于的 Lexer (函数定义语句相关的部分)
 * ! 2. 忽略 函数参数的默认值 
 * ! 3. 作用域后的标识符统一认为是 type，因此如果是方法，那么可能会报错 (这种情况一般只会出现在 {} 中，
 * !        此时是定义语句，直接忽略，故不影响程序正确执行)
 * ! 4. 对于模版直接忽略 (因为 模板一般直接定义在头文件)
 * ! 5. * inline 直接忽略
*/

namespace htf {
    namespace lex {
        class DLL_API Lex {
        public:
            Lex() = delete;
            // hpath 用来标明 is 的文件路径，用于 throw Excep_syntax
            Lex(std::istream& is, const path_deal::Hpath& hpath, const std::vector<std::string>& basic_types = {}, const std::vector<std::string>& containers = {});

			bool eof();
            // 默认放入 ';'
			void putback(const Lexer& lexer = Lexer{ stream::Token{';'} });
            // 默认忽略一条语句
			void ignore(const Lexer& lexer = Lexer{ stream::Token{ ';' } });
            void ignore_between_bracket();
			// 返回 empty Token 说明没有内容了
			Lexer get();
			Lexer peek();
            line_t line() const;

            std::vector<std::string> basic_types() const;
            std::vector<std::string> containers() const;
            path_deal::Hpath hpath() const;

        private:
            stream::Buffer<Lexer> _buffer;
            stream::Token_stream _ts;
            path_deal::Hpath _hpath;
            std::vector<std::string> _basic_types;
            std::vector<std::string> _containers;
            
            bool _is_basic_type(const stream::Token&) const;
            bool _is_container(const stream::Token&) const;
            bool _is_type(const stream::Token&) const;
            // 默认读取的是 type 之前的 namespace(即 A::A::)
            Lexer _get_namespace(bool is_type = true);
            Lexer _get_basic_type();
            Lexer _get_container();
            // 处理 'typedef B A;'  and 'using A = B;' (using namespace A; 直接忽略)
            void _deal_define_type();       
            // 处理模版：
            // 模板函数：直接忽略
            // 模板类：除了将 自定义类 加入 _containers，其余直接忽略
            // ! 如果是函数定义语句，会造成错误，因为忽略的是 { } 之间
            void _deal_template();
            // 处理 class (struct) [1 xxx] A [2 : xxx] [3 {}];  忽略 1 2
            // 默认模板类
            void _deal_class(bool is_template = true);
            stream::Token _get_operator();
        };
    }
}

