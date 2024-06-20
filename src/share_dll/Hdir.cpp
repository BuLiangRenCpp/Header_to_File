#ifdef _WIN32
#define LIB_EXPORT
#endif

#include "Hdir.h"
#include "path_deal.h"
#include "Excep_path.h"

using namespace std;

namespace htf {
	namespace path_deal {
		Hdir::Hdir(const string& s) 
			:_dir{ s }
		{
			if (!is_exist_dir(s)) 
				throw exception::Excep_path{"Hdir::Hdir(const string&)", "directory" + output::mark_string(s) + "not exist"};
		}

		std::string Hdir::str() const
		{
			return _dir;
		}

		bool Hdir::have_file(const string& file) const
		{
			return is_exist_file(normalize(_dir, file));
		}

		bool operator==(const Hdir& h1, const Hdir& h2)
		{
			return h1.str() == h2.str();
		}
	}

}

