// #include "output.h"
// #include "Excep_path.h"
// #include "Excep_syntax.h"
// #include "Lex.h"

// using namespace std;
// using namespace htf;
// using namespace output;

// /**
//  * ! lex 加入对 const 的处理有问题 
// */

// path_deal::Hpath get()
// {
//     string s;
//     while (true) 
//     try{
//         print_enter("hpath");
//         cin >> s;
//         path_deal::Hpath h(s);
//         return h;
//     } 
//     catch (htf::exception::Excep_path& e)
//     {
//         if (s == ";") return path_deal::Hpath{R"(D:\10.cmake_demo\htf\code\share_dll\inc\dll.h)"};
//         print_error(e.str());
//     }
// }

// static void imitate()
// {
//     while (true) 
//     try{
//         auto h = get();
//         if (h.str() == R"(D:\10.cmake_demo\htf\code\share_dll\inc\dll.h)") return;
//         ifstream ifs(h.str());
//         lex::Lex l(ifs, h);
//         int cnt = 0;
//         while (!l.eof()) {
//             auto t = l.get();
//             cout << t.kind << "  " << t.val << "  |  ";
//             cnt++;
//             if (cnt % 5 == 0) cout << endl;
//         }
//         cout << endl;
//     }
//     catch (htf::exception::Excep_syntax& e)
//     {
//         cout << endl;
//         print_error(e.str());
//     }
// }


// int main()
// {
//     imitate();
//     return 0;
// }
