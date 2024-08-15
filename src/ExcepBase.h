#pragma once

#include <sstream>
#include <stdexcept>

#include "debug.h"
#include "output.h"   // 一般都会用到 mark()

namespace htf
{

#define THROW_EXCEP_BASE_IF(expr, msg) \
    do { \
        if ((expr)) { \
            std::ostringstream oss;            \
            oss << HTF_ERROR_WHERE << msg;     \
            throw htf::ExcepBase{oss.str()}; \
        } \
    } while(0)
    

class ExcepBase : public std::exception
{
public:
    ExcepBase() = default;
    ExcepBase(const std::string& msg) :_msg{ "htf-error: " + msg } { }
    virtual ~ExcepBase() = default;

public:
    virtual const char* what() const noexcept { return _msg.c_str(); }

protected:
    std::string _msg;
};

}   // namespace htf