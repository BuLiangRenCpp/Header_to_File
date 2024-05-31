// #include "Arg.h"
// #include "Excep_arg.h"
// #include "Excep_path.h"
// #include "Excep_dev.h"
// #include "Resolve.h"
// #include "output.h"

// using namespace std;
// using namespace htf;

// /**
//  * ! 死循环： -i ../include/header.h -I include
// */

// static void imitate()
// {
//     argument::Htf_args args;
//     argument::Resolve r;
//     while (true) 
//     try{
//         cout << "> ";
//         string s;
//         getline(cin, s);
//         istringstream is(s);
//         is >> args;
//         if (args.args().empty()) return;
//         r.resolve(args);
//     }
//     catch(htf::exception::Excep_arg& e)
//     {
//         output::print_error(e.str());   
//         if (args.args().empty()) return;
//     }
//     catch(htf::exception::Excep_path& e)
//     {
//         output::print_error(e.str());   
//     }
//     catch(htf::exception::Excep_dev& e)
//     {
//         output::print_error(e.str());   
//     }
// }

// int main()
// {
//     imitate();
//     return 0;
// }