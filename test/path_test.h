#pragma once

#include "test.h"
#include "../src/path/Cfile.h"
#include "../src/path/path_regex.h"

namespace htf
{
namespace test
{
namespace path_test
{

using namespace htf::path;

// TEST(Dir_test)
// {
//     std::string dir1 = R"(D:\code\git\Header_to_File\test)";
//     std::string dir2 = R"(D:\code\git\Header_to_File)";
//     std::string path = R"(D:\code\git\Header_to_File\test\path_test.h)";
//     Dir d1{ dir1 };
//     EXPECT_FALSE(d1.have_file("test"));
//     EXPECT_FALSE(d1.have_file("txt"));
//     EXPECT_FALSE(d1.have_file("Cmakelists"));
// #if defined(_WIN32)
//     EXPECT_TRUE(d1.have_file("cmakelists.txt"));    // windows 忽略大小写
// #else 
//     EXPECT_FALSE(d1.have_file("cmakelists.txt"));
// #endif
//     EXPECT_TRUE(d1.have_file("path_test.h"));
//     EXPECT_TRUE(d1.have_file("Cmakelists.txt"));
//     EXPECT_NE(dir2, d1.str());

//     Dir d2{ dir2 };
//     // EXPECT_TRUE(d1 != d2);
//     // EXPECT_FALSE(d1 == d2);
//     // EXPECT_EQ(d2.str(), d1.parent());

//     // d2 = d2 / "test" / "tmp";
//     // EXPECT_FALSE(d1 == d2);
    
//     TRY(Dir d0{"test"}, ExcepBase);
//     TRY(Dir d0{R"(D:\code\git\Header_to_File\test\path_test.h)"}, ExcepBase);
//     TRY(d2 / "test", ExcepBase);

//     // std::string dir11 = R"(D:\code\git\Header_to_File\test\)";  // 末尾多了 '\'
//     // Dir d11{ dir11 };
//     // EXPECT_FALSE(d1 == d11);   // ! 底层是 std::filesystem::path，它的内部实现判断为 false
// }

// TEST(Cfile_test)
// {
//     srand(time(0));
//     const std::vector<std::string> extensions{".h", ".hpp", ".h++", ".inl"
//         , ".cpp", ".cxx", ".c++", ".c", ".cc"};  
//     auto size = extensions.size();
//     int index = random_int(0, size + size / 2);
//     std::string extension = (index >= size) ? random_str() : extensions[index];
//     std::string path1 = R"(D:\code\git\Header_to_File\test\test.h)";

//     Cfile f1{path1};
//     EXPECT_EQ("test.h", f1.filename());
//     EXPECT_EQ("test", f1.filename(false));
//     TRY(f1.change_extension(extension), ExcepBase); 

//     std::string path2 = R"(D:\code\git\Header_to_File\test\test.cpp)";
//     Cfile f2{path2};
//     f1.change_extension();
//     EXPECT_EQ(f1, f2);

//     std::string path3 = R"(D:\code\git\Header_to_File\test\tmp\test.cpp)";
//     Cfile f3{path3};
//     f3.change_extension(".h++");
//     EXPECT_EQ(Cfile{R"(D:\code\git\Header_to_File\test\tmp\test.h++)"}, f3);
// }

TEST(path_regex_test)
{
    // EXPECT_TRUE(is_match_path("" , ""));
    // EXPECT_TRUE(is_match_path("a", "a"));
    // EXPECT_TRUE(is_match_path("a/bc\\", "a/bc\\"));
    // EXPECT_TRUE(is_match_path("aaa", "a*"));
    // EXPECT_TRUE(is_match_path("a", "a*"));
    // EXPECT_TRUE(is_match_path("", "*"));
    // EXPECT_TRUE(is_match_path("c", "?"));
    // EXPECT_TRUE(is_match_path("aa", "*?"));
    // EXPECT_TRUE(is_match_path("aa", "?*"));
    // EXPECT_TRUE(is_match_path("a", "?*"));
    // EXPECT_TRUE(is_match_path("a", "*?"));
    // EXPECT_TRUE(is_match_path("abcd", "a??d"));
    // EXPECT_TRUE(is_match_path("dir\\file.h", "dir\\*.h"));
    // EXPECT_TRUE(is_match_path("dir/file.cpp", "*/*.cpp"));
    // EXPECT_TRUE(is_match_path("dir\\file.cpp", "*\\*.cpp"));

    
    // EXPECT_FALSE(is_match_path("/", "?"));
    // EXPECT_FALSE(is_match_path("\\", "?"));
    // EXPECT_FALSE(is_match_path("a/b\\", "*?"));
    // EXPECT_FALSE(is_match_path("dir/file.h", "*.h"));
    // EXPECT_FALSE(is_match_path("a/b\\c/file.cpp", "*"));
    // EXPECT_FALSE(is_match_path("dir\\file.cpp", "*/*.cpp"));
    // EXPECT_FALSE(is_match_path("dir/file.cpp", "*\\*.cpp"));

    // ------------------------------------------------
    auto res = path_regex("../*/", true);
    for (auto it : res) std::cout << it << std::endl;
}

}//
}//
}//