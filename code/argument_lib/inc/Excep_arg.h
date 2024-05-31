#pragma once
#include "Excep_base.h"

namespace htf {
    namespace exception {
        // 指令参数错误
        class DLL_API Excep_arg : public Excep_base {
        public:
            Excep_arg() = delete;
            Excep_arg(const std::string& where, const std::string& what);

            std::string str() const;
        };

        DLL_API std::ostream& operator<<(std::ostream&, const Excep_arg&);
    }
}