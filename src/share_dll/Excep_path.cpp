#ifdef _WIN32
#define LIB_EXPORT
#endif

#include "Excep_path.h"
#include "output.h"

using namespace std;
using namespace output;

namespace htf {
    namespace excep {
        Excep_path::Excep_path(const string& where, const string& what)
            :Excep_base{where, what}
        {
            
        }

        string Excep_path::str() const
        {
    #ifdef USER
            string res = this->what();
    #else 
            string res = this->all();
    #endif
            return "path-error: " + res;
        }

        ostream& operator<<(ostream& os, const Excep_path& e)
        {
            os << "path-error: ";
    #ifdef USER
            os << e.what();
    #else 
            os << e.all();
    #endif
            return os;
        }
    }
}