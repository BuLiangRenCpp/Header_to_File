#pragma once

// options

#include <map>

namespace htf
{
namespace cmdline
{

class OptionBase;
// default-option 最多有一个

class Option
{
    // * 单值
    template <typename T>
    void add(const std::string& name, char short_name = OptionBase::no_short_name,
             const std::string& description = "", bool is_must = false,
             bool is_default = false, const T& default_value = T{})
    {
        THROW_LOGIC_ERROR_IF(_options.count(name),
                             "template Option::add: multiple definition" << mark(name));
        THROW_LOGIC_ERROR_IF(_short_to_name.count(short_name),
                             "Option::add: multiple definition short option"
                                 << mark(short_name));
        THROW_LOGIC_ERROR_IF(is_default && (_default != nullptr),
                             "Option::add: default option only one");
        _options[name] = new OptionWithVal<T>(
            name, short_name, description, is_must, is_default, default_value);
        if (short_name != OptionBase::no_short_name) _short_to_name[short_name] = name;
        if (is_default)
            _default = _options[name];   // _default 与 _options 中的是同一份内存
        _option_names_order.emplace_back(name);
        if (_options[name]->is_must())
            _must_options.emplace_back(_options[name]->short_name());
    }
};

}   // namespace cmdline
}   // namespace htf