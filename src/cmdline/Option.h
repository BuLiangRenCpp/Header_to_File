#pragma once

// options

#include <map>

#include "ExcepCmdline.h"
#include "OptionWithVal.hpp"
#include "OptionWithoutVal.h"

namespace htf
{
namespace cmdline
{

// default-option 最多有一个

class Option
{
public:   
    Option() = default;
    ~Option();

public:
    void add(OptionBase* option);

    // if (short_name = '\0'): 表示没有 _sname
    void add(const std::string& name, char short_name = OptionBase::no_short_name,
             const std::string& description = "", bool is_must = false);

    // * 单值
    template <typename T>
    void add(const std::string& name, char short_name = OptionBase::no_short_name,
             const std::string& description = "", bool is_must = false, bool is_default = false,
             const T& default_value = T{})
    {
        THROW_EXCEP_CMDLINE_IF(_options.count(name), "multiple definition" << mark(name));
        THROW_EXCEP_CMDLINE_IF(_short_to_name.count(short_name), 
            "multiple definition short option" << mark(short_name));
        THROW_EXCEP_CMDLINE_IF(is_default && (_default != nullptr), "default option only one");
        _options[name] =
            new OptionWithVal<T>(name, short_name, description, is_must, is_default, default_value);
        if (short_name != OptionBase::no_short_name) _short_to_name[short_name] = name;
        if (is_default) _default = _options[name];   // _default 与 _options 中的是同一份内存
        _option_names_order.emplace_back(name);
        if (_options[name]->is_must()) _must_options.emplace_back(_options[name]->short_name());
    }

    // * 多值
    template <typename T>
    void add_more(const std::string& name, char short_name = OptionBase::no_short_name,
                  const std::string& description = "", bool is_default = false,
                  bool is_must = false, const std::vector<T>& default_values = {})
    {
        THROW_EXCEP_CMDLINE_IF(_options.count(name), "multiple definition" << mark(name));
        THROW_EXCEP_CMDLINE_IF(_short_to_name.count(short_name), 
            "multiple definition short option" << mark(short_name));
        THROW_EXCEP_CMDLINE_IF(is_default && (_default != nullptr), "default option only one");
        _options[name] = new OptionWithVal<T>(
            name, short_name, description, is_must, is_default, default_values);
        if (short_name != OptionBase::no_short_name) _short_to_name[short_name] = name;
        if (is_default) _default = _options[name];   // _default 与 _options 中的是同一份内存
        _option_names_order.emplace_back(name);
        if (_options[name]->is_must()) _must_options.emplace_back(_options[name]->short_name());
    }

public:
    bool exist(const std::string& option_name) const { return _options.count(option_name); }

    bool exist(char short_name) const { return _short_to_name.count(short_name); }

    // 设置 option_name 的值
    // - if  option 为 OptionWithoutVal: return false
    bool set_value(const std::string& option_name, const std::string& value);
    bool set_value(char short_name, const std::string& value);

    OptionBase* const get_option(const std::string& option_name) const
    {
        THROW_EXCEP_CMDLINE_IF(!exist(option_name), "No such option: --" << option_name);
        return _options.at(option_name);
    }

    OptionBase* const get_option(char short_name) const
    {
        THROW_EXCEP_CMDLINE_IF(!exist(short_name), "No such short option: -" << short_name);
        return _options.at(_short_to_name.at(short_name));
    }

    std::string get_name(char short_name)
    {
        THROW_EXCEP_CMDLINE_IF(!exist(short_name), "No such short option: -" << short_name);
        return _short_to_name.at(short_name);
    }

    OptionBase* const default_option() const { return _default; }

    std::vector<std::string> option_names() const { return _option_names_order; }
    std::vector<char>        must_options() const { return _must_options; }
    bool                     is_must(char short_name) const
    {
        for (char it : _must_options)
            if (it == short_name) return true;
        return false;
    }

    // 将 _options 的所有 value 清空
    void value_clear();

private:
    std::map<std::string, OptionBase*> _options;              // name 对用的 option
    std::map<char, std::string>        _short_to_name;        // short name 对应的 name
    std::vector<std::string>           _option_names_order;   // name 被加入的顺序
    std::vector<char>                  _must_options;
    OptionBase*                        _default = nullptr;   // 默认值
};

}   // namespace cmdline
}   // namespace htf