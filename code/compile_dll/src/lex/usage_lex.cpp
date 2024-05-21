#include "usage_lex.h"
#include "usage.h"
#include "Lexer_kind.h"

using namespace std;

/**
 * ! 目前方案采用只判断是否可以是类型的一部分，语法正确性交给编译器
 * * [例] unsigned long long 可以是 'unsigned' 'long' 组合而成
*/
const unordered_set<string> Basic_Type_Keys{ "void", "bool", "char", "short", "int", "long", 
"float", "double", "string", "istream", "ostream", "ifstream", "ofstream", "exception" };        // 基本类型关键字 
const unordered_set<string> Container_Type_Keys{ "vector", "list", "deque", "array", "forward_list", "set", "multiset",
    "map", "multimap", "unordered_set", "unordered_multiset", "unordered_map", "unordered_multimap",
    "stack", "queue", "priority_queue", "pair" };
const unordered_set<string> Modifier_Type_Keys{ "unsigned", "signed", "static" };     // 类型修饰词

const unordered_set<string> Class_Keys{ "class", "struct" };       // 自定义类关键字

namespace htf {
    namespace lex {
        namespace usage {
            bool is_kind(char c)
            {
                return Lexer_kind::Kinds.count(c);
            }
            
            bool is_lbracket(const char& c)
            {
                return stream::usage::is_lbracket(c);
            }

            bool is_rbracket(const char& c)
            {
                return stream::usage::is_rbracket(c);
            }

            bool is_bracket(char c)
            {
                return stream::usage::is_bracket(c);
            }

            char ret_lbracket(char c)
            {
                return stream::usage::ret_lbracket(c);
            }

            char ret_rbracket(char c)
            {
                return stream::usage::ret_rbracket(c);
            }

            void delete_space_pre_suf(string& s)
            {
                stream::usage::delete_space_pre_suf(s);
            }

            bool is_dchar(const string& s)
            {
                return stream::usage::is_dchar(s);
            }

            bool is_identifier(const string& temp)
            {
                return stream::usage::is_identifier(temp);
            }

            bool is_class_key(const string& s)
            {
                return Class_Keys.count(s);
            }

            bool is_basic_type_key(const string& s) 
            {
                return Basic_Type_Keys.count(s) || Modifier_Type_Keys.count(s);
            }

            bool is_container_key(const string& s)
            {
                return Container_Type_Keys.count(s);
            }

            char ret_kind(const string& s) 
            {
                if (is_class_key(s)) return Lexer_kind::CLASS_KIND;
                if (is_identifier(s)) return Lexer_kind::IDENTIFIER_KIND;
                if (is_dchar(s)) return Lexer_kind::DCHAR_KIND; 
                return Lexer_kind::LITERAL_KIND;
            }
        }
    }
}