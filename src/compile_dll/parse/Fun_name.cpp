#include "Fun_name.h" 
#include "output.h"
#include "Excep_dev.h"
#include "usage.h"

using namespace std;
using namespace output;

namespace htf {
	namespace parse {
		// public
		Fun_name::Fun_name()
			:_name{ }
		{

		}

		Fun_name::Fun_name(const stream::Identifier& i)
			:_name{ i.str() }
		{

		}

		Fun_name::Fun_name(const std::string& s)
			:_name{ s }
		{
			if (s.find("operator") != string::npos) {		// 1. 重载运算符函数名
				if (s == "operator") 
					throw excep::Excep_dev("Fun_name::Fun_name(const string&)", _LINE + "after" +
						 mark(s) + "lack of operator-char");
			}
			else {		// 2. 一般函数名
				if (!stream::usage::is_identifier(s)) 
					throw excep::Excep_dev("Fun_name::Fun_name",  _LINE + mark(s) + 
						"isn't a legal identifier, cannot to be function-name");
			}
		}

		std::string Fun_name::str() const
		{
			return _name;
		}

		bool Fun_name::empty() const
		{
			return _name.empty();
		}

	}

}

