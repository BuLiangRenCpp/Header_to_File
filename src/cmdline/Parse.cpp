#include "Parse.h"
#include <iomanip>

std::set<char> Parse_Sep_Chs{'-', '='};   // 特殊字符
const char     Escape_Char = '\\';        // 转义字符

namespace htf
{
namespace cmdline
{

void Parse::add(const std::string& name, char short_name, const std::string& description,
                bool is_must)
{
    _options.add(name, short_name, description, is_must);
}

void Parse::set_program_name(const std::string& program_name)
{
    _program = program_name;
}

std::string Parse::show() const
{
    std::ostringstream oss;
    for (char it : _look_up) {
        auto option = _options.get_option(it);
        if (!option->has_value()) {
            oss << it << std::endl;
        }
        else {
            oss << it << ": " << option->val() << std::endl;
        }
    }
    return oss.str();
}

std::string Parse::get_value(const std::string& name)
{
    return _options.get_option(name)->val();
}

std::string Parse::get_value(char short_name)
{
    return _options.get_option(short_name)->val();
}

std::vector<std::string> Parse::get_values(const std::string& name)
{
    return _options.get_option(name)->vals();
}

std::vector<std::string> Parse::get_values(char short_name)
{
    return _options.get_option(short_name)->vals();
}

static int max_size(const std::vector<std::string>& tmp)
{
    int res = 0;
    for (auto it : tmp) {
        int len = it.length();
        if (len > res) res = len;
    }
    return res;
}

std::string Parse::usage() const
{
    std::ostringstream oss;
    oss << "Usage: " << _program << " ";
    auto must_options = _options.must_options();
    for (char c : must_options) {
        auto        opt = _options.get_option(c);
        std::string suf = (opt->has_value()) ? "=(value" : "";
        if (opt->is_more_value()) suf += "s";
        oss << "--" << opt->name() << suf << ") ";
    }
    oss << "[options] ..." << std::endl;
    oss << "Options: " << std::endl;
    auto names       = _options.option_names();
    int  long_width  = max_size(names) + 5;
    int  short_width = 4;   // short_name 的长度
    for (auto name : names) {
        std::ostringstream short_name, long_name;
        char               sname = _options.get_option(name)->short_name();
        std::string        rear  = (_options.is_must(sname)) ? "*" : "";   // must option*
        short_name << '-' << sname << rear;
        long_name << "--" << name << rear;
        oss << "\t" << std::left << std::setw(short_width) << short_name.str() << std::left
            << std::setw(long_width) << long_name.str() << _options.get_option(name)->description()
            << std::endl;
    }
    return oss.str();
}

std::string Parse::errors() const
{
    std::ostringstream oss;
    int                i = 0;
    for (auto it : _errors) {
        oss << "[" << (i++) << "] " << it << std::endl;
    }
    return oss.str();
}

bool Parse::parse(int argc, char* const argv[])
{
    init();
    THROW_EXCEP_CMDLINE_IF(argc < 1, "'argc' must be more than 0");
    if (_program.empty()) set_program_name(argv[0]);   // set_program_name 优先级最高

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.empty()) continue;
        if (arg.substr(0, 1) != "-") {   // * 1. default option
            auto default_option = _options.default_option();
            if (default_option == nullptr) {
                _errors.emplace_back("before" + mark(arg) + "need option");
                continue;
            }
            default_option->push(arg);   // 如果是单值，那么后面的值会覆盖前面的值
            insert_look_up(default_option);
        }
        else if (arg.substr(0, 2) == "--") {   // * 2. long option
            std::string str = arg.substr(2);
            auto        pos = str.find('=');
            if (pos != std::string::npos) {   // 2.1 --option=val
                std::string opt    = str.substr(0, pos);
                std::string val    = str.substr(pos + 1);
                auto        option = get_option(opt);
                if (option == nullptr) continue;
                insert_look_up(option);
                if (val.empty()) {   // 2.1.1 --option=  value
                    if (!option->has_value()) {
                        _errors.emplace_back("no-value option" + mark("--" + opt) + "cannot use" + mark('='));
                        continue;
                    }
                    set_value_after(i, argc, argv, option);
                }
                else {   // 2.1.2 指定值
                    // * 多值: 应该使用 'option=v1 option=v2' or 'option v1 v2'
                    if (!option->has_value()) {
                        _errors.emplace_back("no-value option" + mark("--" + opt) + "cannot use" + mark('='));
                        continue;
                    }
                    if (!deal_spe_value(val)) continue;
                    option->push(val);
                }
            }
            else {   // 2.2 --option val  or  --option = val
                std::string opt    = str;
                auto        option = get_option(opt);
                if (option == nullptr) continue;
                insert_look_up(option);
                if (i + 1 == argc)
                    deal_end(option);
                else {
                    ++i;
                    std::string value{argv[i]};
                    // * 多值: 应该使用 'option=v1 option=v2' or 'option v1 v2'
                    if (value == "=") {   // --option1 = val1   --option2 = val2
                        if (i + 1 == argc) deal_end(option);
                        value = argv[++i];
                    }
                    else if (value.length() > 1 && value[0] == '=') {   // --option =val
                        value = value.substr(1);
                    }
                    if (!deal_spe_value(value)) continue;
                    option->push(value);
                }
            }
        }
        else {                         // * 3. short option
            if (arg.length() == 1) {   // '-'
                _errors.emplace_back("missing short option after" + mark('-'));
                continue;
            }
            char opt    = arg[1];
            auto option = get_option(opt);
            if (option == nullptr) continue;
            insert_look_up(option, opt);
            if (arg.length() == 2) {   // 3.1  -short_option  [val]
                set_value_after(i, argc, argv, option, true);
            }
            else {   // 3.2 -short_option[val]
                std::string val = arg.substr(2);
                if (!option->has_value()) {   // 3.2.1 无值: 则认为之后的都是无值option
                    deal_noval_together(arg);
                }
                else {   // 3.2.2 多值、单值
                    if (val[0] == '=') {
                        _errors.emplace_back(mark('=') + "required use option" + mark("--" + option->name()));
                        continue;
                    }
                    if (!deal_spe_value(val)) continue;
                    option->push(val);
                    if (option->is_more_value()) set_value_after(i, argc, argv, option, true);
                }
            }
        }
    }
    // is_must option 是否都出现了
    check();
    return _errors.empty();
}

// private
void Parse::init()
{
    _look_up.clear();
    _errors.clear();
    _options.value_clear();
}

void Parse::check()
{
    auto must_options = _options.must_options();
    for (char it : must_options) {
        if (!_look_up.count(it)) {
            _errors.emplace_back("lack of must option" + mark("--" + _options.get_name(it)));
        }
    }
}

bool Parse::set_value(const std::string& name, const std::string& value)
{
    return _options.set_value(name, value);
}

bool Parse::is_spe_ch(char c)
{
    return Parse_Sep_Chs.count(c);
}

void Parse::set_value_after(int& i, int argc, char* const argv[], OptionBase* option, bool is_short)
{
    ++i;
    if (i == argc)
        deal_end(option, is_short);
    else {
        if (!option->has_value()) {
            --i;
            return;   // 1. 无值
        }
        if (option->is_more_value()) {   // 2. 多值
            for (; i < argc; ++i) {
                std::string value(argv[i]);
                if (value.empty()) continue;
                if (value[0] == '-') {
                    --i;   // 主函数在一个循环中，会执行 ++i
                    return;
                }
                if (is_short && value[0] == '=') {
                    _errors.emplace_back(mark('=') + "required use option" + mark("--" + option->name()));
                    return;
                }
                if (deal_spe_value(value)) {
                    option->push(value);
                }
            }
        }
        else {   // 3. 单值
            std::string value(argv[i]);
            while (value.empty()) {
                ++i;
                if (i == argc) {
                    deal_end(option, is_short);
                    return;
                }
                value = argv[i];
            }
            if (is_short && value[0] == '=') {
                _errors.emplace_back( mark('=') + "required use option" + mark("--" + option->name()));
                return;
            }
            if (deal_spe_value(value)) {
                option->push(value);
            }
        }
    }
}

bool Parse::deal_spe_value(std::string& value)
{
    if (value.length() >= 1) {
        if (is_spe_ch(value[0])) {
            _errors.emplace_back(mark(value) + ": the value[0] =" + mark(value[0]) +
                                 "is special char which required use" +
                                 mark(std::string{Escape_Char} + std::string{value[0]}) +
                                 "to replace" + mark(value[0]));
            return false;
        }
        if (value.length() > 1 && value[0] == Escape_Char && is_spe_ch(value[1])) {   // 转义字符
            value = value.substr(1);
        }
    }
    return true;
}

void Parse::deal_end(OptionBase* option, bool is_short)
{
    if (!option->empty()) return;
    std::string opt = (is_short) ? "-" + std::string{option->short_name()} : "--" + option->name();
    if (option->is_more_value()) {   // 1. 多值
        auto default_value = option->default_vals();
        if (default_value.empty()) {   // 无默认值
            _errors.emplace_back("missing values after option" + mark(opt));
            return;
        }
        option->set_value(default_value);
    }
    else {                            // 2.单值、无值
        if (!option->has_value()) {   // 2.1 无值
            return;
        }
        auto default_value = option->default_val();   // 2.2 单值
        if (default_value.empty()) {                  // 无默认值
            _errors.emplace_back("missing value after option" + mark(opt));
            return;
        }
        option->set_value(default_value);
    }
}

void Parse::deal_noval_together(const std::string& arg)
{
    for (int j = 2; j < arg.length(); ++j) {
        auto option = get_option(arg[j]);
        if (option == nullptr) return;
        if (option->has_value()) {
            _errors.emplace_back("valuable option" + mark("-" + std::string{arg[j]}) +
                "shoule to replace" + mark(arg));
            continue;
        }
        insert_look_up(option, arg[j]);
    }
}

void Parse::insert_look_up(OptionBase* option, bool is_short)
{
    if (option->has_value() && !option->is_more_value()) {
        if (_look_up.count(option->short_name())) {
            std::string opt =
                (is_short) ? "-" + std::string{option->short_name()} : "--" + option->name();
            _errors.emplace_back("option" + mark(opt) + "is ambiguous");
            return;
        }
    }
    _look_up.insert(option->short_name());
}

OptionBase* Parse::get_option(const std::string& name)
{
    if (!_options.exist(name)) {
        _errors.emplace_back("unrecognized command-line option" + mark("--" + name));
        return nullptr;
    }
    return _options.get_option(name);
}

OptionBase* Parse::get_option(char short_name)
{
    if (!_options.exist(short_name)) {
        _errors.emplace_back("unrecognized command-line short option" + mark("-" + std::string{short_name}));
        return nullptr;
    }
    return _options.get_option(short_name);
}

}   // namespace cmdline
}   // namespace htf
