#include "Ins_key.h"

using namespace std;
using namespace prompt;


// --------------------------------------- Ins_key类 -------------------------------------------

// ---------------------- public -------------------------

Ins_key::Ins_key()
    :_ins{ }
{
    
}

Ins_key::Ins_key(const string& s)
	:_ins{ s }
{
    if (s.empty()) return;
	if (!ins_judge::is_ins(s)) 
        throw string("Ins_key::Ins_key:" + mark_string(s) + "不是指令");
}

string Ins_key::str() const
{
	return _ins;
}

bool Ins_key::empty() const
{
    return _ins.empty();
}

// ---------------------------------- 重载 --------------------------------------------

istream& operator>>(istream& is, Ins_key& t)
{
	string s;
	is >> s;
	t = Ins_key{ s };
	return is;
}