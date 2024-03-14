#include "Identifier.h"

using namespace prompt;
using namespace usage;

static bool is_name_ch(char c)
{
    return (c == '_' || isalnum(c));
}


// --------------------------- Identifier类 ------------------------------------------

// ----------------- pubilc ---------------------

Identifier::Identifier()
    :_name{}
{

}

Identifier::Identifier(const string& s)
    :_name{ s }
{   
    if (s.empty()) return;
    if (!is_identifier(s)) throw string("Identifier::Identifier: 非法标识符" + mark_string(s));
}

string Identifier::str() const
{
    return _name;
}

bool Identifier::empty() const
{
    return _name == "";
}



// ------------------------ 重载 -----------------------------

istream& operator>>(istream& is, Identifier& n)
{
    string name;
    char c;
    while (true) {
        if (isspace(is.peek()) && !name.empty()) break;
        my_std::getchar(is, c);
        if (is.eof()) break;
        if ((name.empty() && isdigit(c)) || !is_name_ch(c)) {       // 标识符不能以数字开头
            is.putback(c);
            break;
        }
        name += c;
    }
    n = Identifier{ name };
    return is;
}

ostream& operator<<(ostream& os, const Identifier& n)
{
    os << n.str();
    return os;
}