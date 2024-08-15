#include "output.h"
#include <sstream>

#include "base_info.h"
#include "color.h"
#include "usage_base.h"

using namespace std;

namespace htf
{
std::string mark(char str, char c)
{
    std::ostringstream oss;
    char               end = c;
    if (is_lbracket(c)) end = ret_rbracket(c);
    oss << " " << c << str << end << " ";
    return oss.str();
}

std::string mark(const std::string& str, char c)
{
    std::ostringstream oss;
    char               end = c;
    if (is_lbracket(c)) end = ret_rbracket(c);
    oss << " " << c << str << end << " ";
    return oss.str();
}


void cout_log(const std::string& what)
{
    cout << HTF_PROGRAM << ": " << COLOR(GREEN, what) << endl;
}

void cout_warn(const std::string& what)
{
    cout << HTF_PROGRAM << ": " << COLOR(YELLOW, "warn: ") << what << endl;
}

void cout_error(const std::string& what)
{
    cerr << HTF_PROGRAM << ": " << COLOR(RED, "error: ") << what << endl;
}

void cout_error_dev(const std::string& what)
{
    cerr << HTF_PROGRAM << ": " << COLOR(RED, "dev-error: " << what) << endl;
}

}   // namespace htf
