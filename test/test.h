#pragma once

// 目前仅用于 API 测试

#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#include "color.h"

namespace htf
{
namespace test
{

// 辅助函数


class TestCase
{
public:
    TestCase(const std::string& n)
        : testcase_name{n}, result{true}, passed_num{0}, failed_num{0} {}

    virtual void run() = 0;

public:
    const std::string testcase_name;
    bool              result;
    double            passed_num;
    double            failed_num;
};

class UnitTest
{
public:
    TestCase* cur_testcase = nullptr;
    double    passed_num   = 0.0;
    double    failed_num   = 0.0;

protected:
    std::vector<TestCase*> _testcases = {};

private:
    UnitTest()                           = default;
    UnitTest(const UnitTest&)            = delete;
    UnitTest& operator=(const UnitTest&) = delete;
    UnitTest(UnitTest&&)                 = delete;
    UnitTest& operator=(UnitTest&&)      = delete;

public:
    static UnitTest* get_instance();
    TestCase*        add_testcase(TestCase*);
    void             run();
};

UnitTest* UnitTest::get_instance()
{
    static UnitTest instance;
    return &instance;
}

TestCase* UnitTest::add_testcase(TestCase* testcase)
{
    _testcases.push_back(testcase);
    return testcase;
}

void UnitTest::run()
{
    for (auto it : _testcases) {
        cur_testcase   = it;
        it->result     = true;
        it->failed_num = 0.0;
        it->passed_num = 0.0;
        std::cout << COLOR(GREEN, "===========================================") << std::endl;
        std::cout << "Run testcase: [ " << it->testcase_name << " ]" << std::endl;
        it->run();
        if (it->result == false) {
            std::cout << RED;
            ++failed_num;
        }
        else {
            std::cout << GREEN;
            ++passed_num;
        }
        auto total = it->failed_num + it->passed_num;
        std::cout << " " << it->passed_num << " / " << total << " passed. ("
                  << (it->passed_num / total) * 100 << "%)" << std::endl;
        std::cout << RESET << "End testcase: [ " << it->testcase_name << " ]" << std::endl;
    }
    std::cout << GREEN << "\n===========================================" << std::endl;
    std::cout << " Total testcases: " << failed_num + passed_num << std::endl;
    std::cout << " Passed testcases: " << passed_num << std::endl;
    std::cout << RED << " Failed testcases: " << failed_num << std::endl;
    if (failed_num == 0)
        std::cout << GREEN << " UnitTest result: Passed" << std::endl;
    else
        std::cout << RED << " UnitTest result: Failed" << std::endl;
    std::cout << RESET;
}

/*********************************************************************************************/

#define TESTCASE_NAME(_testcase_name) \
    _testcase_name##_TEST

#define _MYSTL_TEST(_testcase_name)                               \
    class TESTCASE_NAME(_testcase_name) : public TestCase         \
    {                                                             \
    public:                                                       \
        TESTCASE_NAME(_testcase_name)                             \
        (const std::string& n)                                    \
            : TestCase{n} {}                                      \
        virtual void run();                                       \
                                                                  \
    private:                                                      \
        static TestCase* const _testcase;                         \
    };                                                            \
                                                                  \
    TestCase* const TESTCASE_NAME(_testcase_name)                 \
        ::_testcase = UnitTest::get_instance()->add_testcase(     \
            new TESTCASE_NAME(_testcase_name)(#_testcase_name));  \
    void TESTCASE_NAME(_testcase_name)::run()


/**************************************************************************/

#if defined(TEST_EXIT_ON)   
#define TEST_EXIT exit(1)   // 遇见测试失败就结束
#else 
#define TEST_EXIT   // 默认完成所有测试
#endif  

#define _INFO __FILE__ << ":" << __LINE__

#define SUF_INFO \
    " --> " << _INFO

#define EXPECT_TRUE(condition)                                                        \
    do {                                                                              \
        if (condition) {                                                              \
            ++(UnitTest::get_instance()->cur_testcase->passed_num);                   \
            std::cout << COLOR(GREEN, " EXPECT_TURE passed!") << std::endl;           \
        }                                                                             \
        else {                                                                        \
            UnitTest::get_instance()->cur_testcase->result = false;                   \
            ++(UnitTest::get_instance()->cur_testcase->failed_num);                   \
            std::cout << COLOR(RED, " EXPECT_TURE failed!") << SUF_INFO << std::endl; \
            TEST_EXIT;  \
        }                                                                             \
    } while (0);                                                                      \
    std::cout

#define EXPECT_FALSE(condition)                                                       \
    do {                                                                              \
        if (!(condition)) {                                                           \
            ++(UnitTest::get_instance()->cur_testcase->passed_num);                   \
            std::cout << COLOR(GREEN, " EXPECT_FALSE passed!") << std::endl;           \
        }                                                                             \
        else {                                                                        \
            UnitTest::get_instance()->cur_testcase->result = false;                   \
            ++(UnitTest::get_instance()->cur_testcase->failed_num);                   \
            std::cout << COLOR(RED, " EXPECT_FALSE failed!") << SUF_INFO << std::endl; \
            TEST_EXIT;  \
        }                                                                             \
    } while (0);                                                                      \
    std::cout


#define EXPECT_EQ(expe, fact)                                                       \
    do {                                                                            \
        if (expe == fact) {                                                         \
            ++(UnitTest::get_instance()->cur_testcase->passed_num);                 \
            std::cout << COLOR(GREEN, " EXPECT_EQ passed!") << std::endl;           \
        }                                                                           \
        else {                                                                      \
            UnitTest::get_instance()->cur_testcase->result = false;                 \
            ++(UnitTest::get_instance()->cur_testcase->failed_num);                 \
            std::cout << COLOR(RED, " EXPECT_EQ failed!") << SUF_INFO << std::endl; \
            std::cout << COLOR(YELLOW, " Expect: " << expe) << std::endl;           \
            std::cout << COLOR(YELLOW, " Fact:   " << fact) << std::endl;           \
            TEST_EXIT;  \
        }                                                                           \
    } while (0);                                                                    \
    std::cout

#define EXPECT_NE(expe, fact)                                                       \
    do {                                                                            \
        if (expe != fact) {                                                         \
            ++(UnitTest::get_instance()->cur_testcase->passed_num);                 \
            std::cout << COLOR(GREEN, " EXPECT_NE passed!") << std::endl;             \
        }                                                                           \
        else {                                                                      \
            UnitTest::get_instance()                                                \
                ->cur_testcase->result = false;                                     \
            ++(UnitTest::get_instance()->cur_testcase->failed_num);                 \
            std::cout << COLOR(RED, " EXPECT_NE failed!") << SUF_INFO << std::endl; \
            std::cout << COLOR(YELLOW, " Expect: " << expe) << std::endl;           \
            std::cout << COLOR(YELLOW, " Fact:   " << fact) << std::endl;           \
            TEST_EXIT;  \
        }                                                                           \
    } while (0);                                                                    \
    std::cout

#define EXPECT_LT(expe, fact)                                                       \
    do {                                                                            \
        if (expe < fact) {                                                          \
            ++(UnitTest::get_instance()->cur_testcase->passed_num);                 \
            std::cout << COLOR(GREEN, " EXPECT_LT passed!") << std::endl;           \
        }                                                                           \
        else {                                                                      \
            UnitTest::get_instance()->cur_testcase->result = false;                 \
            ++(UnitTest::get_instance()->cur_testcase->failed_num);                 \
            std::cout << COLOR(RED, " EXPECT_LT failed!") << SUF_INFO << std::endl; \
            std::cout << COLOR(YELLOW, " Expect:" << expe) << std::endl;            \
            std::cout << COLOR(YELLOW, " Fact:" << fact) << std::endl;              \
            TEST_EXIT;  \
        }                                                                           \
    } while (0);                                                                    \
    std::cout

#define EXPECT_LE(expe, fact)                                                       \
    do {                                                                            \
        if (expe <= fact) {                                                         \
            ++(UnitTest::get_instance()->cur_testcase->passed_num);                 \
            std::cout << COLOR(GREEN, " EXPECT_LE passed!") << std::endl;           \
        }                                                                           \
        else {                                                                      \
            UnitTest::get_instance()->cur_testcase->result = false;                 \
            ++(UnitTest::get_instance()->cur_testcase->failed_num);                 \
            std::cout << COLOR(RED, " EXPECT_LE failed!") << SUF_INFO << std::endl; \
            std::cout << COLOR(YELLOW, " Expect:" << expe) << std::endl;            \
            std::cout << COLOR(YELLOW, " Fact:" << fact) << std::endl;              \
            TEST_EXIT;  \
        }                                                                           \
    } while (0);                                                                    \
    std::cout

#define EXPECT_GT(expe, fact)                                                       \
    do {                                                                            \
        if (expe > fact) {                                                          \
            ++(UnitTest::get_instance()->cur_testcase->passed_num);                 \
            std::cout << COLOR(GREEN, " EXPECT_GT passed!") << std::endl;           \
        }                                                                           \
        else {                                                                      \
            UnitTest::get_instance()->cur_testcase->result = false;                 \
            ++(UnitTest::get_instance()->cur_testcase->failed_num);                 \
            std::cout << COLOR(RED, " EXPECT_GT failed!") << SUF_INFO << std::endl; \
            std::cout << COLOR(YELLOW, " Expect:" << expe) << std::endl;            \
            std::cout << COLOR(YELLOW, " Fact:" << fact) << std::endl;              \
            TEST_EXIT;  \
        }                                                                           \
    } while (0);                                                                    \
    std::cout

#define EXPECT_GE(expe, fact)                                                       \
    do {                                                                            \
        if (expe >= fact) {                                                         \
            ++(UnitTest::get_instance()->cur_testcase->passed_num);                 \
            std::cout << COLOR(GREEN, " EXPECT_GE passed!") << std::endl;           \
        }                                                                           \
        else {                                                                      \
            UnitTest::get_instance()->cur_testcase->result = false;                 \
            ++(UnitTest::get_instance()->cur_testcase->failed_num);                 \
            std::cout << COLOR(RED, " EXPECT_GE failed!") << SUF_INFO << std::endl; \
            std::cout << COLOR(YELLOW, " Expect:" << expe) << std::endl;            \
            std::cout << COLOR(YELLOW, " Fact:" << fact) << std::endl;              \
            TEST_EXIT;  \
        }                                                                           \
    } while (0);                                                                    \
    std::cout

// try { statement } catch(const error& e) { std::cerr... }
// 用于测试一些不正确调用语句是否会抛出指定的异常
// - passed: throw
// - failed: no throw
#define TRY(statement, error_type) \
    try {\
        statement; \
        UnitTest::get_instance()->cur_testcase->result = false;                 \
        ++(UnitTest::get_instance()->cur_testcase->failed_num);                 \
        std::cout << COLOR(RED, " TRY failed!") << SUF_INFO << std::endl; \
    } \
    catch (const error_type& e) { \
        ++(UnitTest::get_instance()->cur_testcase->passed_num);                 \
        std::cout << COLOR(GREEN, " TRY passed!") << std::endl;           \
        std::cerr << _INFO << ":\n ~ " << e.what() << std::endl;  \
    } \
    catch (const std::string& e) { \
        std::cerr << _INFO << ":\n ~ " << e << std::endl; \
    }

// 效率测试

// 是否开启性能测试
#if defined(TEST_TIME_ON)
#define _TEST_TIME_ON 1 
#else 
#define _TEST_TIME_ON 0
#endif   // ! #if defined(TEST_TIME_ON)

// 测试级别：相当于函数运行多少次
#if defined(TEST_TIME_LEVEL1)
#define _TIME_LEVEL 10000
#elif defined(TEST_TIME_LEVEL2)
#define _TIME_LEVEL 100000
#elif defined(TEST_TIME_LEVEL3)
#define _TIME_LEVEL 1000000   
#else 
#define _TIME_LEVEL 10000
#endif   // ! #if defined(TIME_LEVEL1)

// time: double
#define _PRINT_TIME(fun, time, color) \
    " func: " << COLOR(color, #fun) << " average elapsed time:  " << COLOR(color, (time) << " ms") 

#define _TIME1(fun) \
    do { \
        std::cout << "*TIME1 test ..." << std::endl;    \
        auto start = std::chrono::high_resolution_clock::now(); \
        for (int i = 0; i < _TIME_LEVEL; i++) {\
            fun;    \
        }\
        auto end = std::chrono::high_resolution_clock::now(); \
        std::chrono::duration<double, std::milli> time = end - start;   \
        std::cout << " TIME1: " <<  _PRINT_TIME(fun, time.count() / _TIME_LEVEL, GREEN) << std::endl; \
    } while(0); \
    std::cout

// time: 接收耗时
#define _TIME(fun, time) \
    do { \
        auto start = std::chrono::high_resolution_clock::now(); \
        for (int i = 0; i < _TIME_LEVEL; i++) {\
            fun;    \
        }\
        auto end = std::chrono::high_resolution_clock::now(); \
        std::chrono::duration<double, std::milli> tmp = (end - start);   \
        time = tmp.count() / _TIME_LEVEL; \
    } while(0); \
    std::cout

// 对比两个函数耗时: 执行顺序随机
// expe: 性能较低的函数
// fact: 性能较高的函数
// 期待 expe.time > fact.time 
#define _TIME2(expe, fact) \
    do { \
        std::cout << "*TIME2 test ..." << std::endl;    \
        srand(time(0)); \
        bool flag = random_int(0, 2);   \
        double time1; \
        double time2;    \
        if (flag) { \
            _TIME(expe, time1);    \
            _TIME(fact, time2);    \
        } else {\
            _TIME(fact, time2);    \
            _TIME(expe, time1);    \
        }   \
        if (time1 - time2 > 1e-6) { \
            ++(UnitTest::get_instance()->cur_testcase->passed_num);  \
            std::cout << COLOR(GREEN, " TIME2 passed!") << std::endl; \
            std::cout << _PRINT_TIME(fact, time2, GREEN) << std::endl; \
            std::cout << _PRINT_TIME(expe, time1, RESET) << std::endl; \
        }   \
        else {  \
            UnitTest::get_instance()->cur_testcase->result = false;                 \
            ++(UnitTest::get_instance()->cur_testcase->failed_num);                 \
            std::cout << COLOR(RED, " TIME2 failed!") << SUF_INFO << std::endl; \
            std::cout << _PRINT_TIME(expe, time1, RESET) << std::endl;   \
            std::cout << _PRINT_TIME(fact, time2, RED) << std::endl;    \
            TEST_EXIT;  \
        }   \
    } while (0); \
    std::cout 

#if _TEST_TIME_ON
#define TIME1(fun) _TIME1(fun)
// - expe: 性能较低的函数
// - fact: 性能较高的函数
// 期待 expe.time > fact.time 
#define TIME2(expe, fact) _TIME2(expe, fact)
#else 
#define TIME1(fun)
// - expe: 性能较低的函数
// - fact: 性能较高的函数
// 期待 expe.time > fact.time 
#define TIME2(expe, fact)
#endif   // ! #if _TEST_TIME_ON

#define TEST(_testcase_name) \
    _MYSTL_TEST(_testcase_name)

// 运行所有测试
#define RUN_ALL_TESTS() \
    htf::test::UnitTest::get_instance()->run()

}   // namespace test
}   // namespace htf