#pragma once
#include "Token_kind.h"

namespace htf {
    namespace lex {
        namespace Lexer_kind {
            constexpr char CLASS_KIND = 'c';     // 'struct' or 'class'
            constexpr char TYPE_KIND = 't';     // modifier or basic_type
            constexpr char IDENTIFIER_KIND = stream::Token_kind::IDENTIFIER_KIND;
            constexpr char LITERAL_KIND = stream::Token_kind::LITERAL_KIND;
            constexpr char DCHAR_KIND = stream::Token_kind::DCHAR_KIND;
            constexpr char NULL_KIND = stream::Token_kind::NULL_KIND;

            const std::unordered_set<char> Kinds{CLASS_KIND, TYPE_KIND, IDENTIFIER_KIND, 
                LITERAL_KIND, DCHAR_KIND, NULL_KIND};
        };
    }
}