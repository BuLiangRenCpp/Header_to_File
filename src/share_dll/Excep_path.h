#pragma once
#ifdef _WIN32
#define LIB_EXPORT
#endif

#include "dll.h"
#include "Excep_base.h"

namespace htf {
    namespace exception {
        // 开发error，用于提示开发错误，以便修复系统BUG
        class DLL_API Excep_path : public Excep_base {
        public:
            Excep_path() = delete;
            Excep_path(const std::string& where, const std::string& what);
            std::string str() const;
        };

        DLL_API std::ostream& operator<<(std::ostream&, const Excep_path&);
    }
}