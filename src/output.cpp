#include "output.h"
#include <sstream>

#include "base_info.h"
#include "color.h"

using namespace std;

namespace htf
{

std::string mark(const std::string& s, char c)
{
    ostringstream oss;
    oss << " " << c << s << c << " ";
    return oss.str();
}

std::string mark(char s, char c)
{
    ostringstream oss;
    oss << " " << c << s << c << " ";
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
