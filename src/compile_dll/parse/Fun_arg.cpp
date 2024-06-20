#include "Fun_arg.h"

using namespace std;

namespace htf {
    namespace parse {
        Fun_arg::Fun_arg(const Type& t, const stream::Identifier& i)
            :_type{ t }, _name{ i }
        {

        }

        string Fun_arg::str() const
        {
            string name = (_name.empty()) ? "" : " " + _name.str();
            return _type.str() + name;
        }
    }
}