#pragma once

/**
 * * [规范]：
 * *    - assertion: 仅用于 dev 
 * *    - ExcepBase: 用于 user
 * *    - debug: 仅 debug 时有效
 * 此文件包含了一些断言：
 *  - assert_*：用于测试条件是否为 true，true 则通过
 *  - expect_*：用于测试 * 是否为 true
 */

#include <iostream>
#include <sstream>

namespace htf
{

// * base
#define _HTF_INFO __FILE__ << ":" << __LINE__

#define HTF_FAILED_MESSAGE(msg)              \
    do {                                     \
        std::ostringstream oss;              \
        oss << msg;                          \
        std::cerr << oss.str() << std::endl; \
        exit(1);                             \
    } while (0)

#define _DEV_PRE_INFO "DEV-ERROR: " << _HTF_INFO

// condition
// - true:  passed
// - false: failed
#define HTF_DEV_ASSERT(condition)                                     \
    do {                                                              \
        if (!(condition)) {                                           \
            HTF_FAILED_MESSAGE(_DEV_PRE_INFO << ": "                  \
                                             << "HTF_ASSERT failed"); \
        }                                                             \
    } while (0)

// condition
// - true:  passed
// - false: failed
#define HTF_DEV_ASSERT_MESSAGE(condition, msg)                \
    do {                                                      \
        if (!(condition)) {                                   \
            HTF_FAILED_MESSAGE(_DEV_PRE_INFO << ": " << msg); \
        }                                                     \
    } while (0)

}   // namespace htf
