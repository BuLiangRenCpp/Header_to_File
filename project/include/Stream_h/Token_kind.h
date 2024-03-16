#pragma once

#include "usage.h"


// ----------------------------- kind --> KIND --------------------------------------
// 注： 1. 优先级从上到下依次减弱，一个 Token的kind 应为满足的并且优先级最高的 KIND
//     2. 特殊单字符（非字母、数字)的 kind 就是他本身，其余的 kind 值必须设为 字母 或者 数字
namespace Token_kind{
    constexpr char CLASS_KEY_KIND = 'c';			// 类关键字 类型  -->  struct、class
    constexpr char TYPE_KIND = 't';		// 类型 类型  -->  包括基本类型、容器类型、类型的修饰词
    constexpr char IDENTIFIER_KIND = 'i';		// 标识符 类型

    constexpr char NULL_KIND = '`';		// 空 类型

    const set<char> Kinds{CLASS_KEY_KIND, TYPE_KIND, IDENTIFIER_KIND, NULL_KIND};

    // 返回字符串对应的kind
    // ** 注：特殊字符的kind就是它本身，不用写此函数
    // ** 如果不是其他优先级高的类型，则返回 NULL_KIND
    char ret_kind(const string&);

}
