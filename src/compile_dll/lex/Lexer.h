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
            // 此函数保留参数token的kind
            // 故不能用于 type 初始化
            Lexer(const stream::Token&);
            // 调用者自行决定类型
            Lexer(char, const stream::Token&);

            bool empty() const;
        };
        
        bool operator==(const Lexer&, const Lexer&);
        bool operator!=(const Lexer&, const Lexer&);
    }
}