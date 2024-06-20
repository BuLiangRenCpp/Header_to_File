#pragma once

#include "usage.h"


namespace htf {
    namespace stream {
        // 基础类型
        namespace Token_kind{
            constexpr char DCHAR_KIND = 'd';        // 双字符 类型  (这里主要是 :: )
            constexpr char IDENTIFIER_KIND = 'i';		// 标识符 类型
            constexpr char LITERAL_KIND = 'l';      // 字面值类型

            constexpr char NULL_KIND = '`';		// 空 类型

            const std::unordered_set<char> Kinds{DCHAR_KIND, IDENTIFIER_KIND, LITERAL_KIND, NULL_KIND};
        }
    }
}
