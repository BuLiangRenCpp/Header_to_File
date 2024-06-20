#pragma once
#ifdef _WIN32
#define LIB_EXPORT
#endif

#include "dll.h"
#include "header.h"

namespace htf {
    namespace exception {
        class DLL_API Excep_base {
        public:
            Excep_base() = delete;
            Excep_base(const std::string& where, const std::string& what);

            std::string where() const;
            std::string what() const;

            // dev-error
            std::string all() const;

        private:
            std::string _where;     // error 发生的地方：如某文件、某命名空间、某类、某函数中(只要能唯一标识即可)，可为空
            std::string _what;      // error content，不能为空
        };

        DLL_API std::ostream& operator<<(std::ostream&, const Excep_base&);
    }
}