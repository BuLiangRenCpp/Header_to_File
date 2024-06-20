#pragma once
#include "Token.h"
#include "Lexer_kind.h"

namespace htf {
    namespace lex {
        // Lex's token
        struct Lexer {
            char kind;
            std::string val;

            Lexer();
            // 不能用于 type
            Lexer(const stream::Token&);
            // 调用者自行决定类型
            Lexer(char, const stream::Token&);

            bool empty() const;
        };
        
        bool operator==(const Lexer&, const Lexer&);
        bool operator!=(const Lexer&, const Lexer&);
    }
}