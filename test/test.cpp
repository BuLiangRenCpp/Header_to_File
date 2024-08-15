#include "ExcepBase.h"

// #define TEST_TIME_ON    // 开启性能测试
// #define TEST_TIME_LEVEL2    // 开启性能测试级别2
// #define TEST_EXIT_ON    // 遇见测试失败就结束

// #include "stream_old_test.h"
// #include "path_test.h"
// #include "stream_test.h"
#include "compiler_test.h"
// #include "cmdline_test.h"

using namespace std;

int main()
try{
    using namespace htf::test;
    RUN_ALL_TESTS();
    return 0;
}
catch(const std::exception& e)
{
    cerr << e.what() << endl;
    return 1;
}
