#include "Option.h"

namespace htf
{
namespace cmdline
{

Option::~Option()
{
    for (auto it : _options) delete it.second;   // _default 与 _options 中的是同一份内存
}

void Option::add(OptionBase* option)
{
    THROW_LOGIC_ERROR_IF(option == nullptr, "Option::add(.): this is nullptr pointer");
    _options[option->name()] = option;
    if (option->is_must()) _must_options.emplace_back(option->short_name());
}

void Option::add(const std::string& name, char short_name, const std::string& description,
                 bool is_must)
{
    THROW_LOGIC_ERROR_IF(_options.count(name),
                         "Option::add(....): multiple definition option" << mark(name));
    THROW_LOGIC_ERROR_IF(_short_to_name.count(short_name),
                         "Option::add: multiple definition short option" << mark(name));
    _options[name] = new OptionWithoutVal(name, short_name, description, is_must);
    if (short_name != OptionBase::no_short_name) _short_to_name[short_name] = name;
    _option_names_order.emplace_back(name);
    if (_options[name]->is_must()) _must_options.emplace_back(_options[name]->short_name());
}

bool Option::set_value(const std::string& option_name, const std::string& value)
{
    THROW_OUT_OF_RANGE_IF(!exist(option_name),
                          "Option::set_value(..c):" << mark(option_name) << "not exist");
    if (_options[option_name]->has_value()) {
        _options[option_name]->set_value(value);
        return true;
    }
    return false;
}

bool Option::set_value(char short_name, const std::string& value)
{
    THROW_OUT_OF_RANGE_IF(!exist(short_name),
                          "Option::set_value(..):" << mark(short_name) << "not exist");
    auto tmp = _options[_short_to_name[short_name]];
    if (tmp->has_value()) {
        tmp->set_value(value);
        return true;
    }
    return false;
}

void Option::value_clear()
{
    for (auto it : _options) {
        it.second->clear();
    }
}

}   // namespace cmdline
}   // namespace htf