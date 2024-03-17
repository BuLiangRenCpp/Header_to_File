#include "user_input.h"
#include "Resolve.h"


// ******************** windows *************************
#ifdef _WIN32
#include <windows.h>

namespace win {
	static bool set_consoleCP_to_UTF8()
	{
		return SetConsoleCP(56001);
	}
}
#endif
// *******************************************************


static void imitate()
{
	Resolve r;
	while (r.is_run()) 
	try{
		prompt::print_enter("ins");
		r.resolve(get_ins());
		std::cout << std::endl;
	}
	catch (std::string& s) {
#ifdef USER
		prompt::print_error(s);
#else 
		prompt::print_error(s, false);		// 开发者模式
#endif
		std::cerr << std::endl;
	}
}

int main()
try{
// ****************************
#ifdef _WIN32
	win::set_consoleCP_to_UTF8();
#endif
// ****************************
    prompt::prompt();
    imitate();
    return 0;
}catch(std::string& e)
{
	prompt::print_error(e);
    return 1;
}