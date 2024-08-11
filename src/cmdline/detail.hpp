#pragma once

#include <sstream>

namespace htf
{
namespace cmdline
{
namespace detail
{

template <typename Target, typename Source, bool is_same> class OptionValCast
{
public:
    static Target cast(const Source& arg)
    {
        Target            ret;
        std::stringstream ss;
        if (!(ss << arg && ss >> arg && ss.eof())) return std::bad_cast();
        return ret;
    }
};

template <typename Target, typename Source> class OptionValCast<Target, Source, true>
{
public:
    static Target cast(const Source& arg) { return arg; }
};

template <typename Source> class OptionValCast<std::string, Source, false>
{
public:
    static std::string cast(const Source& arg)
    {
        std::string       ret;
        std::stringstream ss;
        ss << arg;
        return ss.str();
    }
};

template <typename Target> class OptionValCast<Target, std::string, false>
{
public:
    static Target cast(const std::string& arg)
    {
        Target            ret;
        std::stringstream ss(arg);
        if (!(ss >> ret && ss.eof())) throw std::bad_cast();
        return ret;
    }
};

template <typename T1, typename T2> struct is_same {
    static const bool value = false;
};

template <typename T> struct is_same<T, T> {
    static const bool value = true;
};

}   // namespace detail
}   // namespace cmdline
}   // namespace htf