#ifdef _WIN32
#    define LIB_EXPORT
#endif

#include "Excep_syntax.h"
#include "Excep_dev.h"
#include "output.h"
#include "path_deal.h"

using namespace std;
using namespace output;

namespace htf
{
using namespace path_deal;

namespace excep
{
Excep_syntax::Excep_syntax(const string& where_path, line_t line, const string& what)
    : excep::Excep_base{where_path, what}
    , _line{line}
{
    if (!is_exist(where_path))
        throw excep::Excep_dev{
            "preprocess::Excep_syntax::Excep_syntax",
            _LINE + mark(where_path) + " not exist, argument 'where_path' must be exist"};
}

line_t Excep_syntax::line() const
{
    return _line;
}

string Excep_syntax::str() const
{
    string line = (this->line() == 0) ? "" : mark("line " + to_string(this->line()));
    return "c++ syntax" + mark(this->where()) + ": \n" + line + "-> " + this->what();
}

ostream& operator<<(ostream& os, const Excep_syntax& e)
{
    os << "c++ syntax";
    string line = (e.line() == 0) ? "" : mark("line " + to_string(e.line()));
    os << mark(e.where()) + ": \n" + line + "-> " + e.what();
    return os;
}
}   // namespace excep
}   // namespace htf