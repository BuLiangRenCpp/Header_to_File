#include "Arg_con.h" 
#include "args_const.h"
#include "path_deal.h"
#include "args_judge.h"
#include "Excep_arg.h"

using namespace std;
using namespace output;
using namespace htf;


static bool is_arg_style(const string& s)
{
    return (s.length() == 2 && s[0] == argument::args_const::ARG_STA_CH && argument::args_judge::is_arg(s[1]));
}

// --------------------------------------- Arg_con ------------------------------------------

// -------------------- public -----------------------

namespace htf {
	using namespace exception;
	using namespace path_deal;
	namespace argument{
		Arg_con::Arg_con() 
			:_con{}
		{

		}

		Arg_con::Arg_con(const vector<string>& s) 
			:_con{  }
		{
			if (s.empty()) return;
			for (const auto& x : s)	{
				_con.emplace_back(normalize(current_dir(), x));
			}
		}

		bool Arg_con::insert(const Arg_con& a)
		{
			bool res = false;
			for (const auto& t : a.con()) {
				if (!my_std::is_in(t, _con)) {
					res = true;
					_con.emplace_back(t);
				}
			}
			return res;
		}

		vector<string> Arg_con::con() const
		{
			return _con;
		}

		bool Arg_con::empty() const
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
			if (flag == false) throw Excep_arg("Ins_par_con.cpp::get_content", "缺少" + mark_char(target));
			return s;
		}

		istream& operator>>(istream& is, Arg_con& t) 
		{
			vector<string> con;
			char c;
			while (true) {
				my_std::getchar(is, c);
				if (is.eof()) break;
				if (c == args_const::ARG_STA_CH) {
					is.putback(c);
					break;
				}
				if (c == '"' || c == '\'') con.emplace_back(get_content(is, c)); 
				else {
					string s;
					is.putback(c);
					is >> s;
					con.emplace_back(s);
				}
			}

			t = Arg_con{ con };
			return is;
		}
	}
}


