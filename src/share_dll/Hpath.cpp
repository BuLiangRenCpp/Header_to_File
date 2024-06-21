#ifdef _WIN32
#define LIB_EXPORT
#endif

#include "Hpath.h"
#include "output.h"
#include "Excep_path.h"

using namespace std;

namespace htf {
    namespace path_deal {
        Hpath::Hpath(const string& hpath)
            :_hpath{ hpath }
        {
            if (!is_exist_file(hpath)) 
                throw excep::Excep_path{"path_deal::Hpath::Hpath", "file" + output::mark(hpath) + "not exist"};
            if (!is_header_extension(extension(hpath))) 
                throw excep::Excep_path{"path_deal::Hpath::Hpath", "file" + output::mark(hpath) + "isn't a legal C++ header file"};
        }

        string Hpath::change_extension(const string& extension)
        {
            return path_deal::change_extension(_hpath, extension);
        }

        string Hpath::str() const 
        {
            return _hpath;
        }

        Hdir Hpath::parent() const
        {
            return Hdir{parent_dir(_hpath)};
        }

        string Hpath::filename(bool have_extension) const
        {
            return file_name(_hpath, have_extension);
        }

        ostream& operator<<(ostream& os, const Hpath& h)
        {
            os << h.str();
            return os;
        }

        bool operator==(const Hpath& h1, const Hpath& h2)
        {
            return h1.str() == h2.str();
        }
        
        bool operator!=(const Hpath& h1, const Hpath& h2)
        {
            return !(h1 == h2);
        }
    }
}