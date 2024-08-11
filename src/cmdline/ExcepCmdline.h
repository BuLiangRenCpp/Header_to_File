#pragma once

#include "ExcepBase.h"

namespace htf
{
namespace cmdline
{

class ExcepCmdline : public ExcepBase
{
public:
    ExcepCmdline(const std::string& where, const what_type& what);
    ~ExcepCmdline() = default;

public:
    std::string str() const override
    {
#ifdef USER
        return "cmdline-error: " + _what;
#else
        return "cmdline-error: " + _where + ": " + _what;
#endif
    }
};

}   // namespace cmdline
}   // namespace htf
