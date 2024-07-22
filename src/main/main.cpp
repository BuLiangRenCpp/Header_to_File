#include "Resolve.h"
#include "excep.h"
#include "output.h"

#ifdef _WIN32
#    include "win.h"
#endif

using namespace std;
using namespace htf;

/**
 * * 希望新增功能：
 * - 增加对 c++11 一些特性的支持，比如 virtual、override 等
 * * - [目前感觉没必要] 对于函数参数，如果参数名省略的，自动添加默认参数名
 */

static void imitate(istream& is)
{
    argument::Htf_args a;
    is >> a;
    argument::Resolve r;
    r.resolve(a);
}

static string to_str(int cnt, char* argv[])
{
    string res;
    for (int i = 1; i < cnt; i++) {
        string s = string(argv[i]);
        if (s.find(' ') != string::npos) s = output::mark(string(argv[i]));
        res += s + " ";
    }
    return res;
}


int main(int cnt, char* argv[])
try {
// ********* windows ***********
#ifdef _WIN32
    win::set_consoleCP_to_UTF8();
#endif
    // ****************************

#ifdef USER
    string s = to_str(cnt, argv);
#else
    // ********* Debug ************
    string s;
    cout << "> ";
    getline(cin, s);
// ****************************
#endif
    istringstream is(s);
    imitate(is);
    return 0;
} catch (htf::excep::Excep_path& e) {
    output::print_error(PROGRAM_NAME + "::" + e.str());
    return 1;
} catch (htf::excep::Excep_arg& e) {
    output::print_error(PROGRAM_NAME + "::" + e.str());
    return 2;
} catch (htf::excep::Excep_syntax& e) {
    output::print_error(PROGRAM_NAME + "::" + e.str());
    return 3;
} catch (regex_error& e) {
    output::print_error(PROGRAM_NAME + "::" + e.what());
    return 4;
} catch (htf::excep::Excep_dev& e) {
    output::print_warn("please contact the developer");
    output::print_error(PROGRAM_NAME + "::" + e.str());
    return 5;
} catch (htf::excep::Excep_base& e) {
    output::print_warn("please contact the developer");
    output::print_error(PROGRAM_NAME + "::" + e.all());
    return 6;
} catch (string& e) {
    output::print_warn("please contact the developer");
    output::print_error(PROGRAM_NAME + "::" + e);
    return 7;
} catch (...) {
    output::print_warn("please contact the developer");
    output::print_error(PROGRAM_NAME + "::" + "fatal error!");
    return 8;
}