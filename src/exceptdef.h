#pragma once

#include <sstream>
#include <stdexcept>

#include "output.h"

namespace htf
{

#define THROW_LOGIC_ERROR_IF(expr, what)       \
    do {                                       \
        if ((expr)) {                          \
            std::ostringstream oss;            \
            oss << what;                       \
            throw std::logic_error(oss.str()); \
        }                                      \
    } while (0)

#define THROW_OUT_OF_RANGE_IF(expr, what)       \
    do {                                        \
        if ((expr)) {                           \
            std::ostringstream oss;             \
            oss << what;                        \
            throw std::out_of_range(oss.str()); \
        }                                       \
    } while (0)

#define THROW_RUNTIME_ERROR_IF(expr, what)       \
    do {                                         \
        if ((expr)) {                            \
            std::ostringstream oss;              \
            oss << what;                         \
            throw std::runtime_error(oss.str()); \
        }                                        \
    } while (0)

}   // namespace htf
