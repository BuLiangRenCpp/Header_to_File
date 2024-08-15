#pragma once

// 命令行的选项: option
/**
 * option:
 * - name: 唯一标识 (--)
 * - short_name: 短名称 (-)
 * - description
 * - value:
 *    - without value
 *    - with value:
 *       · single value
 *       · more value
 * - is_must: 是否是必须的，如果是，则必须写
 *  is_default: the option is default.
 *    - 解析时如果只有值，没有 --option，那么认为它是 default option 的值
 *    - 一个 Parse 只能由一个 default option
 * - default_value: 如果只是指出 --option，没有值，将使用 default value
 */

#include <string>
#include <vector>

namespace htf
{
namespace cmdline
{

class OptionBase
{
public:
    static const char no_short_name = '\0';

public:
    // - name: option name
    // - short_name: short option name (if (short_name == '\0'): there's no short name)
    // - description: the comment of name
    // - is_must: if (is_must == true): the option cannot lack
    OptionBase(const std::string name, char short_name = no_short_name,
               const std::string& description = "", bool is_must = false);
    virtual ~OptionBase(){};

public:
    virtual bool is_default() const { return false; };
    virtual bool has_value() const { return false; }
    virtual bool set_value(const std::string& value) { return false; }
    virtual bool set_value(const std::vector<std::string>& value) { return false; }
    // - 无值: 无操作
    // - 单值: 覆盖原值
    // - 多值: push_back
    virtual void push(const std::string& value) {}
    virtual bool empty() const { return true; }
    virtual bool is_more_value() const { return false; }
    // 如果是多值: 每个值之间有空格
    virtual std::string              val() const { return ""; }
    virtual std::vector<std::string> vals() const { return {}; }
    // 如果是多值: 每个值之间有空格
    virtual std::string              default_val() const { return ""; }
    virtual std::vector<std::string> default_vals() const { return {}; }
    // 将值清空
    virtual void clear() {}

public:
    const std::string& name() const { return _name; }
    char               short_name() const { return _sname; }
    const std::string& description() const { return _desc; }
    bool               is_must() const { return _is_must; }

private:
    std::string _name;
    char        _sname;
    std::string _desc;
    bool        _is_must;
};

}   // namespace cmdline
}   // namespace htf