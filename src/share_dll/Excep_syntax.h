#pragma once
#ifdef _WIN32
#define LIB_EXPORT
#endif

#include "Excep_base.h"

namespace htf {
    namespace exception {
        // C++头文件语法错误
        class DLL_API Excep_syntax : public exception::Excep_base {
        public:
            Excep_syntax() = delete;
            // file line content
            Excep_syntax(const std::string& where_path, line_t line, const std::string& what);

            line_t line() const;

            std::string str() const;
            
        private:
            line_t _line;
        };

        DLL_API std::ostream& operator<<(std::ostream&, const Excep_syntax&);
    }
}