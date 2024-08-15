#pragma once

#include <vector>

#include "detail.hpp"
#include "exceptdef.h"

namespace htf
{
namespace cmdline
{

template <typename T> class OptionValue
{
public:
    using value_type = T;

public:
    // 单值
    OptionValue() : _val{}, _is_more_value{false} {}

    OptionValue(const value_type& value) : _val{{value}}, _is_more_value{false} {}

    // 多值
    OptionValue(const std::vector<value_type>& values) : _val{values}, _is_more_value{true} {}

    ~OptionValue() = default;

public:
    bool is_more_value() const { return _is_more_value; }
    bool empty() const { return _val.empty(); }
    void clear() { _val.clear(); }

public:
    // 单值
    // 多值: 值之间用一个空格隔开
    std::string val() const
    {
        std::ostringstream oss;
        for (auto v : _val) {
            oss << " " << v;
        }
        return (oss.str().empty()) ? "" : oss.str().substr(1);
    }
    
    void set_value(const value_type& value)
    {
        _val.clear();
        _val.emplace_back(value);
    }

public:
    // 多值
    std::vector<std::string> vals() const
    {
        std::vector<std::string> res;
        for (auto it : _val) {
            res.emplace_back(
                detail::OptionValCast<std::string,
                                      value_type,
                                      detail::is_same<value_type, std::string>::value>::cast(it));
        }
        return res;
    }

    void set_value(const std::vector<value_type>& values)
    {
        _val = values;
    }

    // 单值：覆盖
    // 多值：push_back
    void push(const value_type& value)
    {
        if (is_more_value() || _val.empty())
            _val.emplace_back(value);
        else
            _val[0] = value;
    }

private:
    std::vector<value_type> _val;
    bool                    _is_more_value;
};

}   // namespace cmdline
}   // namespace htf