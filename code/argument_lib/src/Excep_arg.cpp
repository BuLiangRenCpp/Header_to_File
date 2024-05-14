#ifdef _WIN32
#define LIB_EXPORT
#endif

#include "Excep_arg.h"
#include "output.h"

using namespace std;
using namespace output;

namespace htf {
    namespace exception {
        Excep_arg::Excep_arg(const string& where, const string& what)
            :Excep_base{where, what}
        {
            
        }

        string Excep_arg::str() const
        {
    #ifdef USER
            string res = this->what();
    #else 
            string res = this->all();
    #endif
            return "args-error: " + res;
        }

        ostream& operator<<(ostream& os, const Excep_arg& e)
        {
            os << "args-error: ";
    #ifdef USER
            os << e.what();
    #else 
            os << e.all();
    #endif
            return os;
        }
    }
}

