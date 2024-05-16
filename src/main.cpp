#include "Resolve.h"
#include "output.h"
#include "Excep.h"

#ifdef _WIN32
#include "win.h"
#endif

using namespace std;
using namespace htf;

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
        if (s.find(' ') != string::npos) s = output::mark_string(string(argv[i]));
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
}
catch(htf::exception::Excep_path& e)
{
    output::print_error(e.str());
    return 1;
}
catch(htf::exception::Excep_arg& e)
{
    output::print_error(e.str());
    return 2;
}
catch(htf::exception::Excep_syntax& e)
{
    output::print_error(e.str());
    return 3;
}
catch(regex_error& e)
{
    output::print_error(e.what());
    return 4;
}
catch(htf::exception::Excep_dev& e)
{
    output::print_warn("please contact the developer");
    output::print_error(e.str());
    return 5;
}
catch(htf::exception::Excep_base& e)
{
    output::print_warn("please contact the developer");
    output::print_error(e.all());
    return 6;
}
catch(string& e)
{
    output::print_warn("please contact the developer");
    output::print_error(e);
    return 7;
}
catch (...) {
    output::print_warn("please contact the developer");
    output::print_error("ERROR!");
    return 8;
}