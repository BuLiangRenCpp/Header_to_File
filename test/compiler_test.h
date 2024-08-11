#pragma once

#include <iomanip>

#include "test.h"
#include "../src/compiler/PreProcess.h"
#include "../src/compiler/htf.h"

namespace htf
{
namespace test
{
namespace compiler_test
{

// TEST(PreProcess_test)
// {
//     std::string path = R"(D:\code\git\Header_to_File\test\txt\pre.h)";
//     std::string opath = R"(D:\code\git\Header_to_File\test\txt\pre.i)";
//     std::string dir = R"(D:\code\git\Header_to_File\test\txt)";
//     std::string inl1 = R"(D:\code\git\Header_to_File\test)";
//     std::string inl2 = R"(D:\code\git\Header_to_File\src\cmdline)";
//     std::string inl3 = R"(D:\code\git\Header_to_File\src)";
//     std::string inl4 = R"(D:\code\git\Header_to_File\src\path)";
//     std::vector<path::Dir> dirs{ inl1, inl2, inl3, inl4 };
//     std::string s1 = R"(D:\code\git\Header_to_File\src\cmdline\Parse.h)";
//     std::set<std::string> not_include{ s1 };
//     PreProcess pre(path, dirs, not_include); 
//     if (!pre.run(opath, dir)) std::cout << pre.errors() << std::endl; 
//     pre.clear();
// }

// TEST(TokenStream_test)
// {
//     std::string path = R"(D:\code\git\Header_to_File\test\txt\pre.i)";
//     std::string opath = R"(D:\code\git\Header_to_File\test\txt\ts.out)";
//     std::ofstream ofs(opath);
//     if (!ofs) throw;
//     ofs << "[" << path << "]" << std::endl;
//     TokenStream ts(path);
//     EXPECT_EQ(path, ts.file());
//     while (true) {
//         auto token = ts.get();
//         if (token.empty()) break;
//         if (path != ts.file()) {
//             path = ts.file();
//             ofs << "\n[" << path << "]" << std::endl;
//         }
//         ofs << "#" << token.line << ":" << token.col << ": ";
//         switch (token.kind) {
//         case TokenKind::constant:
//             ofs << "constant: ";
//             break;
//         case TokenKind::string_literal:
//             ofs << "string_literal: ";
//             break;
//         case TokenKind::keyword:
//             ofs << "keyword: ";
//             break;
//         case TokenKind::identifier:
//             ofs << "identifier: ";
//             break;
//         case TokenKind::dchar:
//             ofs << "dchar: ";
//             break;
//         case TokenKind::schar:
//             ofs << "schar: ";
//             break;
//         default:
//             throw;
//         }
//         ofs << token.val << "\n";
//     }
//     std::cout << ts.errors() << std::endl;
// }

// TEST(Lex_test)
// {
//     std::string path = R"(D:\code\git\Header_to_File\test\txt\pre.i)";
//     std::string opath = R"(D:\code\git\Header_to_File\test\txt\lex.out)";
//     std::string s1 = R"(D:\code\git\Header_to_File\test\txt\pre.h)";
//     std::string s2 = R"(D:\code\git\Header_to_File\src\compiler\Lex.h)";
//     std::set<std::string> sources{s1, s2};
//     Ofstream ofs(opath);
//     if (!ofs) throw;
//     Lex lex(path, sources);
//     std::string file = lex.file();
//     ofs << "#" << file << std::endl;
//     while (true) {
//         auto lexer = lex.get();
//         if (lexer.empty()) break;
//         if (file != lex.file()) {
//             file = lex.file();
//             ofs << "#" << file << std::endl;
//         }
//         ofs << std::setw(4) << lexer.line << ":" << std::setw(3) << lexer.col << ":     ";
//         switch (lexer.kind)
//         {
//         case LexerKind::identifier:
//             ofs << "id     ";
//             break;
//         case LexerKind::keyword:
//             ofs << "key    ";
//             break;
//         case LexerKind::literal:
//             ofs << "literal";
//             break;
//         case LexerKind::type:
//             ofs << "type   ";
//             break;
//         case LexerKind::dchar:
//             ofs << "dchar  ";
//             break;
//         case LexerKind::schar:
//             ofs << "schar  ";
//             break;
//         default:
//             break;
//         }
//         ofs << ":  " << lexer.val << "\n";
//     }
//     std::cout << lex.errors() << std::endl;
// }

// TEST(Parse_test)
// {
//     std::string path = R"(D:\code\git\Header_to_File\test\txt\pre.i)";
//     std::string odir = R"(D:\code\git\Header_to_File\test\txt\)";
//     std::string s1 = R"(D:\code\git\Header_to_File\test\txt\pre.h)";
//     std::string s2 = R"(D:\code\git\Header_to_File\src\compiler\Lex.h)";
//     std::string s3 = R"(D:\code\git\Header_to_File\src\compiler\TokenStream.h)";
//     std::string s4 = R"(D:\code\git\Header_to_File\src\compiler\TypeTable.h)";
//     std::string s5 = R"(D:\code\git\Header_to_File\src\cmdline\Parse.h)";
//     std::string s6 = R"(D:\code\git\Header_to_File\src\cmdline\Option.h)";
//     std::string s7 = R"(D:\code\git\Header_to_File\src\cmdline\OptionBase.h)";
//     std::string s8 = R"(D:\code\git\Header_to_File\src\cmdline\OptionValue.hpp)";
//     std::set<std::string> sources{ s1, s2, s3, s4, s5, s6, s7, s8 };
//     Parse p{path, sources};
//     p.run(odir, true);
//     std::cout << p.errors() << std::endl;
// }

TEST(htf_test)
{
    std::string s1 = R"(D:\code\git\Header_to_File\src\compiler\Lex.h)";   
    std::string s2 = R"(D:\code\git\Header_to_File\src\compiler\TokenStream.h)";
    std::string s3 = R"(D:\code\git\Header_to_File\src\path\Dir.h)";
    std::set<std::string> sources{ s1, s2, s3 };
    // path::Cfile source = s1;
    std::string inl1 = R"(D:\code\git\Header_to_File\test)";
    std::string inl2 = R"(D:\code\git\Header_to_File\src\cmdline)";
    std::string inl3 = R"(D:\code\git\Header_to_File\src)";
    std::string inl4 = R"(D:\code\git\Header_to_File\src\path)";
    path::Dir dir{R"(D:\code\git\Header_to_File\test\txt)"};
    std::vector<path::Dir> dirs{ inl1, inl2, inl3, inl4 };
    // header_to_file(source, R"(D:\code\git\Header_to_File\test\txt\Lex.cpp)", dirs);
    header_to_file(sources, dir, dirs);
}

}
}
}