#include "Ins_arg.h" 

using namespace std;
using namespace prompt;
using namespace ins_const;


// --------------------------------------- Ins_arg ------------------------------------------

// -------------------- public -----------------------

Ins_arg::Ins_arg() 
    :_key{}, _con{}, _harg{ false }
{

}

Ins_arg::Ins_arg(const Ins_arg_key& k, const Ins_arg_con& c) 
    :_key{ k }, _con{ c }, _harg{ ins_judge::is_harg(k.ch()) }
{
	if (_harg && _con.empty()) 
        throw string("Ins_arg::Ins_arg:" + mark_char(k.ch()) + "后缺少内容");
}

Ins_arg_key Ins_arg::key() const
{
    return _key;
}

Ins_arg_con Ins_arg::con() const
{
    return _con;
}

bool Ins_arg::empty() const
{
    return _key.empty() && _con.empty();
}

bool Ins_arg::is_darg() const
{
    return _key.empty() && !_con.empty();
}

void Ins_arg::change_darg(const Ins_arg_key& k)
{
    if (!this->is_darg()) throw string("Ins_arg::change_darg: 只能修改默认参数");
	_key = k;
}



istream& operator>>(istream& is, Ins_arg& t) 
{
	Ins_arg_key k;
	Ins_arg_con c;

	is >> k;
	if (ins_judge::is_narg(k.ch())) {
		t = Ins_arg{ k };
		return is;
	}
	is >> c;
	t = Ins_arg{ k, c };
	return is;
}


