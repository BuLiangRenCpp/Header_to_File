#pragma once

// 有值参数的选项:
// - 无值时使用默认参数
// - 必须有值

// ! 目前有个问题：
// * 如果 T = int，那么第一次初始化值为 "0" 而不是 "" 目前无影响，故不改动  (因为用
// value_type{} 去初始化)
// * 但是 vals 为 empty (因为用 {} 初始化)

#include "ExcepCmdline.h"
#include "OptionBase.h"
#include "OptionValue.hpp"

namespace htf
{
namespace cmdline
{

template <typename T> class OptionWithVal : public OptionBase
{
public:
    using value_type = T;

public:
    // 单值
    // 有默认值的：命令行解析处写了 option 但不提供值，则使用默认值
    // - is_default: 表示这个 option 是否是默认的选项, 即可以不用写 --name or -short_name
    OptionWithVal(const std::string& name, char short_name = no_short_name,
                  const std::string& description = "", bool is_must = false,
                  bool is_default = false, const value_type& default_value = value_type{})
        : OptionBase{name, short_name, description, is_must}
        , _val{}
        , _is_default{is_default}
        , _default_val{default_value}
    {
        std::string str = detail::OptionValCast<
            std::string,
            value_type,
            detail::is_same<std::string, value_type>::value>::cast(default_value);
        // THROW_LOGIC_ERROR_IF(str.find(' ') != std::string::npos,
        //     "OptionWithVal::OptionWithVal(......): single value don't include space" <<
        //     mark(str));
    }

    // 多值
    OptionWithVal(const std::string& name, char short_name = no_short_name,
                  const std::string& description = "", bool is_must = false,
                  bool is_default = false, const std::vector<value_type>& default_values = {})
        : OptionBase{name, short_name, description, is_must}
        , _val{std::vector<value_type>()}
        , _is_default{is_default}
        , _default_val{default_values}
    {
        for (auto it : default_values) {
            std::string str =
                detail::OptionValCast<std::string,
                                      value_type,
                                      detail::is_same<std::string, value_type>::value>::cast(it);
            // THROW_LOGIC_ERROR_IF(str.find(' ') != std::string::npos,
            //     "OptionWithVal::OptionWithVal(......): single value don't include
            //     space" << mark(str));
        }
    }

    ~OptionWithVal() = default;

public:
    bool empty() const override { return _val.empty(); }
    bool has_value() const override { return true; }
    bool is_default() const override { return _is_default; }
    bool set_value(const std::string& value) override
    {
        // THROW_LOGIC_ERROR_IF(value.find(' ') != std::string::npos,
        //     "OptionWithVal::set_value(......): single value don't include space" <<
        //     mark(value));
        _val.set_value(
            detail::OptionValCast<value_type,
                                  std::string,
                                  detail::is_same<value_type, std::string>::value>::cast(value));
        return true;
    }

    bool set_value(const std::vector<std::string>& value) override
    {
        std::vector<value_type> val;
        for (auto it : value) {
            // THROW_LOGIC_ERROR_IF(it.find(' ') != std::string::npos,
            //     "OptionWithVal::set_value(<.>): single value don't include space" <<
            //     mark(it));
            val.emplace_back(
                detail::OptionValCast<value_type,
                                      std::string,
                                      detail::is_same<value_type, std::string>::value>::cast(it));
        }
        _val.set_value(val);
        return true;
    }

    bool is_more_value() const override { return _val.is_more_value(); }

    std::string val() const override { return _val.val(); }

    std::vector<std::string> vals() const override { return _val.vals(); }

    std::string default_val() const override { return _default_val.val(); }

    std::vector<std::string> default_vals() const override { return _default_val.vals(); }

    // 单值：覆盖
    // 多值：push_back
    void push(const std::string& value) override
    {
        _val.push(
            detail::OptionValCast<value_type,
                                  std::string,
                                  detail::is_same<value_type, std::string>::value>::cast(value));
    }

    void clear() override { _val.clear(); }

private:
    std::string             _name;
    char                    _sname;
    std::string             _desc;
    OptionValue<value_type> _val;   // 实际值
    bool                    _is_default;
    OptionValue<value_type> _default_val;
};

}   // namespace cmdline
}   // namespace htf