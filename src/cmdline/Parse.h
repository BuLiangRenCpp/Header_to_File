#pragma once

// 解析命令行参数

#include <set>
#include <vector>

#include "Option.h"
#include "using.h"

namespace htf
{
namespace cmdline
{

class Parse
{
public:
    // program: 程序名
    Parse()  = default;
    ~Parse() = default;

public:
    // - name: option name
    // - short_name: short option name (if (short_name == '\0'): there's no short name)
    // - description: the comment of name
    // - is_must: if (is_must == true): the option cannot lack
    void add(const std::string& name, char short_name = OptionBase::no_short_name,
             const std::string& description = "", bool is_must = false);

    // - name: option name
    // - short_name: short option name (if (short_name == '\0'): there's no short name)
    // - description: the comment of name
    // - is_must: if (is_must == true): the option cannot lack
    // - is_default: the option is default.
    // - - 解析时如果只有值，没有 --option，那么认为它是 default option 的值
    // - - 一个 Parse 只能由一个 default option
    // - default_value: 如果只是指出 --option，没有值，将使用 default value
    template <typename T>
    void add(const std::string& name, char short_name = OptionBase::no_short_name,
             const std::string& description = "", bool is_must = false, bool is_default = false,
             const T& default_value = T{})
    {
        _options.add<T>(name, short_name, description, is_must, is_default, default_value);
    }

    // - name: option name
    // - short_name: short option name (if (short_name == '\0'): there's no short name)
    // - description: the comment of name
    // - is_must: if (is_must == true): the option cannot lack
    // - is_default: the option is default.
    // - - 解析时如果只有值，没有 --option，那么认为它是 default option 的值
    // - - 一个 Parse 只能由一个 default option
    // - default_value: 如果只是指出 --option，没有值，将使用 default value
    template <typename T>
    void add_more(const std::string& name, char short_name = OptionBase::no_short_name,
                  const std::string& description = "", bool is_must = false,
                  bool is_default = false, const std::vector<T>& default_value = {})
    {
        _options.add_more<T>(name, short_name, description, is_must, is_default, default_value);
    }
    // 此函数优先级最高
    // 如果没有指定程序名，则程序名为 parse 中的 argc[0]
    void set_program_name(const std::string& program_name);

    // 返回 parse 解析后的所有 option 的值
    // - 无值: ""
    // - 单值: value
    // - 多值: value1 value2  (每个值之间有一个空格)
    std::string show() const;

    std::string program_name() const { return _program; }
    // 无值
    bool exist(const std::string& option_name) const
    {
        return _look_up.count(_options.get_option(option_name)->short_name());
    }
    bool exist(char short_name) const { return _look_up.count(short_name); }
    // 主要用于：单值
    // 如果是多值: 则返回的字符串 每个值之间有一个空格
    std::string get_value(const std::string& name);
    // 主要用于：单值
    // 如果是多值: 则返回的字符串 每个值之间有一个空格
    std::string get_value(char short_name);
    // 多值
    std::vector<std::string> get_values(const std::string& name);
    // 多值
    std::vector<std::string> get_values(char short_name);

public:
    std::string usage() const;
    std::string errors() const;
    /************************************************ */
    bool parse(int argc, char* const argv[]);
    /************************************************ */

private:
    // parse 辅助函数

    // 如果没有 --help (-?)，则自动添加
    // 清理上一次解析后的数据
    void init();
    // 检测 must options 是否都存在
    void check();
    bool set_value(const std::string& name, const std::string& value);
    // 特殊字符: '-'  '='  (如果要使用特殊字符，则在前面加上 '\')
    // * '\' 仅当后面紧跟特殊字符时表示转义字符，其余均表示 '\'
    bool is_spe_ch(char c);
    // 将 i 之后的值赋给 option (处理所有情况)
    void set_value_after(int& i, int argc, char* const argv[], OptionBase* option,
                         bool is_short = false);
    // 处理 value 中开头的特殊字符、转义字符
    // - 如果时特殊字符开头: return false && 将错误加入 _error
    bool deal_spe_value(std::string& value);
    // 处理 argv[argc - 1]
    void deal_end(OptionBase* option, bool is_short = false);
    // 从 arg[2] 开始处理 (处理无值参数一起写：-abc == -a -b -c (从 -b 开始))
    void deal_noval_together(const std::string& arg);
    // 如果是单值，则 将错误加入 _errors
    void insert_look_up(OptionBase* option, bool is_short = false);
    // 值不存在: return nullptr && 将错误加入 _error
    OptionBase* get_option(const std::string& name);
    // 值不存在: return nullptr && 将错误加入 _error
    OptionBase* get_option(char short_name);

private:
    Option         _options;   // 存储 add 加入的 option，相当于是解析规则
    std::set<char> _look_up;   // 存储 parse 解析过程中，解析到的 option (short_name)
    std::string    _program           = "";   // 程序名称
    std::vector<std::string> _errors = {};
};

}   // namespace cmdline
}   // namespace htf