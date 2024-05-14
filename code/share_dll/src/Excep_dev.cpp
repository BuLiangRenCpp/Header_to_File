#ifdef _WIN32
#define LIB_EXPORT
#endif
#include "Excep_dev.h"
#include "output.h"

using namespace std;
using namespace output;

namespace htf {
    namespace exception {
        Excep_dev::Excep_dev(const string& where, const string& what)
            :Excep_base{where, what}
        {
            
        }

        string Excep_dev::str() const
        {
            return "DEV_ERROR: " + this->all();
        }

        ostream& operator<<(ostream& os, const Excep_dev& e)
        {
            os << "DEV_ERROR: ";
            os << e.all();
            return os;
        }
    }    
}