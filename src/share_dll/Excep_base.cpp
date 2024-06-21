#ifdef _WIN32
#define LIB_EXPORT
#endif

#include "Excep_base.h"

using namespace std;

namespace htf {
    namespace excep {    
        Excep_base::Excep_base(const string& where, const string& what)
            :_where{ where }, _what{ what }
        {
            if (_what.empty()) 
                throw Excep_base("my_exception::Excep_base::Excep_base", "argument 'what' not empty");
        }

        string Excep_base::where() const 
        {
            return _where;
        }

        string Excep_base::what() const
        {
            return _what;
        }

        string Excep_base::all() const
        {
            if (_where.empty()) return _what;
            return _where + ": \n" + _what;
        }

        ostream& operator<<(ostream& os, const Excep_base& e)
        {
            os << e.all();
            return os;
        }
    }
}

