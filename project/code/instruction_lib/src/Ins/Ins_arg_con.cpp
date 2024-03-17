#include "Ins_arg_con.h" 

using namespace std;
using namespace prompt;
using namespace path_deal;
using namespace ins_judge;
using namespace ins_const;


static bool is_arg_style(const string& s)
{
    return (s.length() == 2 && s[0] == arg::ARG_STA_CH && is_arg(s[1]));
}

// --------------------------------------- Ins_arg_con ------------------------------------------

// -------------------- public -----------------------

Ins_arg_con::Ins_arg_con() 
    :_con{}
{

}

Ins_arg_con::Ins_arg_con(const string& s, bool is_force) 
    :_con{ normalize(current_dir(), s) }
{
	if (s.empty() || is_force) return;

	if (is_arg_style(s))
		throw string("Ins_par_con::Ins_par_con:" + mark_string(s) + "是参数格式，不能作为路径，若强制要求请加上 \"\"");
}

string Ins_arg_con::str() const
{
    return _con;
}

bool Ins_arg_con::empty() const
{
    return _con.empty();
}


// 读取 target 之间的字符
static string get_content(istream& is, char target)
{
	string s;
	char c;
	bool flag = false;		// 是否遇见 target
	while (is >> noskipws >> c) {
		if (c == target) {
			flag = true;
			break;
		}
		s += c;
	}
	if (flag == false) throw string("Ins_par_con.cpp::get_content: 缺少" + mark_char(target));
	return s;
}

istream& operator>>(istream& is, Ins_arg_con& t) 
{
	string con;
	bool is_force = false;
	char c;
	my_std::getchar(is, c);
	if (is.eof()) {
		t = Ins_arg_con{};
		return is;
	}
	if (c == '"' || c == '\'') {
		con = get_content(is, c); 
		if (is_arg_style(con)) is_force = true;
	}
	else {
		is.putback(c);
		is >> con;
	}
	t = Ins_arg_con{ con, is_force };
	return is;
}


