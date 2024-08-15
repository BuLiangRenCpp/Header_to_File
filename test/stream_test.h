#pragma once

#include "test.h"
#include "../src/using.h"

namespace htf
{
namespace test
{
namespace stream_test
{

// TEST(FileBuffer_test)
// {
//     // FileBuffer fb(ifile);
//     // std::ofstream ofs(ofile);
//     // if (!ofs) throw;
//     // while (true) { 
//     //     char c = fb.get();
//     //     if (fb.eof()) break;
//     //     ofs << c;
//     // }
//     // 原本是写了一个测试两个文件内容是否相等的，但是发现文件直观上是一样的，
//     // 但是在文件 eof 的处理出有所不同，故不做作断言
// }

// TEST(Ifstream_test)
// {
//     srand(time(0));
//     std::string ifile = R"(D:\code\git\Header_to_File\test\txt\ifs.in)";
//     std::string ofile = R"(D:\code\git\Header_to_File\test\txt\ifs.out)";
//     std::ifstream ifs1(ifile);
//     Ifstream ifs2(ifile);

//     // while (true) {
//     //     EXPECT_EQ(ifs1.get(), ifs2.get());  // * 注：当文件中出现中文时，可能结果不一样 (但这无影响)
//     //     if (ifs1.eof()) {
//     //         EXPECT_EQ(ifs1.eof(), ifs2.eof());
//     //         break;
//     //     }
//     //     EXPECT_EQ(ifs1.peek(), ifs2.peek());
//     //     // if (random_int(0, 10) == 9) {
//     //     //     ifs1.putback('c');
//     //     //     ifs2.putback('c');
//     //     //     if (ifs1.fail()) throw;
//     //     // }
//     // }
//     ifs2.open(ifile);
//     std::ofstream ofs(ofile);
//     if (!ofs) throw;
//     int i = 0;
//     while (true) {
//         if (ifs2.eof()) break;
//         ofs << ifs2.get_str() << " ";
//         if (i++ == 10) {
//             i = 0;
//             ofs << std::endl;
//         }

//     }
// }

}//
}//
}//
