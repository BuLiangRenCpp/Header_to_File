#pragma once

#include <fstream>

#include "test.h"
#include "../src/core/htf_cmdline.h"

namespace htf
{
namespace test
{
namespace cmdline_test
{

using namespace cmdline;

// TEST(OptionBase_test)
// {
//     std::vector<std::string> t{};
//     OptionBase* p1 = new OptionWithoutVal("noval", 'n', "desc", true);
//     OptionBase* p2 = new OptionWithVal<int>("val", 's', "desc", false, true, 100);
//     OptionBase* p3 = new OptionWithVal<std::string>("vals", 'm', "desc", false, false, t);

//     EXPECT_TRUE(!p1->has_value());
//     EXPECT_TRUE(p1->is_must());
//     EXPECT_TRUE(p2->has_value());
//     EXPECT_FALSE(p2->is_must());
//     EXPECT_TRUE(p2->is_default());
//     EXPECT_TRUE(p3->is_more_value());
//     EXPECT_FALSE(p3->is_must());
//     EXPECT_FALSE(p3->is_default());
//     EXPECT_TRUE(p3->default_val().empty());
//     EXPECT_TRUE(t == p3->default_vals());
//     EXPECT_TRUE("" == p2->val());   
//     EXPECT_TRUE(p2->empty());
//     EXPECT_TRUE(p2->vals().empty());  
//     EXPECT_TRUE(p3->vals().empty());
    
//     std::vector<std::string> tt{"tt"};
//     EXPECT_FALSE(p1->set_value(""));
//     EXPECT_TRUE(p2->set_value("10"));
//     TRY(p2->set_value("1 0"), std::exception);
//     EXPECT_NE("1 0", p2->val());
//     p2->push("0");
//     EXPECT_EQ("0", p2->val());
//     EXPECT_EQ(1, p2->vals().size());
//     EXPECT_TRUE(p3->set_value(tt));
//     EXPECT_EQ("100", p2->default_val());
//     EXPECT_TRUE(tt == p3->vals());
//     p2->push("1");
//     EXPECT_EQ("1", p2->val());

//     std::vector<std::string> tmp2{"xxx", "xx- xx"};
//     p3->set_value(tmp2);
//     EXPECT_EQ("xxx xx- xx", p3->val());
//     tmp2.emplace_back("xx");
//     p3->push("xx");
//     EXPECT_EQ(tmp2[0] + " " + tmp2[1] + " " + tmp2[2], p3->val());
//     EXPECT_TRUE(tmp2 == p3->vals());

//     std::vector<std::string> ttt{"ttt"};
//     EXPECT_TRUE(p2->set_value("1000"));
//     EXPECT_TRUE(p3->set_value(ttt));
//     EXPECT_EQ("1000", p2->val());
//     EXPECT_EQ("100", p2->default_val());
//     EXPECT_TRUE(ttt == p3->vals());
//     EXPECT_EQ(ttt[0], p3->val());

//     p2->push("1");
//     ttt.emplace_back("aa");
//     p3->push("aa");
//     EXPECT_EQ("1", p2->val());
//     EXPECT_TRUE(ttt == p3->vals());
    
//     delete p1;
//     delete p2;
//     delete p3;
// }

// TEST(Option_test)
// {
//     Option option;
//     option.add("name", 'n', "description", true);
//     EXPECT_EQ("name", option.get_option('n')->name());
//     EXPECT_EQ("name", option.get_name('n'));
//     EXPECT_EQ('n', option.get_option("name")->short_name());
//     EXPECT_EQ("name", option.get_name('n'));
//     EXPECT_EQ("description", option.get_option("name")->description());
//     EXPECT_TRUE(option.get_option("name")->is_must());
//     EXPECT_TRUE(option.exist("name"));
//     EXPECT_TRUE(option.default_option() == nullptr);
//     EXPECT_TRUE(!option.get_option("name")->is_default());
//     EXPECT_FALSE(option.set_value("name", "xx"));

//     option.add("test1");
//     option.add("test2");

//     option.add<int>("int", 'i', "this is int", false, false, 10);
//     EXPECT_TRUE(option.exist('i'));
//     EXPECT_FALSE(option.get_option("int")->is_must());
//     EXPECT_FALSE(option.get_option("int")->is_default());
//     EXPECT_FALSE(option.get_option("int")->is_more_value());
//     EXPECT_EQ("int", option.get_option("int")->name());
//     EXPECT_EQ("int", option.get_name('i'));
//     EXPECT_EQ('i', option.get_option("int")->short_name());
//     EXPECT_EQ("this is int", option.get_option("int")->description());
//     EXPECT_EQ("10", option.get_option("int")->default_val());
//     EXPECT_TRUE(option.set_value("int", "100"));
//     EXPECT_EQ("10", option.get_option("int")->default_val());
//     EXPECT_EQ("100", option.get_option('i')->val());
//     TRY(option.add<int>("ii", 'i', "this is int"), std::exception);

//     TRY(option.add_more<int>("int"), std::exception);
//     EXPECT_TRUE(option.exist("int"));
//     EXPECT_TRUE(option.set_value("int", "100"));
//     EXPECT_EQ("100", option.get_option("int")->val());
    
//     option.add<int>("xxx", 'x', "this is xxx", true, false, 10);
//     EXPECT_EQ("10", option.get_option('x')->default_val());
//     TRY(option.get_option("---"), std::exception);

//     option.add_more<std::string>("string", 's', "this is string", false, false, {"xx", "22"});
//     EXPECT_FALSE(option.get_option("string")->is_default());
//     EXPECT_EQ("string", option.get_option("string")->name());
//     EXPECT_EQ('s', option.get_option("string")->short_name());
//     EXPECT_EQ("this is string", option.get_option("string")->description());
//     EXPECT_TRUE(option.get_option("string")->is_more_value());
// }


// void get(int& argc, char* argv[], bool& is_true, std::istream& ifs)
// {
//     for (int j = 0; j < argc; ++j) delete[] argv[j];
//     argc = 0;
//     if (ifs.eof()) return;
//     if (!ifs) throw;

//     while (true) {
//         char* tmp = new char[41];   // 每个字符数小于 40
//         if (ifs.peek() == '\n') {
//             if (argc == 0) {
//                 ifs.get();
//                 continue;
//             }
//             return;
//         }
//         ifs >> tmp[0];
//         if (ifs.eof()) break;
//         int i = 1;
//         while (!isspace(ifs.peek())) {
//             tmp[i++] = ifs.get();
//             if (ifs.eof()) break;
//             if (i > 40) throw;
//         }
//         tmp[i] = '\0'; 
//         if (std::string(tmp)[0] == '1') {
//             is_true = true;
//             while (ifs.peek() != '\n') {
//                 ifs.get();
//                 if (ifs.eof()) return;
//             }
//         }
//         else if (std::string(tmp)[0] == '0') {
//             is_true = false;
//             while (ifs.peek() != '\n') {
//                 ifs.get();
//                 if (ifs.eof()) return;
//             }
//         }
//         else argv[argc++] = tmp;
//         if (argc > 30) throw;
//     }
// }

// TEST(Parse_test)
// {
//     // htf.exe [files]  -f  -o dir  -I dir1 dir2...
//     Parse p1;
//     p1.add_more<std::string>("input", 'i', "input files", true, true);
//     p1.add("force", 'f', "this is force");
//     p1.add("E", 'E', "this is E");
//     p1.add<std::string>("output", 'o', "output dir", false, false);
//     p1.add_more<std::string>("Include", 'I', "include dirs", false, false);
//     p1.set_program_name("cmd.exe");
//     std::cout << p1.usage() << std::endl;

//     /**
//      * 读取 cmdline.txt
//      * - 按行读取
//      * * 每一行的结尾为 0 or 1，表示该行格式是否正确
//      */
//     int argc = 0;
//     char* argv[31];
//     std::ifstream ifs(R"(D:\code\git\Header_to_File\test\txt\cmdline.txt)");
//     if (!ifs) throw;
//     int num = 1;
//     bool is_true = false;
//     while (true) {
//         get(argc, argv, is_true, ifs);
//         if (argc == 0) break;
//         for (int i = 0; i < argc; i++) std::cout << argv[i] << " ";
//         std::cout << std::endl;
//         bool res = p1.parse(argc, argv);
//         EXPECT_EQ(is_true, res);
//         std::cout << "value" << num << ":\n" << p1.show() << std::endl;
//         if (!res) {
//             std::cout << "error" << num << ":\n" << p1.errors() << std::endl;
//         }
//         std::cout << std::endl;
//         num++;
//     }
// }

TEST(htf_cmdline_TEST)
{
    const int argc = 7;
    char* argv[argc] = { "", "D:/code/git/Header_to_File/src/cmdline/Option.h", "-I", "./../src", "-o", "./txt", "-f" };
    htf_cmdline_parse(argc, argv);
}

}//
}//
}//
