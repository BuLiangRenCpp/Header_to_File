#pragma once
#include "Token_kind.h"

namespace htf {
    namespace lex {
        namespace Lexer_kind {
            constexpr char CLASS_KIND = 'c';     // 'struct' or 'class'
            constexpr char TYPE_KIND = 't';     // modifier or basic_type

            // ! ************************************************************
            // * 目前仅仅为了区别 namespace FS = A; 与 namespace FS {}
            // ! 判断方式为： (仅在 Lex.get() 中判断，然后使用 return Lexer{ NAMESPACE_KIND, str } )
            // *    1. 最后一个 :: 之后的标识符不是类型时，认为是 namespace 
            // *    2. namespace 之后的 标识符
            // ! 不采用特别正确的判断方式，因为目前没必要
            constexpr char NAMESPACE_KIND = 'n';        // namespace 
            // ! ************************************************************

            constexpr char IDENTIFIER_KIND = stream::Token_kind::IDENTIFIER_KIND;
            constexpr char LITERAL_KIND = stream::Token_kind::LITERAL_KIND;
            constexpr char DCHAR_KIND = stream::Token_kind::DCHAR_KIND;
            constexpr char NULL_KIND = stream::Token_kind::NULL_KIND;

            const std::unordered_set<char> Kinds{CLASS_KIND, TYPE_KIND, IDENTIFIER_KIND, 
                LITERAL_KIND, DCHAR_KIND, NULL_KIND};
        };
    }
}