#pragma once

#include "test.h"
#include "../src/path/Path.h"
#include "../src/path/path_regex.h"

namespace htf
{
namespace test
{
namespace path_test
{

using namespace htf::path;


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
    // for (auto it : res) std::cout << it << std::endl;
}

}//
}//
}//