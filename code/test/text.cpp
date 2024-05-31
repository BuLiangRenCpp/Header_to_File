#include "Excep_path.h"
#include "Excep_syntax.h"
#include "Excep_dev.h"
#include "Hpath.h"
#include "preprocess.h"
#include "compile.h"

using namespace std;

/**
 * ! 多文件处理可能会考虑 '多线程'  '文件分块处理'
*/

int main()
try{
    // string s = R"(D:\5.temp_txt\dir\text.h)";
    // string s = R"(D:\10.cmake_demo\htf\code\share_dll\inc)";
    // string s = R"(C:\msys64\ucrt64\include\c++\13.2.0)";
    cout << " ------------------------------- \n";
    // string s1 = R"(D:\10.cmake_demo\htf\code\temp\1.h)";
    // string s2 = R"(D:\10.cmake_demo\htf\code\include\stream\Token_stream.h)";
    // string s3 = R"(D:\10.cmake_demo\htf\code\include\stream\usage.h)";
    // htf::path_deal::Hpath h1{s1};
    // htf::path_deal::Hpath h2{s2};
    // htf::path_deal::Hpath h3{s3};
    string s = R"(D:\10.cmake_demo\htf\code\temp\1.h)";
    htf::path_deal::Hpath h{s};
    htf::path_deal::Hdir dir1{R"(D:\10.cmake_demo\htf\code\include\share)"};
    htf::path_deal::Hdir dir2{R"(D:\10.cmake_demo\htf\code\include\stream)"};
    auto ts = htf::core::preprocess({h}, {dir1, dir2});
    for (auto t : ts) {
        for (auto x : t) {
            cout << "<" << x.first << " " << x.second << ">" << "  ";
        }
        cout << endl;
    }
    htf::path_deal::Hdir hdir{R"(D:\10.cmake_demo\htf\code\temp)"};
    htf::core::compile(ts, hdir, true);
    return 0;
}
catch(htf::exception::Excep_path& e)
{
    output::print_error(e.str());
    return 1;
}
catch(htf::exception::Excep_dev& e)
{
    output::print_error(e.str());
    return 2;
}
catch(htf::exception::Excep_base& e)
{
    cerr << e << endl;
    return 3;
}
catch(string& e)
{
    cerr << e << endl;
    return 4;
}
catch (...) {
    cerr << "ERROR" << endl;
    return 5;
}