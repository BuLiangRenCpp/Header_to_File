#include "Ins_arg_key.h" 

using namespace std;
using namespace prompt;
using namespace ins_const;
using namespace ins_judge;

// --------------------------------------- Ins_arg_key ------------------------------------------

// -------------------- public -----------------------

Ins_arg_key::Ins_arg_key() 
    :_ch{ arg::NULL_ARG }
{

}

Ins_arg_key::Ins_arg_key(char c) 
    :_ch{ c } 
{
    if (!is_arg(c)) 
        throw string("Ins_arg_key::Ins_arg_key:" + mark_char(c) + "不是合法参数");
}

char Ins_arg_key::ch() const
{
    return _ch;
}

bool Ins_arg_key::empty() const
{
    return _ch == arg::NULL_ARG;
}


bool Ins_arg_key::is_harg() const
{
    return ins_judge::is_harg(_ch);
}

bool Ins_arg_key::is_narg() const
{
    return ins_judge::is_narg(_ch);
}


istream& operator>>(istream& is, Ins_arg_key& t) 
{
	char c = arg::NULL_ARG;
	my_std::getchar(is, c);
	if (is.eof()) {
		t = Ins_arg_key{};
		return is;
	}
	if (is.peek() == ' ' || c != arg::ARG_STA_CH) {
		is.putback(c);
		t = Ins_arg_key{};
		return is;
	}
	is >> c;
	t = Ins_arg_key{ c };
	return is;
}


