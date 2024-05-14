#include "output.h"
#include "Arg_key.h" 
#include "args_const.h"
#include "args_judge.h"
#include "Excep_arg.h"

using namespace std;
using namespace output;

// --------------------------------------- Arg_key ------------------------------------------

// -------------------- public -----------------------

namespace htf {
	using namespace exception;

	namespace argument{
		using namespace args_const;
		using namespace args_judge;

		Arg_key::Arg_key() 
			:_ch{ NULL_ARG }
		{

		}

		Arg_key::Arg_key(const char c) 
			:_ch{ c } 
		{
			if (!is_arg(c)) 
				throw Excep_arg("Arg_key::Arg_key", mark_char(c) + "isn't a legal argument");
		}

		char Arg_key::ch() const
		{
			return _ch;
		}

		bool Arg_key::empty() const
		{
			return _ch == NULL_ARG;
		}


		bool Arg_key::is_harg() const
		{
			return args_judge::is_harg(_ch);
		}

		bool Arg_key::is_narg() const
		{
			return args_judge::is_narg(_ch);
		}


		istream& operator>>(istream& is, Arg_key& t) 
		{
			char c = NULL_ARG;
			my_std::getchar(is, c);
			if (is.eof()) {
				t = Arg_key{};
				return is;
			}
			if (is.peek() == ' ' || c != ARG_STA_CH) {
				is.putback(c);
				t = Arg_key{};
				return is;
			}
			is >> c;
			t = Arg_key{ c };
			return is;
		}
	}
}
