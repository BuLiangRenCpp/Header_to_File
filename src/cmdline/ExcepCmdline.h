#pragma once

#include "ExcepBase.h"

namespace htf
{
namespace cmdline
{

#define THROW_EXCEP_CMDLINE_IF(expr, msg)                \
    do {                                                 \
        if ((expr)) {                                    \
            std::ostringstream oss;                      \
            oss << HTF_ERROR_WHERE << msg;               \
            throw htf::cmdline::ExcepCmdline{oss.str()}; \
        }                                                \
    } while (0)

class ExcepCmdline : public ExcepBase
{
public:
    ExcepCmdline(const std::string& what) : ExcepBase{"cmdline-error: " + what} {}
    ~ExcepCmdline() = default;
};

}   // namespace cmdline
}   // namespace htf
